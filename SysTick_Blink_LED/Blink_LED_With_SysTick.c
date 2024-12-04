#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

volatile int32_t TimeDelay;
void Delay(uint32_t nTime);

int main(void){

	__disable_irq();
	SysTick->CTRL = 0;
	SysTick->LOAD = 7200000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;
	__enable_irq();
	
	GPIO_DeInit(GPIOC);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

    while(1){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(1);
	}
	
}
void SysTick_Handler(void){
	if(TimeDelay >0)
		TimeDelay--;
}

void Delay(uint32_t nTime){
	TimeDelay = nTime;
	while(TimeDelay != 0);
}
