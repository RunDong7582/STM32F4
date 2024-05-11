/*******************************************************************************
* File Name: bCORDIC.v
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  The bCORDIC module implements the CORDIC algorithm using 5 Datapaths. Depending
*  on the mode, this module either calculates
*  
*  Rotating mode: the sin(Angle) and cos(Angle) of the specified Angle.
*  Vectoring mode: the arctan2 and the hypotenuse of 2 vectors.
*
********************************************************************************
*                 I*O Signals
********************************************************************************
*    Name          Direction       Description
*
*    interrupt     output          Interrupt signal when frame finished
*    dma_trig      output          Used to trigger the DMA_Angle
*    enable        input           Synchronous active high enable
*    reset         input           Asynchronous reset
*    clock         input           Input clock - at most 0.5 Master clock
*
********************************************************************************
*                 Datapath Register Definitions
********************************************************************************
*
*  INSTANCE NAME: CoreDP
*
*  DESCRIPTION:
*    This 16-bit datapath calculates the sine and cosines of the given angle by 
*    utilizing the dynamic datapath mode.
*
*    X_new = X +/- (Y>>Loop)
*    Y_new = Y +/- (X>>Loop)
*
*    The sine and cosine results are obtained at the completion of the algorithm
*    and are contained in X_new and Y_new values respectively.
*
*  REGISTER USAGE:
*    F0 => Input/Output used to provide the X and Y values.
*    F1 => Input/Output used to provide the X and Y values.
*    D0 => Not used.
*    D1 => Used to hold 0x8000 to signify sign change.
*    A0 => Register for X when in arithmetic mode.
*    A1 => Register for Y when in arithmetic mode. Also holds the ALU result.
*
*********************************************************************************
*  INSTANCE NAME: AngleDP
*
*  DESCRIPTION:
*    This 16-bit datapath incrementally approaches the desired angle value by 
*    adding or subtracting the incoming angle values stored in the LUT. This is 
*    transferred using a DMA.
*
*  REGISTER USAGE:
*    F0 => Input used to accept the angles values from the LUT.
*    F1 => Not Used.
*    D0 => Not used.
*    D1 => Holds the desired angle value.
*    A0 => This is loaded with F0. 
*    A1 => Holds the current accumulated result.
*
*********************************************************************************
*  INSTANCE NAME: CountDP
*
*  DESCRIPTION:
*    This 8-bit datapath implements 2 counters: A small counter that counts up to
*    the current value of the big counter, and a big counter that counts up to a
*    limit set in D0. The small counter is then used to vary the number of shifts
*    in CoreDP state machine, and the big counter is used to signal the end of
*    the CORDIC algorithm.
*
*  REGISTER USAGE:
*    F0 => Not Used.
*    F1 => Not Used.
*    D0 => Holds the number of allowed Angle LUT values.
*    D1 => Not Used.
*    A0 => Holds the current value of the big counter. 
*    A1 => Holds the current value of the small counter.
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

`include "cypress.v"
`ifdef  bCORDIC_V_ALREADY_INCLUDED
`else
`define bCORDIC_V_ALREADY_INCLUDED

module bCORDIC
(
    output reg        interrupt,    /* Interrupt signal when frame finished */
    output reg        dma_trig,     /* trigger the input dma */
    input  wire       enable,       /* Synchronous active high enable */ 
	input  wire       reset,        /* Synchronous reset */
    input  wire       clock        /* Clock the component operates off */
);
	
	/***************************************************************************
    *            Component parameter definitions 
    ***************************************************************************/ 
	localparam SIN_COS_MODE = 1;
	localparam HYP_ARC_MODE = 0;
	parameter CORDIC_MODE = HYP_ARC_MODE;

    /***************************************************************************
    *       Internal signals
    ***************************************************************************/
    wire [1:0] ang_cl1;        /* A1 <= D1 condition                          */
	wire [1:0] ang_ce1;        /* A1 == D1 condition                          */
	wire       cnt_ce1;        /* A0 == A1 condition                          */
	wire       cnt_ce0;        /* A0 == D0 condition                          */
	wire [1:0] core_cl1;       /* A1 <= D1 condition                          */
    reg  [2:0] core_state;     /* State of the CoreDP                         */
	reg  [2:0] angle_state;    /* State of the CoreDP                         */
	reg  [2:0] cnt_state;      /* State of the CountDP                        */
	reg        loop_one;       /* Variable for determining if first loop      */
	wire       arith;          /* Addition or subtraction operation           */
	wire [1:0] f0_blk_stat;    /* AngleDP status of FIFO 0 (used for dma)     */
	wire       core_init;      /* Used for initialization operations          */
	reg        cnt_enable;     /* Enables the counter                         */
	wire       cordic_enable;  /* Enables the CORDIC module                   */
	reg        cordic_enable_buf; /* Buffer to capture the posedge of enable  */
	reg        cordic_complete_buf1; /* One more buffer for alignment         */
	reg        cordic_complete; /* Denotes the completion of CORDIC           */
	reg        cordic_complete_buf; /* Buffer to give Core A0 and A1 loading  */
	reg        dma_init;       /* Used to trigger the first DMA call          */
	reg        shift_en;       /* Transition from initial to normal routine   */
	reg        start_arith;    /* Starts the Angle arithmetic                 */
	reg [1:0]  core_fifo_ctrl; /* Used to control the dynamic mode of CordDP  */
	reg        load_mask;      /* Used to load FIFO 0 and 1 for CoreDP        */
	reg        y_cnt;          /* Used in double loading Y-new to the FIFOs   */
	reg [1:0]  a0_cnt;         /* A0<-F0 load count                           */
	reg [1:0]  a1_cnt;         /* A1<-F1 load count                           */
	wire       cordic_reset;   /* Reset signal for most sub-blocks            */
	reg        core_reset;     /* Reset signal to reset the CoreDP            */
	reg        hold_reset;     /* Used to flush out the CoreDP FIFOs          */
	reg        reset_start;    /* Starts the reset procedure                  */
	reg        arith_reg;      /* For capturing the arith value */
	
	/***************************************************************************
    *         Instantiation of udb_clock_enable primitive 
    ****************************************************************************
    * The udb_clock_enable primitive component allows to support clock enable 
    * mechanism and specify the intended synchronization behavior for the clock 
    * result (operational clock).
    */
    cy_psoc3_udb_clock_enable_v1_0 #(.sync_mode(`TRUE)) ClkSync
    (
        /* input  */    .clock_in(clock),
        /* input  */    .enable(1'b1),
        /* output */    .clock_out(op_clock)
    );

    /***************************************************************************
    *       Control register implementation                                      
    ***************************************************************************/   
    wire [7:0] ctrl;
    /* Control Register bit location (bits 7-1 are unused) */
    localparam [2:0] MDIO_CTRL_ENABLE     = 3'd0;
    localparam [2:0] MDIO_CTRL_RESET      = 3'd1;
    
    /* Control register */
    cy_psoc3_control #(.cy_force_order(1)) CtlReg
    (
        /* output [07:00] */  .control(ctrl)
    );        
    
    assign cordic_enable = ctrl[MDIO_CTRL_ENABLE] | enable;
	assign cordic_reset = ctrl[MDIO_CTRL_RESET] | reset;
	
	/* Capture the enable posedge */
	always @(posedge op_clock)
	begin
	    cordic_enable_buf <= cordic_enable;
	end
	
	assign core_init = cordic_enable & ~cordic_enable_buf;

	/***************************************************************************
    *       Status register                                           
    ***************************************************************************/
	wire[7:0] mystatus;
	
	cy_psoc3_status #(.cy_force_order(`TRUE), .cy_md_select(8'b00000000)) StsReg (
	/* input [07:00] */ .status(mystatus), // Status Bits
	/* input */ .reset(reset), // Reset from interconnect
	/* input */ .clock(op_clock) // Clock used for registering data
	);
	
	assign mystatus[7:1] = 6'b0;
	assign mystatus[0] = hold_reset | core_reset;
	
	/***************************************************************************
    *       Interrupt generation logic                                             
    ***************************************************************************/
	always @(posedge op_clock)
    begin
        /* Generate the interrupt when computation is complete */
        cordic_complete_buf <= cordic_complete;
		cordic_complete_buf1 <= cordic_complete_buf;
		interrupt <= cordic_complete_buf1;
    end 
	
	/***************************************************************************
    *       DMA request logic                                             
    ***************************************************************************/
	/* DMA request circuit */
	always @(posedge op_clock)
	begin
	    if(cordic_enable & ~cordic_reset)
		begin
		    if(dma_init & f0_blk_stat[0])
			begin
			    dma_trig <= 1'b1;
				dma_init <= 1'b0;
			end
			else if(f0_blk_stat[0])
			begin
			    dma_trig <= 1'b1;
				dma_init <= 1'b0;
			end
			else
			begin
			    dma_trig <= 1'b0;
				dma_init <= 1'b0;
			end
		end
		else
		begin
		    dma_trig <= 1'b0;
			dma_init <= 1'b1;
		end
	end
	
	/***************************************************************************
    *       Angle calcuation of the CORDIC control logic                                             
    ***************************************************************************/
	localparam ANGLE_STATE_IDLE       = 3'b000;
    localparam ANGLE_STATE_LOAD       = 3'b001;
    localparam ANGLE_STATE_COMPARE    = 3'b010;
	localparam ANGLE_STATE_ADD        = 3'b011;
    localparam ANGLE_STATE_SUB        = 3'b100;
	
/* Conditional synthesis */
generate
if (CORDIC_MODE == SIN_COS_MODE)
begin
	assign arith = ang_cl1[1]; /* Sin, Cos mode */
end
else
begin
	assign arith = arith_reg; /* Hyp, Arctan mode */
end
endgenerate
	
    always @(posedge op_clock)
    begin
	    if(cordic_reset)
		begin
		    /* flush out the FIFO */
		    angle_state <= ANGLE_STATE_LOAD;
		end
        else 
		begin
			if(cordic_enable)
			begin
				case(angle_state)
				ANGLE_STATE_IDLE:
					if(~shift_en & ~f0_blk_stat[0] & dma_trig) /* Initial run */
					begin
						/* Start the comparison between alpha and angle */
						angle_state <= ANGLE_STATE_COMPARE;
					end
					else if(start_arith) /* Start calculation for next CORDIC iteration */
					begin
						/* Load the next data from FIFO */
						angle_state <= ANGLE_STATE_LOAD;
					end
					else
					begin
						/* Do nothing */
						angle_state <= ANGLE_STATE_IDLE;
					end
				ANGLE_STATE_LOAD:
				begin
					/* Compare current angle(A1) and desired angle(D1) */
					angle_state <= ANGLE_STATE_COMPARE;
				end
				ANGLE_STATE_COMPARE:
				if(cnt_ce0)
				begin
					cordic_complete <= 1'b1;
					angle_state <= ANGLE_STATE_COMPARE;
				end
				else
				begin
					cordic_complete <= 1'b0;
					if(arith)
					begin
						/* A1 + A0 */
						angle_state <= ANGLE_STATE_ADD;
					end
					else
					begin
						/* A1 - A0 */
						angle_state <= ANGLE_STATE_SUB;
					end
				end
				ANGLE_STATE_ADD:
				begin
					angle_state <= ANGLE_STATE_IDLE;
				end
				ANGLE_STATE_SUB:
				begin
					angle_state <= ANGLE_STATE_IDLE;
				end
				default:
					angle_state <= ANGLE_STATE_IDLE;
				endcase
			end
			else
			begin
				angle_state <= ANGLE_STATE_IDLE;
				cordic_complete <= 1'b0;
			end
		end
    end
	
	/***************************************************************************
    *       CORDIC counter logic                                             
    ***************************************************************************/
    localparam COUNT_STATE_IDLE     = 3'b000;
    localparam COUNT_STATE_RESET_A0 = 3'b001;
    localparam COUNT_STATE_RESET_A1 = 3'b010;
	localparam COUNT_STATE_A0_INC   = 3'b011;
    localparam COUNT_STATE_A1_INC   = 3'b100;
	
    always @(posedge op_clock)
    begin
		case(cnt_state)
		COUNT_STATE_IDLE:
			if(cnt_enable)
			begin
				/* Start counting */
				cnt_state <= COUNT_STATE_A1_INC;
			end
			else
			begin
				cnt_state <= COUNT_STATE_IDLE;
			end
		COUNT_STATE_RESET_A0:
			/* Reset the small counter */
			cnt_state <= COUNT_STATE_RESET_A1;
		COUNT_STATE_RESET_A1:
		    if(~loop_one)
			begin
			    /* Iteration over. Increment the period counter */
			    cnt_state <= COUNT_STATE_A0_INC;
			end
			else
			begin
			    /* count again for the other variable */
			    cnt_state <= COUNT_STATE_IDLE;
			end
		COUNT_STATE_A0_INC:
			if(cnt_ce0)
			begin
				/* END of Angles in the AngTable */
				cnt_state <= COUNT_STATE_RESET_A0;
			end
			else
			begin
				/* Wait until at the right condition */
				cnt_state <= COUNT_STATE_IDLE;
			end
		COUNT_STATE_A1_INC:
			if(cnt_ce1)
			begin
				/* small counter reached the period */
				cnt_state <= COUNT_STATE_RESET_A1;
			end
			else
			begin
				/* Keep incrementing the counter */
				cnt_state <= COUNT_STATE_A1_INC;
			end
		default:
			cnt_state <= COUNT_STATE_IDLE;
		endcase
    end

	/***************************************************************************
    *       Core of the CORDIC control logic                                             
    ***************************************************************************/
	localparam CORE_STATE_IDLE      = 3'b000;
	localparam CORE_STATE_LOAD_A0   = 3'b001;
    localparam CORE_STATE_LOAD_A1   = 3'b010;
    localparam CORE_STATE_SHIFT_A0  = 3'b011;
	localparam CORE_STATE_SHIFT_A1  = 3'b100;
	localparam CORE_STATE_ADD       = 3'b101;
    localparam CORE_STATE_SUB_X     = 3'b110;
    localparam CORE_STATE_SUB_Y     = 3'b111;
	
	/* Dynamic mode control of the FIFOs   */
	/* 'Write' denotes dynamic mode enable */
	localparam FIFO_WRITE_BOTH = 2'b00;
	localparam FIFO_WRITE_F1   = 2'b01;
	localparam FIFO_WRITE_F0   = 2'b10;
	localparam FIFO_WRITE_NONE = 2'b11;
	
	always @(negedge op_clock)
	begin
	    hold_reset <= cordic_reset | (hold_reset & ~core_reset);
	end

/***************************************************************************************/
/* Hardware for sine and cosine calculation */
/***************************************************************************************/
generate
if (CORDIC_MODE == SIN_COS_MODE)
begin
	always @(posedge op_clock)
    begin
	    if(hold_reset)
		begin
			shift_en <= 1'b0;
			loop_one <= 1'b0;
			load_mask <= 1'b0;
			start_arith <= 1'b0;
			cnt_enable <= 1'b0;
		    if(reset_start & ~core_reset)
			begin
				if(a0_cnt != 2)
				begin
				    /* Empty FIFO 0 */
					core_fifo_ctrl <= FIFO_WRITE_BOTH;
					core_state <= CORE_STATE_LOAD_A0;
					a0_cnt <= a0_cnt + 1;
					reset_start <= 1'b1;
					core_reset <= 1'b0;
				end
				else
				begin
					if(a1_cnt != 3)
					begin
					    /* Empty FIFO 1 */
						core_fifo_ctrl <= FIFO_WRITE_BOTH;
						core_state <= CORE_STATE_LOAD_A1;
						a1_cnt <= a1_cnt + 1;
						reset_start <= 1'b1;
						core_reset <= 1'b0;
					end
					else
					begin
					    if(~y_cnt)
						begin
						    /* 'Glitch'. Empty one more time */
						    core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_LOAD_A1;
							y_cnt <= 1'b1;
							reset_start <= 1'b1;
							core_reset <= 1'b0;
						end
						else
						begin
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_IDLE;
							y_cnt <= 1'b0;
							a0_cnt <= 0;
							a1_cnt <= 0;
							reset_start <= 1'b0;
							core_reset <= 1'b1;
						end
					end
				end
			end
			else
			begin
				core_fifo_ctrl <= FIFO_WRITE_NONE;
				core_state <= CORE_STATE_IDLE;
				y_cnt <= 1'b0;
				a0_cnt <= 0;
				a1_cnt <= 0;
				reset_start <= 1'b1;
				core_reset <= 1'b0;
			end
		end
		else 
		begin
			if(cordic_enable & ~interrupt)
			begin
			    core_reset <= 1'b0;
				reset_start <= 1'b0;
				case(core_state)
				CORE_STATE_IDLE:
				begin
					if(~shift_en)
					begin
						if(core_init)
						begin
							/* Initial routine: Load X */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_ADD;
							loop_one <= loop_one^1;
							load_mask <= 1'b1;
						end
						else 
						begin
							/* Y has been loaded twice. Start the normal routine */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_LOAD_A0;
							load_mask <= 1'b0;
						end
					end
					else
					begin
						/* Double load Y-new */
						core_fifo_ctrl <= FIFO_WRITE_BOTH;
						core_state <= CORE_STATE_LOAD_A0;
						load_mask <= 1'b0;
					end
				end
				CORE_STATE_LOAD_A0:
				begin
					if(~shift_en)
					begin
						/* Initial run: Load A0 once then load A1 2 times */
						core_fifo_ctrl <= FIFO_WRITE_BOTH;
						core_state <= CORE_STATE_LOAD_A1;
						load_mask <= 1'b1;
					end
					else
					begin
						if (y_cnt)
						begin
							/* Shift the FIFO to make more room. Then load Y-new */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_IDLE;
							load_mask <= 1'b1;
							y_cnt <= 1'b0;
						end
						else
						begin
							if(a0_cnt != 1)
							begin
								/* Normal run: Load A0 4 times */
								core_fifo_ctrl <= FIFO_WRITE_F0;
								core_state <= CORE_STATE_LOAD_A0;
								a0_cnt <= a0_cnt + 1;
								load_mask <= 1'b0;
							end
							else
							begin
								/* Normal run: Load A1 3 times */
								core_fifo_ctrl <= FIFO_WRITE_F1;
								core_state <= CORE_STATE_LOAD_A1;
								a0_cnt <= 0;
								load_mask <= 1'b0;
							end
						end
					end
				end
				CORE_STATE_LOAD_A1:
				begin
					if(~shift_en)
					begin
						if(a1_cnt != 1)
						begin
							/* Load A1 3 times */
							core_fifo_ctrl <= FIFO_WRITE_F1;
							core_state <= CORE_STATE_LOAD_A1;
							a1_cnt <= a1_cnt + 1;
							load_mask <= 1'b0;
						end
						else
						begin
							/* Shift A1 routine */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_SHIFT_A1;
							a1_cnt <= 0;
							load_mask <= 1'b0;
							shift_en <= 1'b1; /* latch on here */
						end
					end
					else
					begin
						if(loop_one)
						begin
							if(a1_cnt != 2)
							begin
								/* Load A1 3 times */
								core_fifo_ctrl <= FIFO_WRITE_F1;
								core_state <= CORE_STATE_LOAD_A1;
								load_mask <= 1'b0;
								a1_cnt <= a1_cnt + 1;
								if (a1_cnt[0])
								begin
									cnt_enable <= 1'b1;
								end
								else
								begin
									cnt_enable <= 1'b0;
								end
							end
							else
							begin
								/* Shift A1 routine */
								core_fifo_ctrl <= FIFO_WRITE_NONE;
								core_state <= CORE_STATE_SHIFT_A1;
								load_mask <= 1'b0;
								a1_cnt <= 0;
								cnt_enable <= 1'b0;
							end
						end
						else
						begin
							/* Shift A0 routine */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_SHIFT_A0;
							load_mask <= 1'b0;
							a1_cnt <= 1'b0;
							cnt_enable <= 1'b0;
						end
					end
				end
				CORE_STATE_SHIFT_A0:
					if(~cnt_ce1)
					begin
						/* Shift right A0 */
						core_state <= CORE_STATE_SHIFT_A0;
						load_mask <= 1'b0;
					end
					else
					begin
						start_arith <= 1'b1; /* signal used to trigger the new angle calculation */
						if(~arith)
						begin
							/* A1 + A0 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_ADD;
							load_mask <= 1'b1;
						end
						else
						begin
							/* A1 - A0 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_SUB_Y;
							load_mask <= 1'b1;
						end
					end
				CORE_STATE_SHIFT_A1:
					if(~cnt_ce1)
					begin
						/* Shift right A1 */
						core_state <= CORE_STATE_SHIFT_A1;
						load_mask <= 1'b0;
					end
					else
					begin
						if(arith)
						begin
							/* A0 + A1 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_ADD;
							load_mask <= 1'b1;
						end
						else
						begin
							/* A0 - A1 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_SUB_X;
							load_mask <= 1'b1;
						end
					end
				CORE_STATE_ADD:
				begin
					if(~shift_en)
					begin
						/* Need to switch off and then on for FIFO capture */
						core_fifo_ctrl <= FIFO_WRITE_NONE;
						core_state <= CORE_STATE_SUB_Y;
						loop_one <= loop_one^1;
						load_mask <= 1'b0;
					end
					else
					begin
						if(loop_one)
						begin
							/* Start the Y-new routine */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_LOAD_A1;
							loop_one <= loop_one^1;
							load_mask <= 1'b0;
							cnt_enable <= 1'b1; /* trigger the counter */
						end
						else
						begin
							/* Load Y twice into FIFO */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_IDLE;
							loop_one <= loop_one^1;
							load_mask <= 1'b0;
							y_cnt <= 1'b1;
						end
					end
					start_arith <= 1'b0; /* signal used to trigger the new angle calculation */
				end
				CORE_STATE_SUB_X:
				begin
					/* Feed F0(X-new) and move to Y-new routine */
					core_fifo_ctrl <= FIFO_WRITE_NONE;
					core_state <= CORE_STATE_LOAD_A1;
					loop_one <= loop_one^1;
					load_mask <= 1'b0;
					cnt_enable <= 1'b1; /* trigger the counter */
				end
				CORE_STATE_SUB_Y:
				begin
					if(~shift_en)
					begin
						if(~y_cnt)
						begin
							/* Do it twice (155+0),(155-155),(0-155) */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_SUB_Y;
							y_cnt <= 1'b1;
							load_mask <= 1'b0;
						end
						else
						begin
							/* Load the second Y and start the normal routine */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_IDLE;
							load_mask <= 1'b1;
							y_cnt <= 1'b0;
							loop_one <= loop_one^1;
						end
					end
					else
					begin
						/* Load Y twice into the FIFO */
						core_fifo_ctrl <= FIFO_WRITE_NONE;
						core_state <= CORE_STATE_IDLE;
						load_mask <= 1'b0;
						y_cnt <= 1'b1;
						loop_one <= loop_one^1;
					end
					start_arith <= 1'b0; /* signal used to trigger the new angle calculation */
				end
				default:
				begin
					core_state <= CORE_STATE_IDLE;
					core_fifo_ctrl <= FIFO_WRITE_NONE;
					load_mask <= 1'b0;
					shift_en <= 1'b0;
				end
				endcase
			end
			else
			begin
				core_state <= CORE_STATE_IDLE;
				core_fifo_ctrl <= FIFO_WRITE_NONE;
				load_mask <= 1'b0;
				core_reset <= 1'b0;
				reset_start <= 1'b0;
			end
		end
	end
end

/***************************************************************************************/		
/* Hardware for arctan2 and hypotenuse calculation */
/***************************************************************************************/
else
begin
	always @(posedge op_clock)
    begin
	    if(hold_reset)
		begin
			shift_en <= 1'b0;
			loop_one <= 1'b0;
			load_mask <= 1'b0;
			start_arith <= 1'b0;
			cnt_enable <= 1'b0;
		    if(reset_start & ~core_reset)
			begin
				if(a0_cnt != 2)
				begin
				    /* Empty FIFO 0 */
					core_fifo_ctrl <= FIFO_WRITE_BOTH;
					core_state <= CORE_STATE_LOAD_A0;
					a0_cnt <= a0_cnt + 1;
					reset_start <= 1'b1;
					core_reset <= 1'b0;
				end
				else
				begin
					if(a1_cnt != 3)
					begin
					    /* Empty FIFO 1 */
						core_fifo_ctrl <= FIFO_WRITE_BOTH;
						core_state <= CORE_STATE_LOAD_A1;
						a1_cnt <= a1_cnt + 1;
						reset_start <= 1'b1;
						core_reset <= 1'b0;
					end
					else
					begin
					    if(~y_cnt)
						begin
						    /* 'Glitch'. Empty one more time */
						    core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_LOAD_A1;
							y_cnt <= 1'b1;
							reset_start <= 1'b1;
							core_reset <= 1'b0;
						end
						else
						begin
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_IDLE;
							y_cnt <= 1'b0;
							a0_cnt <= 0;
							a1_cnt <= 0;
							reset_start <= 1'b0;
							core_reset <= 1'b1;
						end
					end
				end
			end
			else
			begin
				core_fifo_ctrl <= FIFO_WRITE_NONE;
				core_state <= CORE_STATE_IDLE;
				y_cnt <= 1'b0;
				a0_cnt <= 0;
				a1_cnt <= 0;
				reset_start <= 1'b1;
				core_reset <= 1'b0;
			end
		end
        else 
		begin
			if(cordic_enable & ~interrupt)
			begin
			    core_reset <= 1'b0;
				reset_start <= 1'b0;
				case(core_state)
				CORE_STATE_IDLE:
				begin
				    arith_reg <= core_cl1[1]; /* latch on here */
					core_fifo_ctrl <= FIFO_WRITE_BOTH;
					core_state <= CORE_STATE_LOAD_A0;
					load_mask <= 1'b0;
				end
				CORE_STATE_LOAD_A0:
				begin
					if(~shift_en)
					begin
						/* Initial run: Load A0 once then load A1 2 times */
						core_fifo_ctrl <= FIFO_WRITE_F1;
						core_state <= CORE_STATE_LOAD_A1;
						load_mask <= 1'b0;
						loop_one <= loop_one^1;
					end
					else
					begin
						if (y_cnt)
						begin
							/* Shift the FIFO to make more room. Then load Y-new */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_IDLE;
							load_mask <= 1'b1;
							y_cnt <= 1'b0;
							start_arith <= 1'b1; /* signal used to trigger the new angle calculation (doesn't matter if twice) */
						end
						else
						begin
							start_arith <= 1'b0; /* signal used to trigger the new angle calculation */
							if(a0_cnt != 1)
							begin
								/* Normal run: Load A0 4 times */
								core_fifo_ctrl <= FIFO_WRITE_F0;
								core_state <= CORE_STATE_LOAD_A0;
								a0_cnt <= a0_cnt + 1;
								load_mask <= 1'b0;
							end
							else
							begin
								/* Normal run: Load A1 3 times */
								core_fifo_ctrl <= FIFO_WRITE_F1;
								core_state <= CORE_STATE_LOAD_A1;
								a0_cnt <= 0;
								load_mask <= 1'b0;
							end
						end
					end
				end
				CORE_STATE_LOAD_A1:
				begin
					if(~shift_en)
					begin
						if(loop_one)
						begin
							if(a1_cnt != 1)
							begin
								/* Load A1 twice */
								core_fifo_ctrl <= FIFO_WRITE_F1;
								core_state <= CORE_STATE_LOAD_A1;
								a1_cnt <= a1_cnt + 1;
								load_mask <= 1'b0;
							end
							else
							begin
								/* Xnew routine */
								core_fifo_ctrl <= FIFO_WRITE_BOTH;
								core_state <= CORE_STATE_ADD;
								a1_cnt <= 0;
								load_mask <= 1'b1;
							end
						end
						else
						begin
							/* Ynew routine */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_SUB_Y;
							a1_cnt <= 0;
							load_mask <= 1'b1;
							shift_en <= 1'b1; /* latch on here */
						end
					end
					else
					begin
						if(loop_one)
						begin
							if(a1_cnt != 2)
							begin
								/* Load A1 3 times */
								core_fifo_ctrl <= FIFO_WRITE_F1;
								core_state <= CORE_STATE_LOAD_A1;
								load_mask <= 1'b0;
								a1_cnt <= a1_cnt + 1;
								if (a1_cnt[0])
								begin
									cnt_enable <= 1'b1;
								end
								else
								begin
									cnt_enable <= 1'b0;
								end
							end
							else
							begin
								/* Shift A1 routine */
								core_fifo_ctrl <= FIFO_WRITE_NONE;
								core_state <= CORE_STATE_SHIFT_A1;
								load_mask <= 1'b0;
								a1_cnt <= 0;
								cnt_enable <= 1'b0;
							end
						end
						else
						begin
							/* Shift A0 routine */
							core_fifo_ctrl <= FIFO_WRITE_NONE;
							core_state <= CORE_STATE_SHIFT_A0;
							load_mask <= 1'b0;
							a1_cnt <= 1'b0;
							cnt_enable <= 1'b0;
						end
					end
				end
				CORE_STATE_SHIFT_A0:
					if(~cnt_ce1)
					begin
						/* Shift right A0 */
						core_state <= CORE_STATE_SHIFT_A0;
						load_mask <= 1'b0;
					end
					else
					begin
						if(~arith)
						begin
							/* A1 + A0 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_ADD;
							load_mask <= 1'b1;
						end
						else
						begin
							/* A1 - A0 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_SUB_Y;
							load_mask <= 1'b1;
						end
					end
				CORE_STATE_SHIFT_A1:
					if(~cnt_ce1)
					begin
						/* Shift right A1 */
						core_state <= CORE_STATE_SHIFT_A1;
						load_mask <= 1'b0;
					end
					else
					begin
						if(arith)
						begin
							/* A0 + A1 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_ADD;
							load_mask <= 1'b1;
						end
						else
						begin
							/* A0 - A1 */
							core_fifo_ctrl <= FIFO_WRITE_BOTH;
							core_state <= CORE_STATE_SUB_X;
							load_mask <= 1'b1;
						end
					end
				CORE_STATE_ADD:
				begin
					if(loop_one)
					begin
						/* Start the Y-new routine */
						core_fifo_ctrl <= FIFO_WRITE_NONE;
						core_state <= CORE_STATE_LOAD_A1;
						loop_one <= loop_one^1;
						load_mask <= 1'b0;
						cnt_enable <= 1'b1; /* trigger the counter */
					end
					else
					begin
						/* Load Y twice into FIFO */
						core_fifo_ctrl <= FIFO_WRITE_NONE;
						core_state <= CORE_STATE_IDLE;
						loop_one <= loop_one^1;
						load_mask <= 1'b0;
						y_cnt <= 1'b1;
					end
				end
				CORE_STATE_SUB_X:
				begin
					/* Feed F0(X-new) and move to Y-new routine */
					core_fifo_ctrl <= FIFO_WRITE_NONE;
					core_state <= CORE_STATE_LOAD_A1;
					loop_one <= loop_one^1;
					load_mask <= 1'b0;
					cnt_enable <= 1'b1; /* trigger the counter */
				end
				CORE_STATE_SUB_Y:
				begin
					/* Load Y twice into the FIFO */
					core_fifo_ctrl <= FIFO_WRITE_NONE;
					core_state <= CORE_STATE_IDLE;
					load_mask <= 1'b0;
					y_cnt <= 1'b1;
					loop_one <= loop_one^1;
				end
				default:
				begin
					core_state <= CORE_STATE_IDLE;
					core_fifo_ctrl <= FIFO_WRITE_NONE;
					load_mask <= 1'b0;
					shift_en <= 1'b0;
				end
				endcase
			end
			else
			begin
				core_state <= CORE_STATE_IDLE;
				core_fifo_ctrl <= FIFO_WRITE_NONE;
				load_mask <= 1'b0;
				core_reset <= 1'b0;
				reset_start <= 1'b0;
			end
		end
	end
end
endgenerate
	
cy_psoc3_dp16 #(.a0_init_a(8'b00000000), .a1_init_a(8'b00000000), .a0_init_b(8'b00101101), 
.a1_init_b(8'b00000000), 
.cy_dpconfig_a(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0: Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1: Load F0 to A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2: Compare A1, D1*/
    `CS_ALU_OP__ADD, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3: A1 + A0*/
    `CS_ALU_OP__SUB, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4: A1 - A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7: */
    8'hFF, 8'h00,  /*CFG9: */
    8'hFF, 8'hFF,  /*CFG11-10: */
    `SC_CMPB_A1_D1, `SC_CMPA_A1_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12: Cmp A1,D1*/
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, 1'h0,
    1'h0, `SC_FIFO1_ALU, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14: F0 in, chained*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16: Edge triggered*/
}
), .cy_dpconfig_b(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0: Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1: Load F0 to A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2: Compare A1, D1*/
    `CS_ALU_OP__ADD, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3: A1 + A0*/
    `CS_ALU_OP__SUB, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4: A1 - A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7: */
    8'hFF, 8'h00,  /*CFG9: */
    8'hFF, 8'hFF,  /*CFG11-10: */
    `SC_CMPB_A1_D1, `SC_CMPA_A1_D1, `SC_CI_B_ARITH,
    `SC_CI_A_CHAIN, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12: Cmp A1,D1*/
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, 1'h0,
    1'h0, `SC_FIFO1_ALU, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_CHNED,
    `SC_CMP0_NOCHN, /*CFG15-14: F0 in, chained*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16: Edge triggered*/
}
)) AngleDP(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(angle_state),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(dma_trig),
        /*  input                   */  .f1_load(1'b0),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output  [01:00]                  */  .ce0(),
        /*  output  [01:00]                  */  .cl0(),
        /*  output  [01:00]                  */  .z0(),
        /*  output  [01:00]                  */  .ff0(),
        /*  output  [01:00]                  */  .ce1(ang_ce1),
        /*  output  [01:00]                  */  .cl1(ang_cl1),
        /*  output  [01:00]                  */  .z1(),
        /*  output  [01:00]                  */  .ff1(),
        /*  output  [01:00]                  */  .ov_msb(),
        /*  output  [01:00]                  */  .co_msb(),
        /*  output  [01:00]                  */  .cmsb(),
        /*  output  [01:00]                  */  .so(),
        /*  output  [01:00]                  */  .f0_bus_stat(),
        /*  output  [01:00]                  */  .f0_blk_stat(f0_blk_stat),
        /*  output  [01:00]                  */  .f1_bus_stat(),
        /*  output  [01:00]                  */  .f1_blk_stat()
);
cy_psoc3_dp8 #(.a0_init_a(0), .a1_init_a(0), 
.cy_dpconfig_a(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0: Idle*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1: Reset A0*/
    `CS_ALU_OP__XOR, `CS_SRCA_A1, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2: Reset A1*/
    `CS_ALU_OP__INC, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3: A0++*/
    `CS_ALU_OP__INC, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4: A1++ until A0=A1*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6: */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7: */
    8'hFF, 8'h00,  /*CFG9: */
    8'hFF, 8'hFF,  /*CFG11-10: */
    `SC_CMPB_A1_D1, `SC_CMPA_A1_A0, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_ENBL, `SC_C0_MASK_ENBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12: */
    `SC_A0_SRC_ACC, `SC_SHIFT_SL, 1'h0,
    1'h0, `SC_FIFO1_BUS, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14: */
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16: */
}
)) CountDP(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(cnt_state),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(1'b0),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(cnt_ce0),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(cnt_ce1),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(),
        /*  output                  */  .f0_blk_stat(),
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat()
);
cy_psoc3_dp16 #(.a0_init_a(8'b10011011), .a1_init_a(8'b00000000), .a0_init_b(8'b00000000), 
.a1_init_b(8'b00000000), 
.cy_dpconfig_a(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0: Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1: Load A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC___F1,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2: Load A1*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3: Shift right A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4: Shift right A1*/
    `CS_ALU_OP__ADD, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5: A1 + A0 */
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6: A0 - A1 for Xnew*/
    `CS_ALU_OP__SUB, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7: A1 - A0 for Ynew*/
    8'hFF, 8'h00,  /*CFG9: */
    8'hFF, 8'hFF,  /*CFG11-10: */
    `SC_CMPB_A1_D1, `SC_CMPA_A1_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_CHAIN, /*CFG13-12: Cmp A1,D1*/
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, 1'h0,
    1'h0, `SC_FIFO1_ALU, `SC_FIFO0_ALU,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14: F0 in, chained*/
    6'h00, `SC_FIFO1_DYN_ON,`SC_FIFO0_DYN_ON,
    2'h00,`SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE /*CFG17-16: Edge triggered*/
,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,`SC_WRK16CAT_DSBL}
), .cy_dpconfig_b(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0: Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1: Load A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC___F1,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2: Load A1*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3: Shift right A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4: Shift right A1*/
    `CS_ALU_OP__ADD, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5: A1 + A0 */
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6: A0 - A1 for Xnew*/
    `CS_ALU_OP__SUB, `CS_SRCA_A1, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7: A1 - A0 for Ynew*/
    8'hFF, 8'h00,  /*CFG9: */
    8'hFF, 8'hFF,  /*CFG11-10: */
    `SC_CMPB_A1_D1, `SC_CMPA_A1_D1, `SC_CI_B_ARITH,
    `SC_CI_A_CHAIN, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12: Cmp A1,D1*/
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, 1'h0,
    `SC_SR_SRC_MSB, `SC_FIFO1_ALU, `SC_FIFO0_ALU,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14: F0 in, chained*/
    6'h00, `SC_FIFO1_DYN_ON,`SC_FIFO0_DYN_ON,
    2'h00,`SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE /*CFG17-16: Edge triggered*/
,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,`SC_WRK16CAT_DSBL}
)) CoreDP(
        /*  input                   */  .reset(core_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(load_mask),// captures when high(transition) and d0_load is low
        /*  input                   */  .f1_load(load_mask),// captures when high(transition) and d1_load is low
        /*  input                   */  .d0_load(core_fifo_ctrl[0]),
        /*  input                   */  .d1_load(core_fifo_ctrl[1]),
        /*  output  [01:00]                  */  .ce0(),
        /*  output  [01:00]                  */  .cl0(),
        /*  output  [01:00]                  */  .z0(),
        /*  output  [01:00]                  */  .ff0(),
        /*  output  [01:00]                  */  .ce1(),
        /*  output  [01:00]                  */  .cl1(core_cl1),
        /*  output  [01:00]                  */  .z1(),
        /*  output  [01:00]                  */  .ff1(),
        /*  output  [01:00]                  */  .ov_msb(),
        /*  output  [01:00]                  */  .co_msb(),
        /*  output  [01:00]                  */  .cmsb(),
        /*  output  [01:00]                  */  .so(),
        /*  output  [01:00]                  */  .f0_bus_stat(),
        /*  output  [01:00]                  */  .f0_blk_stat(),
        /*  output  [01:00]                  */  .f1_bus_stat(),
        /*  output  [01:00]                  */  .f1_blk_stat()
);
endmodule

`endif /* bCORDIC_V_ALREADY_INCLUDED */
