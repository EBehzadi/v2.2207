// -------------------------------------------------------------------------
// Board Driver
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __BOARD__H__
#define __BOARD__H__
// -------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -------------------------------------------------------------------------
#include <stdint.h>
#include "utilities.h"
#include "config.h"
// -------------------------------------------------------------------------
#define MCU_ID_LENGTH				12
#define MCU_ID_ADDRESS				(0x1FF09800)
// -------------------------------------------------------------------------
#define SDRAM_START_ADDR			0xD0000000
#define SDRAM_END_ADDR				0xD0FFFFFF

#define SDRAM_FRAMEBUFFER_START		SDRAM_START_ADDR
#define SDRAM_FRAMEBUFFER_END		SDRAM_END_ADDR
#define SDRAM_CACHE_START			SDRAM_START_ADDR + 0x00400000
#define SDRAM_CACHE_SIZE			0x00200000
#define SDRAM_RESOURCE_START		SDRAM_CACHE_START+SDRAM_CACHE_SIZE
// -------------------------------------------------------------------------
	void BoardCriticalSectionBegin(uint32_t* mask);
	void BoardCriticalSectionEnd(uint32_t* mask);
	void BoardInitMcu(void);
	void BoardResetMcu(void);
	void BoardGetUniqueId(uint8_t* id);
	uint32_t BoardGetRandomSeed(void);
	void BoardClockConfig(void);
	void BoardMpuConfig();
	void BoardEnableTick();
	double BoardGetCpuTemperature();
	// -------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif
// -------------------------------------------------------------------------
