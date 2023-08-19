// -------------------------------------------------------------------------
// RGB Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __RGB_CONFIG_H__
#define __RGB_CONFIG_H__
// -------------------------------------------------------------------------
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define RGB_USE_WS2812
#define WS2812_RESET_PERIOD			50
#define WS2812_TIMER_CLOCK_DIV		2
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define WS2812B_NUMBER_OF_LEDS		6

#define WS2812_PIN					PB_7
#define WS2812_ALTERNATE			GPIO_AF1_TIM17

#define WS2812_TIMER				TIM17
#define WS2812_TIMER_CLK_ENABLE()	__HAL_RCC_TIM17_CLK_ENABLE();
#define WS2812_TIMER_IRQ			TIM17_IRQn
#define WS2812_TIMER_IRQ_HANDLER	TIM17_IRQHandler

#define WS2812_TIMER_CHANNEL		TIM_CHANNEL_1
#define WS2812_TIMER_DUTY_REG		CCR1
#define WS2812_TIMER_DUTY_DMA		TIM_DMABASE_CCR1

#define WS2812_TIMER_DMA			TIM_DMA_UPDATE
#define WS2812_TIMER_DMA_ID			TIM_DMA_ID_UPDATE

#define WS2812_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE();
#define WS2812_DMA_CHANNEL_REQ		DMA_REQUEST_TIM17_UP
#define WS2812_DMA_CHANNEL			DMA1_Stream0
#define WS2812_DMA_IRQ				DMA1_Stream0_IRQn
#define WS2812_DMA_IRQ_HANDLER		DMA1_Stream0_IRQHandler
#define WS2812_DMA_IRQ_FLAG			(DMA_FLAG_TCIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TEIF0_4)

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define WS2812B_NUMBER_OF_LEDS		6

#define WS2812_PIN					PB_7
#define WS2812_ALTERNATE			GPIO_AF1_TIM17

#define WS2812_TIMER				TIM17
#define WS2812_TIMER_CLK_ENABLE()	__HAL_RCC_TIM17_CLK_ENABLE();
#define WS2812_TIMER_IRQ			TIM17_IRQn
#define WS2812_TIMER_IRQ_HANDLER	TIM17_IRQHandler

#define WS2812_TIMER_CHANNEL		TIM_CHANNEL_1
#define WS2812_TIMER_DUTY_REG		CCR1
#define WS2812_TIMER_DUTY_DMA		TIM_DMABASE_CCR1
#define WS2812_TIMER_CHANNEL_N

#define WS2812_TIMER_DMA			TIM_DMA_UPDATE
#define WS2812_TIMER_DMA_ID			TIM_DMA_ID_UPDATE

#define WS2812_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE();
#define WS2812_DMA_CHANNEL_REQ		DMA_REQUEST_TIM17_UP
#define WS2812_DMA_CHANNEL			DMA1_Stream0
#define WS2812_DMA_IRQ				DMA1_Stream0_IRQn
#define WS2812_DMA_IRQ_HANDLER		DMA1_Stream0_IRQHandler
#define WS2812_DMA_IRQ_FLAG			(DMA_FLAG_TCIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TEIF0_4)

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define WS2812B_NUMBER_OF_LEDS		1

#define WS2812_PIN					PC_9
#define WS2812_ALTERNATE			GPIO_AF3_TIM8

#define WS2812_TIMER				TIM8
#define WS2812_TIMER_CLK_ENABLE()	__HAL_RCC_TIM8_CLK_ENABLE();
#define WS2812_TIMER_IRQ			TIM8_UP_TIM13_IRQn
#define WS2812_TIMER_IRQ_HANDLER	TIM8_UP_TIM13_IRQHandler

#define WS2812_TIMER_CHANNEL		TIM_CHANNEL_4
#define WS2812_TIMER_DUTY_REG		CCR4
#define WS2812_TIMER_DUTY_DMA		TIM_DMABASE_CCR4

#define WS2812_TIMER_DMA			TIM_DMA_UPDATE
#define WS2812_TIMER_DMA_ID			TIM_DMA_ID_UPDATE

#define WS2812_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE();
#define WS2812_DMA_CHANNEL_REQ		DMA_REQUEST_TIM8_UP
#define WS2812_DMA_CHANNEL			DMA1_Stream0
#define WS2812_DMA_IRQ				DMA1_Stream0_IRQn
#define WS2812_DMA_IRQ_HANDLER		DMA1_Stream0_IRQHandler
#define WS2812_DMA_IRQ_FLAG			(DMA_FLAG_TCIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TEIF0_4)


#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define WS2812B_NUMBER_OF_LEDS		6

#define WS2812_PIN					PB_8
#define WS2812_ALTERNATE			GPIO_AF1_TIM16

#define WS2812_TIMER				TIM16
#define WS2812_TIMER_CLK_ENABLE()	__HAL_RCC_TIM16_CLK_ENABLE();
#define WS2812_TIMER_IRQ			TIM16_IRQn
#define WS2812_TIMER_IRQ_HANDLER	TIM16_IRQHandler

#define WS2812_TIMER_CHANNEL		TIM_CHANNEL_1
#define WS2812_TIMER_DUTY_REG		CCR1
#define WS2812_TIMER_DUTY_DMA		TIM_DMABASE_CCR1

#define WS2812_TIMER_DMA			TIM_DMA_UPDATE
#define WS2812_TIMER_DMA_ID			TIM_DMA_ID_UPDATE

#define WS2812_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE();
#define WS2812_DMA_CHANNEL_REQ		DMA_REQUEST_TIM16_UP
#define WS2812_DMA_CHANNEL			DMA1_Stream0
#define WS2812_DMA_IRQ				DMA1_Stream0_IRQn
#define WS2812_DMA_IRQ_HANDLER		DMA1_Stream0_IRQHandler
#define WS2812_DMA_IRQ_FLAG			(DMA_FLAG_TCIF0_4 | DMA_FLAG_HTIF0_4 | DMA_FLAG_TEIF0_4)
#endif 
// -------------------------------------------------------------------------
#endif 
