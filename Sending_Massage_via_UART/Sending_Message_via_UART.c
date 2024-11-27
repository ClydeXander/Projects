#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

int main (void){

    //USART2 TX Pin (PA2) Config
    GPIO_InitTypeDef GPIO_InitStruct_A2;
	GPIO_InitStruct_A2.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct_A2.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct_A2.GPIO_Speed=GPIO_Speed_50MHz;

    //USART2 RX Pin (PA3) Config
    GPIO_InitTypeDef GPIO_InitStruct_A3;
	GPIO_InitStruct_A3.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct_A3.GPIO_Mode=GPIO_Mode_IPU;


     //Enable USART2
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN, ENABLE);

    GPIO_DeInit(GPIOA);
    //Functions to initialize USART2 at PA3 as RX and PA2 as TX
    GPIO_Init(GPIOA,&GPIO_InitStruct_A3);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);

    
    
    while (1){

    }
}