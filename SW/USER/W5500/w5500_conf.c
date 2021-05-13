#include "w5500_conf.h"


void W5500_SPI_Init(void)
{
	// 初始化GPIO管脚和SPI的参数设置：建立SPI和GPIO的初始化结构体
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// APB1、2外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);		// 开启SPI2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO , ENABLE);	// 开启GPIOB、AFIO时钟

	// 配置SCS、RST
	GPIO_InitStructure.GPIO_Pin = W5500_SCS | W5500_RST; 								// 选定PB12进行后续设置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// PB12通信速度设为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				// PB12设为推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);									// PB12完成配置
	GPIO_SetBits(W5500_GIPOx, W5500_SCS);										// 配置FLASH的片选信号线，并设为高电平，也就是不选中FLASH
	GPIO_SetBits(W5500_GIPOx, W5500_RST);										// Reset信号，设为高电平(低电平有效)
	
	// 配置INT
  GPIO_InitStructure.GPIO_Pin = W5500_INT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// 上拉输入
  GPIO_Init(W5500_GIPOx, &GPIO_InitStructure);
	
	// 配置SPI2的SCK, MISO and MOSI 
	GPIO_InitStructure.GPIO_Pin = W5500_SCLK | W5500_MISO | W5500_MOSI;		// 选定PB13/14/15进行后续设置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// PB13/14/15通信速度设为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					// PB13/14/15设为复用推挽输出，输入输出的方向完全由内部控制，不需要程序处理
	GPIO_Init(W5500_GIPOx, &GPIO_InitStructure);						// PB13/14/15完成配置

	// SPI配置
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		// SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			// 设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				// 选择了串行时钟的稳态:时钟低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			// 数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;					// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		// 定义波特率预分频的值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 指定数据传输从MSB位还是LSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;					// CRC值计算的多项式

	SPI_Init(SPI2, &SPI_InitStructure);								// 利用SPI结构体初始化函数初始化SPI结构体 
	SPI_Cmd(SPI2, ENABLE);														// 使能SPI2外设
}

void Reset_W5500(void)
{
  GPIO_ResetBits(W5500_GIPOx, W5500_RST);
  delay_ms(2);  
  GPIO_SetBits(W5500_GIPOx, W5500_RST);
  delay_us(1600);
}

void W5500_CS(uint8_t val)
{
	if (val == LOW) 
	{
   		GPIO_ResetBits(W5500_GIPOx, W5500_SCS); 	// GPIOB的WIZ_SCS（GPIO_Pin_12）引脚清零拉低
	}
	else if (val == HIGH)
	{
   		GPIO_SetBits(W5500_GIPOx, W5500_SCS); 		// GPIOB的WIZ_SCS（GPIO_Pin_12）引脚置1拉高
	}
}

uint8_t SPI2_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  	// 等待发送区空
			 
	SPI_I2S_SendData(SPI2, byte);									   									// 发送一个字节
				
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	// 等待接收完一个字节
				
	return SPI_I2S_ReceiveData(SPI2);								   								// 返回收到的数据 
}

