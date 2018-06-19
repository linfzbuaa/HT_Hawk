/******************* (C) COPYRIGHT 2016 Hentotech Team ***************************
 * 文件名   ：NRF24l01.c
 * 描述     ：NRF24l01配置
 * 实验平台 ：HT-Hawk开源飞控
 * 库版本   ：ST3.5.0
 * 作者     ：Hentotech Team 
 * 官网     ：http://www.hentotech.com 
 * 论坛     ：http://bbs.hentotech.com
 * 商城     ：http://hentotech.taobao.com   
*********************************************************************************/

#include "NRF24L01.h"
#include "include.h"
extern uint8 rxdata1;
  
u8  TX_ADDRESS[TX_ADR_WIDTH]= {0xE1,0xE2,0xE3,0xE4,0xE5};	//本地地址
u8  RX_ADDRESS[RX_ADR_WIDTH]= {0xE1,0xE2,0xE3,0xE4,0xE5};	//接收地址
u8 SwitchtoRX,NowMode;

u8 NRFwatchdog;
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Init
**功能 : 初始化24L01的IO口
**输入 : None
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
void NRF24L01_Init(void)
{ 	
	NRF24L01_CE_L();
	NRF24L01_CSN_H();
	SPI3_Config();
	EXTI_NRFinit();//接收中断配置	
}
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Check
**功能 : 检测24L01是否存在
**输入 : None
**輸出 : 0，成功;1，失败	
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;  	 
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	rxdata1=buf[1];
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 0;//检测24L01错误	
	return 1;		 //检测到24L01
}	 	 
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Write_Reg
**功能 : SPI写寄存器
**输入 : reg:指定寄存器地址  value:写入的值
**輸出 : 返回状态值
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	  u8 status;	
   	NRF24L01_CSN_L();                 //使能SPI传输
  	status =SPI3_ReadWriteByte(reg);//发送寄存器号 
  	SPI3_ReadWriteByte(value);      //写入寄存器的值
  	NRF24L01_CSN_H();                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Read_Reg
**功能 : 读取SPI寄存器值
**输入 : reg:要读的寄存器
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_Read_Reg(u8 reg)
{
	  u8 reg_val;	    
 	  NRF24L01_CSN_L();;          //使能SPI传输		
  	SPI3_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI3_ReadWriteByte(0XFF);//读取寄存器内容
  	NRF24L01_CSN_H();;          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}	
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Read_Buf
**功能 : 在指定位置读出指定长度的数据
**输入 : reg:寄存器(位置)  *pBuf:数据指针  len:数据长度
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	  u8 status,u8_ctr;	       
  	NRF24L01_CSN_L();           //使能SPI传输
  	status=SPI3_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	  for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI3_ReadWriteByte(0XFF);//读出数据
  	NRF24L01_CSN_H();       //关闭SPI传输
  	return status;        //返回读到的状态值
}
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Write_Buf
**功能 : 在指定位置写指定长度的数据
**输入 : reg:寄存器(位置)  *pBuf:数据指针   len:数据长度
**輸出 : 此次读到的状态寄存器值
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	  u8 status,u8_ctr;	    
 	  NRF24L01_CSN_L();          //使能SPI传输
  	status = SPI3_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI3_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF24L01_CSN_H();       //关闭SPI传输
  	return status;          //返回读到的状态值
}				   
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_TxPacket
**功能 : 启动NRF24L01发送一次数据
**输入 : txbuf:待发送数据首地址
**輸出 : 发送完成状况
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_16);//spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF24L01_CE_L();
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节  
  NRF24L01_CE_H();	
	delay(2);  //改为延时，而不是一直等待
	//while(NRF24L01_IRQ!=0);//等待发送完成
		   
	//while(!(sta&TX_OK))
		sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值;
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_RxPacket
**功能 : 启动NRF24L01发送一次数据
**输入 : txbuf:待发送数据首地址
**輸出 : 0，接收完成；其他，错误代码
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI3_SetSpeed(SPI_BaudRatePrescaler_32); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
			GPIO_SetBits(GPIOE, GPIO_Pin_15 );	
		return 0; 
	}
	return 1;//没收到任何数据
}					    
/*====================================================================================================*/
/*====================================================================================================*
**函数 : NRF24L01_Mode
**功能 : 设置2401工作模式
**输入 : None
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
void NRF24L01_Mode(u8 model)
{
	 NRF24L01_CE_L();     
   NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
   NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

   NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);     //使能通道0的自动应答    00表示不应答
   NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); //使能通道0的接收地址  为0，不接收
   NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 不重发
   NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,0x73);       //设置RF通道为40设置信道工作为2.5GHZ，收发必须一致
   NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启  
/////////////////////////////////////////////////////////
	if(model==1)				//RX
	{
		NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 
		NRF24L01_Write_Reg(WRITE_REG_NRF + CONFIG, 0x0f);   		 // IRQ收发完成中断开启,16位CRC,主接收
	}
	else if(model==2)		//TX
	{
		NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 
		NRF24L01_Write_Reg(WRITE_REG_NRF + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
	}
	else if(model==3)		//RX2
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		NRF24L01_Write_Reg(WRITE_REG_NRF + CONFIG, 0x3f);   		 // IRQ收发完成中断开启,16位CRC,主接收0x0f
																													//高位为3表示屏蔽发送中断
		
		SPI3_ReadWriteByte(0x50);
		SPI3_ReadWriteByte(0x73);
		NRF24L01_Write_Reg(WRITE_REG_NRF+0x1c,0x01);
		NRF24L01_Write_Reg(WRITE_REG_NRF+0x1d,0x07);
	}
	else								//TX2
	{
		NRF24L01_Write_Reg(WRITE_REG_NRF + CONFIG, 0x2e);   		 // IRQ收发完成中断开启,16位CRC,主发送0x0e
		NRF24L01_Write_Reg(FLUSH_TX,0xff);											//高位bit4MAX_RT bit5 为1屏蔽TX_DS bit6 RX_DS
		NRF24L01_Write_Reg(FLUSH_RX,0xff);											//对于飞控，发送模式2e才是好用的
		
		SPI3_ReadWriteByte(0x50);
		SPI3_ReadWriteByte(0x73);
		NRF24L01_Write_Reg(WRITE_REG_NRF+0x1c,0x01);
		NRF24L01_Write_Reg(WRITE_REG_NRF+0x1d,0x07);
	}
	NRF24L01_CE_H(); 
}

/*====================================================================================================*
**函数 : NRF24L01_Txframes
**功能 : 启动NRF24L01发送一帧数据
**输入 : txbuf:待发送数据首地址，packlen数据包长度
**輸出 : 发送完成状况
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
u8 tempbuf[64];
u8 NRF24L01_Txframes(u8 *txbuf,u8 packlen)
{
	u8 txresult;
	/*开始发送处理后的帧*/
	txresult=NRF24L01_TxPacket(txbuf);
  if(packlen>32)
	  txresult=NRF24L01_TxPacket(&txbuf[32]);

  if(packlen>64)
	  txresult=NRF24L01_TxPacket(&txbuf[64]);

  if(packlen>96)txresult=!TX_OK;//数据长度大于96暂且认为是错误帧

	if(txresult==TX_OK)//发送完成
	{
		return 1;
	}
	return 0;//其他原因发送失败
}

void EXTI_NRFinit(void)
{
	/* 配置NRF接收中断 */
		EXTI_InitTypeDef EXTI_InitStructure;
		/* 配置外部中断线4连接到PD4 */
		EXTI_ClearITPendingBit(EXTI_Line4);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);

		EXTI_InitStructure.EXTI_Line = EXTI_Line4; /*  */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
}
