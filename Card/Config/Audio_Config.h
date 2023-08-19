// -------------------------------------------------------------------------
// Audio Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __AUDIO_CONFIG_H__
#define __AUDIO_CONFIG_H__
// -------------------------------------------------------------------------
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define AUDIO_USE_BUZZER
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define BUZZER_PIN						PA_9
#define BEEPER_TIM						TIM7
#define BEEPER_TIM_IRQN					TIM7_IRQn
#define BEEPER_TIM_HANDLER				TIM7_IRQHandler
#define BEEPER_TIM_ENABLE				__HAL_RCC_TIM7_CLK_ENABLE
#define BEEPER_TIM_DISABLE				__HAL_RCC_TIM7_CLK_DISABLE

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define BUZZER_PIN						PA_4
#define BEEPER_TIM						TIM7
#define BEEPER_TIM_IRQN					TIM7_IRQn
#define BEEPER_TIM_HANDLER				TIM7_IRQHandler
#define BEEPER_TIM_ENABLE				__HAL_RCC_TIM7_CLK_ENABLE
#define BEEPER_TIM_DISABLE				__HAL_RCC_TIM7_CLK_DISABLE

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define BUZZER_PIN						PA_15
#define BEEPER_TIM						TIM3
#define BEEPER_TIM_IRQN					TIM3_IRQn
#define BEEPER_TIM_HANDLER				TIM3_IRQHandler
#define BEEPER_TIM_ENABLE				__HAL_RCC_TIM3_CLK_ENABLE
#define BEEPER_TIM_DISABLE				__HAL_RCC_TIM3_CLK_DISABLE

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define BUZZER_PIN						PB_9
#define BEEPER_TIM						TIM7
#define BEEPER_TIM_IRQN					TIM7_IRQn
#define BEEPER_TIM_HANDLER				TIM7_IRQHandler
#define BEEPER_TIM_ENABLE				__HAL_RCC_TIM7_CLK_ENABLE
#define BEEPER_TIM_DISABLE				__HAL_RCC_TIM7_CLK_DISABLE
#endif
// -------------------------------------------------------------------------
#endif 
