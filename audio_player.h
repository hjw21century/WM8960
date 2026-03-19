/**
  ******************************************************************************
  * @file    audio_player.h
  * @brief   Audio Player using WM8960 and I2S DMA
  ******************************************************************************
  */

#ifndef __AUDIO_PLAYER_H
#define __AUDIO_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "wm8960.h"

/* Audio Configuration */
#define AUDIO_BUFFER_SIZE       4096    /* DMA buffer size in samples */
#define AUDIO_BUFFER_HALF       (AUDIO_BUFFER_SIZE / 2)

/* Audio State */
typedef enum
{
    AUDIO_STATE_IDLE = 0,
    AUDIO_STATE_PLAYING,
    AUDIO_STATE_PAUSED,
    AUDIO_STATE_STOPPED,
    AUDIO_STATE_ERROR
} AudioState_t;

/* Audio Format Structure */
typedef struct
{
    uint32_t SampleRate;
    uint8_t  BitDepth;
    uint8_t  Channels;
} AudioFormat_t;

/* Function Prototypes */

/* Initialization */
uint8_t Audio_Init(I2C_HandleTypeDef *hi2c, I2S_HandleTypeDef *hi2s, DMA_HandleTypeDef *hdma);
uint8_t Audio_DeInit(void);

/* Playback Control */
uint8_t Audio_Play(const uint16_t *data, uint32_t length);
uint8_t Audio_Stop(void);
uint8_t Audio_Pause(void);
uint8_t Audio_Resume(void);

/* Volume Control */
uint8_t Audio_SetVolume(uint8_t volume);
uint8_t Audio_GetVolume(void);
uint8_t Audio_SetMute(uint8_t mute);

/* Status */
AudioState_t Audio_GetState(void);
uint32_t Audio_GetProgress(void);
uint32_t Audio_GetTotalLength(void);

/* Callbacks (weak, can be overridden) */
void Audio_HalfTransferCallback(void);
void Audio_TransferCompleteCallback(void);
void Audio_ErrorCallback(void);

/* DMA Buffer Management */
void Audio_FillBuffer(uint16_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_PLAYER_H */