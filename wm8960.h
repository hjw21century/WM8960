/**
  ******************************************************************************
  * @file    wm8960.h
  * @brief   WM8960 Audio Codec Driver for STM32H7
  *          I2C Control + I2S Audio Data Interface
  ******************************************************************************
  */

#ifndef __WM8960_H
#define __WM8960_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* WM8960 I2C Address */
#define WM8960_ADDR             0x34  /* (0x1A << 1) */

/* WM8960 Register Map */
#define WM8960_REG_LEFT_INPUT_VOL   0x00
#define WM8960_REG_RIGHT_INPUT_VOL  0x01
#define WM8960_REG_LOUT1_VOL        0x02
#define WM8960_REG_ROUT1_VOL        0x03
#define WM8960_REG_CLOCK1           0x04
#define WM8960_REG_ADC_DAC_CTRL     0x05
#define WM8960_REG_INTERFACE        0x07
#define WM8960_REG_CLOCK2           0x08
#define WM8960_REG_LEFT_DAC_VOL     0x0A
#define WM8960_REG_RIGHT_DAC_VOL    0x0B
#define WM8960_REG_BYPASS1          0x0C
#define WM8960_REG_BYPASS2          0x0D
#define WM8960_REG_PWR_MGMT1        0x19
#define WM8960_REG_PWR_MGMT2        0x1A
#define WM8960_REG_ADD_CTRL1        0x1B
#define WM8960_REG_ADD_CTRL4        0x1E
#define WM8960_REG_LEFT_ADC_VOL     0x15
#define WM8960_REG_RIGHT_ADC_VOL    0x16
#define WM8960_REG_LEFT_OUT_MIX     0x22
#define WM8960_REG_RIGHT_OUT_MIX    0x25
#define WM8960_REG_MONO_OUT_MIX     0x26
#define WM8960_REG_PWR_MGMT3        0x2F
#define WM8960_REG_LEFT_SPEAKER_VOL 0x28
#define WM8960_REG_RIGHT_SPEAKER_VOL 0x29
#define WM8960_REG_CLASS_D_CTRL1    0x31
#define WM8960_REG_CLASS_D_CTRL3    0x33
#define WM8960_REG_RESET            0x0F

/* Audio Sample Rates */
#define WM8960_SAMPLE_RATE_8000     8000
#define WM8960_SAMPLE_RATE_11025    11025
#define WM8960_SAMPLE_RATE_16000    16000
#define WM8960_SAMPLE_RATE_22050    22050
#define WM8960_SAMPLE_RATE_32000    32000
#define WM8960_SAMPLE_RATE_44100    44100
#define WM8960_SAMPLE_RATE_48000    48000
#define WM8960_SAMPLE_RATE_88200    88200
#define WM8960_SAMPLE_RATE_96000    96000

/* Audio Bit Depth */
#define WM8960_BIT_DEPTH_16         16
#define WM8960_BIT_DEPTH_24         24
#define WM8960_BIT_DEPTH_32         32

/* Function Prototypes */
uint8_t WM8960_Init(I2C_HandleTypeDef *hi2c);
uint8_t WM8960_DeInit(void);
uint8_t WM8960_WriteReg(uint8_t reg, uint16_t value);
uint8_t WM8960_ReadReg(uint8_t reg, uint16_t *value);

/* Audio Control Functions */
uint8_t WM8960_SetVolume(uint8_t volume);
uint8_t WM8960_SetMute(uint8_t mute);
uint8_t WM8960_SetSampleRate(uint32_t sample_rate);
uint8_t WM8960_SetBitDepth(uint8_t bit_depth);

/* Power Management */
uint8_t WM8960_PowerOn(void);
uint8_t WM8960_PowerOff(void);
uint8_t WM8960_EnterLowPower(void);
uint8_t WM8960_ExitLowPower(void);

/* Output Control */
uint8_t WM8960_EnableHeadphone(void);
uint8_t WM8960_DisableHeadphone(void);
uint8_t WM8960_EnableSpeaker(void);
uint8_t WM8960_DisableSpeaker(void);

/* Configuration */
void WM8960_SetI2CHandler(I2C_HandleTypeDef *hi2c);
I2C_HandleTypeDef* WM8960_GetI2CHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __WM8960_H */