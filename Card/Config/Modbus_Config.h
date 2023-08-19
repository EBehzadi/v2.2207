// -------------------------------------------------------------------------
// Modbus Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __MODBUS_CONFIG_H__
#define __MODBUS_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "uart.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define MODBUS_UART_SELECT					UART_1
#define MODBUS_UART_ALT						GPIO_AF4_USART1
#define MODBUS_UART_RX						PB_15
#define MODBUS_UART_TX						PB_14
#define MODBUS_DR							PD_8
#define MODBUS_COM_LED						NC

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define MODBUS_UART_SELECT					UART_1
#define MODBUS_UART_ALT						GPIO_AF4_USART1
#define MODBUS_UART_RX						PB_15
#define MODBUS_UART_TX						PB_14
#define MODBUS_DR							PD_8
#define MODBUS_COM_LED						NC

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define MODBUS_UART_ALT						GPIO_AF7_UART7
#define MODBUS_UART_RX						PE_7
#define MODBUS_UART_TX						PE_8
#define MODBUS_DR							PE_9
#define MODBUS_COM_LED						NC

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define MODBUS_UART_SELECT					UART_6
#define MODBUS_UART_ALT						GPIO_AF7_USART6
#define MODBUS_UART_RX						PC_7
#define MODBUS_UART_TX						PC_6
#define MODBUS_DR							PC_8
#define MODBUS_COM_LED						NC
#endif 
// -------------------------------------------------------------------------
typedef enum
{
	MODBUS_MODE_ASCII = 0,
	MODBUS_MODE_RTU
}Modbus_Mode;
// -------------------------------------------------------------------------
#define MODBUS_CONFIG_ADDRESS				245
#define MODBUS_DEFAULT_ADDRESS				99
#define MODBUS_DEFAULT_MODE					MODBUS_MODE_RTU
#define MODBUS_DEFAULT_BAUDRATE				115200
#define MODBUS_DEFAULT_BAUDRATE_INDEX		UART_9600
#define MODBUS_DEFAULT_CHAR_LENGTH			UART_8_BIT
#define MODBUS_DEFAULT_STOP_BIT				UART_1_STOP_BIT
#define MODBUS_DEFAULT_PARITY				NO_PARITY
#define MODBUS_DEFAULT_FLOW_CONTROL			NO_FLOW_CTRL
#define MODBUS_CONFIG_TIMEOUT_SEC			15
	// -------------------------------------------------------------------------
#define MODBUS_SECURITY_CODE_VALUE			0xAAFD
	// -------------------------------------------------------------------------
#define MODBUS_REG_SLAVE_ADDRESS			0x4000
#define MODBUS_REG_GROUP_ADDRESS			0x4001
#define MODBUS_REG_BRANCH_ADDRESS			0x4002
#define MODBUS_REG_SECURITY					0x4003
#define MODBUS_REG_RESERVED					0x4004
#define MODBUS_REG_ACII_RTU					0x4005
#define MODBUS_REG_PARITY					0x4006
#define MODBUS_REG_STOPBIT					0x4007
#define MODBUS_REG_CHAR_LENGTH				0x4008
#define MODBUS_REG_BAUDRATE					0x4009
#define MODBUS_REG_CONFIG_TIMEOUT			0x4020
#define MODBUS_REG_RESET_TO_FACTORY			0x4040
#define MODBUS_REG_MODULE_ADDRESS			0x4050
#define MODBUS_REG_SERIAL_NUMBER_START		0x4060
#define MODBUS_REG_SERIAL_NUMBER_END		0x406F
#define MODBUS_REG_ALIVE_TIME_YEAR			0x4070
#define MODBUS_REG_ALIVE_TIME_MONTH			0x4071
#define MODBUS_REG_ALIVE_TIME_DAY			0x4072
#define MODBUS_REG_ALIVE_TIME_HOUR			0x4073
#define MODBUS_REG_ALIVE_TIME_MINUTE		0x4074
#define MODBUS_REG_ALIVE_TIME_SECOND		0x4075
#define MODBUS_REG_LAST_SERVICE_YEAR		0x4080
#define MODBUS_REG_LAST_SERVICE_MONTH		0x4081
#define MODBUS_REG_LAST_SERVICE_DAY			0x4082
#define MODBUS_REG_LAST_SERVICE_HOUR		0x4083
#define MODBUS_REG_LAST_SERVICE_MINUTE		0x4084
#define MODBUS_REG_LAST_SERVICE_SECOND		0x4085
#define MODBUS_REG_TIMEOUT_SEC				0x4100

typedef enum
{
	MODBUS_REGISTER_CONFIG_RESET_TO_FACTORY = 0,
	MODBUS_REGISTER_CONFIG_SLAVE_ADDRESS,
	MODBUS_REGISTER_CONFIG_GROUP_ADDRESS,
	MODBUS_REGISTER_CONFIG_BRANCH_ADDRESS,
	MODBUS_REGISTER_CONFIG_SECURITY,
	MODBUS_REGISTER_CONFIG_ASCII_RTU,
	MODBUS_REGISTER_CONFIG_PARITY,
	MODBUS_REGISTER_CONFIG_STOPBIT,
	MODBUS_REGISTER_CONFIG_CHAR_LENGTH,
	MODBUS_REGISTER_CONFIG_BAUDRATE,
	MODBUS_REGISTER_CONFIG_TIMEOUT,

	MODBUS_REGISTER_MODULE_ADDRESS,

	MODBUS_REGISTER_ALIVE_TIME_YEAR,
	MODBUS_REGISTER_ALIVE_TIME_MONTH,
	MODBUS_REGISTER_ALIVE_TIME_DAY,
	MODBUS_REGISTER_ALIVE_TIME_HOUR,
	MODBUS_REGISTER_ALIVE_TIME_MINUTE,
	MODBUS_REGISTER_ALIVE_TIME_SECOND,

	MODBUS_REGISTER_LAST_SERVICE_TIME_YEAR,
	MODBUS_REGISTER_LAST_SERVICE_TIME_MONTH,
	MODBUS_REGISTER_LAST_SERVICE_TIME_DAY,
	MODBUS_REGISTER_LAST_SERVICE_TIME_HOUR,
	MODBUS_REGISTER_LAST_SERVICE_TIME_MINUTE,
	MODBUS_REGISTER_LAST_SERVICE_TIME_SECOND,

	MODBUS_REGISTER_SERIAL_NUMBER_1,
	MODBUS_REGISTER_SERIAL_NUMBER_2,
	MODBUS_REGISTER_SERIAL_NUMBER_3,
	MODBUS_REGISTER_SERIAL_NUMBER_4,
	MODBUS_REGISTER_SERIAL_NUMBER_5,
	MODBUS_REGISTER_SERIAL_NUMBER_6,
	MODBUS_REGISTER_SERIAL_NUMBER_7,
	MODBUS_REGISTER_SERIAL_NUMBER_8,
	MODBUS_REGISTER_SERIAL_NUMBER_9,
	MODBUS_REGISTER_SERIAL_NUMBER_10,
	MODBUS_REGISTER_SERIAL_NUMBER_11,
	MODBUS_REGISTER_SERIAL_NUMBER_12,
	MODBUS_REGISTER_SERIAL_NUMBER_13,
	MODBUS_REGISTER_SERIAL_NUMBER_14,
	MODBUS_REGISTER_SERIAL_NUMBER_15,
	MODBUS_REGISTER_SERIAL_NUMBER_16,

	MODBUS_REGISTER_CONFIG_RESERVED,

	MODBUS_REGISTER_CONFIG_COUNT,
}Modbus_ConfigParameter;
// -------------------------------------------------------------------------
#define MODBUS_REGISTER_STATUS				0x1000
#define MODBUS_REGISTER_TEMPERATURE			0x2000
#define MODBUS_REGISTER_HUMIDITY			0x2001
// -------------------------------------------------------------------------
typedef enum
{
	REGID_STATUS = MODBUS_REGISTER_CONFIG_COUNT,
	REGID_SENSOR_HUMIDITY,
	MODBUS_REGISTER_COUNT,
}ModbusRegisterId_t;
// -------------------------------------------------------------------------
#endif 
