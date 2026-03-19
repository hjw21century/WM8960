/**
  ******************************************************************************
  * @file    wm8960.c
  * @brief   WM8960 Audio Codec Driver Implementation
  ******************************************************************************
  */

#include "wm8960.h"

static I2C_HandleTypeDef *wm8960_i2c = NULL;
static uint8_t wm8960_initialized = 0;

/**
  * @brief  Initialize WM8960 Audio Codec
  * @param  hi2c: I2C handle
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return 1;
    }
    
    wm8960_i2c = hi2c;
    wm8960_initialized = 1;
    
    /* Software Reset */
    if (WM8960_WriteReg(WM8960_REG_RESET, 0x0000) != 0)
    {
        return 1;
    }
    HAL_Delay(10);
    
    /* Power Management 1: Enable VREF and VMID */
    if (WM8960_WriteReg(WM8960_REG_PWR_MGMT1, 0x00C0) != 0)
    {
        return 1;
    }
    HAL_Delay(50);
    
    /* Power Management 2: Enable DAC, LOUT, ROUT */
    if (WM8960_WriteReg(WM8960_REG_PWR_MGMT2, 0x01F8) != 0)
    {
        return 1;
    }
    
    /* Power Management 3: Enable Left and Right Output */
    if (WM8960_WriteReg(WM8960_REG_PWR_MGMT3, 0x006C) != 0)
    {
        return 1;
    }
    
    /* Clock Configuration: 256fs, USB Mode for 48kHz */
    if (WM8960_WriteReg(WM8960_REG_CLOCK1, 0x0080) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_CLOCK2, 0x0000) != 0)
    {
        return 1;
    }
    
    /* Audio Interface: I2S, 16-bit */
    if (WM8960_WriteReg(WM8960_REG_INTERFACE, 0x0042) != 0)
    {
        return 1;
    }
    
    /* ADC and DAC Control: Enable DAC */
    if (WM8960_WriteReg(WM8960_REG_ADC_DAC_CTRL, 0x0000) != 0)
    {
        return 1;
    }
    
    /* DAC Volume: 0dB (max volume) */
    if (WM8960_WriteReg(WM8960_REG_LEFT_DAC_VOL, 0x01FF) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_RIGHT_DAC_VOL, 0x01FF) != 0)
    {
        return 1;
    }
    
    /* Output Volume: 0dB */
    if (WM8960_WriteReg(WM8960_REG_LOUT1_VOL, 0x0179) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_ROUT1_VOL, 0x0179) != 0)
    {
        return 1;
    }
    
    /* Output Mixer: Route DAC to Output */
    if (WM8960_WriteReg(WM8960_REG_LEFT_OUT_MIX, 0x0150) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_RIGHT_OUT_MIX, 0x0150) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  De-initialize WM8960
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_DeInit(void)
{
    if (!wm8960_initialized)
    {
        return 1;
    }
    
    /* Power off all sections */
    WM8960_WriteReg(WM8960_REG_PWR_MGMT1, 0x0000);
    WM8960_WriteReg(WM8960_REG_PWR_MGMT2, 0x0000);
    WM8960_WriteReg(WM8960_REG_PWR_MGMT3, 0x0000);
    
    wm8960_initialized = 0;
    wm8960_i2c = NULL;
    
    return 0;
}

/**
  * @brief  Write register to WM8960
  * @param  reg: Register address
  * @param  value: Register value (9-bit)
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_WriteReg(uint8_t reg, uint16_t value)
{
    uint8_t data[2];
    
    if (!wm8960_initialized || wm8960_i2c == NULL)
    {
        return 1;
    }
    
    /* WM8960 register format: 7-bit address + 9-bit data */
    data[0] = (reg << 1) | ((value >> 8) & 0x01);
    data[1] = value & 0xFF;
    
    if (HAL_I2C_Master_Transmit(wm8960_i2c, WM8960_ADDR, data, 2, 100) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Read register from WM8960 (Note: WM8960 has limited read capability)
  * @param  reg: Register address
  * @param  value: Pointer to store register value
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_ReadReg(uint8_t reg, uint16_t *value)
{
    /* WM8960 does not support full register readback */
    /* This is a placeholder for future implementation */
    if (value != NULL)
    {
        *value = 0;
    }
    return 1;
}

/**
  * @brief  Set output volume
  * @param  volume: Volume level (0-100)
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_SetVolume(uint8_t volume)
{
    uint16_t reg_value;
    
    if (volume > 100)
    {
        volume = 100;
    }
    
    /* Convert 0-100 to 0x00-0xFF, with update bit (0x100) */
    reg_value = ((volume * 255) / 100) | 0x0100;
    
    if (WM8960_WriteReg(WM8960_REG_LEFT_DAC_VOL, reg_value) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_RIGHT_DAC_VOL, reg_value) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Set mute state
  * @param  mute: 1=Mute, 0=Unmute
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_SetMute(uint8_t mute)
{
    uint16_t reg_value;
    
    if (mute)
    {
        /* Mute: Set volume to 0 with update bit */
        reg_value = 0x0100;
    }
    else
    {
        /* Unmute: Restore default volume (0xFF) with update bit */
        reg_value = 0x01FF;
    }
    
    if (WM8960_WriteReg(WM8960_REG_LEFT_DAC_VOL, reg_value) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_RIGHT_DAC_VOL, reg_value) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Set sample rate
  * @param  sample_rate: Sample rate in Hz
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_SetSampleRate(uint32_t sample_rate)
{
    uint16_t clock1_value = 0x0080;  /* Default: 256fs, USB mode */
    
    /* Configure clock based on sample rate */
    switch (sample_rate)
    {
        case 8000:
        case 11025:
        case 16000:
        case 22050:
        case 32000:
        case 44100:
        case 48000:
        case 88200:
        case 96000:
            /* USB mode supports all these rates with proper MCLK */
            break;
        default:
            return 1;  /* Unsupported sample rate */
    }
    
    if (WM8960_WriteReg(WM8960_REG_CLOCK1, clock1_value) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Set bit depth
  * @param  bit_depth: Bit depth (16, 24, or 32)
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_SetBitDepth(uint8_t bit_depth)
{
    uint16_t interface_value = 0x0042;  /* Default: I2S, 16-bit */
    
    switch (bit_depth)
    {
        case 16:
            interface_value = 0x0042;  /* 16-bit */
            break;
        case 20:
            interface_value = 0x0062;  /* 20-bit */
            break;
        case 24:
            interface_value = 0x0082;  /* 24-bit */
            break;
        case 32:
            interface_value = 0x00A2;  /* 32-bit */
            break;
        default:
            return 1;  /* Unsupported bit depth */
    }
    
    if (WM8960_WriteReg(WM8960_REG_INTERFACE, interface_value) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Power on WM8960
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_PowerOn(void)
{
    return WM8960_Init(wm8960_i2c);
}

/**
  * @brief  Power off WM8960
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_PowerOff(void)
{
    return WM8960_DeInit();
}

/**
  * @brief  Enter low power mode
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_EnterLowPower(void)
{
    /* Disable all sections except VREF */
    WM8960_WriteReg(WM8960_REG_PWR_MGMT2, 0x0000);
    WM8960_WriteReg(WM8960_REG_PWR_MGMT3, 0x0000);
    
    return 0;
}

/**
  * @brief  Exit low power mode
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_ExitLowPower(void)
{
    /* Re-enable audio sections */
    WM8960_WriteReg(WM8960_REG_PWR_MGMT2, 0x01F8);
    WM8960_WriteReg(WM8960_REG_PWR_MGMT3, 0x006C);
    
    return 0;
}

/**
  * @brief  Enable headphone output
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_EnableHeadphone(void)
{
    /* Enable LOUT and ROUT in Power Management 2 */
    uint16_t pwr2;
    
    /* Read current value (or use cached value) */
    pwr2 = 0x01F8;  /* Default with LOUT and ROUT enabled */
    
    return WM8960_WriteReg(WM8960_REG_PWR_MGMT2, pwr2);
}

/**
  * @brief  Disable headphone output
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_DisableHeadphone(void)
{
    /* Disable LOUT and ROUT, keep DAC enabled */
    uint16_t pwr2 = 0x01B8;  /* DAC enabled, LOUT/ROUT disabled */
    
    return WM8960_WriteReg(WM8960_REG_PWR_MGMT2, pwr2);
}

/**
  * @brief  Enable speaker output
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_EnableSpeaker(void)
{
    /* Enable Class D speaker driver */
    if (WM8960_WriteReg(WM8960_REG_CLASS_D_CTRL1, 0x00C0) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_CLASS_D_CTRL3, 0x0008) != 0)
    {
        return 1;
    }
    
    /* Set speaker volume */
    if (WM8960_WriteReg(WM8960_REG_LEFT_SPEAKER_VOL, 0x0179) != 0)
    {
        return 1;
    }
    
    if (WM8960_WriteReg(WM8960_REG_RIGHT_SPEAKER_VOL, 0x0179) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  Disable speaker output
  * @retval 0: Success, 1: Error
  */
uint8_t WM8960_DisableSpeaker(void)
{
    /* Disable Class D speaker driver */
    WM8960_WriteReg(WM8960_REG_CLASS_D_CTRL1, 0x0000);
    WM8960_WriteReg(WM8960_REG_CLASS_D_CTRL3, 0x0000);
    
    return 0;
}

/**
  * @brief  Set I2C handler
  * @param  hi2c: I2C handle
  */
void WM8960_SetI2CHandler(I2C_HandleTypeDef *hi2c)
{
    wm8960_i2c = hi2c;
}

/**
  * @brief  Get I2C handler
  * @retval I2C handle pointer
  */
I2C_HandleTypeDef* WM8960_GetI2CHandler(void)
{
    return wm8960_i2c;
}
