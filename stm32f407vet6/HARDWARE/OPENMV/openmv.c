#include "openmv.h"

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF;     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	

void USART3_IRQHandler(void) {//�������0x0d 0x0a
    u8 Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        Res = USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������
		// USART_SendData(USART3, Res);         //�򴮿�3��������
        // while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		if((USART3_RX_STA&0x8000)==0 )//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else {
                    USART3_RX_STA|=0x8000;	//���������
                    USART_SendData(USART3, 'b');         //�򴮿�3��������
                    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
                }
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d) {
                    USART3_RX_STA|=0x4000;
                    USART_SendData(USART3, 'a');         //�򴮿�3��������
                    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
                }
                else
				{
					USART3_RX_BUF=Res;

					USART3_RX_STA++;
					if(USART3_RX_STA> 1)USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		} 
    }

}

void Usart3_Init(u32 bound) {
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10����ΪUSART3 TX
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);//PB11����ΪUSART3 RX
  	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù��� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴����� 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //���� 

    GPIO_Init(GPIOB,&GPIO_InitStructure); 

    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600; 
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ 
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ 
    USART_Init(USART3, &USART_InitStructure); //��ʼ������ 

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;    //��Ӧ���ȼ�3 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //IRQ ͨ��ʹ�� 
    NVIC_Init(&NVIC_InitStructure);  //����ָ���Ĳ�����ʼ��VIC�Ĵ����� 

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����жϣ����յ������ж�

}
void Openmv_Init(void) {
    Usart3_Init(115200);

    USART_Cmd(USART3, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    
}