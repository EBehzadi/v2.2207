// -------------------------------------------------------------------------
// HARDWARE PRFILE
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : June 2022
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __HARDWARE_PROFILE_H__
#define __HARDWARE_PROFILE_H__
// -------------------------------------------------------------------------
//#define HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define HARDWARE_PROFILE_DAQ_CABINET_V1_2206
//#define HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
//#define HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203
//#define HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
// -------------------------------------------------------------------------
#define HARDWARE_PROFILE_DAQ_CARD_TYPE_420
// -------------------------------------------------------------------------
//#define COM_USE_USB
#define COM_USE_MODBUS
#define COM_USE_LAN
//#define COM_USE_WIFI
#define BOARD_USE_INTOSC
// -------------------------------------------------------------------------
#define SAMPLE_TRIGGER_MODE_ENABLE	
//#define DATA_TYPE_MSB_FIRST
//#define CLIENT_USE_TCP	
//#define DATA_SIMULATED_ENABLE	

#define DATA_USE_CRC_16			
//#define DATA_USE_CRC_32	
#define DATA_CRC_PUT_AFTER_HEADER
// -------------------------------------------------------------------------
#endif 
