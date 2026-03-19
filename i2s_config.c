/**
  ******************************************************************************
  * @file    i2s_config.c
  * @brief   I2S and DMA configuration implementation
  ******************************************************************************
  */

#include "i2s_config.h"

/* I2S and DMA handles */
I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

/**
  * @brief  Initialize I2S3 for WM8960 audio output
  */
void WM8960_I2S_Init(void)
{
    /* Enable clocks */
    WM8960_I2S_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Configure I2S pins */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* PA4: I2S3_WS (LRCK) */
    GPIO_InitStruct.Pin = WM8960_I2S_WS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(WM8960_I2S_WS_PORT, &GPIO_InitStruct);
    
    /* PC10: I2S3_CK (SCLK) */
    GPIO_InitStruct.Pin = WM8960_I2S_CK_PIN;
    HAL_GPIO_Init(WM8960_I2S_CK_PORT, &GPIO_InitStruct);
    
    /* PC12: I2S3_SD (DATA) */
    GPIO_InitStruct.Pin = WM8960_I2S_SD_PIN;
    HAL_GPIO_Init(WM8960_I2S_SD_PORT, &GPIO_InitStruct);
    
    /* Configure I2S */
    hi2s3.Instance = WM8960_I2S_INSTANCE;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = WM8960_AUDIO_BIT_DEPTH;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s3.Init.AudioFreq = WM8960_AUDIO_FREQ;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.FirstBit = I2S_FIRSTBIT_MSB;
    hi2s3.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
    hi2s3.Init.IOSwap = I2S_IO_SWAP_DISABLE;
    
    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  Initialize DMA for I2S3
  */
void WM8960_DMA_Init(void)
{
    /* Enable DMA clock */
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    /* Configure DMA */
    hdma_spi3_tx.Instance = WM8960_DMA_STREAM;
    hdma_spi3_tx.Init.Request = DMA_REQUEST_SPI3_TX;
    hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi3_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi3_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* Link DMA to I2S */
    __HAL_LINKDMA(&hi2s3, hdmatx, hdma_spi3_tx);
    
    /* Configure DMA interrupt */
    HAL_NVIC_SetPriority(WM8960_DMA_IRQ, 5, 0);
    HAL_NVIC_EnableIRQ(WM8960_DMA_IRQ);
}

/**
  * @brief  De-initialize I2S
  */
void WM8960_I2S_DeInit(void)
{
    HAL_I2S_DeInit(&hi2s3);
    HAL_GPIO_DeInit(WM8960_I2S_WS_PORT, WM8960_I2S_WS_PIN);
    HAL_GPIO_DeInit(WM8960_I2S_CK_PORT, WM8960_I2S_CK_PIN);
    HAL_GPIO_DeInit(WM8960_I2S_SD_PORT, WM8960_I2S_SD_PIN);
}

/**
  * @brief  DMA IRQ Handler
  */
void DMA1_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi3_tx);
}