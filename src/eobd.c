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


portTASK_FUNCTION( vEOBDTask, pvParameters )
{



	vDebugString( "EOBD task started.\r\n");

	while(1)
	{


	}

}
