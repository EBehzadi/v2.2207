// -------------------------------------------------------------------------
// AD Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __AD_CONFIG_H__
#define __AD_CONFIG_H__
// -------------------------------------------------------------------------
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define AD_USE_AD7686	
#define AD7686_COUNT				4
#define AD7686_VREF_VALUE		 	{2.5, 2.5, 2.5, 2.5}	
#define AD7686_SPI_PRI_CLOCK		{96000000,96000000,96000000,96000000}

#define AD7686_SPI_SELECT			{SPI_2, SPI_4, SPI_6, SPI_3}
#define AD7686_SPI_ALTERNATE		{GPIO_AF5_SPI2, GPIO_AF5_SPI4, GPIO_AF8_SPI6, GPIO_AF6_SPI3}
#define AD7686_CNV					{PE_15, PC_13, PB_6, PD_0}
#define AD7686_MOSI					{PC_3, PE_6, PB_5, PC_12}
#define AD7686_MISO					{PC_2, PE_5, PB_4, PC_11}
#define AD7686_SCLK					{PB_10, PE_2, PB_3, PC_10}	

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define AD_USE_AD7686	
#define AD7686_COUNT				4
#define AD7686_VREF_VALUE		 	{2.5, 2.5, 2.5, 2.5}	
#define AD7686_SPI_PRI_CLOCK		{96000000,96000000,96000000,96000000}

#define AD7686_SPI_SELECT			{SPI_3, SPI_6, SPI_4, SPI_2}
#define AD7686_SPI_ALTERNATE		{GPIO_AF6_SPI3, GPIO_AF8_SPI6, GPIO_AF5_SPI4, GPIO_AF5_SPI2}
#define AD7686_CNV					{PD_0, PB_6, PC_13, PE_15}
#define AD7686_MOSI					{PC_12, PB_5, PE_6, PC_3}
#define AD7686_MISO					{PC_11, PB_4, PE_5, PC_2}
#define AD7686_SCLK					{PC_10, PB_3, PE_2, PB_10}

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define AD_USE_AD7686	
#define AD7686_COUNT				4
#define AD7686_VREF_VALUE		 	{2.5, 2.5, 2.5, 2.5}	
#define AD7686_SPI_PRI_CLOCK		{96000000,96000000,96000000,96000000}

#define AD7686_SPI_SELECT			{SPI_6, SPI_4, SPI_1, SPI_3}
#define AD7686_SPI_ALTERNATE		{GPIO_AF8_SPI6, GPIO_AF5_SPI4, GPIO_AF5_SPI1, GPIO_AF6_SPI3}
#define AD7686_CNV					{PA_4, PE_4, PB_6, PA_15}
#define AD7686_MOSI					{NC, PE_6, PB_5, PC_12}
#define AD7686_MISO					{PA_6, PE_5, PB_4, PC_11}
#define AD7686_SCLK					{PA_5, PE_2, PB_3, PC_10}	
#endif						    
// -------------------------------------------------------------------------
#endif 
