#include "stm32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "w5500_conf.h"

#include <string.h>

#define NORMAL_STATE          0
#define NEW_APP_IN_BACK       1 //there is new app in back address
#define CONFIGTOOL_FW_UP      2 //configtool update f/w in app

#define FW_VER_HIGH  		1
#define FW_VER_LOW    	0

typedef struct _CONFIG_MSG
{
  uint8 op[4];//header: FIND;SETT;FACT...
  uint8 mac[6];
  uint8 sw_ver[2];
  uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];	
  uint8 dhcp;
  uint8 debug;

  uint16 fw_len;
  uint8 state;
  
}CONFIG_MSG;

CONFIG_MSG  ConfigMsg, RecvMsg;
CONFIG_MSG  ConfigMsg, RecvMsg;
uint8 txsize[MAX_SOCK_NUM] = {4,2,2,2,2,2,2,0};
uint8 rxsize[MAX_SOCK_NUM] = {4,2,2,2,2,2,2,0};
extern uint8 MAC[6];//public buffer for DHCP, DNS, HTTP
uint8 pub_buf[1460];
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};
uint8 lip[4]={192,168,1,110};
uint8 sub[4]={255,255,255,0};
uint8 gw[4]={192,168,1,1};
uint8 dns[4]={8,8,8,8};

void set_default(void)
{  
  memcpy(ConfigMsg.lip, lip, 4);
  memcpy(ConfigMsg.sub, sub, 4);
  memcpy(ConfigMsg.gw,  gw, 4);
  memcpy(ConfigMsg.mac, mac,6);
  memcpy(ConfigMsg.dns,dns,4);

  ConfigMsg.dhcp=0;
  ConfigMsg.debug=1;
  ConfigMsg.fw_len=0;
  
  ConfigMsg.state=NORMAL_STATE;
  ConfigMsg.sw_ver[0]=FW_VER_HIGH;
  ConfigMsg.sw_ver[1]=FW_VER_LOW;
	
}

void set_network(void)
{
	//uint8 lip_t[4]={0,0,0,0};
	//uint8 sub_t[4]={0,0,0,0};
	//uint8 gw_t[4]={0,0,0,0};
	//setSHAR(mac);
  setSHAR(ConfigMsg.mac);/*配置Mac地址*/
  setSUBR(ConfigMsg.sub);/*配置子网掩码*/
  setGAR(ConfigMsg.gw);/*配置默认网关*/
  setSIPR(ConfigMsg.lip);/*配置Ip地址*/

  sysinit(txsize, rxsize); /*初始化8个socket*/
  
  setRTR(2000);/*设置溢出时间值*/
  setRCR(3);/*设置最大重新发送次数*/
	
  getSIPR (lip);
 // printf(" IP : %d.%d.%d.%d\r\n", lip[0],lip[1],lip[2],lip[3]);
  getSUBR(sub);
  //printf(" SN : %d.%d.%d.%d\r\n", sub[0],sub[1],sub[2],sub[3]);
  getGAR(gw);
 // printf(" GW : %d.%d.%d.%d\r\n", gw[0],gw[1],gw[2],gw[3]); 
}

int main(void)
{ 
	/***** 延时初始化 *****/	
	delay_init();
	
	/***** W5500初始化 *****/
	W5500_SPI_Init();
	/***** 硬重启W5500 *****/
	Reset_W5500();
	
	/***** W5500的IP信息初始化 *****/
	set_default();
	set_network();
	
  while (1)	
  {} 

}
 
 


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
