#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "sys.h"
#include <inttypes.h>

#define ADDRESS		0x7e


#define LCD1602_I2C_WR	0		/* д����bit */
#define LCD1602_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define LCD1602_GPIO_PORT_I2C	GPIOE			/* GPIO�˿� */
#define LCD1602_RCC_I2C_PORT 	RCC_AHB1Periph_GPIOE		/* GPIO�˿�ʱ�� */
#define LCD1602_I2C_SCL_PIN		GPIO_Pin_11			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define LCD1602_I2C_SDA_PIN		GPIO_Pin_13			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define LCD1602_I2C_SCL_1()  GPIO_SetBits(LCD1602_GPIO_PORT_I2C, LCD1602_I2C_SCL_PIN)		/* SCL = 1 */
	#define LCD1602_I2C_SCL_0()  GPIO_ResetBits(LCD1602_GPIO_PORT_I2C, LCD1602_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define LCD1602_I2C_SDA_1()  GPIO_SetBits(LCD1602_GPIO_PORT_I2C, LCD1602_I2C_SDA_PIN)		/* SDA = 1 */
	#define LCD1602_I2C_SDA_0()  GPIO_ResetBits(LCD1602_GPIO_PORT_I2C, LCD1602_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define LCD1602_I2C_SDA_READ()  GPIO_ReadInputDataBit(LCD1602_GPIO_PORT_I2C, LCD1602_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    #define LCD1602_I2C_SCL_1() (PBout(9) = 1)		/* SCL = 1 */
	#define LCD1602_I2C_SCL_0() (PBout(9) = 0)		/* SCL = 0 */
	#define LCD1602_I2C_SDA_1() (PBout(8) = 1)		/* SDA = 1 */
	#define LCD1602_I2C_SDA_0() (PBout(8) = 0)		/* SDA = 0 */
	#define LCD1602_I2C_SDA_READ() (PBin(8))			/* ��SDA����״̬ */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
uint8_t  i2c_WriteByte(uint8_t data);
uint8_t i2c_R_Byte(void);
void i2c_CfgGpio(void);

	

#endif

