// -------------------------------------------------------------------------
// Serial Communication Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __WIFI_CONFIG_H__
#define __WIFI_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define WIFI_USE_ESP32
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define ESP32_MODE_AT_ENABLE
#define ESP32_UART_SELECT					UART_4
#define ESP32_UART_ALT						GPIO_AF8_UART4
#define ESP32_UART_TX						PD_1
#define ESP32_UART_RX						PD_0
#define ESP32_ENABLE						NC
#define ESP32_BOOT							NC
#define ESP32_BAUDRATE						115200
#define ESP32_RX_BUFFER_SIZE				1024
#define ESP32_TX_BUFFER_SIZE				256
#endif 
// -------------------------------------------------------------------------
#endif 
