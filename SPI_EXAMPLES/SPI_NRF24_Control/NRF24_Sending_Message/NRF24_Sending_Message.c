//#define STM32F10X_MD
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

/*Command Registers*/
#define R_REGISTER            ((uint8_t)0x00)
#define W_REGISTER            ((uint8_t)0x20)
#define R_RX_PAYLOAD          ((uint8_t)0x61)
#define W_TX_PAYLOAD          ((uint8_t)0xA0)
#define FLUSH_TX              ((uint8_t)0xE1)
#define FLUSH_RX              ((uint8_t)0xE2)
#define REFUSE_TX_PL          ((uint8_t)0xE3)
#define R_RX_PL_WID           ((uint8_t)0x60)
#define W_ACK_PAYLOAD_0       ((uint8_t)0x98)
#define W_ACK_PAYLOAD_1       ((uint8_t)0x99)
#define W_ACK_PAYLOAD_2       ((uint8_t)0x9A)
#define W_ACK_PAYLOAD_3       ((uint8_t)0x9B)
#define W_ACK_PAYLOAD_4       ((uint8_t)0x9C)
#define W_ACK_PAYLOAD_5       ((uint8_t)0x9D)
#define W_TX_PAYLOAD_NO_ACK   ((uint8_t)0xB0)
#define NRF24_NOP                   ((uint8_t)0xFF)   

/*NRF_Register Addresses*/
#define CONFIG         ((uint8_t)0x00)
#define EN_AA          ((uint8_t)0x01)
#define EN_RXADDR      ((uint8_t)0x02)
#define SETUP_AW       ((uint8_t)0x03)
#define SETUP_RETR     ((uint8_t)0x04)
#define RF_CH          ((uint8_t)0x05)
#define RF_SETUP       ((uint8_t)0x06)
#define STATUS         ((uint8_t)0x07)
#define OBSERVE_TX     ((uint8_t)0x08)
#define RPD            ((uint8_t)0x09)
#define RX_ADDR_P0     ((uint8_t)0x0A)
#define RX_ADDR_P1     ((uint8_t)0x0B)
#define RX_ADDR_P2     ((uint8_t)0x0C)
#define RX_ADDR_P3     ((uint8_t)0x0D)
#define RX_ADDR_P4     ((uint8_t)0x0E)
#define RX_ADDR_P5     ((uint8_t)0x0F)
#define TX_ADDR        ((uint8_t)0x10)
#define RX_PW_P0       ((uint8_t)0x11)
#define RX_PW_P1       ((uint8_t)0x12)
#define RX_PW_P2       ((uint8_t)0x13)
#define RX_PW_P3       ((uint8_t)0x14)
#define RX_PW_P4       ((uint8_t)0x15)
#define RX_PW_P5       ((uint8_t)0x16)
#define FIFO_STATUS    ((uint8_t)0x17)
#define DYNPD          ((uint8_t)0x1C)
#define FEATURE        ((uint8_t)0x1D)

/*Registers*/
#define MASK_RX_DR    ((uint8_t)0x40U)
#define MASK_TX_DS    ((uint8_t)0x20U)
#define MASK_MAX_RT   ((uint8_t)0x10U)
#define EN_CRC        ((uint8_t)0x08U)
#define CRCO          ((uint8_t)0x04U)
#define PWR_UP        ((uint8_t)0x02U)
#define PRIM_RX       ((uint8_t)0x01U)

#define ENAA_P5       ((uint8_t)0x20U)
#define ENAA_P4       ((uint8_t)0x10U)
#define ENAA_P3       ((uint8_t)0x08U)
#define ENAA_P2       ((uint8_t)0x04U)
#define ENAA_P1       ((uint8_t)0x02U)
#define ENAA_P0       ((uint8_t)0x01U)

#define ERX_P5        ((uint8_t)0x20U)
#define ERX_P4        ((uint8_t)0x10U)
#define ERX_P3        ((uint8_t)0x08U)
#define ERX_P2        ((uint8_t)0x04U)
#define ERX_P1        ((uint8_t)0x02U)
#define ERX_P0        ((uint8_t)0x01U)

#define AW_Mask       ((uint8_t)0x03U)
#define AW_3Byte      ((uint8_t)0x01U)
#define AW_4Byte      ((uint8_t)0x02U)
#define AW_5Byte      AW_Mask

#define ARD_Mask     ((uint8_t)0xF0U)
#define ARD_250uS     ((uint8_t)0x00U)
#define ARD_500uS     ((uint8_t)0x10U)
#define ARD_750uS     ((uint8_t)0x20U)
#define ARD_1000uS    ((uint8_t)0x30U)
#define ARD_1250uS    ((uint8_t)0x40U)
#define ARD_1500uS    ((uint8_t)0x50U)
#define ARD_1750uS    ((uint8_t)0x60U)
#define ARD_2000uS    ((uint8_t)0x70U)
#define ARD_2250uS    ((uint8_t)0x80U)
#define ARD_2500uS    ((uint8_t)0x90U)
#define ARD_2750uS    ((uint8_t)0xA0U)
#define ARD_3000uS    ((uint8_t)0xB0U)
#define ARD_3250uS    ((uint8_t)0xC0U)
#define ARD_3500uS    ((uint8_t)0xD0U)
#define ARD_3750uS    ((uint8_t)0xE0U)
#define ARD_4000uS    ((uint8_t)0xF0U)

#define ARC_Mask                 ((uint8_t)0x0FU)
#define ARC_ReTransmit_Disable   ((uint8_t)0x00U)
#define ARC_ReTransmit_1         ((uint8_t)0x01U)
#define ARC_ReTransmit_2         ((uint8_t)0x02U)
#define ARC_ReTransmit_3         ((uint8_t)0x03U)
#define ARC_ReTransmit_4         ((uint8_t)0x04U)
#define ARC_ReTransmit_5         ((uint8_t)0x05U)
#define ARC_ReTransmit_6         ((uint8_t)0x06U)
#define ARC_ReTransmit_7         ((uint8_t)0x07U)
#define ARC_ReTransmit_8         ((uint8_t)0x08U)
#define ARC_ReTransmit_9         ((uint8_t)0x09U)
#define ARC_ReTransmit_10        ((uint8_t)0x0AU)
#define ARC_ReTransmit_11        ((uint8_t)0x0BU)
#define ARC_ReTransmit_12        ((uint8_t)0x0CU)
#define ARC_ReTransmit_13        ((uint8_t)0x0DU)
#define ARC_ReTransmit_14        ((uint8_t)0x0EU)
#define ARC_ReTransmit_15        ((uint8_t)0x0FU)

#define RF_CH_Mask          ((uint8_t)0x7FU)
#define COUNT_WAVE          ((uint8_t)0X80U)
#define RF_DR_LOW           ((uint8_t)0X20U)
#define PLL_LOCK            ((uint8_t)0X10U)
#define RF_DR_HIGH          ((uint8_t)0X08U)
#define RF_PWR_Mask         ((uint8_t)0X06U)
#define RF_PWR_Minus18_dBm  ((uint8_t)0X00U)
#define RF_PWR_Minus12_dBm  ((uint8_t)0X02U)
#define RF_PWR_Minus6_dBm   ((uint8_t)0X04U)
#define RF_PWR_0_dBm        RF_PWR_Mask

#define RX_DR     ((uint8_t)0X40U)
#define TX_DS     ((uint8_t)0X20U)
#define MAX_RT    ((uint8_t)0X10U)
#define RX_P_NO_Mask     ((uint8_t)0X0EU)
#define RX_P_NO_Pipe_1   ((uint8_t)0X02U)
#define RX_P_NO_Pipe_2   ((uint8_t)0X04U)
#define RX_P_NO_Pipe_3   ((uint8_t)0X06U)
#define RX_P_NO_Pipe_4   ((uint8_t)0X08U)
#define RX_P_NO_Pipe_5   ((uint8_t)0X0AU)
#define RX_P_NO_FIFO_EMPTY  ((uint8_t)0X0EU)
#define TX_FULL          ((uint8_t)0x01U)
#define PLOS_CNT         ((uint8_t)0xF0U)
#define ARC_CNT          ((uint8_t)0x0FU)
#define RPD_Mask         ((uint8_t)0x01U)

#define RX_PW_P0_Mask      ((uint8_t)0x1FU)
#define RX_PW_P0_Not_Used  ((uint8_t)0x00U)
#define RX_PW_P0_1       ((uint8_t)0x01U)
#define RX_PW_P0_2       ((uint8_t)0x02U)
#define RX_PW_P0_3       ((uint8_t)0x03U)
#define RX_PW_P0_4       ((uint8_t)0x04U)
#define RX_PW_P0_5       ((uint8_t)0x05U)
#define RX_PW_P0_6       ((uint8_t)0x06U)
#define RX_PW_P0_7       ((uint8_t)0x07U)
#define RX_PW_P0_8       ((uint8_t)0x08U)
#define RX_PW_P0_9       ((uint8_t)0x09U)
#define RX_PW_P0_10      ((uint8_t)0x0AU)
#define RX_PW_P0_11      ((uint8_t)0x0BU)
#define RX_PW_P0_12      ((uint8_t)0x0CU)
#define RX_PW_P0_13      ((uint8_t)0x0DU)
#define RX_PW_P0_14      ((uint8_t)0x0EU)
#define RX_PW_P0_15      ((uint8_t)0x0FU)
#define RX_PW_P0_16      ((uint8_t)0x10U)
#define RX_PW_P0_17      ((uint8_t)0x11U)
#define RX_PW_P0_18      ((uint8_t)0x12U)
#define RX_PW_P0_19      ((uint8_t)0x13U)
#define RX_PW_P0_20      ((uint8_t)0x14U)
#define RX_PW_P0_21      ((uint8_t)0x15U)
#define RX_PW_P0_22      ((uint8_t)0x16U)
#define RX_PW_P0_23      ((uint8_t)0x17U)
#define RX_PW_P0_24      ((uint8_t)0x18U)
#define RX_PW_P0_25      ((uint8_t)0x19U)
#define RX_PW_P0_26      ((uint8_t)0x1AU)
#define RX_PW_P0_27      ((uint8_t)0x1BU)
#define RX_PW_P0_28      ((uint8_t)0x1CU)
#define RX_PW_P0_29      ((uint8_t)0x1DU)
#define RX_PW_P0_30      ((uint8_t)0x1EU)
#define RX_PW_P0_31      RX_PW_P0_Mask

#define RX_PW_P1_Mask       ((uint8_t)0x1FU)
#define RX_PW_P1_Not_Used   ((uint8_t)0x00U)
#define RX_PW_P1_1       ((uint8_t)0x01U)
#define RX_PW_P1_2       ((uint8_t)0x02U)
#define RX_PW_P1_3       ((uint8_t)0x03U)
#define RX_PW_P1_4       ((uint8_t)0x04U)
#define RX_PW_P1_5       ((uint8_t)0x05U)
#define RX_PW_P1_6       ((uint8_t)0x06U)
#define RX_PW_P1_7       ((uint8_t)0x07U)
#define RX_PW_P1_8       ((uint8_t)0x08U)
#define RX_PW_P1_9       ((uint8_t)0x09U)
#define RX_PW_P1_10      ((uint8_t)0x0AU)
#define RX_PW_P1_11      ((uint8_t)0x0BU)
#define RX_PW_P1_12      ((uint8_t)0x0CU)
#define RX_PW_P1_13      ((uint8_t)0x0DU)
#define RX_PW_P1_14      ((uint8_t)0x0EU)
#define RX_PW_P1_15      ((uint8_t)0x0FU)
#define RX_PW_P1_16      ((uint8_t)0x10U)
#define RX_PW_P1_17      ((uint8_t)0x11U)
#define RX_PW_P1_18      ((uint8_t)0x12U)
#define RX_PW_P1_19      ((uint8_t)0x13U)
#define RX_PW_P1_20      ((uint8_t)0x14U)
#define RX_PW_P1_21      ((uint8_t)0x15U)
#define RX_PW_P1_22      ((uint8_t)0x16U)
#define RX_PW_P1_23      ((uint8_t)0x17U)
#define RX_PW_P1_24      ((uint8_t)0x18U)
#define RX_PW_P1_25      ((uint8_t)0x19U)
#define RX_PW_P1_26      ((uint8_t)0x1AU)
#define RX_PW_P1_27      ((uint8_t)0x1BU)
#define RX_PW_P1_28      ((uint8_t)0x1CU)
#define RX_PW_P1_29      ((uint8_t)0x1DU)
#define RX_PW_P1_30      ((uint8_t)0x1EU)
#define RX_PW_P1_31      RX_PW_P0_Mask

#define RX_PW_P2_Mask       ((uint8_t)0x1FU)
#define RX_PW_P2_Not_Used   ((uint8_t)0x00U)
#define RX_PW_P2_1       ((uint8_t)0x01U)
#define RX_PW_P2_2       ((uint8_t)0x02U)
#define RX_PW_P2_3       ((uint8_t)0x03U)
#define RX_PW_P2_4       ((uint8_t)0x04U)
#define RX_PW_P2_5       ((uint8_t)0x05U)
#define RX_PW_P2_6       ((uint8_t)0x06U)
#define RX_PW_P2_7       ((uint8_t)0x07U)
#define RX_PW_P2_8       ((uint8_t)0x08U)
#define RX_PW_P2_9       ((uint8_t)0x09U)
#define RX_PW_P2_10      ((uint8_t)0x0AU)
#define RX_PW_P2_11      ((uint8_t)0x0BU)
#define RX_PW_P2_12      ((uint8_t)0x0CU)
#define RX_PW_P2_13      ((uint8_t)0x0DU)
#define RX_PW_P2_14      ((uint8_t)0x0EU)
#define RX_PW_P2_15      ((uint8_t)0x0FU)
#define RX_PW_P2_16      ((uint8_t)0x10U)
#define RX_PW_P2_17      ((uint8_t)0x11U)
#define RX_PW_P2_18      ((uint8_t)0x12U)
#define RX_PW_P2_19      ((uint8_t)0x13U)
#define RX_PW_P2_20      ((uint8_t)0x14U)
#define RX_PW_P2_21      ((uint8_t)0x15U)
#define RX_PW_P2_22      ((uint8_t)0x16U)
#define RX_PW_P2_23      ((uint8_t)0x17U)
#define RX_PW_P2_24      ((uint8_t)0x18U)
#define RX_PW_P2_25      ((uint8_t)0x19U)
#define RX_PW_P2_26      ((uint8_t)0x1AU)
#define RX_PW_P2_27      ((uint8_t)0x1BU)
#define RX_PW_P2_28      ((uint8_t)0x1CU)
#define RX_PW_P2_29      ((uint8_t)0x1DU)
#define RX_PW_P2_30      ((uint8_t)0x1EU)
#define RX_PW_P2_31      RX_PW_P0_Mask

#define RX_PW_P3_Mask      ((uint8_t)0x1FU)
#define RX_PW_P3_Not_Used  ((uint8_t)0x00U)
#define RX_PW_P3_1       ((uint8_t)0x01U)
#define RX_PW_P3_2       ((uint8_t)0x02U)
#define RX_PW_P3_3       ((uint8_t)0x03U)
#define RX_PW_P3_4       ((uint8_t)0x04U)
#define RX_PW_P3_5       ((uint8_t)0x05U)
#define RX_PW_P3_6       ((uint8_t)0x06U)
#define RX_PW_P3_7       ((uint8_t)0x07U)
#define RX_PW_P3_8       ((uint8_t)0x08U)
#define RX_PW_P3_9       ((uint8_t)0x09U)
#define RX_PW_P3_10      ((uint8_t)0x0AU)
#define RX_PW_P3_11      ((uint8_t)0x0BU)
#define RX_PW_P3_12      ((uint8_t)0x0CU)
#define RX_PW_P3_13      ((uint8_t)0x0DU)
#define RX_PW_P3_14      ((uint8_t)0x0EU)
#define RX_PW_P3_15      ((uint8_t)0x0FU)
#define RX_PW_P3_16      ((uint8_t)0x10U)
#define RX_PW_P3_17      ((uint8_t)0x11U)
#define RX_PW_P3_18      ((uint8_t)0x12U)
#define RX_PW_P3_19      ((uint8_t)0x13U)
#define RX_PW_P3_20      ((uint8_t)0x14U)
#define RX_PW_P3_21      ((uint8_t)0x15U)
#define RX_PW_P3_22      ((uint8_t)0x16U)
#define RX_PW_P3_23      ((uint8_t)0x17U)
#define RX_PW_P3_24      ((uint8_t)0x18U)
#define RX_PW_P3_25      ((uint8_t)0x19U)
#define RX_PW_P3_26      ((uint8_t)0x1AU)
#define RX_PW_P3_27      ((uint8_t)0x1BU)
#define RX_PW_P3_28      ((uint8_t)0x1CU)
#define RX_PW_P3_29      ((uint8_t)0x1DU)
#define RX_PW_P3_30      ((uint8_t)0x1EU)
#define RX_PW_P3_31      RX_PW_P0_Mask

#define RX_PW_P4_Mask      ((uint8_t)0x1FU)
#define RX_PW_P4_Not_Used  ((uint8_t)0x00U)
#define RX_PW_P4_1       ((uint8_t)0x01U)
#define RX_PW_P4_2       ((uint8_t)0x02U)
#define RX_PW_P4_3       ((uint8_t)0x03U)
#define RX_PW_P4_4       ((uint8_t)0x04U)
#define RX_PW_P4_5       ((uint8_t)0x05U)
#define RX_PW_P4_6       ((uint8_t)0x06U)
#define RX_PW_P4_7       ((uint8_t)0x07U)
#define RX_PW_P4_8       ((uint8_t)0x08U)
#define RX_PW_P4_9       ((uint8_t)0x09U)
#define RX_PW_P4_10      ((uint8_t)0x0AU)
#define RX_PW_P4_11      ((uint8_t)0x0BU)
#define RX_PW_P4_12      ((uint8_t)0x0CU)
#define RX_PW_P4_13      ((uint8_t)0x0DU)
#define RX_PW_P4_14      ((uint8_t)0x0EU)
#define RX_PW_P4_15      ((uint8_t)0x0FU)
#define RX_PW_P4_16      ((uint8_t)0x10U)
#define RX_PW_P4_17      ((uint8_t)0x11U)
#define RX_PW_P4_18      ((uint8_t)0x12U)
#define RX_PW_P4_19      ((uint8_t)0x13U)
#define RX_PW_P4_20      ((uint8_t)0x14U)
#define RX_PW_P4_21      ((uint8_t)0x15U)
#define RX_PW_P4_22      ((uint8_t)0x16U)
#define RX_PW_P4_23      ((uint8_t)0x17U)
#define RX_PW_P4_24      ((uint8_t)0x18U)
#define RX_PW_P4_25      ((uint8_t)0x19U)
#define RX_PW_P4_26      ((uint8_t)0x1AU)
#define RX_PW_P4_27      ((uint8_t)0x1BU)
#define RX_PW_P4_28      ((uint8_t)0x1CU)
#define RX_PW_P4_29      ((uint8_t)0x1DU)
#define RX_PW_P4_30      ((uint8_t)0x1EU)
#define RX_PW_P4_31      RX_PW_P0_Mask

#define RX_PW_P5_Mask      ((uint8_t)0x1FU)
#define RX_PW_P5_Not_Used  ((uint8_t)0x00U)
#define RX_PW_P5_1       ((uint8_t)0x01U)
#define RX_PW_P5_2       ((uint8_t)0x02U)
#define RX_PW_P5_3       ((uint8_t)0x03U)
#define RX_PW_P5_4       ((uint8_t)0x04U)
#define RX_PW_P5_5       ((uint8_t)0x05U)
#define RX_PW_P5_6       ((uint8_t)0x06U)
#define RX_PW_P5_7       ((uint8_t)0x07U)
#define RX_PW_P5_8       ((uint8_t)0x08U)
#define RX_PW_P5_9       ((uint8_t)0x09U)
#define RX_PW_P5_10      ((uint8_t)0x0AU)
#define RX_PW_P5_11      ((uint8_t)0x0BU)
#define RX_PW_P5_12      ((uint8_t)0x0CU)
#define RX_PW_P5_13      ((uint8_t)0x0DU)
#define RX_PW_P5_14      ((uint8_t)0x0EU)
#define RX_PW_P5_15      ((uint8_t)0x0FU)
#define RX_PW_P5_16      ((uint8_t)0x10U)
#define RX_PW_P5_17      ((uint8_t)0x11U)
#define RX_PW_P5_18      ((uint8_t)0x12U)
#define RX_PW_P5_19      ((uint8_t)0x13U)
#define RX_PW_P5_20      ((uint8_t)0x14U)
#define RX_PW_P5_21      ((uint8_t)0x15U)
#define RX_PW_P5_22      ((uint8_t)0x16U)
#define RX_PW_P5_23      ((uint8_t)0x17U)
#define RX_PW_P5_24      ((uint8_t)0x18U)
#define RX_PW_P5_25      ((uint8_t)0x19U)
#define RX_PW_P5_26      ((uint8_t)0x1AU)
#define RX_PW_P5_27      ((uint8_t)0x1BU)
#define RX_PW_P5_28      ((uint8_t)0x1CU)
#define RX_PW_P5_29      ((uint8_t)0x1DU)
#define RX_PW_P5_30      ((uint8_t)0x1EU)
#define RX_PW_P5_31      RX_PW_P0_Mask

#define TX_REUSE         ((uint8_t)0x40U)
#define TX_FULL_FIFO_STATUS  ((uint8_t)0x20U)
#define TX_EMPTY         ((uint8_t)0x10U)
#define RX_FULL          ((uint8_t)0x02U)
#define RX_EMPTY         ((uint8_t)0x01U)
#define DPL_P5           ((uint8_t)0x20U)
#define DPL_P4           ((uint8_t)0x10U)
#define DPL_P3           ((uint8_t)0x08U)
#define DPL_P2           ((uint8_t)0x04U)
#define DPL_P1           ((uint8_t)0x02U)
#define DPL_P0           ((uint8_t)0x01U)
#define EN_DPL           ((uint8_t)0x04U)
#define EN_ACK_PAY       ((uint8_t)0x02U)
#define EN_DYN_ACK       ((uint8_t)0x01U)

volatile uint32_t ntime = 0;


/*============================================================================
============================RX_DATA_PIPES======================================
============================================================================*/

static uint8_t RX_ADDR_0_DataPipe_Value[5] = {0xE7U,0xE7U,0xE7U,0xE7U,0xE7U};
//static uint8_t RX_ADDR_1_DataPipe_Value[5] = {0xC2U,0xC2U,0xC2U,0xC2U,0xC2U};
//static uint8_t RX_ADDR_2_Value = 0x0CU;
//static uint8_t RX_ADDR_3_Value = 0x0DU;
//static uint8_t RX_ADDR_4_Value = 0x0EU;
//static uint8_t RX_ADDR_5_Value = 0x0FU;
//static uint8_t TX_ADDR_Value[5] = {0xE7U,0xE7U,0xE7U,0xE7U,0xE7U};

//volatile uint8_t NRF_Read_Payload[32] = {0};
//volatile uint8_t NRF_Write_Payload[32] = {0};


void GPIO_SETUP(void);
//void EXTI_SETUP(void);
void SPI_SETUP(void);
void USART_SETUP(void);
void SysTick_SETUP(void);


void SysTick_Delay_uS(uint16_t ntime);
void SysTick_Delay_mS (uint16_t ntime);

void NRF24_Reading_Status_Register(uint8_t *NRF_Status);
void NRF_Testing_Write_Function(uint8_t NRF24_Register, uint8_t NRF24_Register_Bit);
void NRF24_Write_8Bit_Register(uint8_t NRF24_Register, uint8_t NRF24_Register_Bit, uint8_t NRF24_Set_Reset);
void NRF24_Read_8Bit_Register(uint8_t NRF24_Register, uint8_t *Data_Store);
void NRF24_40Bit_Write_Register(uint8_t NRF_Register, uint8_t *Register_Data);
void NRF24_40Bit_Read_Register(uint8_t NRF_Register, uint8_t *Read_Register_Data);
void NRF_Payload_Write(uint8_t *NRF_Payload, uint8_t len);






int main(void){

    __disable_irq();
    NVIC_EnableIRQ(EXTI0_IRQn);
    __enable_irq();

    GPIO_SETUP();
    //EXTI_SETUP();
    SPI_SETUP();
    USART_SETUP();
    SysTick_SETUP();

    uint8_t testing_some = 0;
    uint8_t testing_array[5] = {0xAA, 0xBB,0xCC, 0xDD, 0xEE};
    //uint8_t testing_array01[5] = {0};

    SysTick_Delay_mS(4000);
    NRF24_Write_8Bit_Register(FEATURE, EN_DPL | EN_ACK_PAY , 1);
    NRF24_Write_8Bit_Register(FEATURE, EN_DYN_ACK, 0);
    
    while(1){
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_mS(2000);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_mS(2000);
    }
}


/*===========================================================================
============================NRF24 FUNCTIONS==================================
=============================================================================*/

void NRF24_Reading_Status_Register(uint8_t *NRF_Status){

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, NRF24_NOP);
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    *NRF_Status = SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
}




void NRF_Testing_Write_Function(uint8_t NRF24_Register, uint8_t NRF24_Register_Bit){

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, (0x20 | NRF24_Register));
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    (void)SPI_I2S_ReceiveData(SPI1);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, NRF24_Register_Bit);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    (void)SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}






void NRF24_Read_8Bit_Register(uint8_t NRF24_Register, uint8_t *Data_Store){

    volatile uint8_t buffer_read = 0;

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    
    SPI_I2S_SendData(SPI1, NRF24_Register);
    
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    buffer_read = SPI_I2S_ReceiveData(SPI1);


    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))); //sending null data for spi reading
    SPI_I2S_SendData(SPI1, 0x00);


    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));
    *Data_Store = SPI_I2S_ReceiveData(SPI1);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}






void NRF24_Write_8Bit_Register(uint8_t NRF24_Register, uint8_t NRF24_Register_Bit, uint8_t NRF24_Set_Reset){

    uint8_t buffer_write = 0;
    uint8_t volatile buffer_read = 0;

    NRF24_Read_8Bit_Register(NRF24_Register, &buffer_write);

    if(NRF24_Set_Reset == 1){
        buffer_write |= NRF24_Register_Bit;
    }else{
        buffer_write &=~ NRF24_Register_Bit;
    }

    SysTick_Delay_uS(10);

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, (0x20 | NRF24_Register));
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






void NRF24_40Bit_Write_Register(uint8_t NRF_Register, uint8_t *Register_Data){

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, (0x20 | NRF_Register));
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    (void)SPI_I2S_ReceiveData(SPI1);

    for(uint8_t i = 5; i >= 1 ; i--){
        while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
        SPI_I2S_SendData(SPI1,Register_Data[i - 1]);

        while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
        (void)SPI_I2S_ReceiveData(SPI1);
    }

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}






void NRF24_40Bit_Read_Register(uint8_t NRF_Register, uint8_t *Read_Register_Data){

    uint8_t buffer_read = 0;

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, NRF_Register);
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    buffer_read = SPI_I2S_ReceiveData(SPI1);

    for(uint8_t i = 5; i >= 1 ; i--){
        while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
        SPI_I2S_SendData(SPI1,0x00);

        while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
        Read_Register_Data[i - 1] = SPI_I2S_ReceiveData(SPI1);
    }

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);
}







void NRF_Payload_Write(uint8_t *NRF_Payload, uint8_t len){

    GPIO_ResetBits(GPIOA,GPIO_Pin_4);

    while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, W_TX_PAYLOAD_NO_ACK);
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    (void)SPI_I2S_ReceiveData(SPI1);

    for (uint8_t i = len ; i >= 1; i--){

        while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
        SPI_I2S_SendData(SPI1, NRF_Payload[i -1]);

        while (!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
        (void)SPI_I2S_ReceiveData(SPI1);
    }

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    SysTick_Delay_uS(20);

}


/*==================================================================
=======================INTERRUPT FUNCTIONS==========================
====================================================================*/

void EXTI0_IRQHandler(void){
    uint8_t buffer_read = 0;
    
    NRF24_Reading_Status_Register(&buffer_read);


}



/*==================================================================
==========================DELAY FUNCTIONS===========================
====================================================================*/

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



/*==================================================================
==========================SETUP FUNCTIONS===========================
====================================================================*/

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

    GPIO_InitTypeDef GPIO_InitStruct_B1;
    GPIO_InitStruct_B0.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct_B0.GPIO_Speed = GPIO_Speed_50MHz;
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
    GPIO_Init(GPIOB,&GPIO_InitStruct_B1);
    GPIO_Init(GPIOC,&GPIO_InitStruct_C13);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);

    GPIO_SetBits(GPIOA,GPIO_Pin_1);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

}

void EXTI_SETUP(void){

    EXTI_InitTypeDef EXTI_InitStruct_B0;
    EXTI_InitStruct_B0.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct_B0.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct_B0.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct_B0.EXTI_LineCmd = ENABLE;
    
    EXTI_Init(&EXTI_InitStruct_B0);

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
    //USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    //NVIC_EnableIRQ(USART2_IRQn);
    USART_Cmd(USART2,ENABLE);
}