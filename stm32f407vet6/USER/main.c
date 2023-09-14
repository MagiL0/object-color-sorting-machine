#include "delay.h"
#include "usart.h"
#include "led.h"
#include "exti.h"
#include "timer.h"
#include "key.h"
#include "channel.h"
#include "motor.h"
#include "main.h"
#include "openmv.h"
#include "lcd1602.h"

/**
 * ���ʹ�����Ʒ����ɫ
 * ������ɫ���ֱ��� 0~3 ��ʾ
 * 0:��
 * 1:��
 * 2:��
 * 3:��
 */
uint8_t item;

/**
 * ��ʾÿ��ͨ����ת�˼���90�㣬���ֵΪ3����ʼֵΪ0
 */
uint8_t angle_count;

/**
 * ÿ��ͨ����ǰ�Ƿ�������
 */
uint8_t channel_item[4];

/**
 * ͨ����״̬
 * ͨ��TIM6����ɨ������飬���Ϊ1���ͨ���ţ�Ϊ0��ر�ͨ����
 */
uint8_t channel_door[4];

/**
 * �ĸ�Ͱ�ı�ţ���Χ 0~3�������޸Ĵ˴����޸�Ͱ�ڴ�ŵ����壬�ֱ�������0~3��Ӧ
 */
uint8_t bucket[4] = {0, 1, 2, 3};

uint8_t message[] = "How are you?";


int main(void)
{ 
	u8 t;
	u8 len;	
	u16 times=0;  
	u8 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	KEY_Init();
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	Channel_Init();			//�Ű��ʼ��
	Motor_Init();			//�����ʼ��
	TIM3_Int_Init(50-1,8400-1);	//��Ƶϵ��8400 ����50�Σ�84000000/8400/50=200Hz=5ms
	Openmv_Init();			//openmv��ʼ��

	delay_ms(1000);

	LCD_Init();

	LCD_ShowStr(2,0,message); 
	
	delay_ms(1000);
	while(1)
	{


		// LED2 = !LED2;

		//***********���Ե����********
		// delay(4000);
		// for(i = 0; i < 4; i++) {
		// 	channel_door[i] = !channel_door[i];
		// }

		//***********���Դ���***********
		// if((USART3_RX_STA & 0x8000) !=0)//�������
		// {
		// 	USART_SendData(USART1, USART3_RX_BUF);         //�򴮿�1��������
		//     while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		// 	USART3_RX_STA = 0;
		// }

		//***********��������***********
		// if(KEY_Scan(1) == 1) {
		// 	LED2 = ~ LED2;
		// }
		
		delay_ms(1000);
		
			
	}
}

