#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"


volatile int32_t TimeDelay = 0;
void Delay(uint32_t nTime);

uint32_t stack_Blink_1[18]= {0};
uint32_t *sp_Blink_1 = &stack_Blink_1[17];

uint32_t stack_Blink_2[18]= {0};
uint32_t *sp_Blink_2 = &stack_Blink_2[17];

uint32_t curr_Task = 0;
uint32_t next_Task = 1;
uint32_t *pCurr_Task = 0;
uint32_t *pNext_Task = 0;
uint32_t *PSP_Array[2]={0};
uint32_t **pPSP_Array = 0;

uint32_t tmp = 0;

/*=======================================================================
========================== Function declaration =========================
=======================================================================*/

void GPIO_SETUP(void);
void EXTI_SETUP(void);
void Blink_1(void);
void Blink_2(void);

int main(void){

    SysTick_Config(7200000);
    GPIO_SETUP();
    EXTI_SETUP();
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_SetPriority(PendSV_IRQn, 0xEF);
    NVIC_SetPriority(EXTI2_IRQn, 0xFF);

    SCB->CCR |= SCB_CCR_STKALIGN_Msk;

    sp_Blink_1--;
    *(sp_Blink_1--) = (1U << 24); //xPSR
    *sp_Blink_1 = (uint32_t)&Blink_1; //PC

    sp_Blink_2--;
    *(sp_Blink_2--) = (1U << 24); //xPSR
    *sp_Blink_2 = (uint32_t)&Blink_2; //PC

    pCurr_Task = &curr_Task;
    pNext_Task = &next_Task;

    sp_Blink_1 = (uint32_t *)((((uint32_t)stack_Blink_1 / 8) * 8) + 8);
    sp_Blink_2 = (uint32_t *)((((uint32_t)stack_Blink_2 / 8) * 8) + 8);
    
    PSP_Array[0]=sp_Blink_1;
    PSP_Array[1]=sp_Blink_2;
    pPSP_Array = &PSP_Array[0];

    Blink_1();

    while(1){
        
    }
}

void PendSV_Handler(void){

    __ASM volatile(
        /* DON'T WRITE R4 OR BELOW REGISTERS A DECIMAL VALUE I HAVE NO IDEA
            WHY, BUT IT BREAKS THE WHOLE SYSTEM. YOU CAN'T WRITE ANY DECIMAL 
            VALUE TO ANY REGISTERS AFTERWARDS. JUST USE R0-R3 REGS. YOU CAN 
            WRITE TO ANY MEMORY VALUES, BUT YOU ARE SAVING R4-R11 REGS TO STACK, 
            SO DON'T DO THAT EITHER. JUST USE ONE OF R0-R3 REGS WHEN YOU ARE
            DONE WITH ONE OF THEM AND USE IT FOR OTHER STUFF*/
        
        /*MAKE SURE THERE IS NO SPACES BETWEEN EACH CLOBBER SECTION IN INLINE ASSEMBLLY 
        (THE THIRD COLLON :)*/

                    //save current context
                    "CPSID I                   \n\t" //__disable_irq()
                    "LDR R0, %[pPSP_Array]     \n\t" //loadin the address of PSP_Array
                    "LDR R2, %[curr_Task]      \n\t"
                    "LDR R1, [R0, R2, LSL #2]  \n\t" //getting the value of first element (&stack_Blink_1)
                    "ADD R1, #32               \n\t" //stack_Blink_1[8]
                    : // Output Operands
                    : [curr_Task] "m" (curr_Task),
                      [pPSP_Array] "m" (pPSP_Array),
                      [next_task] "m" (next_Task),
                      [pCurr_Task] "m" (pCurr_Task),
                      [pNext_Task] "m" (pNext_Task) // Input Operands
                    :"memory","r0","r1","r2","r3","r12","r14"
    );
    uint8_t sumvar2 = 15;
    __ASM volatile(
                    "STMDB R1!, {R4-R11}       \n\t" // Save R4 to R11 in task stack (8 regs) after saving the r1 goes back to stack_Blink_1[0]
                    "LDR R1, %[curr_Task]      \n\t"
                    "LDR R2, %[pCurr_Task]     \n\t"
                    : // Output Operands
                    : [curr_Task] "m" (curr_Task),
                      [pPSP_Array] "m" (pPSP_Array),
                      [next_task] "m" (next_Task),
                      [pCurr_Task] "m" (pCurr_Task),
                      [pNext_Task] "m" (pNext_Task) // Input Operands
                    :"memory","r0","r1","r2","r3","r12","r14"
    );
    uint8_t sumvar1 = 15;
    __ASM volatile(
                    //load next context
                    "LDR R3, %[next_task]      \n\t"
                    "STR R3, [R2]              \n\t" // Set curr_task = next_task
                    "LDR R2, %[pNext_Task]     \n\t"
                    "STR R1, [R2]              \n\t" // set next_task = curr_task (we are toggling)
                    "LDR R1, [R0, R3, LSL #2]  \n\t" // Load PSP value from PSP_array (&stack_Blink_2)
                    "LDMIA R1!, {R4-R11}       \n\t" // Load R4 to R11 from stack
                    "MSR MSP, R1               \n\t" // Set PSP to next task
                    "ISB                       \n\t"
                    : // Output Operands
                    : [curr_Task] "m" (curr_Task),
                      [pPSP_Array] "m" (pPSP_Array),
                      [next_task] "m" (next_Task),
                      [pCurr_Task] "m" (pCurr_Task),
                      [pNext_Task] "m" (pNext_Task) // Input Operands
                    :"memory","r0","r1","r2","r3","r12","r14"
                );
    uint8_t sumvar = 15;
    __ASM volatile (
                    "CPSIE I      \n\t" //__enable_irq()
                    "BX LR         \n\t" // Return
    );
}

void SysTick_Handler(void){
    //__disable_irq();
	if(TimeDelay > 0)
		TimeDelay--;
    //__enable_irq();
}

void EXTI2_IRQHandler(void){
    EXTI_ClearITPendingBit(EXTI_Line2);
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void Delay(uint32_t nTime){
	TimeDelay = nTime;
    while(TimeDelay != 0);
}

void Blink_1(void){   
    while(1){
        //GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
        //Delay(10);
        //GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
        //Delay(10);
    }
}

void Blink_2(void){  
    while(1){
        //GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
        //Delay(10);
        //GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
        //Delay(10);
    }
}

void GPIO_SETUP(void){

    GPIO_InitTypeDef GPIO_InitStruct_A2;
    GPIO_InitStruct_A2.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct_A2.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A2.GPIO_Mode = GPIO_Mode_IPD;

    GPIO_InitTypeDef GPIO_InitStruct_B0;
    GPIO_InitStruct_B0.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct_B0.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_B0.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_B1;
    GPIO_InitStruct_B1.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct_B1.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_B1.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_C13;
    GPIO_InitStruct_C13.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct_C13.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_C13.GPIO_Mode = GPIO_Mode_Out_PP;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);
    GPIO_Init(GPIOB,&GPIO_InitStruct_B0);
    GPIO_Init(GPIOB,&GPIO_InitStruct_B1);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, 2);

    GPIO_SetBits(GPIOB,GPIO_Pin_0);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

}

void EXTI_SETUP(void){

    EXTI_InitTypeDef EXTI_InitStruct_2;
    EXTI_InitStruct_2.EXTI_Line = EXTI_Line2;
    EXTI_InitStruct_2.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct_2.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct_2.EXTI_LineCmd = ENABLE;

    EXTI_Init(&EXTI_InitStruct_2);
    
}
