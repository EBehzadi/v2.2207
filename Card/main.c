// -------------------------------------------------------------------------
// DATALL - DAQ Syetm - CARD
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : August 2021
// -------------------------------------------------------------------------
#include "config.h"
#include "board.h"
#include "gpio.h"
#include "delay.h"
#include "rng.h"
#include "crc.h"
#include "RGB.h"
#include "Modbus.h"

#include "General_Config.h"
#include "GlobalVariables.h"
#include "Task/TaskMain.h"
#include "Task/TaskCom.h"
// -------------------------------------------------------------------------

UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_rx;



// -------------------------------------------------------------------------
vGlobal mainVariables;
vGlobal* pMainVariables;
uint16_t Data_test[4][3600];
// -------------------------------------------------------------------------
void HardFault_Handler()
{
	int yy = 0;
}
void MemManage_Handler()
{
	int yy = 0;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifdef DEVICE_ID_PIN_COUNT
Gpio_t inDeviceID[DEVICE_ID_PIN_COUNT];
#endif
Gpio_t outLanPower;
// -------------------------------------------------------------------------
int main(void)
{
	BoardInitMcu();
	RngInit();
	CrcInit();
	Modbus_Init();
	Modbus_Enable();
	pMainVariables = &mainVariables;
	mainVariables.Lock = false;
	mainVariables.Reset = false;
	mainVariables.Update = false;
	//mainVariables.Semaphore = xSemaphoreCreateMutex();
	BoardGetUniqueId(mainVariables.System.UID);
	mainVariables.System.CardType = CardType_Multi;
	
	mainVariables.System.DeviceID = 0;
#ifdef DEVICE_ID_PIN_COUNT
	PinNames pinDeviceID[] = DEVICE_ID_PINS;
	for (int i = 0; i < DEVICE_ID_PIN_COUNT; ++i)
	{
		GpioInit(&inDeviceID[i], pinDeviceID[i], PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0, PIN_SPEED_FREQ_HIGH);
	}
	mainVariables.System.DeviceID = 0;
	for (int i = 0; i < DEVICE_ID_PIN_COUNT; ++i)
	{
		if (GpioRead(&inDeviceID[i]) == 0)
		{
			mainVariables.System.DeviceID |= (1 << i);
		}
	}
#endif
	mainVariables.System.DeviceID += 1;
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(100);
	Main_TaskInit();
	Com_TaskInit();
//	uint8_t Test[20] = "Hello!";
//	HAL_UART_Transmit(USART6, Test, 10, 100);

#ifdef FREERTOS_ENABLE
	DelaySetTaskMode();
	osKernelStart();
#endif
	while (1)
	{
	}
}
// -------------------------------------------------------------------------
