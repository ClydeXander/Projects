#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

volatile char buffer;
volatile uint32_t TimerDelay;
void Delay(uint32_t ntime); 

int main(void){

    //USART2 TX (PA2) pin
    GPIO_InitTypeDef GPIO_InitStruct_A2;
    GPIO_InitStruct_A2.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct_A2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct_A2.GPIO_Mode = GPIO_Mode_AF_PP;

    //USART2 RX (PA3) Pin
    GPIO_InitTypeDef GPIO_InitStruct_A3;
    GPIO_InitStruct_A3.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStruct_A3.GPIO_Speed = 0;
    GPIO_InitStruct_A3.GPIO_Mode = GPIO_Mode_IPU;

    //GPIOC 13 pin (LED) config
    GPIO_InitTypeDef GPIO_InitStruct_C13;
    GPIO_InitStruct_C13.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct_C13.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_C13.GPIO_Mode = GPIO_Mode_Out_PP;

    //USART2 Config
    USART_InitTypeDef USART_InitStruct_2;
    USART_InitStruct_2.USART_BaudRate = 9600;
    USART_InitStruct_2.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct_2.USART_StopBits = USART_StopBits_1;
    USART_InitStruct_2.USART_Parity = USART_Parity_No ;
    USART_InitStruct_2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct_2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    SysTick_Config(7200000);

    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A3);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);

    NVIC_EnableIRQ(USART2_IRQn);

    USART_Init(USART2,&USART_InitStruct_2);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);

    
    while(1){}
}

void SysTick_Handler(void){
    if(TimerDelay != 0){
        TimerDelay --;
    }
}


void Delay (uint32_t ntime){
    TimerDelay = ntime;
    while (TimerDelay != 0);
}

void USART2_IRQHandler(void){
    
    if(USART_GetITStatus(USART2,USART_IT_RXNE)){

        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        buffer = USART_ReceiveData(USART2);

        if (USART_GetFlagStatus(USART2,USART_FLAG_TXE)){
            USART_SendData(USART2,buffer);
        }
    }

}