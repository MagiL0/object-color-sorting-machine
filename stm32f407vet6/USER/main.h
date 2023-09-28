#ifndef __MAIN_H
#define __MAIN_H
#define USART3_REC_LEN  			3  	//�����������ֽ��� 200
#include "sys.h"
extern u8 USART3_RX_BUF; 
extern uint8_t angle_count;
extern uint8_t channel_item[4];
extern uint8_t bucket[4];
extern uint8_t item;
extern uint8_t channel_door[4];
extern u16 USART3_RX_STA;
extern uint32_t delay_count;
extern uint8_t debug_flag;
extern uint8_t switch_state;
extern uint16_t motor_state_count;
extern uint8_t motor_state;
extern uint8_t switch_state_once;
extern uint8_t current_color;
extern u8 bcount;
extern u8 color_buffer[4];
extern uint8_t switch_lock;
extern uint16_t switch_lock_count;

#define clip(x) (((x) > 3) ? (x) - 4 : (x))
// ÿ��ͨ�������������ʱ�䣬��λ��5ms������50���ǿ���50*5ms=250ms
#define CHANNEL_OPEN_TIME 50
// ���뿪��ÿ�μ���ʱ��������λ5ms������100����100*5ms=500ms
#define SWITCH_CHECK_TIME 200
// ��ɫ���ļ��������⵽��ɫʱ���������ʱ���ټ����һ����ɫ����λ5ms
#define COLOR_CHECK_TIME 200
// ���ʹ�ֹͣ����ʱ������⵽��ɫʱ���ȴ�����ʱ����ֹͣ���ʹ�����λ5ms
#define MOTOR_STOP_TIME 200

#endif // !__
