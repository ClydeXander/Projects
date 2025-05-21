#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

void SPI_SETUP(void);
void GPIO_SETUP(void);
void SysTick_SETUP(void);
void SysTick_Delay_uS (uint16_t ntime);
void SysTick_Delay_mS (uint16_t ntime);

void MAX34_Read_8Bit_Register(uint8_t MAX34_Register, uint8_t *Data_Store);
void MAX34_Write_8Bit_Register(uint8_t MAX34_Register, uint8_t MAX34_Register_Bit, uint8_t NRF24_Set_Reset);
void MAX34_Testing_Write_Function(uint8_t MAX34_Register, uint8_t MAX34_Register_Bit);


int main(void){

    SPI_SETUP();
    GPIO_SETUP();
    SysTick_SETUP();

    uint8_t testing_some = 0;

    SysTick_Delay_mS(4000);
    MAX34_Write_8Bit_Register(0x88, 0x10, 0);
    MAX34_Write_8Bit_Register(0x88, 0x10, 1);
    //MAX34_Testing_Write_Function(0x8A, 0x10);
    //SysTick_Delay_uS(20);
    MAX34_Read_8Bit_Register(0x88, &testing_some);
    //MAX34_Read_8Bit_Register(0xC8, &testing_some);
    //MAX34_Read_8Bit_Register(0x10, &testing_some);
    //MAX34_Read_8Bit_Register(0x18, &testing_some);

    while(1){
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_mS(2000);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_mS(2000);
    }

}

void MAX34_Testing_Write_Function(uint8_t MAX34_Register, uint8_t MAX34_Register_Bit){

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    SPI_I2S_SendData(SPI1, (MAX34_Register));
    (void)SPI_I2S_ReceiveData(SPI1);

    SysTick_Delay_uS(10);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, MAX34_Register_Bit);

    (void)SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void MAX34_Read_8Bit_Register(uint8_t MAX34_Register, uint8_t *Data_Store){

    uint8_t buffer_read = 0;

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    
    SPI_I2S_SendData(SPI1, MAX34_Register);
    
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    buffer_read = SPI_I2S_ReceiveData(SPI1);

    SysTick_Delay_uS(10);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))); //sending null data for spi reading
    SPI_I2S_SendData(SPI1, 0x00);


    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    *Data_Store = SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}






void MAX34_Write_8Bit_Register(uint8_t MAX34_Register, uint8_t MAX34_Register_Bit, uint8_t NRF24_Set_Reset){

    uint8_t buffer_write = 0xFF;
    uint8_t buffer_read = 0;

    MAX34_Read_8Bit_Register(MAX34_Register, &buffer_write);

    if(NRF24_Set_Reset == 1){
        buffer_write |= MAX34_Register_Bit;
    }else{
        buffer_write &=~ MAX34_Register_Bit;
    }

    SysTick_Delay_uS(10);

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    SPI_I2S_SendData(SPI1, (0x02 | MAX34_Register));
    buffer_read = SPI_I2S_ReceiveData(SPI1);
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    buffer_read = SPI_I2S_ReceiveData(SPI1);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, buffer_write);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    buffer_read = SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}





void SysTick_SETUP(void){
    SysTick->LOAD = 9;
    SysTick->VAL = 0;
}

void SysTick_Delay_uS (uint16_t ntime){
    
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    for(int i = ntime - 1 ; i > 0 ; i--){
        while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
    }

    SysTick->CTRL &=~ SysTick_CTRL_ENABLE;
}

void SysTick_Delay_mS (uint16_t ntime){
    
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    for(int i = (ntime * 1000) - 1 ; i > 0 ; i--){
        while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
    }

    SysTick->CTRL &=~ SysTick_CTRL_ENABLE;
}

void GPIO_SETUP(void){

    GPIO_InitTypeDef GPIO_InitStruct_A1;
    GPIO_InitStruct_A1.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct_A1.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct_A1.GPIO_Mode = GPIO_Mode_Out_PP;

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
    GPIO_InitStruct_A4.GPIO_Mode = GPIO_Mode_Out_OD;
    
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

    GPIO_SetBits(GPIOA,GPIO_Pin_1);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

}




void SPI_SETUP(void){

    SPI_InitTypeDef SPI_InitStruct_1;
    SPI_InitStruct_1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct_1.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct_1.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct_1.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct_1.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct_1.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct_1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStruct_1.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct_1.SPI_CRCPolynomial = 7;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    SPI_Init(SPI1,&SPI_InitStruct_1);
    SPI_SSOutputCmd(SPI1,ENABLE);
    SPI_Cmd(SPI1,ENABLE);

}