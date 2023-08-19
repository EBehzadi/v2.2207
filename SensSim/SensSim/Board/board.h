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
#include <stdbool.h>
#include "utilities.h"
#include "config.h"
	// -------------------------------------------------------------------------
	void SystemClockConfig(void);
	void BoardInitMcu(void);
	void SystemClockConfig(void);
	void BoardResetMcu(void);
	void BoardDeInitMcu(void);
	uint32_t BoardGetRandomSeed(void);
	void BoardGetUniqueId(uint8_t* id);
	void BoardJumpTobootloader();
	// -------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
// -------------------------------------------------------------------------
