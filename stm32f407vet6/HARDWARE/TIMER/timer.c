#include "timer.h"
#include "led.h"
#include "main.h"
#include "channel.h"
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F407������
//��ʱ�� ��������	   
							  
////////////////////////////////////////////////////////////////////////////////// 	 

uint32_t delay_count = 0;

/**
 * @brief �ӳٺ�������λms���ӳٱ���Ϊ5�ı���������ȡ��
 * 
 * @param n 
 */
void delay(uint32_t n) {
	delay_count = n / 5;
	while(delay_count);
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	static uint16_t interval_count;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		int i = 0;
		// �ĸ��Ű忪��ʱ���ʱ
		int door_count[4] = {0};

		if(++interval_count == 100) {  //ÿ�� 500ms ����һ�� ȷ����Ƭ��û�п���
			LED2 = !LED2;
			interval_count = 0;
		}
		
		// ���������뿪��ʱ��ִ�иó���
		if(switch_state_once) {

			// �жϵ�ǰ��ɫ�Ƿ��ͨ����ͬ
			if (current_color) {
				// �ж�Ͱ�ź�ͨ�����Ƿ���ͬ��Ҳ���Ƿ�ͬ�����壩
				if(current_color == angle_count) {
					// �򿪵�����ٸ� 5ms ��5ms�ı�����
					motor_state_count = 30;
					// �Ž�����
					channel_item[angle_count] = current_color;
					// ��յ�ǰ��ɫ
					current_color = 0;
					// ���ڼ�������
					USART3_RX_STA = 0;
				}
			}

			// �� ������ �� �����Ͱ����ͬ ʱ������
			for(i = 0; i < 4; i++) {
				if (channel_item[i] == bucket[clip(angle_count + i)]) {
					//��ͨ������150ms����Զ��ر�
					channel_door[i] = 1;
					//��������
					channel_item[i] = 0;
				}
			}

			switch_state_once = 0;
		}

		

		// ����motor_state_count 	��motor_state_countΪ0ʱ���رյ������־λ��0
		if (motor_state_count == 0) {
			motor_state = 0;
		} else if (motor_state == 1){ //��motor_stateΪ1ʱ��motor_state_count�Լ�
			motor_state_count--;
		}

		// ����delay_count
		delay_count = delay_count > 0 ? delay_count - 1 : 0;


		// ��ֹ�Ű忪��ʱ�����
		for(i = 0; i < 4; i++) {
			// �����м�ʱ���Ű�ر�ʱ���رռ�ʱ
			if(door_count[i] != 0 && channel_door[i] == 0) {
				door_count[i] = 0;
			}
			// ���Ű忪��ʱ����ʼ��ʱ
			door_count[i] += channel_door[i];
			// ����ʱ���� 30*5ms ʱ���ر��Ű�
			if(door_count[i] >= 30) {
				channel_door[i] = 0;
			}
		}


		// ����motor_state_count��ֵ���Ƶ��
		//Motor = motor_state && motor_state_count;
		// ����channel_door�����ֵ�����Ű�
		if(!debug_flag) {
			C0 = channel_door[0];
			C1 = channel_door[1];
			C2 = channel_door[2];
			C3 = channel_door[3];
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}




//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

