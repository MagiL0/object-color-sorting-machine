#include "key.h"
#include "delay.h" 
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 

// STM32F407������
//����������������	   
							  
////////////////////////////////////////////////////////////////////////////////// 	 



//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOA,ʱ��
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0--K1����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;// PA2��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA1
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� --��ӦK1����
//key 0,2 ��ӦPA0 PA2
u8 KEY_Scan(u8 mode, u8 key)
{	
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(PAin(key)==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(PAin(key)==1)return 1;
	}else if((PAin(key))==0)key_up=1; 	    
 	return 0;// �ް�������
}

// ���뿪��ɨ�躯��
// �ú����޸�switch_state_once��switch_state��ֵ
// key 0/2 ��ʾ PA0 PA2
// value 0-1 ��ʾ��ȡ���ǵ͵�ƽ���Ǹߵ�ƽ
void DIP_Scan(u8 key, u8 value) {
	static u8 key_up=1;//�������ɿ���־
	u8 key_value = 0;

	if (PAin(key) == value) {
		delay_ms(10);//ȥ���� 


		if (PAin(key) == value) {
			
			switch_state = 1;

			if (key_up) {
				key_up = 0;
				switch_state_once = 1;
			}
		} else {
			switch_state = 0;
			key_up  = 0;
		}
		
	} else {
		key_up = 1;
		switch_state = 0;
	}
}



void test_DIP(void) {
	debug_flag = 1;
	while(1) {
		DIP_Scan(2, 0);
		LED2 = switch_state;
	}
}

















