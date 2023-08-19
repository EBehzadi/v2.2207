// -------------------------------------------------------------------------
// RTCC Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __RTCC_CONFIG_H__
#define __RTCC_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define RTCC_USE_DS1307
#define DS1307_I2C_ID				I2C_4
#define DS1307_I2C_ALT				GPIO_AF4_I2C4
#define DS1307_I2C_SCL				PD_12
#define DS1307_I2C_SDA				PD_13

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define RTCC_USE_DS1307
#define DS1307_I2C_ID				I2C_4
#define DS1307_I2C_ALT				GPIO_AF4_I2C4
#define DS1307_I2C_SCL				PD_12
#define DS1307_I2C_SDA				PD_13

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#endif 
// -------------------------------------------------------------------------
#endif 
