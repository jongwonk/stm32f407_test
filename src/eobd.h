/*
 * eobd.h
 *
 *  Created on: Jun 3, 2012
 *      Author: jk
 */

#ifndef EOBD_H_
#define EOBD_H_

// GPIO port
#define GPIO_CAN GPIOD

// GPIO port number
#define GPIO_Pin_CAN_Rx GPIO_Pin_0
#define GPIO_Pin_CAN_Tx GPIO_Pin_1

// CAN baudrate
#define CAN_500K 500000

// configure functions
void EOBD_Primitive_Configure(void);
void EOBD_GPIO_Configure(void);
void EOBD_NVIC_Configure(void);
void EOBD_CAN_Configure(int baudrate);
void EOBD_CAN_Filter_Configure(void);


#endif /* EOBD_H_ */
