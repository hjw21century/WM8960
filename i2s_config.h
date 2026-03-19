/**
  ******************************************************************************
  * @file    i2s_config.h
  * @brief   I2S and DMA configuration for WM8960
  ******************************************************************************
  */

#ifndef __I2S_CONFIG_H
#define __I2S_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* I2S3 Configuration */
#define WM8960_I2S_INSTANCE     SPI3
#define WM8960_I2S_CLK_ENABLE() __HAL_RCC_SPI3_CLK_ENABLE()

/* I2S3 Pins */
#define WM8960_I2S_WS_PIN       GPIO_PIN_4
#define WM8960_I2S_WS_PORT      GPIOA
#define WM8960_I2S_CK_PIN       GPIO_PIN_10
#define WM8960_I2S_CK_PORT      GPIOC
#define WM8960_I2S_SD_PIN       GPIO_PIN_12
#define WM8960_I2S_SD_PORT      GPIOC

/* DMA Configuration */
#define WM8960_DMA_INSTANCE     DMA1
#define WM8960_DMA_STREAM       DMA1_Stream4
#define WM8960_DMA_CHANNEL      DMA_CHANNEL_3
#define WM8960_DMA_IRQ          DMA1_Stream4_IRQn

/* Audio Parameters */
#define WM8960_AUDIO_FREQ       I2S_AUDIOFREQ_48K
#define WM8960_AUDIO_BIT_DEPTH  I2S_DATAFORMAT_16B

/* Function Prototypes */
void WM8960_I2S_Init(void);
void WM8960_DMA_Init(void);
void WM8960_I2S_DeInit(void);

/* External variables */
extern I2S_HandleTypeDef hi2s3;
extern DMA_HandleTypeDef hdma_spi3_tx;

#ifdef __cplusplus
}
#endif

#endif /* __I2S_CONFIG_H */