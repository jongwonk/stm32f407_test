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

#define mainEOBDPRIM_TASK_PRIORITY  ( tskIDLE_PRIORITY + 3)
#define QLENGTH 4 // queue length

typedef struct EOBDMessage
{
	portCHAR uMsgID;
	unsigned short uLength;
	portCHAR Data[256];
}EOBDMSG;

xQueueHandle xEOBDQueueHandleToPrim;
xQueueHandle xEOBDQueueHandle;

xTaskHandle hEOBDPrimitiveTask;

portBASE_TYPE SendRcvd(EOBDMSG *pMsgA, EOBDMSG *pMsgB, portTickType timeout);

portTASK_FUNCTION_PROTO( vEOBDPrimitiveTask, pvParameters );

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

void EOBD_Primitive_Configure(void)
{
	EOBD_GPIO_Configure();
	EOBD_NVIC_Configure();
	EOBD_CAN_Configure(CAN_500K);
	EOBD_CAN_Filter_Configure();
}

portTASK_FUNCTION( vEOBDTask, pvParameters )
{
	EOBDMSG RxMsg;
	EOBDMSG TxMsg;

	xEOBDQueueHandleToPrim = xQueueCreate(QLENGTH,sizeof(struct EOBSMessage *));
	if(xEOBDQueueHandleToPrim == 0)
	{
		vDebugPrintf( "xEOBDQueueHandle == 0\r\n");
	}

    xTaskCreate( vEOBDPrimitiveTask, (signed char *) "EOBDPrim", configMINIMAL_STACK_SIZE,
            NULL, mainEOBDPRIM_TASK_PRIORITY, &hEOBDPrimitiveTask );

	vDebugString( "EOBD task started.\r\n");

	while(1)
	{
		if(SendRcvd(&TxMsg, &RxMsg, (portTickType)50))
		{

		}
		else {
			vDebugString( "SendRcvd timeout\r\n");
		}
	}

}

portTASK_FUNCTION( vEOBDPrimitiveTask, pvParameters )
{
	EOBDMSG TxMsg;
	EOBDMSG RxMsg;

	EOBD_Primitive_Configure();

	xEOBDQueueHandle = xQueueCreate(QLENGTH,sizeof(struct EOBSMessage *));
	if(xEOBDQueueHandle == 0)
	{
		vDebugPrintf( "xEOBDQueueHandle == 0\r\n");
	}


	vDebugString( "EOBD primitive task started.\r\n");

	while(1)
	{
		if(xQueueReceive(xEOBDQueueHandleToPrim,&TxMsg , portMAX_DELAY))
		{


			xQueueSend(xEOBDQueueHandle,&RxMsg, (portTickType)0);
		}
	}

}

portBASE_TYPE SendRcvd(EOBDMSG *pMsgA,EOBDMSG *pMsgB, portTickType timeout)
{
	xQueueSend(xEOBDQueueHandleToPrim,(void*)pMsgA, (portTickType)0);
	return xQueueReceive(xEOBDQueueHandleToPrim,(void*)pMsgB, timeout);
}


