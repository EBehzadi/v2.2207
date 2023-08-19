// -------------------------------------------------------------------------
// Task :: Communication Handler
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __TASK_COM_H__
#define __TASK_COM_H__
// -------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -------------------------------------------------------------------------
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
// -------------------------------------------------------------------------
#define COM_TASK_PRIORITY				osPriorityNormal
#define COM_TASK_STACK_SIZE				10000
// -------------------------------------------------------------------------
	void Com_TaskInit(void);
	void Com_Reset();
	bool Com_UpdateAndSendData(uint8_t select, uint32_t counter);
	// -------------------------------------------------------------------------
#endif
#ifdef __cplusplus
}
#endif
// -------------------------------------------------------------------------
