#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_spi.h>

/*Command Registers*/
#define R_REGISTER            0x00
#define W_REGISTER            0x20
#define R_RX_PAYLOAD          0x61
#define W_TX_PAYLOAD          0xA0
#define FLUSH_TX              0xE1
#define FLUSH_RX              0xE2
#define REFUSE_TX_PL          0xE3
#define R_RX_PL_WID           0x60
#define W_ACK_PAYLOAD_0       0x98
#define W_ACK_PAYLOAD_1       0x99
#define W_ACK_PAYLOAD_2       0x9A
#define W_ACK_PAYLOAD_3       0x9B
#define W_ACK_PAYLOAD_4       0x9C
#define W_ACK_PAYLOAD_5       0x9D
#define W_TX_PAYLOAD_NO_ACK   0xB0
#define NOP                   0xFF   

/*Registers*/
#define CONFIG         0x00
#define EN_AA          0x01
#define EN_RXADDR      0x02
#define SETUP_AW       0x03
#define SETUP_RETR     0x04
#define RF_CH          0x05
#define RF_SETUP       0x06
#define STATUS         0x07
#define OBSERVE_TX     0x08
#define RPD            0x09
#define RX_ADDR_P0     0x0A
#define RX_ADDR_P1     0x0B
#define RX_ADDR_P2     0x0C
#define RX_ADDR_P3     0x0D
#define RX_ADDR_P4     0x0E
#define RX_ADDR_P5     0x0F
#define TX_ADDR        0x10
#define RX_PW_P0       0x11
#define RX_PW_P1       0x12
#define RX_PW_P2       0x13
#define RX_PW_P3       0x14
#define RX_PW_P4       0x15
#define RX_PW_P5       0x16
#define FIFO_STATUS    0x17
#define DYNPD          0x1C
#define FEATURE        0x1D

volatile uint32_t ntime = 0;
volatile uint32_t TimeDelay = 0;
volatile uint8_t NRF_Command = 0;
volatile uint8_t NRF_Status = 0;
volatile uint16_t NRF_Register = 0;
volatile uint16_t NRF_Write_Register = 0;

void GPIO_SETUP(void);
void EXTI_SETUP(void);
void SPI_SETUP(void);
void USART_SETUP(void);
void DelayMS(uint16_t ntime);
uint8_t NRF_Send_Command_Read_Status(uint8_t NRF_Command);
uint16_t NRF_Send_Dummy_Read_Register(void);
void NRF_Send_Dummy_Write_Register(uint16_t NRF_Write_Register);

int main(void){

    GPIO_SETUP();
    EXTI_SETUP();
    SPI_SETUP();
    USART_SETUP();
    SysTick_Config(720000);


    while(1){}
}

uint8_t NRF_Send_Command_Read_Status(uint8_t NRF_Command){

    SPI_I2S_SendData(SPI1,NRF_Command);
    NRF_Status = SPI_I2S_ReceiveData(SPI1);
    return NRF_Status;

}

uint16_t NRF_Send_Dummy_Read_Register(void){

}

void NRF_Send_Dummy_Write_Register(uint16_t NRF_Write_Register){

}

void EXTI0_IRQHandler(void){
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void Systick_Handler(void){
    while (TimeDelay != 0){
        TimeDelay--;
    }
}

void DelayMS(uint16_t ntime){
    TimeDelay = ntime;
    while(TimeDelay == 0);
}

void GPIO_SETUP(void){

    GPIO_InitTypeDef GPIO_InitStruct_A1;
    GPIO_InitStruct_A1.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct_A1.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A1.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A2;
    GPIO_InitStruct_A2.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct_A2.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A2.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A3;
    GPIO_InitStruct_A3.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStruct_A3.GPIO_Speed = 0;
    GPIO_InitStruct_A3.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_InitTypeDef GPIO_InitStruct_A4;
    GPIO_InitStruct_A4.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStruct_A4.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A4.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A5;
    GPIO_InitStruct_A5.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStruct_A5.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A5.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitTypeDef GPIO_InitStruct_A6;
    GPIO_InitStruct_A6.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStruct_A6.GPIO_Speed = 0;
    GPIO_InitStruct_A6.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_InitTypeDef GPIO_InitStruct_A7;
    GPIO_InitStruct_A7.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStruct_A7.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A7.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitTypeDef GPIO_InitStruct_B0;
    GPIO_InitStruct_B0.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct_B0.GPIO_Speed = 0;
    GPIO_InitStruct_B0.GPIO_Mode = GPIO_Mode_IPD;

    GPIO_InitTypeDef GPIO_InitStruct_C13;
    GPIO_InitStruct_C13.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct_C13.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_C13.GPIO_Mode = GPIO_Mode_Out_PP;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A1);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A2);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A3);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A4);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A5);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A6);
    GPIO_Init(GPIOA,&GPIO_InitStruct_A7);
    GPIO_Init(GPIOB,&GPIO_InitStruct_B0);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);

    GPIO_SetBits(GPIOA,GPIO_Pin_1);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

}

void EXTI_SETUP(void){

    EXTI_InitTypeDef EXTI_InitStruct_B0;
    EXTI_InitStruct_B0.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct_B0.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct_B0.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct_B0.EXTI_LineCmd = ENABLE;
    
    EXTI_Init(&EXTI_InitStruct_B0);
    NVIC_EnableIRQ(EXTI0_IRQn);

}

void SPI_SETUP(void){

    SPI_InitTypeDef SPI_InitStruct_1;
    SPI_InitStruct_1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct_1.SPI_Mode = 0x04;
    SPI_InitStruct_1.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct_1.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct_1.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct_1.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStruct_1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct_1.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct_1.SPI_CRCPolynomial = 7;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    SPI_Init(SPI1,&SPI_InitStruct_1);
    SPI_SSOutputCmd(SPI1,ENABLE);
    SPI_Cmd(SPI1,ENABLE);

}

void USART_SETUP(void){

    USART_InitTypeDef USART_InitStruct_2;
    USART_InitStruct_2.USART_BaudRate = 9600;
    USART_InitStruct_2.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct_2.USART_StopBits = USART_StopBits_1;
    USART_InitStruct_2.USART_Parity = USART_Parity_No ;
    USART_InitStruct_2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct_2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_Init(USART2,&USART_InitStruct_2);
    USART_Cmd(USART2,ENABLE);
}