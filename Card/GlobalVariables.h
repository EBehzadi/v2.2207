// -------------------------------------------------------------------------
// Global Variables
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __GLOBAL_VARIABLES__H__
#define __GLOBAL_VARIABLES__H__
// -------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -------------------------------------------------------------------------
#include "board.h"
#include <config.h>
#include <stdint.h>
#include <stdbool.h>
	// -------------------------------------------------------------------------
#define SYSTEM_ID_LENGTH						MCU_ID_LENGTH

#define MAC_NUMBER_COUNT						6
#define IP_NUMBER_COUNT							4
#define MEMORY_SAVE_TIMEOUT_SECOND				30

#define CHANNEL_COUNT							4
#define DATA_SAMPLES_PER_ROUND					2400
#define CHANNEL_DATA_BUFFER_MAX_SIZE			((DATA_SAMPLES_PER_ROUND*2)+64)
	
#define DATA_SAMPLE_SCALE						1
#define AD_SAMPLING_FREUENCY					8000000
#define COUNTER_FOR_ONE_CYCLE					14382000	
	
#define SEND_TIMEOUT_INTERVAL					0

	// -------------------------------------------------------------------------
	typedef enum
	{
		LedSelect_Trigger  = 5,
		LedSelect_Channel1 = 4,
		LedSelect_Channel2 = 3,
		LedSelect_Channel3 = 2,
		LedSelect_Channel4 = 1,
		LedSelect_Power    = 0,
	}LedSelec_t;
	
	typedef enum
	{
		ChannelType_Undefined           = 0,
		ChannelType_SensorPressure_420  = 11,
		ChannelType_SensorVibration_RAW = 20,
		ChannelType_SensorVibration_420 = 21,
		ChannelType_SensorProximity_020 = 31,
	}ChannelType_t;
	// -------------------------------------------------------------------------
	typedef enum
	{
		ChannelStatus_Disabled              = 0,
		ChannelStatus_Connected             = 1,
		ChannelStatus_Warning_NotConfigured = 2,
		ChannelStatus_Error_OverRange       = 4,
		ChannelStatus_Error_UnderRange      = 8,
		ChannelStatus_Error_BreakIn         = 16,
		ChannelStatus_Error_ShortCircuit    = 32,
		ChannelStatus_Error_Unknown         = 4096,
	}ChannelStatus_t;
	// -------------------------------------------------------------------------
	typedef enum
	{
		CardType_Trigger   = 0,
		CardType_4_20mA    = 1,
		CardType_0_10V     = 2,
		CardType_Vibration = 11,
		CardType_Proximity = 12,
		CardType_Multi     = 20,
	}CardType_t;
	// -------------------------------------------------------------------------
	typedef enum
	{
		MessageType_None     = 0,
		MessageType_LoadData,
		MessageType_SaveData,
		MessageType_SendData,
		MessageType_SendDone,
	}MessageType_t;
	// -------------------------------------------------------------------------
	typedef struct
	{
		uint8_t UID[SYSTEM_ID_LENGTH];
		uint8_t DeviceID;
		uint8_t McuLoad;
		float CpuTemperature;
		uint32_t MemoryCapcity;
		uint32_t MemoryFree;
		uint32_t TimeStamp;
		uint32_t TimeStampOffset;
		float CardTemperature;
		float CardHumidity;
		CardType_t CardType;
	}vSystem;

	typedef struct
	{
		bool Enable;
		bool Connect;
		bool SettingUpdate;
		uint8_t IP[IP_NUMBER_COUNT];
		uint8_t Gateway[IP_NUMBER_COUNT];
		uint8_t NetMask[IP_NUMBER_COUNT];
	} vLAN;

	typedef struct
	{
		bool Enable;
		bool Connect;
		bool SettingUpdate;
		uint8_t ID;
		uint32_t Baudrate;
	} vSerial;

	typedef struct
	{
		float a;
		float b;
	} vChannelCalibration;

	typedef struct
	{
		uint16_t Short;
		uint16_t Break;
		uint16_t LimitLow;
		uint16_t LimitHigh;
	} vChannelMargin;
	
	typedef struct
	{
		bool Enable;
		uint16_t Type;
		uint16_t Status;
		uint32_t Interval;
		uint16_t Length;
		uint16_t DataBuffer[DATA_SAMPLES_PER_ROUND];
		vChannelCalibration Calibration;
		vChannelMargin Margin;
	} vChannelParam;

	typedef struct
	{
		uint8_t M;
		bool Lock;
		bool Reset;
		bool Update;
		vSystem System;
		vLAN LAN;
		vChannelParam Channel[CHANNEL_COUNT];
	}vGlobal;
	// -------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
// -------------------------------------------------------------------------
