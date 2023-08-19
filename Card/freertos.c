// -------------------------------------------------------------------------
// FreeRTOS Manager
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
// -------------------------------------------------------------------------
void PreSleepProcessing(uint32_t* ulExpectedIdleTime);
void PostSleepProcessing(uint32_t* ulExpectedIdleTime);
// -------------------------------------------------------------------------
__weak void PreSleepProcessing(uint32_t* ulExpectedIdleTime)
{
}
// -------------------------------------------------------------------------
__weak void PostSleepProcessing(uint32_t* ulExpectedIdleTime)
{
}
// -------------------------------------------------------------------------
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize);
// -------------------------------------------------------------------------
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
// -------------------------------------------------------------------------
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
// -------------------------------------------------------------------------
