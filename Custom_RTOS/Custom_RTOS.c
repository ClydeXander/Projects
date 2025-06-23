#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

volatile int32_t TimeDelay = 0;
void Delay(uint32_t nTime);

uint32_t stack_Blink_1[8]= {0};
uint32_t *sp_Blink_1 = &stack_Blink_1[8];

uint32_t stack_Blink_2[8]= {0};
uint32_t *sp_Blink_2 = &stack_Blink_2[8];

/*=======================================================================
========================== Function declaration =========================
=======================================================================*/

void GPIO_SETUP(void);
void Blink_1(void);
void Blink_2(void);

int main(void){

    SysTick_Config(7200000);
    GPIO_SETUP();

    sp_Blink_1--;
    sp_Blink_1--;
    *sp_Blink_1 = (1U << 24); //xPSR
    sp_Blink_1--;
    *sp_Blink_1 = (uint32_t)&Blink_1; //PC
    sp_Blink_1-= 6;

    sp_Blink_2--;
    sp_Blink_2--;
    *sp_Blink_2 = (1U << 24); //xPSR
    sp_Blink_2--;
    *sp_Blink_2 = (uint32_t)&Blink_2; //PC
    sp_Blink_2-= 6;
    

    //SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    while(1){
        
    }
}

void SysTick_Handler(void){
    //__set_MSP((uint32_t)sp_Blink_1);
	if(TimeDelay > 0)
		TimeDelay--;
}

void Delay(uint32_t nTime){
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

void Blink_1(void){
    while(1){
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Delay(10);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Delay(10);
    }
}

void Blink_2(void){
    while(1){
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
        Delay(10);
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
        Delay(10);
    }
}

//void PendSV_Handler(void){
//    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//    //SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
//}

void GPIO_SETUP(void){

    GPIO_InitTypeDef GPIO_InitStruct_B0;
    GPIO_InitStruct_B0.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct_B0.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_B0.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_C13;
    GPIO_InitStruct_C13.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct_C13.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_C13.GPIO_Mode = GPIO_Mode_Out_PP;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_Init(GPIOB,&GPIO_InitStruct_B0);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);

    GPIO_SetBits(GPIOB,GPIO_Pin_0);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

}
