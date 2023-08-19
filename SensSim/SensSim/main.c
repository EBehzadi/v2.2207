// -------------------------------------------------------------------------
// SIGNAL SIMULATOR - DATALL
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : November 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#pragma region INCLUDE
#include "config.h"
#include "board.h"
#include "gpio.h"
#include "delay.h"
#include "led.h"
#include "Serial.h"
#include "DAC.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "SampleData_Vibration.h"
#pragma endregion
// -------------------------------------------------------------------------
#define LED_STATUS_PIN					PB_10
Led_t ledStatus;
// -------------------------------------------------------------------------
#define TRIG_OUT_PIN					PA_15
Gpio_t outTrig;
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#define PI								3.14159265
#define DATA_COUNT						3600
// -------------------------------------------------------------------------
#define HEALTHY_DATA_TIMEOUT			30000
#define FAULTY_DATA_TIMEOUT				10000

//#define DATA_USE_SAMPLE
#define DATA_USE_SIN
// -------------------------------------------------------------------------
int main(void)
{
	BoardInitMcu();
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	//__HAL_AFIO_REMAP_SWJ_DISABLE();
	
	GpioInit(&outTrig, TRIG_OUT_PIN, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0, PIN_SPEED_FREQ_HIGH);

	LedInit(&ledStatus, LED_STATUS_PIN, LED_POLARITY_ON_HIGH, false);
	Serial_Init();
	DAC_Init(false, true);

	uint32_t tick = 0, duration = 0;
	uint16_t bufData[DATA_COUNT];

#ifdef DATA_USE_SIN
	float freq = 100;
	for (int i = 0; i < DATA_COUNT; ++i)
	{
		double d = sin((PI * i * (freq / 152.0)) / 180.0); 
		bufData[i] = d * 32000 + 32000;
	}
#endif

#ifdef DATA_USE_SAMPLE
	for (int i = 0; i < DATA_COUNT; ++i)
	{
		bufData[i] = ((SampleData_Vibration_Healthy[i] - 4.0) * 65535.0) / (20.0 - 4.0);
	}
#endif

	uint32_t tTimeout_HealthyData = 0;
	uint32_t tTimeout_FaultyData = 0;
	const uint16_t* bufDataToSend = SampleData_Vibration_Healthy;
	bool bModeDataHelathy = true;
	
	uint8_t cReceivedChar = 0;
	uint8_t cSendChar = 0x02;
	int8_t mNotReceivedCounter = 0;
	uint32_t iLastDataReceived = HAL_GetTick();
	uint32_t iLastAckSend = HAL_GetTick();
	bool bReceivedTimeout = false;
	while (1)
	{
		tick = HAL_GetTick();
		LedProcess();
		if (bModeDataHelathy)
		{
			if (tick - tTimeout_HealthyData > HEALTHY_DATA_TIMEOUT)
			{
				bufDataToSend = SampleData_Vibration_Faulty;
				tTimeout_FaultyData = tick;
				bModeDataHelathy = false;
			}
		}
		else
		{
			if (tick - tTimeout_FaultyData > FAULTY_DATA_TIMEOUT)
			{
				bufDataToSend = SampleData_Vibration_Healthy;
				tTimeout_HealthyData = tick;
				bModeDataHelathy = true;
			}
		}
		
		int DataModeCounter = 0;
		while (1)
		{
			DataModeCounter++;
			if (DataModeCounter == 1)
			{
				LedSetBlink(&ledStatus, 100, 900);
				DAC_SetCurrent(2.0);
			}
			else if (DataModeCounter == 100)
			{
				LedSetBlink(&ledStatus, 100, 1900);
				DAC_SetCurrent(4.0);
			}
			else if (DataModeCounter == 200)
			{
				LedSetBlink(&ledStatus, 100, 2900);
				DAC_SetCurrent(20.0);
			}
			else if (DataModeCounter == 300)
			{
				LedSetBlink(&ledStatus, 100, 3900);
				DAC_SetCurrent(22.0);
			}
			else if (DataModeCounter == 400)
			{
				DataModeCounter = 0;
			}
			LedProcess();
			DelayMs(100);
		}
		
		GpioWrite(&outTrig, 1);
		for (int i = 0; i < DATA_COUNT; ++i)
		{
			DAC_SetCurrent(4.0);
			//DAC_SetCurrentRaw(bufDataToSend[i]);
			//DAC_SetVoltageRaw(bufData[i]);
		}
		//GpioWrite(&outTrig, 0);
		LedToggle(&ledStatus);
		duration = HAL_GetTick() - tick;
		//DelayMs(duration);
	}
}
// -------------------------------------------------------------------------
