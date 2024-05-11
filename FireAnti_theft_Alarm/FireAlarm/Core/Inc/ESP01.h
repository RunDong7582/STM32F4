#ifndef __ESP01_H__
#define __ESP01_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include <cmsis_os.h>
#include <cmsis_os2.h>

// �޸���������Ϊ��Ĺ����ȵ����ơ�����
#define AP_NAME		"CXR"
#define AP_PSW		"66675821"

// �޸���������Ϊ����ȵ����ڵ��Ի��ֻ�IP��ַ��TCP�������˿�
#define TCP_SERVER 		"192.168.137.1"
#define TCP_PORT 6789

/* ������USART_RX_DATA�������Ͷ���ŵ�usart.hͷ�ļ�
#define RXBUF_SIZE 	512
typedef struct {
	uint16_t rx_len;
	uint8_t rx_buf[RXBUF_SIZE - 2];
} USART_RX_DATA;
*/

/* �����´��븴�Ƶ����������н��н������ݴ���
if (EspRxDataOk())
{
	// �������ݴ���
}
*/

typedef struct {
	USART_RX_DATA rxdata;
	uint8_t bAtOK;
	uint8_t bConnect;
	char strAPName[20];
	char strESPName[20];
	uint32_t Sendbytes;
	uint32_t Recvbytes;
} ESP01;

extern ESP01 g_esp01;
uint8_t EspRxDataOk(void);		// �������ݴ���
void SendEspStr(char *str);		// �����ַ���
void InitEsp01(UART_HandleTypeDef* pUartHandle);	// ��ʼ��ESP01����

#ifdef __cplusplus
}
#endif

#endif /* __ESP01_H__ */
