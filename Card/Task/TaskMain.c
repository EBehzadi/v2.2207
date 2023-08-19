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
#include "crc.h"
#include "Audio.h"
#include "RGB.h"
#include "RTCC.h"
#include "AD.h"
#include "Memory.h"
#include "Sensor.h"
#include "General_Config.h"
#include "Channel_Config.h"
#include <stdio.h>
#include <string.h>
#include <Modbus.h>

#include "../SampleData_Pressure.h"
#include "../SampleData_Proximity.h"
#include "../SampleData_Vibrtion.h"
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//#define TEST_TIMING
// -------------------------------------------------------------------------
#define RPM_CALC_TIM					TIM5
#define RPM_CALC_TIM_CLOCK_ENABLE()		__HAL_RCC_TIM5_CLK_ENABLE()
#define RPM_CALC_TIM_CLOCK_DISABLE()	__HAL_RCC_TIM5_CLK_DISABLE()
#define RPM_CALC_TIM_RESET()			__HAL_RCC_TIM5_FORCE_RESET();__HAL_RCC_TIM5_RELEASE_RESET()

#define DATA_SAMPLE_TIM					TIM2
#define DATA_SAMPLE_TIM_IRQ				TIM2_IRQn
#define DATA_SAMPLE_TIM_HANDLER			TIM2_IRQHandler
#define DATA_SAMPLE_TIM_CLOCK_ENABLE()	__HAL_RCC_TIM2_CLK_ENABLE()
#define DATA_SAMPLE_TIM_CLOCK_DISABLE()	__HAL_RCC_TIM2_CLK_DISABLE()
#define DATA_SAMPLE_TIM_RESET()			__HAL_RCC_TIM2_FORCE_RESET();__HAL_RCC_TIM2_RELEASE_RESET()

#define DATA_OVER_SAMPLE_COUNT			1
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
Gpio_t GlobalSync;
Adc_t adcVoltageInput;
Led_t LedAlive;
Gpio_t Relay_IPC;
Gpio_t Button_Command;
// -------------------------------------------------------------------------
bool trigger_enable = true;
Gpio_t SampleTriggerPin;
bool SampleTrigIsHappen = false;
bool SampleTrigIsAvailable = false;
uint32_t LastTimeTrigReceived = 0;
__IO uint32_t SampleTriggerDuration;
uint32_t LastSampleTriggerDuration = 0;
uint32_t AvgSampleTriggerDuration = 0;
TIM_HandleTypeDef RpmTimer_Handle;
TIM_HandleTypeDef SampleTimer_Handle;
volatile uint32_t SampleCounter = 0;
volatile bool SamplingIsTrigged = false;
volatile bool SamplingIsDone = false;
ChannelStatus_t CahnnelStatus[CHANNEL_COUNT] = { ChannelStatus_Disabled };
bool IsThereAnyError = false;
uint32_t LastTimeLedUpdated = 0, LastTimeStatuUpdated = 0;
uint32_t startAcq, durationAcq, durationSend, durationSampleStatus;
uint64_t TimeStamp = 0;
int sum_sample_time = 0;
int sample_c = 0;
uint64_t Reference_time = 0;
uint64_t OffSet_time = 0;
bool SetFlag = true;
extern int OffSet;
extern uint32_t mPacketCounter;
// -------------------------------------------------------------------------
static void Main_Task(void* argument);
void interval_calculator(void);
// -------------------------------------------------------------------------
extern vGlobal* pMainVariables;
extern uint16_t Data_test[4][3600];
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
	SampleTrigIsHappen = false;
}
// -------------------------------------------------------------------------
bool RpmCalculator_Init()
{
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	RPM_CALC_TIM_RESET();
	RPM_CALC_TIM->CNT = 0;
	RPM_CALC_TIM_CLOCK_ENABLE();
	RpmTimer_Handle.Instance = RPM_CALC_TIM;
	RpmTimer_Handle.Init.Prescaler = 0;
	RpmTimer_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	RpmTimer_Handle.Init.Period = 0xFFFFFFFF;
	RpmTimer_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	RpmTimer_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&RpmTimer_Handle) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&RpmTimer_Handle, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
	if (HAL_TIM_SlaveConfigSynchro(&RpmTimer_Handle, &sSlaveConfig) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&RpmTimer_Handle, &sMasterConfig) != HAL_OK)
	{
		return false;
	}
	if (HAL_TIM_Base_Start(&RpmTimer_Handle) != HAL_OK)
	{
		return false;
	}
	//HAL_NVIC_SetPriority(RPM_CALC_TIM_IRQ, 0x0b, 0);
	//HAL_NVIC_EnableIRQ(RPM_CALC_TIM_IRQ);
	return true;
}
// -------------------------------------------------------------------------
void RpmCalculator_ResetCounter()
{
	//RpmCalculator_Init();
	RPM_CALC_TIM->CNT = 0;
}
// -------------------------------------------------------------------------
uint32_t RpmCalculator_GetCounter()
{
	return RPM_CALC_TIM->CNT;
}
// -------------------------------------------------------------------------
int intTrigCounter = 0;
void SensorTriggerInterrupt_Handler(void* context)
{
	intTrigCounter++;
	if (intTrigCounter == 100)
	{
		intTrigCounter = 0;
		SampleTriggerDuration = RpmCalculator_GetCounter() / 100;
		//	SampleTriggerDuration = (0.95*AvgSampleTriggerDuration) + (0.05018*RpmCalculator_GetCounter());
		//	AvgSampleTriggerDuration = SampleTriggerDuration;
		//	SampleTriggerDuration = 24000000;
		RpmCalculator_ResetCounter();
		SampleTrigIsHappen = true;
		SampleTrigIsAvailable = true;
		LastTimeTrigReceived = HAL_GetTick();
	}
}
// -------------------------------------------------------------------------
bool SampleTrigger_IsRun()
{
	if (SampleTrigIsHappen == false)
	{
		return false;
	}
	if (SampleTriggerDuration < DATA_SAMPLES_PER_ROUND || SampleTriggerDuration >= 0xFFFFFFFF - 2)
	{
		return false;
	}
	return true;
}
// -------------------------------------------------------------------------
void Channel_CheckStatus(uint8_t select, uint32_t value, bool rgb)
{
	if (pMainVariables->Channel[select].Enable)
	{
		if (pMainVariables->Channel[select].Type == ChannelType_SensorPressure_420 ||
			pMainVariables->Channel[select].Type == ChannelType_SensorVibration_420)
		{
			if (value < pMainVariables->Channel[select].Margin.Break)
			{
				CahnnelStatus[select] = ChannelStatus_Error_BreakIn;
			}
			else if (value < pMainVariables->Channel[select].Margin.LimitLow)
			{
				CahnnelStatus[select] = ChannelStatus_Error_UnderRange;
			}
			else if (value > pMainVariables->Channel[select].Margin.Short)
			{
				CahnnelStatus[select] = ChannelStatus_Error_ShortCircuit;
			}
			else if (value > pMainVariables->Channel[select].Margin.LimitHigh)
			{
				CahnnelStatus[select] = ChannelStatus_Error_OverRange;
			}
			else
			{
				CahnnelStatus[select] = ChannelStatus_Connected;
			}
		}
		else if (pMainVariables->Channel[select].Type == ChannelType_SensorProximity_020)
		{
			if (value < pMainVariables->Channel[select].Margin.Break)
			{
				CahnnelStatus[select] = ChannelStatus_Error_BreakIn;
			}
			else if (value < pMainVariables->Channel[select].Margin.LimitLow)
			{
				CahnnelStatus[select] = ChannelStatus_Error_UnderRange;
			}
			else if (value > pMainVariables->Channel[select].Margin.LimitHigh)
			{
				CahnnelStatus[select] = ChannelStatus_Error_OverRange;
			}
			else
			{
				CahnnelStatus[select] = ChannelStatus_Connected;
			}
		}
		else if (pMainVariables->Channel[select].Type == ChannelType_SensorVibration_RAW)
		{
			if (value < pMainVariables->Channel[select].Margin.LimitLow)
			{
				CahnnelStatus[select] = ChannelStatus_Error_UnderRange;
			}
			else if (value > pMainVariables->Channel[select].Margin.Break)
			{
				CahnnelStatus[select] = ChannelStatus_Error_BreakIn;
			}
			else if (value > pMainVariables->Channel[select].Margin.LimitHigh)
			{
				CahnnelStatus[select] = ChannelStatus_Error_OverRange;
			}
			else
			{
				CahnnelStatus[select] = ChannelStatus_Connected;
			}
		}
	}
	else
	{
		CahnnelStatus[select] = ChannelStatus_Disabled;
	}
	if (rgb)
	{
		int ledIndex = -1;
		switch (select)
		{
		case 0:
			ledIndex = LedSelect_Channel1;
			break;
		case 1:
			ledIndex = LedSelect_Channel2;
			break;
		case 2:
			ledIndex = LedSelect_Channel3;
			break;
		case 3:
			ledIndex = LedSelect_Channel4;
			break;
		}
		
		if (CahnnelStatus[select] & ChannelStatus_Connected)
		{
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_GREEN);
		}
		else if (CahnnelStatus[select] == ChannelStatus_Disabled)
		{
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_OFF);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Warning_NotConfigured)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_ORANGE);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Error_UnderRange)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_AQUA);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Error_OverRange)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_YELLOW);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Error_BreakIn)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_BLUE);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Error_ShortCircuit)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_MAGENTA);
		}
		else if (CahnnelStatus[select] & ChannelStatus_Error_Unknown)
		{
			IsThereAnyError = true;
			RGB_SetColor_ByIndex(ledIndex, RGB_COLOR_RED);
		}
	}
}
// -------------------------------------------------------------------------
void DATA_SAMPLE_TIM_HANDLER()
{
	interval_calculator();
	HAL_TIM_IRQHandler(&SampleTimer_Handle);
	SamplingIsTrigged = true;
}
// -------------------------------------------------------------------------
bool SamplingSetup(uint32_t interval)
{
	LastSampleTriggerDuration = SampleTriggerDuration;
	sum_sample_time = 0;
	sample_c = 0;
	SamplingIsDone = false;
	SampleCounter = 0;
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };

	DATA_SAMPLE_TIM_RESET();
	DATA_SAMPLE_TIM_CLOCK_ENABLE();
	SampleTimer_Handle.Instance = DATA_SAMPLE_TIM;
	SampleTimer_Handle.Init.Prescaler = 0;
	SampleTimer_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	SampleTimer_Handle.Init.Period = interval;
	SampleTimer_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	SampleTimer_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&SampleTimer_Handle) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&SampleTimer_Handle, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	HAL_NVIC_SetPriority(DATA_SAMPLE_TIM_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(DATA_SAMPLE_TIM_IRQ);
	if (HAL_TIM_Base_Start_IT(&SampleTimer_Handle) != HAL_OK)
	{
		return false;
	}
	return true;
}
// -------------------------------------------------------------------------
bool SamplingRun(uint32_t maxSample)
{
	while (SamplingIsTrigged == false) ;
	SamplingIsTrigged = false;
	uint16_t valRead = 0;
	uint8_t iStep = DATA_SAMPLES_PER_ROUND / maxSample;
	if (iStep == 0)
	{
		iStep = 1;
	}
	CRITICAL_SECTION_BEGIN();
	if (SampleCounter == 0)
	{
		for (uint8_t s = 0; s < CHANNEL_COUNT; ++s)
		{
			AD_StartAquisition(s);
			bool bRead = AD_ReadSpi(s, &valRead);
			AD_StopAquisition(s);
		}
	}
	for (uint8_t s = 0; s < CHANNEL_COUNT; ++s)
	{
		AD_StartAquisition(s);
		//for (uint32_t d = 0; d < 1; ++d);
		bool bRead = AD_ReadSpi(s, &valRead);
		if (bRead)
		{
			for (uint8_t i = 0; i < iStep; ++i)
			{
				pMainVariables->Channel[s].DataBuffer[SampleCounter * iStep + i] = valRead;
//				Data_test[s][SampleCounter * iStep + i] = (valRead * 0.63158) - 7164;
			}
		}
		AD_StopAquisition(s);
		//Channel_CheckStatus(s, pMainVariables->Channel[s].DataBuffer[SampleCounter * iStep], false);
		//Channel_CheckStatus(s, Data_test[s][SampleCounter * iStep], false);
	}
	CRITICAL_SECTION_END();

	SampleCounter++;
	if (SampleCounter >= maxSample)
	{
		HAL_TIM_Base_Stop(&SampleTimer_Handle);
		DATA_SAMPLE_TIM_CLOCK_DISABLE();
		SamplingIsDone = true;
	}
	TimeStamp = Reference_time + ((((uint64_t)(HAL_GetTick())) / 1000) - OffSet_time);
	return SamplingIsDone;
}
// -------------------------------------------------------------------------
void GlobalSyncInterrupt_Handler(void* context)
{

}
// -------------------------------------------------------------------------
static void Main_Task(void* argument)
{
#pragma region INITIALIZE
	Audio_Init();
	Memory_Init();
	Sensor_Init();
	AD_Init(AD_SAMPLING_FREUENCY);
	RTCC_Init();
	RGB_Init();
#if 0
	RTC_datetime_t datetimeSet, datetimeGet;
	datetimeSet.year = 2022;
	datetimeSet.month = 7;
	datetimeSet.date = 8;
	datetimeSet.hours = 9;
	datetimeSet.minutes = 4;
	RTCC_SetDatetime(datetimeSet);
	RTCC_GetDatetime(&datetimeGet);
#endif

	LedInit(&LedAlive, LED_ALIVE, LED_POLARITY_ON_HIGH, false);
	LedSetBlink(&LedAlive, 100, 900);

	GpioInit(&SampleTriggerPin, SENSOR_TRIG_PIN, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0, PIN_SPEED_FREQ_VERY_HIGH);
	GpioSetInterrupt(&SampleTriggerPin, IRQ_RISING_EDGE, IRQ_VERY_HIGH_PRIORITY, &SensorTriggerInterrupt_Handler);

	GpioInit(&GlobalSync, GLOBAL_SYNC_PIN, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
	GpioSetInterrupt(&GlobalSync, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, &GlobalSyncInterrupt_Handler);

#ifdef VIN_DET_PIN
	AdcInit(&adcVoltageInput, VIN_DET_PIN, VIN_DET_ADC);
	AdcConfig(&adcVoltageInput);
	uint32_t v = AdcReadChannel(&adcVoltageInput, VIN_DET_CH);
#endif
	
#ifdef RELAY_IPC_PIN
	GpioInit(&Relay_IPC, RELAY_IPC_PIN, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1, PIN_SPEED_FREQ_HIGH);
	DelayMs(500);
	GpioWrite(&Relay_IPC, 0);
#endif

#ifdef BUTTON_FUNCTION
	GpioInit(&Button_Command, BUTTON_FUNCTION, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
#endif
		
	RpmCalculator_Init();
#pragma endregion

#pragma region POWER UP
	DelayMs(100);
	AudioOut_PlayTones(BeeprTone_2BeepsFast);
	RGB_SetColor(RGB_COLOR_RED);
	RGB_Render();	
	DelayMs(1000);
	RGB_SetColor(RGB_COLOR_GREEN);
	RGB_Render();
	DelayMs(1000);
	pMainVariables->System.TimeStampOffset = 0;
#pragma endregion

#pragma region SENSOR CONFIG 
	vChannelConfig ChannelConfig[CHANNEL_COUNT];
	ChannelConfig[0] = (vChannelConfig)CHANNEL_CONFIG_1;
	ChannelConfig[1] = (vChannelConfig)CHANNEL_CONFIG_2;
	ChannelConfig[2] = (vChannelConfig)CHANNEL_CONFIG_3;
	ChannelConfig[3] = (vChannelConfig)CHANNEL_CONFIG_4;
	for (int i = 0; i < CHANNEL_COUNT; ++i)
	{
		pMainVariables->Channel[i].Enable = ChannelConfig[i].Enable;
		pMainVariables->Channel[i].Type = ChannelConfig[i].Type;
		pMainVariables->Channel[i].Margin.Short = ChannelConfig[i].MarginShort;
		pMainVariables->Channel[i].Margin.Break = ChannelConfig[i].MarginBreak;
		pMainVariables->Channel[i].Margin.LimitLow = ChannelConfig[i].MarginLow;
		pMainVariables->Channel[i].Margin.LimitHigh = ChannelConfig[i].MarginHigh;
		pMainVariables->Channel[i].Calibration.a = ChannelConfig[i].CalibrationRatio;
		pMainVariables->Channel[i].Calibration.b = ChannelConfig[i].CalibrationOffset;
	}

	while (1)
	{
#pragma region SAMPLE SENSOR 
		uint32_t iSampleCount = DATA_SAMPLES_PER_ROUND / DATA_SAMPLE_SCALE;
#ifdef SAMPLE_TRIGGER_MODE_ENABLE
		if (trigger_enable)
		{
			if (SampleTrigger_IsRun())
			{
				RpmCalculator_ResetCounter();
				SampleTrigIsHappen = false;
				uint32_t timeout = 0;
				while (!SampleTrigIsHappen && timeout < 0xFFFFFFF0) 
				{
					timeout++;
				}
				if (SampleTriggerDuration < 1000000)
				{
					SampleTriggerDuration = 1000000;
				}
				if (SampleTriggerDuration > 1000000000)
				{
					SampleTriggerDuration = 1000000000;
				}
				SamplingSetup(SampleTriggerDuration / iSampleCount);
				startAcq = HAL_GetTick();
				while (SamplingRun(iSampleCount) == false)
				{
				}
				SamplingIsDone = true;
				SampleTrigIsHappen = false;
			}
			else
			{
				SampleTrigIsHappen = false;
			}
		}
		else
		{
			RpmCalculator_ResetCounter();
			SampleTriggerDuration = COUNTER_FOR_ONE_CYCLE;
			SamplingSetup(SampleTriggerDuration / iSampleCount);
			startAcq = HAL_GetTick();
			while (SamplingRun(iSampleCount) == false)
			{
			}
			SamplingIsDone = true;
			SampleTrigIsHappen = false;
		}
#else
		RpmCalculator_ResetCounter();
		SampleTriggerDuration = COUNTER_FOR_ONE_CYCLE;
		SamplingSetup(SampleTriggerDuration / iSampleCount);
		startAcq = HAL_GetTick();
		while (SamplingRun(iSampleCount) == false)
		{
		}
		SamplingIsDone = true;
		SampleTrigIsHappen = false;
#ifdef DATA_SIMULATED_ENABLE
		for (int i = 0; i < DATA_SAMPLES_PER_ROUND; ++i)
		{
			pMainVariables->Channel[0].DataBuffer[i] = SampleData_Pressure1[i] + (RngGetUint8() / 8);
			pMainVariables->Channel[0].DataBuffer[i] = SampleData_Pressure1[i] + (RngGetUint8() / 8);
			pMainVariables->Channel[1].DataBuffer[i] = SampleData_Pressure2[i] + (RngGetUint8() / 8);
			pMainVariables->Channel[2].DataBuffer[i] = SampleData_Proximity[i] + (RngGetUint8() / 4);
			pMainVariables->Channel[3].DataBuffer[i] = SampleData_Vibration_Healthy[i] + (RngGetUint8() / 8);
		}
		uint16_t minVibration = 0xffff;
		uint16_t minIndex = 0;
		for (int i = 0; i < DATA_SAMPLES_PER_ROUND; ++i)
		{
			if (pMainVariables->Channel[3].DataBuffer[i] < minVibration)
			{
				minVibration = pMainVariables->Channel[3].DataBuffer[i];
				minIndex = i;
			}
		}
		for (int i = 0; i < DATA_SAMPLES_PER_ROUND; ++i)
		{
			pMainVariables->Channel[2].Buffer[i] = pMainVariables->Channel[3].DataBuffer[(i + minIndex) % DATA_SAMPLES_PER_ROUND];
		}
#endif
#endif
		durationAcq = HAL_GetTick() - startAcq;
		float fRPM = 10000;		
		if (SampleTriggerDuration > 0)
		{
			fRPM = (HAL_RCC_GetHCLKFreq() / SampleTriggerDuration) * 60;
		}
		uint16_t mMotorRPM = fRPM;
#pragma endregion

#pragma region CHECK STATUS
		if (HAL_GetTick() - LastTimeStatuUpdated > 5 * 60000 || LastTimeStatuUpdated == 0)
		{
			LastTimeStatuUpdated = HAL_GetTick();
			Sensor_GetTemperature(&pMainVariables->System.CardTemperature);
			//Sensor_GetHumidity(&pMainVariables->System.CardHumidity);
			RTCC_GetTimeStamp(&pMainVariables->System.TimeStamp);
			pMainVariables->System.TimeStampOffset = pMainVariables->System.TimeStamp - HAL_GetTick();
			durationSampleStatus = HAL_GetTick() - durationAcq - startAcq;
		}
		
		if (HAL_GetTick() - LastTimeLedUpdated > 1000 || LastTimeLedUpdated == 0)
		{
			LastTimeLedUpdated = HAL_GetTick();
#ifdef VIN_DET_PIN
			uint32_t v = AdcReadChannel(&adcVoltageInput, VIN_DET_CH) * VIN_DET_SCALE;
			if (v < 11000)
			{
				RGB_SetColor_ByIndex(LedSelect_Power, RGB_COLOR_YELLOW);
			}
			else if (v > 28000)
			{
				RGB_SetColor_ByIndex(LedSelect_Power, RGB_COLOR_RED);
			}
			else
			{
				RGB_SetColor_ByIndex(LedSelect_Power, RGB_COLOR_GREEN);
			}
#else
			RGB_SetColor_ByIndex(LedSelect_Power, RGB_COLOR_GREEN);
#endif		
			
			if (HAL_GetTick() - LastTimeTrigReceived > 3000)
			{
				SampleTrigIsAvailable = false;
			}
			if (SampleTrigIsAvailable)
			{
				RGB_SetColor_ByIndex(LedSelect_Trigger, RGB_COLOR_GREEN);
			}
			else
			{
				RGB_SetColor_ByIndex(LedSelect_Trigger, RGB_COLOR_RED);
			}
			for (uint8_t s = 0; s < CHANNEL_COUNT; ++s)
			{
				Channel_CheckStatus(s, pMainVariables->Channel[s].DataBuffer[100], true);
				//Channel_CheckStatus(s, Data_test[s][100], true);
			}
			RGB_Render();
			LedProcess();
		}
#pragma endregion

#pragma region SIGNAL CONDITION
		for (uint8_t s = 0; s < CHANNEL_COUNT; ++s)
		{
			for (uint16_t i = 0; i < DATA_SAMPLES_PER_ROUND; ++i)
			{
				pMainVariables->Channel[s].DataBuffer[i] = pMainVariables->Channel[s].DataBuffer[i] * pMainVariables->Channel[s].Calibration.a + pMainVariables->Channel[s].Calibration.b;
			}
		}
#pragma endregion

#pragma region SEND
			if (SamplingIsDone)
//			if ((SamplingIsDone)&&(SampleTrigIsAvailable))
			{
			for (int c = 0; c < CHANNEL_COUNT; ++c)
			{
				pMainVariables->Channel[c].Status = (uint16_t)CahnnelStatus[c];
				pMainVariables->Channel[c].Interval = ((SampleTriggerDuration / DATA_SAMPLES_PER_ROUND) * 1000) / (HAL_RCC_GetHCLKFreq() / 1000000);
				pMainVariables->Channel[c].Length = DATA_SAMPLES_PER_ROUND;
			}
			/*
			for (int c = 2; c < CHANNEL_COUNT; ++c)
			{
				pMainVariables->Channel[c].Status = (uint16_t)CahnnelStatus[c];
				pMainVariables->Channel[c].Interval = ((SampleTriggerDuration / DATA_SAMPLES_PER_ROUND) * 1000) / (HAL_RCC_GetHCLKFreq() / 1000000);
				pMainVariables->Channel[c].Length = 1;
			}
			*/
			msgMain = MessageType_SendData;
			xQueueSend(qMessageHandlerCom, &msgMain, portMAX_DELAY);
			xQueueReceive(qMessageHandlerMain, &msgMain, portMAX_DELAY);
			if (msgMain == MessageType_SendDone)
			{
			}
		}
		durationSend = HAL_GetTick() - startAcq - durationAcq;
		SamplingIsDone = false;
#pragma endregion
	}
}
// -------------------------------------------------------------------------

void interval_calculator(void)
{
	return;

	sum_sample_time = sum_sample_time + SampleTimer_Handle.Init.Period;
	sample_c++;
	if (sample_c < DATA_SAMPLES_PER_ROUND)
	{
		SampleTimer_Handle.Init.Period = ((LastSampleTriggerDuration - sum_sample_time) / ((DATA_SAMPLES_PER_ROUND) - sample_c));
		for (uint8_t s = 0; s < CHANNEL_COUNT; ++s)
		{
			Data_test[s][sample_c] = SampleTimer_Handle.Init.Period;
		}
		HAL_TIM_Base_Init(&SampleTimer_Handle);
	}
}
