#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_spi.h>

void GPIO_SETUP(void);
void UART_SETUP(void);
void SPI_SETUP(void);
void Delay(uint32_t ntime);
uint8_t NRF24_Command_Status(uint8_t command);
uint8_t NRF24_Dummy_Register(void);

volatile uint32_t TimerDelay;
volatile uint8_t buffer ;
volatile uint8_t status;
volatile uint8_t Register;
volatile uint8_t data_spi;

int main (void){
    
    GPIO_SETUP();
    SPI_SETUP();
    UART_SETUP();
    SysTick_Config(720000);

    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    

    while(1){
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Delay(200);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Delay(200); 
    }
}



void USART2_IRQHandler(void){
    if(USART_GetITStatus(USART2,USART_IT_RXNE)){

        while(!(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)));
        buffer = USART_ReceiveData(USART2);

        GPIO_ResetBits(GPIOA,GPIO_Pin_4);

        NRF24_Command_Status(buffer);
        data_spi = NRF24_Dummy_Register();

        while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
        GPIO_SetBits(GPIOA,GPIO_Pin_4);

        while(!(USART_GetFlagStatus(USART2,USART_FLAG_TXE)));
        USART_SendData(USART2,data_spi);
    }
}

uint8_t NRF24_Command_Status(uint8_t command){

    while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, command);
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    return status = SPI_I2S_ReceiveData(SPI1);
}

uint8_t NRF24_Dummy_Register(void){

    while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, 0x00);
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    return Register = SPI_I2S_ReceiveData(SPI1);
}

void SysTick_Handler(void){
    if(TimerDelay != 0){
        TimerDelay--;
    }
}

void Delay (uint32_t ntime){
    TimerDelay = ntime;
    while (TimerDelay != 0);
}


void GPIO_SETUP(void){

    //GPIO_InitTypeDef GPIO_InitStruct_A1;
    //GPIO_InitStruct_A1.GPIO_Pin  = GPIO_Pin_2;
    //GPIO_InitStruct_A1.GPIO_Speed = GPIO_Speed_2MHz;
    //GPIO_InitStruct_A1.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A2;
    GPIO_InitStruct_A2.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct_A2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct_A2.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A3;
    GPIO_InitStruct_A3.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStruct_A3.GPIO_Speed = 0;
    GPIO_InitStruct_A3.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_InitTypeDef GPIO_InitStruct_A4;
    GPIO_InitStruct_A4.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStruct_A4.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct_A4.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_InitTypeDef GPIO_InitStruct_A5;
    GPIO_InitStruct_A5.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStruct_A5.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct_A5.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitTypeDef GPIO_InitStruct_A6;
    GPIO_InitStruct_A6.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStruct_A6.GPIO_Speed = 0;
    GPIO_InitStruct_A6.GPIO_Mode = GPIO_Mode_IPU;
    
    GPIO_InitTypeDef GPIO_InitStruct_A7;
    GPIO_InitStruct_A7.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStruct_A7.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct_A7.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitTypeDef GPIO_InitStruct_C13;
    GPIO_InitStruct_C13.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct_C13.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_C13.GPIO_Mode = GPIO_Mode_Out_PP;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    //GPIO_Init(GPIOA,&GPIO_InitStruct_A1);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A3);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A4);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A5);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A6);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A7);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);
}

void UART_SETUP(void){

    USART_InitTypeDef USART_InitStruct_2;
    USART_InitStruct_2.USART_BaudRate = 9600;
    USART_InitStruct_2.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct_2.USART_StopBits = USART_StopBits_1;
    USART_InitStruct_2.USART_Parity = USART_Parity_No ;
    USART_InitStruct_2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct_2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_Init(USART2,&USART_InitStruct_2);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
    USART_Cmd(USART2,ENABLE);
}

void SPI_SETUP(void){
    
    SPI_InitTypeDef SPI_InitStruct_1;
    SPI_InitStruct_1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct_1.SPI_Mode = 0x04;
    SPI_InitStruct_1.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct_1.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct_1.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct_1.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStruct_1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruct_1.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct_1.SPI_CRCPolynomial = 7;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    SPI_Init(SPI1,&SPI_InitStruct_1);
    SPI_SSOutputCmd(SPI1,ENABLE);
    SPI_Cmd(SPI1,ENABLE);
}