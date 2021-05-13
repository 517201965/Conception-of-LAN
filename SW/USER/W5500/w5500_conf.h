#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f10x.h"
#include "systick.h"
#include "w5500.h"

#define W5500_SCS		GPIO_Pin_12
#define W5500_SCLK	GPIO_Pin_13
#define W5500_MISO	GPIO_Pin_14
#define W5500_MOSI	GPIO_Pin_15

#define W5500_RST		GPIO_Pin_9
#define W5500_INT		GPIO_Pin_8
 
#define W5500_GIPOx	GPIOB


#define HIGH	           	 		1
#define LOW		             		0

void W5500_SPI_Init(void);
void Reset_W5500(void);
void W5500_CS(uint8_t val);
uint8_t SPI2_SendByte(uint8_t byte);

#endif

