/*******************************************************************************
* FILENAME: cyfitter_cfg.c
* PSoC Creator 2.2 Service Pack 1
*
* Description:
* This file is automatically generated by PSoC Creator with device 
* initialization code.  Except for the user defined sections in
* CyClockStartupError(), this file should not be modified.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include <cytypes.h>
#include <cydevice_trm.h>
#include <cyfitter.h>
#include <CyLib.h>
#include <cyfitter_cfg.h>

/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u

#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__ ((unused))
#endif
static void CyClockStartupError(uint8 errorCode);
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__ ((unused))
#endif
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}
#endif


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
	#define CYPACKED __attribute__ ((packed))
	#define CYALIGNED __attribute__ ((aligned))
	
	#if defined(__ARMCC_VERSION)
		#define CY_CFG_MEMORY_BARRIER() __memory_changed()
	#else
		#define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
	#endif
	

	__attribute__ ((unused))
	static void CYMEMZERO(void *s, size_t n);
	__attribute__ ((unused))
	static void CYMEMZERO(void *s, size_t n)
	{
		(void)memset(s, 0, n);
	}
	__attribute__ ((unused))
	static void CYCONFIGCPY(void *dest, const void *src, size_t n);
	__attribute__ ((unused))
	static void CYCONFIGCPY(void *dest, const void *src, size_t n)
	{
		(void)memcpy(dest, src, n);
	}
	__attribute__ ((unused))
	static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
	__attribute__ ((unused))
	static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
	{
		(void)memcpy(dest, src, n);
	}
#else
	#error Unsupported toolchain
#endif

#define CY_CFG_BASE_ADDR_COUNT 9u
typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_XTND_REG8((void CYFAR *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}

/*******************************************************************************
* Function Name: SetIMOBGTrims
********************************************************************************
*
* Summary:
*   Performs the configuration of the bandgap trim for the IMO based on the given
*   expected frequency of the IMO. 
*
* Parameters:
*   imoFreq: Frequency for operation of the IMO.
*
* Return:
*   void
*
*******************************************************************************/
static void SetIMOBGTrims(uint8 imoFreq);
__attribute__ ((section(".psocinit")))
static void SetIMOBGTrims(uint8 imoFreq)
{
	typedef struct {
		uint8 freq;
		uint8 absolute;
		uint8 tmpco;
	} CYPACKED imo_trim_entry_t;

	typedef struct {
		imo_trim_entry_t entries[4];
	} CYPACKED imo_trim_table_t;

	static const imo_trim_table_t *imo_trim_table = (const imo_trim_table_t *)CYREG_SFLASH_IMO_MAXF0;
	static const uint32 IMO_TRIM_FREQ_TABLE_ENTRIES = sizeof(imo_trim_table->entries)/sizeof(imo_trim_table->entries[0]);

	int32 imoTrimIndex;
	uint8 setAbs;
	uint8 setTmpCo;

	/* Traverse the SFLASH IMO frequency-dependent bandgap trim table to
	 * locate the appropriate value to trim the IMO bandgap based on freq. */
	setAbs = CY_GET_REG8((const void *)CYREG_SFLASH_IMO_ABS4);
	setTmpCo = CY_GET_REG8((const void *)CYREG_SFLASH_IMO_TMPCO4);
	for (imoTrimIndex = ((int32)IMO_TRIM_FREQ_TABLE_ENTRIES)-1;
	     (imoTrimIndex >= 0) && (imoFreq <= imo_trim_table->entries[imoTrimIndex].freq);
	     imoTrimIndex--)
	{
		setAbs = imo_trim_table->entries[imoTrimIndex].absolute;
		setTmpCo = imo_trim_table->entries[imoTrimIndex].tmpco;
	}

	CY_SET_REG32((void *)CYREG_PWR_BG_TRIM4, setAbs);
	CY_SET_REG32((void *)CYREG_PWR_BG_TRIM5, setTmpCo);
}



/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*   Performs the initialization of all of the clocks in the device based on the
*   settings in the Clock tab of the DWR.  This includes enabling the requested
*   clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void ClockSetup(void);
__attribute__ ((section(".psocinit")))
static void ClockSetup(void)
{
	/* Enable HALF_EN before trimming for the flash accelerator. */
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_SELECT), (CY_GET_REG32((void *)CYREG_CLK_SELECT) | 0x00040000u));

	/* Trim IMO BG based on desired frequency. */
	SetIMOBGTrims(48u);

	/* Going faster than 24MHz, so update trim value then adjust to new clock speed. */
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_IMO_TRIM1), (CY_GET_REG8((void *)CYREG_SFLASH_IMO_TRIM45)));
	CyDelayUs(5u);
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_IMO_TRIM2), (53u));

	/* CYDEV_CLK_SELECT00 Starting address: CYDEV_CLK_SELECT00 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_SELECT12), 0x00000010u);

	/* CYDEV_CLK_IMO_CONFIG Starting address: CYDEV_CLK_IMO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_IMO_CONFIG), 0x80000000u);

	/* CYDEV_CLK_ILO_CONFIG Starting address: CYDEV_CLK_ILO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_ILO_CONFIG), 0x80000000u);

	/* CYDEV_CLK_SELECT Starting address: CYDEV_CLK_SELECT */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_SELECT), 0x00040000u);

	/* CYDEV_CLK_DIVIDER_A00 Starting address: CYDEV_CLK_DIVIDER_A00 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_DIVIDER_A00), 0x80000003u);

}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
}

#define CY_AMUX_UNUSED CYREG_SAR_MUX_SWITCH_STATUS


/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/
__attribute__ ((section(".psocinit")))
void cyfitter_cfg(void)
{
	/* Disable interrupts by default. Let user enable if/when they want. */
	CyGlobalIntDisable;

	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x400F0001u, /* Base address: 0x400F0000 Count: 1 */
			0x400F3037u, /* Base address: 0x400F3000 Count: 55 */
			0x400F3126u, /* Base address: 0x400F3100 Count: 38 */
			0x400F3225u, /* Base address: 0x400F3200 Count: 37 */
			0x400F3343u, /* Base address: 0x400F3300 Count: 67 */
			0x400F4002u, /* Base address: 0x400F4000 Count: 2 */
			0x400F4102u, /* Base address: 0x400F4100 Count: 2 */
			0x400F4308u, /* Base address: 0x400F4300 Count: 8 */
			0x400F6002u, /* Base address: 0x400F6000 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x81u, 0x11u},
			{0x00u, 0x02u},
			{0x02u, 0x0Du},
			{0x04u, 0x09u},
			{0x06u, 0x06u},
			{0x28u, 0x05u},
			{0x2Au, 0x0Au},
			{0x2Cu, 0x03u},
			{0x2Eu, 0x0Cu},
			{0x30u, 0x0Fu},
			{0x3Au, 0x02u},
			{0x54u, 0x40u},
			{0x58u, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Fu, 0x01u},
			{0x80u, 0x44u},
			{0x81u, 0x04u},
			{0x82u, 0x08u},
			{0x85u, 0x07u},
			{0x88u, 0x37u},
			{0x8Bu, 0x04u},
			{0x8Cu, 0x08u},
			{0x8Eu, 0x34u},
			{0x91u, 0x04u},
			{0x92u, 0x01u},
			{0x94u, 0x08u},
			{0x95u, 0x01u},
			{0x96u, 0xE0u},
			{0x97u, 0x06u},
			{0x99u, 0x01u},
			{0x9Au, 0x80u},
			{0x9Bu, 0x06u},
			{0x9Cu, 0x37u},
			{0x9Fu, 0x03u},
			{0xA0u, 0x34u},
			{0xA1u, 0x04u},
			{0xA2u, 0x03u},
			{0xA4u, 0x37u},
			{0xA5u, 0x04u},
			{0xA8u, 0x08u},
			{0xAAu, 0xD0u},
			{0xACu, 0x37u},
			{0xB0u, 0x3Cu},
			{0xB2u, 0x01u},
			{0xB4u, 0xC0u},
			{0xB5u, 0x07u},
			{0xB6u, 0x02u},
			{0xBAu, 0x02u},
			{0xBBu, 0x20u},
			{0xBEu, 0x44u},
			{0xD6u, 0x02u},
			{0xD7u, 0x2Cu},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDBu, 0x04u},
			{0xDFu, 0x01u},
			{0x00u, 0x04u},
			{0x01u, 0x02u},
			{0x04u, 0x40u},
			{0x07u, 0x44u},
			{0x0Eu, 0x25u},
			{0x0Fu, 0x80u},
			{0x12u, 0x02u},
			{0x13u, 0x20u},
			{0x14u, 0x01u},
			{0x15u, 0x04u},
			{0x16u, 0x22u},
			{0x17u, 0x21u},
			{0x19u, 0x02u},
			{0x1Cu, 0x52u},
			{0x1Du, 0x80u},
			{0x1Eu, 0x06u},
			{0x25u, 0x20u},
			{0x27u, 0x08u},
			{0x2Du, 0x04u},
			{0x2Eu, 0x01u},
			{0x37u, 0x69u},
			{0x3Cu, 0x04u},
			{0x3Eu, 0x12u},
			{0x60u, 0x10u},
			{0x62u, 0x40u},
			{0x63u, 0x08u},
			{0x65u, 0x02u},
			{0x6Du, 0x04u},
			{0x6Eu, 0x11u},
			{0x6Fu, 0x15u},
			{0x76u, 0x02u},
			{0xC0u, 0xDCu},
			{0xC2u, 0xF0u},
			{0xC4u, 0xF3u},
			{0xCAu, 0xC0u},
			{0xCCu, 0xF0u},
			{0xCEu, 0xE0u},
			{0xD8u, 0x8Eu},
			{0x84u, 0x04u},
			{0x86u, 0x09u},
			{0x88u, 0x0Eu},
			{0x94u, 0x05u},
			{0x96u, 0x0Au},
			{0xACu, 0x03u},
			{0xAEu, 0x04u},
			{0xB2u, 0x0Fu},
			{0xB6u, 0x0Fu},
			{0xB8u, 0x88u},
			{0xC0u, 0x32u},
			{0xC1u, 0x01u},
			{0xC2u, 0x50u},
			{0xC7u, 0xB0u},
			{0xC8u, 0x20u},
			{0xC9u, 0x01u},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCCu, 0x23u},
			{0xCEu, 0x80u},
			{0xCFu, 0x08u},
			{0xD0u, 0x04u},
			{0xD8u, 0x04u},
			{0xDAu, 0x04u},
			{0xDFu, 0x01u},
			{0xE2u, 0xC0u},
			{0xE4u, 0x40u},
			{0xE5u, 0x01u},
			{0xE6u, 0x10u},
			{0xE7u, 0x11u},
			{0xE8u, 0xC0u},
			{0xE9u, 0x01u},
			{0xEBu, 0x11u},
			{0xECu, 0x40u},
			{0xEDu, 0x01u},
			{0xEEu, 0x40u},
			{0xEFu, 0x01u},
			{0x00u, 0x14u},
			{0x05u, 0x10u},
			{0x07u, 0x61u},
			{0x09u, 0x08u},
			{0x0Cu, 0x01u},
			{0x0Du, 0x20u},
			{0x0Eu, 0x29u},
			{0x12u, 0x02u},
			{0x15u, 0x02u},
			{0x17u, 0x14u},
			{0x18u, 0x11u},
			{0x1Du, 0x10u},
			{0x1Fu, 0x41u},
			{0x24u, 0x04u},
			{0x25u, 0x08u},
			{0x26u, 0x12u},
			{0x27u, 0x68u},
			{0x28u, 0x20u},
			{0x2Bu, 0x20u},
			{0x2Cu, 0x01u},
			{0x2Eu, 0x21u},
			{0x2Fu, 0x20u},
			{0x36u, 0x04u},
			{0x37u, 0x61u},
			{0x3Bu, 0xC0u},
			{0x3Cu, 0x04u},
			{0x3Eu, 0x10u},
			{0x40u, 0x04u},
			{0x41u, 0x08u},
			{0x43u, 0x40u},
			{0x45u, 0x80u},
			{0x47u, 0x28u},
			{0x48u, 0x01u},
			{0x4Eu, 0x02u},
			{0x4Fu, 0x04u},
			{0x51u, 0x01u},
			{0x56u, 0x40u},
			{0x57u, 0x64u},
			{0x5Cu, 0x20u},
			{0x5Eu, 0x02u},
			{0x5Fu, 0x44u},
			{0x67u, 0x02u},
			{0x8Cu, 0x01u},
			{0x8Du, 0x10u},
			{0x92u, 0x11u},
			{0x94u, 0x04u},
			{0x98u, 0x01u},
			{0x9Au, 0x20u},
			{0x9Bu, 0x15u},
			{0x9Eu, 0x40u},
			{0x9Fu, 0x60u},
			{0xA0u, 0x01u},
			{0xA1u, 0xA0u},
			{0xA2u, 0x01u},
			{0xA6u, 0x12u},
			{0xA7u, 0x44u},
			{0xAEu, 0x04u},
			{0xC0u, 0xF6u},
			{0xC2u, 0xF4u},
			{0xC4u, 0x71u},
			{0xCAu, 0xF0u},
			{0xCCu, 0xF0u},
			{0xCEu, 0x60u},
			{0xD0u, 0x7Eu},
			{0xD2u, 0x18u},
			{0xD6u, 0xF0u},
			{0xD8u, 0x10u},
			{0x62u, 0x02u},
			{0xD8u, 0x40u},
			{0xAEu, 0x02u},
			{0xEAu, 0x40u},
			{0x57u, 0x04u},
			{0x61u, 0x02u},
			{0x8Du, 0x02u},
			{0x8Fu, 0x04u},
			{0xD4u, 0x02u},
			{0xD8u, 0x02u},
			{0xE0u, 0x02u},
			{0xE4u, 0x08u},
			{0x10u, 0x01u},
			{0x11u, 0x01u},
		};



		typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED cfg_memset_t;


		typedef struct {
			void CYFAR *dest;
			const void CYCODE *src;
			uint16 size;
		} CYPACKED cfg_memcpy_t;

		static const cfg_memset_t CYCODE cfg_memset_list [] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_P0_U0_BASE), 512u},
			{(void CYFAR *)(CYDEV_UDB_P1_U1_BASE), 384u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), 1024u},
		};

		/* UDB_0_0_1_CONFIG Address: CYDEV_UDB_P1_U0_BASE Size (bytes): 128 */
		static const uint8 CYCODE BS_UDB_0_0_1_CONFIG_VAL[] = {
			0x07u, 0x00u, 0x38u, 0x00u, 0x0Au, 0xD4u, 0x10u, 0x28u, 0x01u, 0x20u, 0x00u, 0x90u, 0x04u, 0x00u, 0x00u, 0x00u, 
			0x04u, 0x90u, 0x00u, 0x20u, 0x56u, 0x90u, 0x09u, 0x00u, 0x04u, 0x0Eu, 0x00u, 0x91u, 0x04u, 0x39u, 0x00u, 0x42u, 
			0x60u, 0xB0u, 0x00u, 0x00u, 0x00u, 0x24u, 0x04u, 0x00u, 0x04u, 0x90u, 0x00u, 0x20u, 0x00u, 0xB0u, 0x00u, 0x00u, 
			0x02u, 0x38u, 0x00u, 0xE8u, 0x7Fu, 0x07u, 0x08u, 0x00u, 0x20u, 0x00u, 0x00u, 0x28u, 0x00u, 0x00u, 0x41u, 0x00u, 
			0x23u, 0x05u, 0x10u, 0x00u, 0x00u, 0x0Eu, 0xCDu, 0x0Fu, 0x1Du, 0xFFu, 0xFFu, 0xFFu, 0x02u, 0x00u, 0x70u, 0x08u, 
			0x04u, 0x00u, 0x00u, 0x00u, 0x09u, 0x00u, 0x00u, 0x00u, 0x04u, 0x04u, 0x04u, 0x04u, 0x00u, 0x00u, 0x00u, 0x01u, 
			0x00u, 0x00u, 0xC0u, 0x00u, 0x40u, 0x01u, 0x10u, 0x11u, 0xC0u, 0x01u, 0x00u, 0x11u, 0x40u, 0x01u, 0x40u, 0x01u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

		static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
			/* dest, src, size */
			{(void CYFAR *)(CYDEV_UDB_P1_U0_BASE), BS_UDB_0_0_1_CONFIG_VAL, 128u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, (uint32)(ms->size));
		}

		/* Copy device configuration data into registers */
		for (i = 0u; i < (sizeof(cfg_memcpy_list)/sizeof(cfg_memcpy_list[0])); i++)
		{
			const cfg_memcpy_t CYCODE * CYDATA mc = &cfg_memcpy_list[i];
			void * CYDATA destPtr = mc->dest;
			const void CYCODE * CYDATA srcPtr = mc->src;
			uint16 CYDATA numBytes = mc->size;
			CYCONFIGCPYCODE(destPtr, srcPtr, numBytes);
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* Perform normal device configuration. Order is not critical for these items. */
		CY_SET_XTND_REG16((void CYFAR *)(CYREG_UDB_BCTL0_DRV), 0x0100u);
		CY_SET_XTND_REG8((void CYFAR *)(CYREG_UDB_BCTL0_DRV + 0x2u), 0x01u);

		/* HSIOM Starting address: CYDEV_HSIOM_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_HSIOM_BASE), 0x03000000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL3), 0x0300EE03u);

		/* IOPINS0_0 Starting address: CYDEV_PRT0_DR */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_PRT0_DR), 0x000000C1u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_PRT0_PC), 0x00F80007u);

		/* IOPINS0_2 Starting address: CYDEV_PRT2_DR */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_PRT2_PC), 0x00180000u);

		/* IOPINS0_3 Starting address: CYDEV_PRT3_DR */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_PRT3_DR), 0x000000F1u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_PRT3_PC), 0x00FBED86u);

		/* UDB_PA_0 Starting address: CYDEV_UDB_PA0_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA0_BASE), 0x00990000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA0_CFG4), 0x80020000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA0_CFG8), 0x20000000u);

		/* UDB_PA_2 Starting address: CYDEV_UDB_PA2_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA2_BASE), 0x00990000u);

		/* UDB_PA_3 Starting address: CYDEV_UDB_PA3_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA3_BASE), 0x00990000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA3_CFG4), 0x8A000000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA3_CFG8), 0x20010000u);

		/* Enable digital routing */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x06u);
	}

	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
