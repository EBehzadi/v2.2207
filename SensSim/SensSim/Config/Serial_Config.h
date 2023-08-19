// -------------------------------------------------------------------------
// Serial Communication Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __SERIAL_CONFIG_H__
#define __SERIAL_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
// -------------------------------------------------------------------------
#define SERIAL_RX_BUFFER_SIZE				128
#define SERIAL_TX_BUFFER_SIZE				128
// -------------------------------------------------------------------------
#define SERIAL_UART_SELECT					UART_2
#define SERIAL_UART_ALT						0
//#define SERIAL_UART_REMAP
#define SERIAL_UART_TX						PA_2
#define SERIAL_UART_RX						PA_3
#define SERIAL_LED_COM						NC
#define SERIAL_BAUDRATE						9600
// -------------------------------------------------------------------------
#endif 
