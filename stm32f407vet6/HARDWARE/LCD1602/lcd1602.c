#include "lcd1602.h"

unsigned char  LCD_data = 0x08;

void LCD1602_Wait_Ready(void)
{
	uint8_t sta;  

	LCD_data |= 0xf0;     //���ݸ���λ��1
	
	i2c_WriteByte(LCD_data);
	LCD_data &=~(1<<(1-1));//RS=0;               // 0  0  0  0      0  0  0   0
  	LCD_data |=(1<<(2-1));//RW=1;                // D7 D6 D5 D4        E  RW  RS
	i2c_WriteByte(LCD_data);	
	do
	{
		LCD_data |= 1<<2;          // 0000�����ݣ�0000������λ��
								   // 0xxx 0100  -------> E=1
		i2c_WriteByte(LCD_data);
		
		delay_ms(5);		//��ʱ
		
		sta = i2c_R_Byte();       /* ��ȡ״̬�� */
		
		LCD_data ^= 1<<2;              	//������㣨��ԭ��
										// 0xxx 0000  ----------> E=0
									   //or  LCD_data &=~(1<<2)  						  
		//I2C_WriteByte(LCD_data);//1��0 ִ��ָ��	
		i2c_WriteByte(LCD_data);	
	}while(sta &0x80);           /* ״̬�ֵ����λ 1æ��0���� */ 
}




void LCD_EN(void)
{
	LCD_data |= 1<<2;          // 0000�����ݣ�0000������λ��
	                           // 0xxx 0100  -------> E=1
	//I2C_WriteByte(LCD_data);	//1 ��ȡ����
	i2c_WriteByte(LCD_data);
	delay_us(10);		//��ʱ
	LCD_data ^= 1<<2;              	//������㣨��ԭ��
									// 0xxx 0000  ----------> E=0
	                               //or  LCD_data &=~(1<<2)  						  
	//I2C_WriteByte(LCD_data);//1��0 ִ��ָ��	
	i2c_WriteByte(LCD_data);
	
}



void LCD_WriteCMD(uint8_t cmd)
{
	
	LCD1602_Wait_Ready();
	LCD_data &=~(1<<(1-1));//RS=0;
	LCD_data &=~(1<<(2-1));//RW=0;
	i2c_WriteByte(LCD_data);   //��Ҫ�޸������ķ�������
	
	
	LCD_data &= 0X0f; //�����λ
	LCD_data |= cmd & 0xf0; //дָ�����λ
	i2c_WriteByte(LCD_data);
	LCD_EN();
	
	cmd = cmd<<4;
	LCD_data &= 0X0f; //�����λ
	LCD_data |= cmd & 0xf0; //дָ�����λ
	i2c_WriteByte(LCD_data);
	LCD_EN();
}



void LCD_WriteDAT(uint8_t dat)
{
	
	LCD1602_Wait_Ready();
	LCD_data |= (1<<(1-1));			//RS=1;
	LCD_data &=~(1<<(2-1));			//RW=0;
	i2c_WriteByte(LCD_data);
	
	LCD_data &= 0X0f; 				//�����λ
	LCD_data |= dat & 0xf0; 		//д���ݸ���λ
	i2c_WriteByte(LCD_data);
	LCD_EN();
	
	dat = dat<<4;
	LCD_data &= 0X0f; 				//�����λ
	LCD_data |= dat & 0xf0; 		//д���ݵ���λ
	i2c_WriteByte(LCD_data);
	LCD_EN();
}


void Set_Position(uint8_t x,uint8_t y)
{
	
	uint8_t x_pos;
	x_pos = x%0x10;
	if(y==0)
	{
		LCD_WriteCMD(0x80+x_pos);
	}
	else 
	{
		LCD_WriteCMD(0xc0+x_pos);
	}
	
}


/*-----------------�ַ�����-------------------------------*/
void LCD_ShowStr(uint8_t x, uint8_t y, uint8_t * str)
{
	Set_Position(x,y);
	while(*str != '\0')
	{
		
		LCD_WriteDAT(*str);
		str++;
	}
	
}

/*-------------charת�ַ���-----------------------------*/
void LCD_ShowNum(uint8_t x, uint8_t y, uint8_t dat)
{
	uint8_t  str[10];
    str[0] = (dat/100) + '0';  //ת��Ϊʮ�����ַ�����ʽ
    str[1] = (dat/10%10) + '0';
    str[2] = (dat%10) + '0';
    str[3] = '\0';
	if(str[0] == '0')
	{
		LCD_ShowStr(x,y,str+1);
	}
	
	
}

void LCD_Clear(void)
{
	LCD_WriteCMD(0x01);/* ����    */
}


void LCD_DispConf(uint8_t DispON, uint8_t CursorON, uint8_t GlareON)
{
	uint8_t x = 0x08;
	if(DispON!=0)
	{
		x = x|(DispON<<2);
	}
	if(CursorON!=0)
	{
		x = x|(CursorON<<1);
	}
	if(GlareON!=0)
	{
		x = x|GlareON;
	}	
	LCD_WriteCMD(x);
}



void LCD_Init(void)
{                      
	i2c_CfgGpio();		/* ����GPIO */
	
	LCD_WriteCMD(0x02);/*����Ҫ�����û���˵�һ���ϵ��û����*/
	LCD_WriteCMD(0x28);/* 0x28  4�߲���ģʽ   0x38 8��ģʽ  */  
	
	/* 0000  1DCB  D������ʾ C���ع�� B���ع����˸   */
	LCD_WriteCMD(0x0c);/* ����ʾ���ع��    */
	LCD_WriteCMD(0x06);/* ���ֲ�������ַ�Զ���1   */
	LCD_WriteCMD(0x01);/* ����    */
	delay_ms( 5);
}

