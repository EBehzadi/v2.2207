// -------------------------------------------------------------------------
// Ethernet Communication Config
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __LAN_Config_H__
#define __LAN_Config_H__
// -------------------------------------------------------------------------
#include "config.h"
#include "HardwareProfile.h"
// -------------------------------------------------------------------------
#define ETH_USE_LAN8720	
// -------------------------------------------------------------------------
#define LAN_MAC_ADDR0				0x02U
#define LAN_MAC_ADDR1				0x03U
#define LAN_MAC_ADDR2				0x00U
#define LAN_MAC_ADDR3				0x00U
#define LAN_MAC_ADDR4				0x00U
#define LAN_MAC_ADDR5				0x00U     

#define LAN_DEST_IP_ADDR0			((uint8_t)192U)
#define LAN_DEST_IP_ADDR1			((uint8_t)168U)
#define LAN_DEST_IP_ADDR2			((uint8_t)2U)
#ifdef CLIENT_USE_TCP
#define LAN_DEST_IP_ADDR3			((uint8_t)10U)
#else
#define LAN_DEST_IP_ADDR3			((uint8_t)255U)
#endif
									
#define LAN_DEST_PORT				((uint16_t)7777U)
									
#define LAN_IP_ADDR0				((uint8_t) 192U)
#define LAN_IP_ADDR1				((uint8_t) 168U)
#define LAN_IP_ADDR2				((uint8_t) 2U)
#define LAN_IP_ADDR3				((uint8_t) 100U)
									
#define LAN_NETMASK_ADDR0			((uint8_t) 255U)
#define LAN_NETMASK_ADDR1			((uint8_t) 255U)
#define LAN_NETMASK_ADDR2			((uint8_t) 255U)
#define LAN_NETMASK_ADDR3			((uint8_t) 0U)
		
#define LAN_GW_ADDR0				((uint8_t) 192)
#define LAN_GW_ADDR1				((uint8_t) 168)
#define LAN_GW_ADDR2				((uint8_t) 2)
#define LAN_GW_ADDR3				((uint8_t) 1)
// -------------------------------------------------------------------------
#if defined HARDWARE_PROFILE_DAQ_CABINET_V1_2205
#define LAN_PIN_MDC					PC_1
#define LAN_PIN_REF_CLK				PA_1
#define LAN_PIN_MDIO				PA_2
#define LAN_PIN_CRS_DV				PA_7
#define LAN_PIN_RXD0				PC_4
#define LAN_PIN_RXD1				PC_5
#define LAN_PIN_TX_EN				PB_11
#define LAN_PIN_TXD0				PB_12
#define LAN_PIN_TXD1				PB_13
#define LAN_PIN_PWR					PE_11
#define LAN_PIN_RST					PA_3
#define LAN_PIN_ALTERNATE			GPIO_AF11_ETH
#define LAN_LED_LINK_UP				NC

#elif defined HARDWARE_PROFILE_DAQ_CABINET_V1_2206
#define LAN_PIN_MDC					PC_1
#define LAN_PIN_REF_CLK				PA_1
#define LAN_PIN_MDIO				PA_2
#define LAN_PIN_CRS_DV				PA_7
#define LAN_PIN_RXD0				PC_4
#define LAN_PIN_RXD1				PC_5
#define LAN_PIN_TX_EN				PB_11
#define LAN_PIN_TXD0				PB_12
#define LAN_PIN_TXD1				PB_13
#define LAN_PIN_PWR					PE_11
#define LAN_PIN_RST					PA_3
#define LAN_PIN_ALTERNATE			GPIO_AF11_ETH
#define LAN_LED_LINK_UP				NC

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2108
#define LAN_PIN_MDC					PC_1
#define LAN_PIN_REF_CLK				PA_1
#define LAN_PIN_MDIO				PA_2
#define LAN_PIN_CRS_DV				PA_7
#define LAN_PIN_RXD0				PC_4
#define LAN_PIN_RXD1				PC_5
#define LAN_PIN_TX_EN				PB_11
#define LAN_PIN_TXD0				PB_12
#define LAN_PIN_TXD1				PB_13
#define LAN_PIN_PWR					PB_0
#define LAN_PIN_RST					PA_3
#define LAN_PIN_ALTERNATE			GPIO_AF11_ETH
#define LAN_LED_LINK_UP				NC

#elif defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2203 || defined HARDWARE_PROFILE_DAQ_PORTABLE_V1_2205
#define LAN_PIN_MDC					PC_1
#define LAN_PIN_REF_CLK				PA_1
#define LAN_PIN_MDIO				PA_2
#define LAN_PIN_CRS_DV				PA_7
#define LAN_PIN_RXD0				PC_4
#define LAN_PIN_RXD1				PC_5
#define LAN_PIN_TX_EN				PB_11
#define LAN_PIN_TXD0				PB_12
#define LAN_PIN_TXD1				PB_13
#define LAN_PIN_PWR					PA_0
#define LAN_PIN_RST					PA_3
#define LAN_PIN_ALTERNATE			GPIO_AF11_ETH
#define LAN_LED_LINK_UP				NC
#endif
// -------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------
