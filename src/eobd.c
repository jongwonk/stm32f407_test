/*
 * eobd.c
 *
 *  Created on: Jun 3, 2012
 *      Author: jk
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "utils.h"
#include "debug.h"
#include "stm32f4_discovery_lis302dl.h"

#include "eobd.h"

CanTxMsg TxMessage;

void EOBD_GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure CAN pin: RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_Rx;
	GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN1;
	GPIO_Init(GPIO_CAN,&GPIO_InitStructure);

	// Configure CAN pin: TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_Tx;
	GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN1;
	GPIO_Init(GPIO_CAN,&GPIO_InitStructure);

}

void EOBD_NVIC_Configure(void)
{

}

void EOBD_CAN_Filter_Configure(void)
{
	CAN_FilterInitTypeDef	CAN_FilterInitStruct;

	CAN_FilterInitStruct.CAN_FilterNumber 			= 0;
	CAN_FilterInitStruct.CAN_FilterMode 			= CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale 			= CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh 			= 0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow 			= 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh 		= 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow 		= 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment	= 0;
	CAN_FilterInitStruct.CAN_FilterActivation 		= ENABLE;

	CAN_FilterInit(&CAN_FilterInitStruct);
}

void EOBD_CAN_Configure(int baudrate)
{
	CAN_InitTypeDef	CAN_InitStructure;

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_SJW		= CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1		= CAN_BS1_10tq;
	CAN_InitStructure.CAN_BS2		= CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 2;

	CAN_Init(CAN1,&CAN_InitStructure);
}

void EOBD_Configure(void)
{
	EOBD_GPIO_Configure();
	EOBD_NVIC_Configure();
	EOBD_CAN_Configure(CAN_500K);
	EOBD_CAN_Filter_Configure();
}

portTASK_FUNCTION( vEOBDTask, pvParameters )
{

	EOBD_Configure();

	vDebugString( "EOBD task started.\r\n");

	while(1)
	{


	}

}
