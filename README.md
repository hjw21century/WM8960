# WM8960 Audio Codec Driver for STM32H7

基于 STM32H743 + WM8960 的音频播放驱动程序，使用 I2C 控制 + I2S DMA 传输。

## 硬件连接

| STM32H743 | WM8960 | 功能 |
|-----------|--------|------|
| PB6 | SCL | I2C 时钟 |
| PB7 | SDA | I2C 数据 |
| PA4 | LRCK | I2S 左右声道时钟 |
| PC10 | SCLK | I2S 位时钟 |
| PC12 | SD | I2S 数据 |

## 文件说明

| 文件 | 说明 |
|------|------|
| `wm8960.h/c` | WM8960 驱动程序 |
| `audio_player.h` | 音频播放器接口 |
| `main.c` | 示例主程序 |

## CubeMX 配置

### I2C1 配置
- Mode: I2C
- Speed: Standard Mode (100kHz)
- PB6: I2C1_SCL
- PB7: I2C1_SDA

### I2S3 配置
- Mode: Master Transmit
- Standard: Phillips
- Data Format: 16 bits on 16 bits
- Audio Frequency: 48kHz
- PA4: I2S3_WS
- PC10: I2S3_CK
- PC12: I2S3_SD

### DMA 配置
- I2S3_TX: DMA1 Stream 4
- Direction: Memory to Peripheral
- Mode: Circular

## 使用方法

```c
#include "wm8960.h"

/* 初始化 */
WM8960_Init(&hi2c1);

/* 设置音量 (0-100) */
WM8960_SetVolume(80);

/* 播放音频 */
HAL_I2S_Transmit_DMA(&hi2s3, audio_buffer, buffer_size);
```

## 函数接口

### 初始化
- `WM8960_Init(I2C_HandleTypeDef *hi2c)` - 初始化 WM8960
- `WM8960_DeInit()` - 反初始化

### 音量控制
- `WM8960_SetVolume(uint8_t volume)` - 设置音量 (0-100)
- `WM8960_SetMute(uint8_t mute)` - 静音控制

### 输出控制
- `WM8960_EnableHeadphone()` - 使能耳机输出
- `WM8960_EnableSpeaker()` - 使能扬声器输出

## 注意事项

1. 需要替换 `main.c` 中的 `WaveData` 为真实的音频数据
2. 音频格式：16-bit PCM, 48kHz, Stereo
3. 确保 CubeMX 正确配置了 I2S 和 DMA

## 作者

STM32H743 + WM8960 音频驱动开发