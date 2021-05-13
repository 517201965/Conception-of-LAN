#include "w5500_conf.h"


void W5500_SPI_Init(void)
{
	// ��ʼ��GPIO�ܽź�SPI�Ĳ������ã�����SPI��GPIO�ĳ�ʼ���ṹ��
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// APB1��2����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);		// ����SPI2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO , ENABLE);	// ����GPIOB��AFIOʱ��

	// ����SCS��RST
	GPIO_InitStructure.GPIO_Pin = W5500_SCS | W5500_RST; 								// ѡ��PB12���к�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// PB12ͨ���ٶ���Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				// PB12��Ϊ�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);									// PB12�������
	GPIO_SetBits(W5500_GIPOx, W5500_SCS);										// ����FLASH��Ƭѡ�ź��ߣ�����Ϊ�ߵ�ƽ��Ҳ���ǲ�ѡ��FLASH
	GPIO_SetBits(W5500_GIPOx, W5500_RST);										// Reset�źţ���Ϊ�ߵ�ƽ(�͵�ƽ��Ч)
	
	// ����INT
  GPIO_InitStructure.GPIO_Pin = W5500_INT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// ��������
  GPIO_Init(W5500_GIPOx, &GPIO_InitStructure);
	
	// ����SPI2��SCK, MISO and MOSI 
	GPIO_InitStructure.GPIO_Pin = W5500_SCLK | W5500_MISO | W5500_MOSI;		// ѡ��PB13/14/15���к�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// PB13/14/15ͨ���ٶ���Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					// PB13/14/15��Ϊ���������������������ķ�����ȫ���ڲ����ƣ�����Ҫ������
	GPIO_Init(W5500_GIPOx, &GPIO_InitStructure);						// PB13/14/15�������

	// SPI����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		// SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			// ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				// ѡ���˴���ʱ�ӵ���̬:ʱ�ӵ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			// ���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;					// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		// ���岨����Ԥ��Ƶ��ֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ָ�����ݴ����MSBλ����LSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;					// CRCֵ����Ķ���ʽ

	SPI_Init(SPI2, &SPI_InitStructure);								// ����SPI�ṹ���ʼ��������ʼ��SPI�ṹ�� 
	SPI_Cmd(SPI2, ENABLE);														// ʹ��SPI2����
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
   		GPIO_ResetBits(W5500_GIPOx, W5500_SCS); 	// GPIOB��WIZ_SCS��GPIO_Pin_12��������������
	}
	else if (val == HIGH)
	{
   		GPIO_SetBits(W5500_GIPOx, W5500_SCS); 		// GPIOB��WIZ_SCS��GPIO_Pin_12��������1����
	}
}

uint8_t SPI2_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  	// �ȴ���������
			 
	SPI_I2S_SendData(SPI2, byte);									   									// ����һ���ֽ�
				
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	// �ȴ�������һ���ֽ�
				
	return SPI_I2S_ReceiveData(SPI2);								   								// �����յ������� 
}

