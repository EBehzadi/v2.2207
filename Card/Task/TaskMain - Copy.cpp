// -------------------------------------------------------------------------
// Task :: Main Handler
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include "TaskMain.h"
#include "delay.h"
#include "gpio.h"
#include "adc.h"
#include "board.h"
#include "led.h"
#include "rng.h"
#include "Audio.h"
#include "RGB.h"
#include "RTCC.h"
#include "AD.h"
#include "Memory.h"
#include <stdio.h>
#include <string.h>
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#define LED_ALIVE						PC_7
#define GLOBAL_SYNC_PIN					PB_1
#define SENSOR_TRIG_PIN					PD_3

#define DEVICE_ID_PIN_COUNT				4
#define DEVICE_ID_PINS					{PD_1, PD_4, PD_5, PD_6}

#define VIN_DET_PIN						PA_4
#define VIN_DET_ADC						ADC1
#define VIN_DET_CH						ADC_CHANNEL_18
// -------------------------------------------------------------------------
#define RPM_CALC_TIM					TIM5
#define RPM_CALC_TIM_IRQ				TIM5_IRQn
#define RPM_CALC_TIM_HANDLER			TIM5_IRQHandler
#define RPM_CALC_TIM_CLOCK_ENABLE		__HAL_RCC_TIM5_CLK_ENABLE
#define RPM_CALC_TIM_CLOCK_DISABLE		__HAL_RCC_TIM5_CLK_DISABLE
// -------------------------------------------------------------------------
Gpio_t GlobalSync;
Adc_t adcVoltageInput;
Gpio_t inDeviceID[DEVICE_ID_PIN_COUNT];
Led_t LedAlive;
// -------------------------------------------------------------------------
Gpio_t Sensor_Trigger;
uint32_t Sensor_TriggerDuration;
bool Sensor_TrigIsHappen = false;

SensorStatus_t Sensor_Status[SENSOR_COUNT] = { SensorStatus_Disconnected };
uint16_t Sensor_RawValue[SENSOR_COUNT] = { 0 };
uint16_t Sensor_DataSampleBuffer[SENSOR_COUNT][DATA_COUNT_BUFFER];
// -------------------------------------------------------------------------
static void Main_Task(void* argument);
// -------------------------------------------------------------------------
extern vGlobal* pMainVariables;
extern xQueueHandle qMessageHandlerCom;
xQueueHandle qMessageHandlerMain;
MessageType_t msgMain;
// -------------------------------------------------------------------------
void Main_TaskInit(void)
{
	xTaskCreate(Main_Task, "MainTask", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);
	qMessageHandlerMain = xQueueGenericCreate(1, sizeof(msgMain), 0);
}
// -------------------------------------------------------------------------
void RPM_CALC_TIM_HANDLER(void)
{
	Sensor_TrigIsHappen = false;
}
// -------------------------------------------------------------------------
bool RpmCalculator_Init()
{
	RPM_CALC_TIM_CLOCK_ENABLE();

	TIM_HandleTypeDef hTimer;
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	hTimer.Instance = RPM_CALC_TIM;
	hTimer.Init.Prescaler = 0;
	hTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTimer.Init.Period = 0xFFFFFFFF;
	hTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&hTimer) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&hTimer, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	if (HAL_TIM_SlaveConfigSynchro(&hTimer, &sSlaveConfig) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hTimer, &sMasterConfig) != HAL_OK)
	{
		return false;
	}
	if (HAL_TIM_Base_Start(&hTimer) != HAL_OK)
	{
		return false;
	}
	RPM_CALC_TIM->CNT = 0;
	//HAL_NVIC_SetPriority(RPM_CALC_TIM_IRQ, 0x0b, 0);
	//HAL_NVIC_EnableIRQ(RPM_CALC_TIM_IRQ);
}
// -------------------------------------------------------------------------
void RpmCalculator_ResetCounter()
{
	RPM_CALC_TIM->CNT = 0;
}
// -------------------------------------------------------------------------
uint32_t RpmCalculator_GetCounter()
{
	return RPM_CALC_TIM->CNT;
}
// -------------------------------------------------------------------------
void SensorTriggerInterrupt_Handler(void* context)
{
	Sensor_TriggerDuration = RpmCalculator_GetCounter();
	RpmCalculator_ResetCounter();
	Sensor_TrigIsHappen = true;
}
// -------------------------------------------------------------------------
bool SensorTrigger_IsRun()
{
	if (Sensor_TrigIsHappen == false)
	{
		return false;
	}
	if (Sensor_TriggerDuration < DATA_SAMPLES_PER_ROUND || Sensor_TriggerDuration >= 0xFFFFFFFF - 2)
	{
		return false;
	}
	return true;
}
// -------------------------------------------------------------------------
void GlobalSyncInterrupt_Handler(void* context)
{

}
// -------------------------------------------------------------------------
static void Main_Task(void* argument)
{
	AD_Init();
	float vAD[4] = { 0 };
	while (1)
	{
		for (int i = 2; i < 4; ++i)
		{
			vAD[i] = AD_ReadVoltage(i);
		}
	}

#pragma region INITIALIZE
	RGB_Init();
	Audio_Init();
	Memory_Init();
	RTCC_Init();
	AD_Init();

	LedInit(&LedAlive, LED_ALIVE, LED_POLARITY_ON_HIGH, false);
	LedSetBlink(&LedAlive, 100, 900);

	GpioInit(&Sensor_Trigger, SENSOR_TRIG_PIN, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
	GpioSetInterrupt(&Sensor_Trigger, IRQ_FALLING_EDGE, IRQ_HIGH_PRIORITY, &SensorTriggerInterrupt_Handler);

	PinNames pinDeviceID[] = DEVICE_ID_PINS;
	for (int i = 0; i < DEVICE_ID_PIN_COUNT; ++i)
	{
		GpioInit(&inDeviceID[i], pinDeviceID[i], PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
	}

	GpioInit(&GlobalSync, GLOBAL_SYNC_PIN, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
	GpioSetInterrupt(&GlobalSync, IRQ_FALLING_EDGE, IRQ_HIGH_PRIORITY, &GlobalSyncInterrupt_Handler);

	AdcInit(&adcVoltageInput, VIN_DET_PIN, VIN_DET_ADC);
	AdcConfig(&adcVoltageInput);
	uint32_t v = AdcReadChannel(&adcVoltageInput, VIN_DET_CH);
#pragma endregion

#pragma region POWER UP
	AudioOut_PlayTones(BeeprTone_2BeepsFast);
	RGB_SetColor(RGB_COLOR_RED);
	RGB_Render();
	DelayMs(1000);
	RGB_SetColor(RGB_COLOR_GREEN);
	RGB_Render();
	DelayMs(1000);
#pragma endregion

	while (1)
	{
#pragma region CHECK SENSOR STATUS
		for (int i = 0; i < SENSOR_COUNT; ++i)
		{
			Sensor_RawValue[i] = AD_ReadRaw(i);
			if (Sensor_RawValue[i] > 0)
			{
				Sensor_Status[i] == SensorStatus_Disconnected;
			}
			else if (Sensor_RawValue[i] > 1)
			{
				Sensor_Status[i] == SensorStatus_Error;
			}
			else
			{
				Sensor_Status[i] == SensorStatus_Connected;
			}
		}
		for (int i = 0; i < SENSOR_COUNT; ++i)
		{
			if (Sensor_Status[i] == SensorStatus_Disconnected)
			{
				RGB_SetColor_ByIndex(i, RGB_COLOR_BLUE);
			}
			else if (Sensor_Status[i] == SensorStatus_Connected)
			{
				RGB_SetColor_ByIndex(i, RGB_COLOR_GREEN);
			}
			else if (Sensor_Status[i] == SensorStatus_Error)
			{
				RGB_SetColor_ByIndex(i, RGB_COLOR_RED);
			}
			RGB_Render();
		}
#pragma endregion

#pragma region SAMPLE SENSOR 
		if (SensorTrigger_IsRun())
		{
			Sensor_TrigIsHappen = false;
			while (!Sensor_TrigIsHappen);
			PerformSampling();
			while (IsSamplingDone() == false)
			{
				LedProcess();
			}
			if ()
			{
				pMainVariables->Data.LastDuration = Sensor_TriggerDuration;
				pMainVariables->Data.Count = SENSOR_COUNT;
				pMainVariables->Data.Length = sizeof(int16_t) * DATA_COUNT_BUFFER * DATA_COLLECT_COUNT;
				msgMain = MessageType_SendData;
				xQueueSend(qMessageHandlerCom, &msgMain, 1000);
				DelayMs(100);
			}
			for (int i = 0; i < DATA_COUNT_BUFFER * DATA_COLLECT_COUNT; ++i)
			{
				for (int s = 0; s < SENSOR_COUNT; ++s)
				{
					pMainVariables->Data.Buffer[s][i] = AD_ReadRaw(i);
				}
			}
		}
#pragma endregion
	}
}
// -------------------------------------------------------------------------
