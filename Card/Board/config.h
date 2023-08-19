// -------------------------------------------------------------------------
// System Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__
// -------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -------------------------------------------------------------------------
#define FREERTOS_ENABLE
// -------------------------------------------------------------------------
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_pcd.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_ltdc.h"

#ifdef FREERTOS_ENABLE
#include <../CMSIS_RTOS/cmsis_os.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif 

#define RAM1		__attribute__((section(".RAM1"), used))		
#define RAM2		__attribute__((section(".RAM2"), used))		
#define RAM3		__attribute__((section(".RAM3"), used))		
#define ExtFlash	__attribute__((section("ExtFlashSection"), used))		
#define DMA_RAM		__attribute__((section(".DMA_RAM")))
	// -------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
// -------------------------------------------------------------------------
