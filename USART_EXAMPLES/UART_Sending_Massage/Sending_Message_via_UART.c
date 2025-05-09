#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

volatile int32_t TimeDelay = 0;
void Delay(uint32_t nTime);


int main (void){

   SysTick_Config(7200000);

    //USART2 TX Pin (PA2) Config
    GPIO_InitTypeDef GPIO_InitStruct_A2;
	GPIO_InitStruct_A2.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct_A2.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct_A2.GPIO_Speed=GPIO_Speed_50MHz;

    //USART2 RX Pin (PA3) Config
    GPIO_InitTypeDef GPIO_InitStruct_A3;
	GPIO_InitStruct_A3.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct_A3.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct_A3.GPIO_Speed=0x0;


    USART_InitTypeDef USART2_InitStruct_9600;
    USART2_InitStruct_9600.USART_BaudRate = 9600;
    USART2_InitStruct_9600.USART_WordLength = USART_WordLength_8b;
    USART2_InitStruct_9600.USART_StopBits = USART_StopBits_1;
    USART2_InitStruct_9600.USART_Parity = USART_Parity_No ;
    USART2_InitStruct_9600.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART2_InitStruct_9600.USART_HardwareFlowControl = USART_HardwareFlowControl_None;


    //Enable USART2
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_DeInit(GPIOA);
    //Functions to initialize USART2 at PA3 as RX and PA2 as TX
    GPIO_Init(GPIOA,&GPIO_InitStruct_A3);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);

    //Enable USART2 
    USART_Init(USART2, &USART2_InitStruct_9600);
    USART_Cmd(USART2, ENABLE);

    GPIO_DeInit(GPIOC);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);


    while (1){

        //Seending Message
        while(!(USART_GetFlagStatus(USART2, USART_FLAG_TXE)));
        USART_SendData(USART2, 75);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Delay(10);
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Delay(10);
    }
}


void SysTick_Handler(void){
	if(TimeDelay > 0)
		TimeDelay--;
}

void Delay(uint32_t nTime){
	TimeDelay = nTime;
	while(TimeDelay != 0);
}