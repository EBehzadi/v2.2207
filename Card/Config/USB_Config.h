// -------------------------------------------------------------------------
// USB Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __USB_CONFIG_H__
#define __USB_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define USB_USE_DEVICE
#define USE_USB_FS
#define USE_USB_HS_IN_FS
#define USB_RX_DATA_SIZE                    128U
#define USB_TX_DATA_SIZE                    128U
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define USB_DET_PIN     					PA_10

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define USB_DET_PIN     					PE_13

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define USB_DET_PIN     					PA_10

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define USB_DET_PIN     					PA_10
#endif 
// -------------------------------------------------------------------------
#endif 
