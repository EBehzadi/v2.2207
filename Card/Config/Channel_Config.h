// -------------------------------------------------------------------------
// CHANNEL CONFIG
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : June 2022
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __CHANNEL_CONFIG_H__
#define __CHANNEL_CONFIG_H__
// -------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -------------------------------------------------------------------------
#include "HardwareProfile.h"
#include "GlobalVariables.h"
// -------------------------------------------------------------------------
	typedef struct
	{
		bool Enable;
		ChannelType_t Type;
		uint16_t  MarginShort;
		uint16_t  MarginBreak;
		uint16_t  MarginLow;
		uint16_t  MarginHigh;
		float CalibrationRatio;
		float CalibrationOffset;
	}vChannelConfig;
	// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#ifdef HARDWARE_PROFILE_DAQ_CARD_TYPE_420
#define CHANNEL_CONFIG_1		{ true, ChannelType_SensorPressure_420, 65535, 15000, 17000, 65535, 0.63158, -7164.0 }
#define CHANNEL_CONFIG_2		{ true, ChannelType_SensorPressure_420, 65535, 15000, 17000, 65535, 0.63158, -7164.0 }
#define CHANNEL_CONFIG_3		{ true, ChannelType_SensorPressure_420, 50000, 15000, 17000, 45000, 0.63158, -7164.0 }
#define CHANNEL_CONFIG_4		{ true, ChannelType_SensorPressure_420, 50000, 15000, 17000, 45000, 0.63158, -7164.0 }
#endif
#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define CHANNEL_CONFIG_1		{ true, ChannelType_SensorPressure_420, 50000, 15000, 17000, 45000, 0.063158, -7164.0 }
#define CHANNEL_CONFIG_2		{ true, ChannelType_SensorPressure_420, 50000, 15000, 17000, 45000, 0.063158, -7164.0 }
#define CHANNEL_CONFIG_3		{ true, ChannelType_SensorProximity_020, 11400, 11400, 12000, 64000, 0.3958, -4463.0 }
#define CHANNEL_CONFIG_4		{ true, ChannelType_SensorVibration_RAW, 64000, 64000, 22000, 60000, 1, 0 }
#endif
// -------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
// -------------------------------------------------------------------------
