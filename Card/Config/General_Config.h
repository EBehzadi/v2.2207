// -------------------------------------------------------------------------
// General Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __GENERAL_CONFIG_H__
#define __GENERAL_CONFIG_H__
// -------------------------------------------------------------------------
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define LED_ALIVE						PC_7
#define GLOBAL_SYNC_PIN					PB_0
#define SENSOR_TRIG_PIN					PD_3
#define VIN_DET_PIN						PA_4
#define VIN_DET_ADC						ADC1
#define VIN_DET_CH						ADC_CHANNEL_18
#define VIN_DET_SCALE					1.0
#define DEVICE_ID_PIN_COUNT				4
#define DEVICE_ID_PINS					{PD_1, PD_4, PD_5, PD_6}

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define LED_ALIVE						PA_15
#define BUTTON_FUNCTION					PA_0
#define GLOBAL_SYNC_PIN					PD_10
#define SENSOR_TRIG_PIN					PD_3
#define DEVICE_ID_PIN_COUNT				4
#define DEVICE_ID_PINS					{PB_8, PB_9, PE_0, PE_1}

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define LED_ALIVE						PD_8
#define GLOBAL_SYNC_PIN					NC
#define SENSOR_TRIG_PIN					PA_5
#define VIN_DET_PIN						PA_4
#define VIN_DET_ADC						ADC1
#define VIN_DET_CH						ADC_CHANNEL_18
#define VIN_DET_SCALE					1.0

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define LED_ALIVE						PD_5
#define SENSOR_TRIG_PIN					PB_0
#define GLOBAL_SYNC_PIN					NC
#define BUTTON_FUNCTION					PE_13
#define RELAY_IPC_PIN					PD_2
#define VIN_DET_PIN						PC_0
#define VIN_DET_ADC						ADC1
#define VIN_DET_CH						ADC_CHANNEL_10
#define VIN_DET_SCALE					0.4755
#endif						    
// -------------------------------------------------------------------------
#endif 
