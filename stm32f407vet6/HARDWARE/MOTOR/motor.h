#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define Motor PBout(5)
//MR 0 Ϊ��ת��1 ��ת
#define MR PBout(1) 


/**
 * @brief ����������ų�ʼ��
 * 
 */
void Motor_Init(void);
void test_Motor(void);


#endif // !__M
