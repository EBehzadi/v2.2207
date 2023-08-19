// -------------------------------------------------------------------------
// Task :: Communication Handler
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//#define SEND_FOR_EVER
// -------------------------------------------------------------------------
#include "TaskCom.h"
#include "GlobalVariables.h"
#include "HardwareProfile.h"
#include "crc.h"
#include "delay.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// -------------------------------------------------------------------------
extern xQueueHandle qMessageHandlerMain;
xQueueHandle qMessageHandlerCom;
MessageType_t msgCom;
// -------------------------------------------------------------------------
#ifdef COM_USE_USB
#include "USB.h"
#endif
#ifdef COM_USE_MODBUS
#include "Modbus.h"
#endif
#ifdef COM_USE_WIFI
#include "WiFi.h"
#endif
#ifdef COM_USE_LAN
#include "LAN.h"
#include "LAN_Config.h"
#include "client_netconn.h"
#endif
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void Com_Task(void* argument);
void test(uint8_t select, uint32_t counter);
void Modbus_check(void);
// -------------------------------------------------------------------------
extern vGlobal* pMainVariables;
extern uint16_t Data_test[4][3600];
extern bool trigger_enable;
extern uint8_t Modbus_RxBuffer[];
extern uint32_t SampleTriggerDuration;

bool MData_R = false;
extern uint64_t Reference_time;
extern uint64_t OffSet_time;


bool test_flag = false;
// -------------------------------------------------------------------------
#ifdef COM_USE_MODBUS
Modbus_CommunicationParameter Modbus_Parameter;
#endif

#ifdef COM_USE_USB
uint8_t Usb_Buffer[64];
uint32_t Usb_ReceivedLength = 0;
#endif

#ifdef COM_USE_WIFI
uint8_t WiFi_BufferReceive[1024];
bool bUpdateWeatherFromWiFi = true;
bool bUpdateTimeFromWiFi = true;
#endif

#ifdef COM_USE_LAN
uint8_t PacketBuffer[CHANNEL_DATA_BUFFER_MAX_SIZE];
uint32_t Data_SendCounter = 0;
uint32_t mPacketCounter = 0;
uint64_t Data_SendTimeStamp = 0;
extern uint64_t TimeStamp;
int OffSet = 0;


#endif
// -------------------------------------------------------------------------
void Com_TaskInit(void)
{
	xTaskCreate(Com_Task, "ComTask", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, NULL);
	qMessageHandlerCom = xQueueGenericCreate(1, sizeof(msgCom), 0);
}
// -------------------------------------------------------------------------
void Com_Task(void* argument)
{
#ifdef COM_USE_USB
	UsbInit(1152000, 0x01, 0x00, 0x08);
#endif

#ifdef COM_USE_LAN
	pMainVariables->LAN.IP[0] = LAN_IP_ADDR0;
	pMainVariables->LAN.IP[1] = LAN_IP_ADDR1;
	pMainVariables->LAN.IP[2] = LAN_IP_ADDR2;
	pMainVariables->LAN.IP[3] = LAN_IP_ADDR3 + pMainVariables->System.DeviceID;

	pMainVariables->LAN.NetMask[0] = LAN_NETMASK_ADDR0;
	pMainVariables->LAN.NetMask[1] = LAN_NETMASK_ADDR1;
	pMainVariables->LAN.NetMask[2] = LAN_NETMASK_ADDR2;
	pMainVariables->LAN.NetMask[3] = LAN_NETMASK_ADDR3;

	pMainVariables->LAN.Gateway[0] = LAN_GW_ADDR0;
	pMainVariables->LAN.Gateway[1] = LAN_GW_ADDR1;
	pMainVariables->LAN.Gateway[2] = LAN_GW_ADDR2;
	pMainVariables->LAN.Gateway[3] = LAN_GW_ADDR3;

	uint8_t LAN_IP[4] = { pMainVariables->LAN.IP[0], pMainVariables->LAN.IP[1], pMainVariables->LAN.IP[2], pMainVariables->LAN.IP[3] };
	uint8_t LAN_NetMask[4] = { pMainVariables->LAN.NetMask[0], pMainVariables->LAN.NetMask[1], pMainVariables->LAN.NetMask[2], pMainVariables->LAN.NetMask[3] };
	uint8_t LAN_Gateway[4] = { pMainVariables->LAN.Gateway[0], pMainVariables->LAN.Gateway[1], pMainVariables->LAN.Gateway[2], pMainVariables->LAN.Gateway[3] };
	//uint8_t LAN_Gateway[4] = { 0, 0, 0, 0 };
	LAN_Init(LAN_IP, LAN_NetMask, LAN_Gateway, LAN_APPLICATION_CLIENT);
#endif

#ifdef COM_USE_MODBUS
	Modbus_Init();
	if (Modbus_Parameter.BaudRate < 4800)
	{
		Modbus_Parameter.DeviceID = pMainVariables->System.DeviceID;
		Modbus_Parameter.BaudRate = 115200;
		Modbus_Parameter.WordLength = UART_8_BIT;
		Modbus_Parameter.StopBits = UART_1_STOP_BIT;
		Modbus_Parameter.Parity = NO_PARITY;
		Modbus_Parameter.FlowControl = NO_FLOW_CTRL;
	}
	Modbus_Config(Modbus_Parameter.BaudRate, Modbus_Parameter.WordLength, Modbus_Parameter.StopBits, Modbus_Parameter.Parity, Modbus_Parameter.FlowControl);
	Modbus_SetAddress(Modbus_Parameter.DeviceID);
#endif


#ifdef COM_USE_WIFI
	WiFi_Init();
#endif

#ifdef DATA_USE_CRC_32
	//CrcConfig(CRC_LEGNTH_32B, 0x1021, 0xFFFF, CRC_INPUT_INVERSION_BYTE, CRC_INPUT_FORMAT_BYTE, true);
#elif defined DATA_USE_CRC_16
	CrcConfig(CRC_LEGNTH_16B, 0x1021, 0xFFFF, CRC_INPUT_INVERSION_BYTE, CRC_INPUT_FORMAT_BYTE, true);
#endif
	uint8_t Test[20] = "\fDone";
	while (1)
	{
		Modbus_check();
		if (xQueueReceive(qMessageHandlerCom, &msgCom, portMAX_DELAY) == pdTRUE)
		{
			switch ((MessageType_t)msgCom)
			{
			case MessageType_SendData:
				mPacketCounter++;
				CRITICAL_SECTION_BEGIN();
				for (int c = 0; c < CHANNEL_COUNT; ++c)
				{
					Com_UpdateAndSendData(c, mPacketCounter);
					test_flag = false;
				}
//				Modbus_Write(Test, 10);
				msgCom = MessageType_SendDone;
				CRITICAL_SECTION_END();
				xQueueSend(qMessageHandlerMain, &msgCom, portMAX_DELAY);
				break;
			}

#ifdef COM_USE_USB
			if (UsbGetChar(Usb_Buffer))
			{
			}
#endif
		}
	}
}
// -------------------------------------------------------------------------
void Com_Reset()
{
#ifdef COM_USE_LAN
	pMainVariables->LAN.Enable = true;
#else
	pMainVariables->LAN.Enable = false;
#endif
	pMainVariables->LAN.Connect = false;
}
// -------------------------------------------------------------------------
bool Com_UpdateAndSendData(uint8_t select, uint32_t counter)
{
	if (pMainVariables->Channel[0].Status != 1)
	{
		int dd = 0;
	}
	int index = 0;
	// Header
	PacketBuffer[index++] = 0xAF;
	PacketBuffer[index++] = 0xDB;
	PacketBuffer[index++] = 0xAA;
	PacketBuffer[index++] = 0x00;
	
#ifdef DATA_CRC_PUT_AFTER_HEADER
	int indexCRC = index;
	index++;
	index++;
#endif

	// Counter
	uint32_t crcStartIndex = index;
	if (counter != 0)
	{
		Data_SendCounter = counter;
	}
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendCounter >> 24;
	PacketBuffer[index++] = Data_SendCounter >> 16;
	PacketBuffer[index++] = Data_SendCounter >> 8;
	PacketBuffer[index++] = Data_SendCounter >> 0;
#else
	PacketBuffer[index++] = Data_SendCounter >> 0;
	PacketBuffer[index++] = Data_SendCounter >> 8;
	PacketBuffer[index++] = Data_SendCounter >> 16;
	PacketBuffer[index++] = Data_SendCounter >> 24;
#endif
	Data_SendCounter++;

	// Time Stamp

//	Data_SendTimeStamp = (uint64_t)(HAL_GetTick()); //+ pMainVariables->System.TimeStampOffset);// * (uint64_t)10000;
	Data_SendTimeStamp = TimeStamp;

#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendTimeStamp >> 56;
	PacketBuffer[index++] = Data_SendTimeStamp >> 48;
	PacketBuffer[index++] = Data_SendTimeStamp >> 40;
	PacketBuffer[index++] = Data_SendTimeStamp >> 32;
	PacketBuffer[index++] = Data_SendTimeStamp >> 24;
	PacketBuffer[index++] = Data_SendTimeStamp >> 16;
	PacketBuffer[index++] = Data_SendTimeStamp >> 8;
	PacketBuffer[index++] = Data_SendTimeStamp >> 0;
#else
	PacketBuffer[index++] = Data_SendTimeStamp >> 0;
	PacketBuffer[index++] = Data_SendTimeStamp >> 8;
	PacketBuffer[index++] = Data_SendTimeStamp >> 16;
	PacketBuffer[index++] = Data_SendTimeStamp >> 24;
	PacketBuffer[index++] = Data_SendTimeStamp >> 32;
	PacketBuffer[index++] = Data_SendTimeStamp >> 40;
	PacketBuffer[index++] = Data_SendTimeStamp >> 48;
	PacketBuffer[index++] = Data_SendTimeStamp >> 56;
#endif

#ifdef DATA_TYPE_MSB_FIRST
	// UID
	/*
	PacketBuffer[index++] = pMainVariables->System.UID[7];
	PacketBuffer[index++] = pMainVariables->System.UID[6];
	PacketBuffer[index++] = pMainVariables->System.UID[5];
	PacketBuffer[index++] = pMainVariables->System.UID[4];
	PacketBuffer[index++] = pMainVariables->System.UID[3];
	PacketBuffer[index++] = pMainVariables->System.UID[2];
	PacketBuffer[index++] = pMainVariables->System.UID[1];
	PacketBuffer[index++] = pMainVariables->System.UID[0];
	*/
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = pMainVariables->System.DeviceID;
	PacketBuffer[index++] = channel;
#else
	// UID
	PacketBuffer[index++] = select + 1;
	PacketBuffer[index++] = pMainVariables->System.DeviceID;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	/*
	PacketBuffer[index++] = pMainVariables->System.UID[0];
	PacketBuffer[index++] = pMainVariables->System.UID[1];
	PacketBuffer[index++] = pMainVariables->System.UID[2];
	PacketBuffer[index++] = pMainVariables->System.UID[3];
	PacketBuffer[index++] = pMainVariables->System.UID[4];
	PacketBuffer[index++] = pMainVariables->System.UID[5];
	PacketBuffer[index++] = pMainVariables->System.UID[6];
	PacketBuffer[index++] = pMainVariables->System.UID[7];
	*/
#endif

	// Payload
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 24;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 16;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 0;
	for (int i = 0; i < pMainVariables->Channel[select].Length; ++i)
	{
		DataBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 8);
		DataBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 0);
	}
#else
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 8;
	PacketBuffer[index++] = 1;
	PacketBuffer[index++] = 0;
//	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 0;
//	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 16;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 24;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 8;
	for (int i = 0; i < pMainVariables->Channel[select].Length; ++i)
	{
//		PacketBuffer[index++] = (Data_test[select][i] >> 0);
//		PacketBuffer[index++] = (Data_test[select][i] >> 8);
//		PacketBuffer[index++] = (SampleTriggerDuration >> 0);
//		PacketBuffer[index++] = (SampleTriggerDuration >> 8);
		PacketBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 0);
		PacketBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 8);
		
	}
#endif

	/*
	// Reserved
	uint16_t resLen = SENSOR_DATA_BUFFER_SIZE - index - 8;
	for (int i = 0; i < resLen; ++i)
	{
		PacketBuffer[index++] = 0x00;
	}
	*/

	// CRC
#ifdef DATA_USE_CRC_32
	uint32_t crcLength = (index - crcStartIndex) / sizeof(uint32_t);
	uint32_t Data_SendCRC32 = CrcCalculate32((uint32_t*)&PacketBuffer[crcStartIndex], crcLength, true);
	Data_SendCRC32 ~= Data_SendCRC32;
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendCRC32 >> 24;
	PacketBuffer[index++] = Data_SendCRC32 >> 16;
	PacketBuffer[index++] = Data_SendCRC32 >> 8;
	PacketBuffer[index++] = Data_SendCRC32 >> 0;
#else
	PacketBuffer[index++] = Data_SendCRC32 >> 0;
	PacketBuffer[index++] = Data_SendCRC32 >> 8;
	PacketBuffer[index++] = Data_SendCRC32 >> 16;
	PacketBuffer[index++] = Data_SendCRC32 >> 24;
#endif
#elif defined DATA_USE_CRC_16
	uint32_t crcLength = (index - crcStartIndex);
	uint16_t Data_SendCRC16 = CrcCalculate((uint32_t*)&PacketBuffer[crcStartIndex], crcLength, true) & 0xFFFF;

#ifdef DATA_TYPE_MSB_FIRST
#ifdef DATA_CRC_PUT_AFTER_HEADER
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 8;
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 0;
#else
	PacketBuffer[index++] = Data_SendCRC16 >> 8;
	PacketBuffer[index++] = Data_SendCRC16 >> 0;
#endif
#else
#ifdef DATA_CRC_PUT_AFTER_HEADER
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 0;
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 8;
#else
	PacketBuffer[index++] = Data_SendCRC16 >> 0;
	PacketBuffer[index++] = Data_SendCRC16 >> 8;
#endif
#endif
#endif

	// Footer
	PacketBuffer[index++] = 0xFF;
	PacketBuffer[index++] = 0xAA;
	PacketBuffer[index++] = 0xDF;
	PacketBuffer[index++] = 0x00;

	// SEND...
#ifdef SEND_FOR_EVER
	while (1)
#endif
	{
		// Wait Befor Last Send
#ifdef SEND_TIMEOUT_INTERVAL
		static uint32_t Com_LastSentTime = 0;
		while (HAL_GetTick() - Com_LastSentTime < SEND_TIMEOUT_INTERVAL)
		{
			DelayMs(1);
		}
		Com_LastSentTime = HAL_GetTick();
#endif
		int dd = 0;
		for (int i = 100; i < 250; i++)
		{
			dd += pMainVariables->Channel[select].DataBuffer[i];
		}
		if ((dd == 0)&&(!test_flag))
		{
			test_flag = true;
			test(select, counter);
		}		
	Client_UpdateData(PacketBuffer, index);
	}
	return true;
}
// -------------------------------------------------------------------------

void test(uint8_t select, uint32_t counter)
{
		int index = 0;
	// Header
	PacketBuffer[index++] = 0xAF;
	PacketBuffer[index++] = 0xDB;
	PacketBuffer[index++] = 0xAA;
	PacketBuffer[index++] = 0x00;
	
#ifdef DATA_CRC_PUT_AFTER_HEADER
	int indexCRC = index;
	index++;
	index++;
#endif

	// Counter
	uint32_t crcStartIndex = index;
	if (counter != 0)
	{
		Data_SendCounter = counter;
	}
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendCounter >> 24;
	PacketBuffer[index++] = Data_SendCounter >> 16;
	PacketBuffer[index++] = Data_SendCounter >> 8;
	PacketBuffer[index++] = Data_SendCounter >> 0;
#else
	PacketBuffer[index++] = Data_SendCounter >> 0;
	PacketBuffer[index++] = Data_SendCounter >> 8;
	PacketBuffer[index++] = Data_SendCounter >> 16;
	PacketBuffer[index++] = Data_SendCounter >> 24;
#endif
	Data_SendCounter++;

	// Time Stamp

//	Data_SendTimeStamp = (uint64_t)(HAL_GetTick()); //+ pMainVariables->System.TimeStampOffset);// * (uint64_t)10000;
	Data_SendTimeStamp = TimeStamp;

#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendTimeStamp >> 56;
	PacketBuffer[index++] = Data_SendTimeStamp >> 48;
	PacketBuffer[index++] = Data_SendTimeStamp >> 40;
	PacketBuffer[index++] = Data_SendTimeStamp >> 32;
	PacketBuffer[index++] = Data_SendTimeStamp >> 24;
	PacketBuffer[index++] = Data_SendTimeStamp >> 16;
	PacketBuffer[index++] = Data_SendTimeStamp >> 8;
	PacketBuffer[index++] = Data_SendTimeStamp >> 0;
#else
	PacketBuffer[index++] = Data_SendTimeStamp >> 0;
	PacketBuffer[index++] = Data_SendTimeStamp >> 8;
	PacketBuffer[index++] = Data_SendTimeStamp >> 16;
	PacketBuffer[index++] = Data_SendTimeStamp >> 24;
	PacketBuffer[index++] = Data_SendTimeStamp >> 32;
	PacketBuffer[index++] = Data_SendTimeStamp >> 40;
	PacketBuffer[index++] = Data_SendTimeStamp >> 48;
	PacketBuffer[index++] = Data_SendTimeStamp >> 56;
#endif

#ifdef DATA_TYPE_MSB_FIRST
	// UID
	/*
	PacketBuffer[index++] = pMainVariables->System.UID[7];
	PacketBuffer[index++] = pMainVariables->System.UID[6];
	PacketBuffer[index++] = pMainVariables->System.UID[5];
	PacketBuffer[index++] = pMainVariables->System.UID[4];
	PacketBuffer[index++] = pMainVariables->System.UID[3];
	PacketBuffer[index++] = pMainVariables->System.UID[2];
	PacketBuffer[index++] = pMainVariables->System.UID[1];
	PacketBuffer[index++] = pMainVariables->System.UID[0];
	*/
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = pMainVariables->System.DeviceID;
	PacketBuffer[index++] = channel;
#else
	// UID
	PacketBuffer[index++] = select + 1;
	PacketBuffer[index++] = pMainVariables->System.DeviceID;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	PacketBuffer[index++] = 0x00;
	/*
	PacketBuffer[index++] = pMainVariables->System.UID[0];
	PacketBuffer[index++] = pMainVariables->System.UID[1];
	PacketBuffer[index++] = pMainVariables->System.UID[2];
	PacketBuffer[index++] = pMainVariables->System.UID[3];
	PacketBuffer[index++] = pMainVariables->System.UID[4];
	PacketBuffer[index++] = pMainVariables->System.UID[5];
	PacketBuffer[index++] = pMainVariables->System.UID[6];
	PacketBuffer[index++] = pMainVariables->System.UID[7];
	*/
#endif

	// Payload
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 24;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 16;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 0;
	for (int i = 0; i < pMainVariables->Channel[select].Length; ++i)
	{
		DataBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 8);
		DataBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 0);
	}
#else
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Type >> 8;
	PacketBuffer[index++] = 1;
	PacketBuffer[index++] = 0;
//	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 0;
//	PacketBuffer[index++] = pMainVariables->Channel[select].Status >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 8;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 16;
	PacketBuffer[index++] = pMainVariables->Channel[select].Interval >> 24;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 0;
	PacketBuffer[index++] = pMainVariables->Channel[select].Length >> 8;
	for (int i = 0; i < pMainVariables->Channel[select].Length; ++i)
	{

		PacketBuffer[index++] = (Data_test[select][i] >> 0);
		PacketBuffer[index++] = (Data_test[select][i] >> 8);
		//PacketBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 0);
		//PacketBuffer[index++] = (pMainVariables->Channel[select].DataBuffer[i] >> 8);
	}
#endif

	/*
	// Reserved
	uint16_t resLen = SENSOR_DATA_BUFFER_SIZE - index - 8;
	for (int i = 0; i < resLen; ++i)
	{
		PacketBuffer[index++] = 0x00;
	}
	*/

	// CRC
#ifdef DATA_USE_CRC_32
	uint32_t crcLength = (index - crcStartIndex) / sizeof(uint32_t);
	uint32_t Data_SendCRC32 = CrcCalculate32((uint32_t*)&PacketBuffer[crcStartIndex], crcLength, true);
	Data_SendCRC32 ~= Data_SendCRC32;
#ifdef DATA_TYPE_MSB_FIRST
	PacketBuffer[index++] = Data_SendCRC32 >> 24;
	PacketBuffer[index++] = Data_SendCRC32 >> 16;
	PacketBuffer[index++] = Data_SendCRC32 >> 8;
	PacketBuffer[index++] = Data_SendCRC32 >> 0;
#else
	PacketBuffer[index++] = Data_SendCRC32 >> 0;
	PacketBuffer[index++] = Data_SendCRC32 >> 8;
	PacketBuffer[index++] = Data_SendCRC32 >> 16;
	PacketBuffer[index++] = Data_SendCRC32 >> 24;
#endif
#elif defined DATA_USE_CRC_16
	uint32_t crcLength = (index - crcStartIndex);
	uint16_t Data_SendCRC16 = CrcCalculate((uint32_t*)&PacketBuffer[crcStartIndex], crcLength, true) & 0xFFFF;

#ifdef DATA_TYPE_MSB_FIRST
#ifdef DATA_CRC_PUT_AFTER_HEADER
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 8;
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 0;
#else
	PacketBuffer[index++] = Data_SendCRC16 >> 8;
	PacketBuffer[index++] = Data_SendCRC16 >> 0;
#endif
#else
#ifdef DATA_CRC_PUT_AFTER_HEADER
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 0;
	PacketBuffer[indexCRC++] = Data_SendCRC16 >> 8;
#else
	PacketBuffer[index++] = Data_SendCRC16 >> 0;
	PacketBuffer[index++] = Data_SendCRC16 >> 8;
#endif
#endif
#endif

	// Footer
	PacketBuffer[index++] = 0xFF;
	PacketBuffer[index++] = 0xAA;
	PacketBuffer[index++] = 0xDF;
	PacketBuffer[index++] = 0x00;

}

void Modbus_check(void)
{
	char data[8] = "";
	int i = 0;
	uint8_t M1[20];
	if (MData_R)
	{
		if ((Modbus_RxBuffer[4] == 0x4B)&&(Modbus_RxBuffer[3] == 0x43)&&(Modbus_RxBuffer[2] == 0x41))
		{
			Reference_time = Modbus_RxBuffer[5];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[6];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[7];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[8];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[9];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[10];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[11];
			Reference_time = (Reference_time << 8) + Modbus_RxBuffer[12];
			OffSet_time = ((uint64_t)(HAL_GetTick())) / 1000;
			mPacketCounter = 0;
			M1[0]  = 0x41;
			M1[1]  = 0x43;
			M1[2]  = 0x4B;
			Modbus_Write(M1, 3);
		}
			
		Modbus_RxBuffer[0] = 0;
		MData_R = false;
	}
}