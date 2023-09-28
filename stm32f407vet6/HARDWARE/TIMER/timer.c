#include "timer.h"
#include "led.h"
#include "main.h"
#include "motor.h"
#include "channel.h"
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F407������
//��ʱ�� ��������	   
							  
////////////////////////////////////////////////////////////////////////////////// 	 

uint8_t color_flag = 0;
uint32_t delay_count = 0;
uint16_t ucount = 0;;
uint8_t switch_lock = 0;
uint16_t switch_lock_count = 0;
uint8_t channel_pos[4] = {0, 1, 2, 3};
uint8_t last_color;
uint16_t color_count;
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
		static  door_count[4] = {0};

		//ÿ�� 500ms ����һ�� ȷ����Ƭ��û�п���
		if(++interval_count == 200 && !debug_flag) {  
			// LED2 = !LED2;
			interval_count = 0;
		}




		// ���������뿪��ʱ��ִ�иó���
		if(switch_state_once) {
			// ȷ��ÿ�����ټ��1s�ٴ���
			switch_lock_count = SWITCH_CHECK_TIME;
			switch_lock = 1;

			USART_SendString(USART1, "t\r\n");
			angle_count = (angle_count + 1) % 4;

			// �� ������ �� �����Ͱ����ͬ ʱ������
			for(i = 0; i < 4; i++) {
				if (channel_item[i] == bucket[(7 - angle_count + i) % 4]) {
					//��ͨ�������Զ��ر�
					channel_door[i] = 1;
					//��������
					channel_item[i] = 0;
				}
			}



			// �жϵ�ǰ��ɫ
			if (current_color) {
				// �򿪵�����ٸ� 5ms 
				// motor_state_count = 200;
				ucount = COLOR_CHECK_TIME;
				motor_state = 1;
				// �Ž�����
				channel_item[angle_count] = current_color;
				
				// ��յ�ǰ��ɫ
				current_color = 0;
				color_buffer[0] = color_buffer[1] = color_buffer[2] = 0;
				color_flag = 1;
			}


			

			// ����ñ�־λ
			switch_state_once = 0;
		}


		// ����motor_state_count 	��motor_state_countΪ0ʱ���رյ������־λ��0
		// if (motor_state_count == 0) {
		// 	motor_state = 0;
		// } else if (motor_state == 1){ //��motor_stateΪ1ʱ��motor_state_count�Լ�
		// 	motor_state_count--;
		// }

		// ����switch_lock
		if (switch_lock_count != 0) {
			switch_lock_count --;
		} else {
			switch_lock	= 0;
		}
		
		// ����ucount Ҳ����ɫ�����
		if (ucount != 0) {
			ucount --;
		}

		// ��if����ʾ����ɫ���������ʱ����մ��ڻ��������������ܴ�����Ϣ
		if (ucount == 0 && color_flag == 1) {
			color_flag = 0;
			USART3_RX_STA = 0;
			current_color = 0;
			color_buffer[0] = color_buffer[1] = color_buffer[2] = 0;
		}
		
		// ����color_count
		// Ҳ��������ɫ��⵽��ɫ�󣬴��ʹ�ֹͣ����ʱ
		if (current_color != 0 && last_color != current_color) {
			color_count = MOTOR_STOP_TIME;
		} else if (current_color != 0 && last_color == current_color) {
			if (color_count == 0) {
				motor_state = 0;
			} else {
				color_count -= 1;
			}
		} else {
			motor_state = 1;
		}
		last_color = current_color;

		// ��������ʱ
		if(color_buffer[0] == color_buffer[1] 
		&& color_buffer[1] == color_buffer[2]
		&& color_buffer[0] != 0
		&& ucount == 0) {
			current_color = color_buffer[0];
		} else if (ucount == 0){
			USART3_RX_STA = 0;
		}


		// ����delay_count
		delay_count = delay_count > 0 ? delay_count - 1 : 0;
		
		// ����color_count
		if (color_count >= 1) {
			color_count --;
		}

		// ��ֹ�Ű忪��ʱ�����
		for(i = 0; i < 4; i++) {
			// �����м�ʱ���Ű�ر�ʱ���رռ�ʱ  ���մ�ʩ
			if(channel_door[i] == 0) {
				door_count[i] = 0;
			}

			// ���Ű忪��ʱ����ʼ��ʱ
			door_count[i] += channel_door[i];

			// ����ʱ���� 30*5ms ʱ���ر��Ű�
			if(door_count[i] >= CHANNEL_OPEN_TIME) {
				channel_door[i] = 0;
				door_count[i] = 0;
			}
		}


		// ����motor_state��ֵ���Ƶ��
		Motor = motor_state;
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

