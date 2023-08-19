// -------------------------------------------------------------------------
// Memory Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __MEMORY_CONFIG_H__
#define __MEMORY_CONFIG_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define MEMORY_USE_W25QXX
#define W25QXX_USE_FREERTOS			0
#define W25QXX_DEBUG				0
#define W25QXX_ID					W25Q32
#define W25QXX_BLOCK_COUNT			64
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define W25QXX_SPI_SELECT			SPI_1
#define W25QXX_SPI_ALTERNATE		GPIO_AF5_SPI1
#define W25QXX_CS					PE_2
#define W25QXX_SCLK					PA_5
#define W25QXX_MISO					PA_6
#define W25QXX_MOSI					PD_7

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define W25QXX_SPI_SELECT			SPI_1
#define W25QXX_SPI_ALTERNATE		GPIO_AF5_SPI1
#define W25QXX_CS					PC_0
#define W25QXX_SCLK					PA_5
#define W25QXX_MISO					PA_6
#define W25QXX_MOSI					PD_7

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define W25QXX_SPI_SELECT			SPI_1
#define W25QXX_SPI_ALTERNATE		GPIO_AF5_SPI1
#define W25QXX_CS					PD_7
#define W25QXX_SCLK					PB_3
#define W25QXX_MISO					PB_4
#define W25QXX_MOSI					PB_5

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define W25QXX_SPI_SELECT			SPI_2
#define W25QXX_SPI_ALTERNATE		GPIO_AF5_SPI2
#define W25QXX_CS					PD_8
#define W25QXX_SCLK					PB_10
#define W25QXX_MISO					PB_14
#define W25QXX_MOSI					PB_15
#endif 
// -------------------------------------------------------------------------
#endif
 
