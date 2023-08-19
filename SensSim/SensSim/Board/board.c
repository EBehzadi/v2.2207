// -------------------------------------------------------------------------
// Board Driver
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include "board.h"
// -------------------------------------------------------------------------
#if defined (STM32F0) || defined (STM32F3)
#define ID_ADDRESS		(0x1FFFF7AC)
#elif defined (STM32F1) 
#define ID_ADDRESS		(0x1FFFF7E8)
#elif defined (STM32F2) || defined (STM32F4)
#define ID_ADDRESS		(0x1FFF7A10)
#elif defined (STM32F7)
#define ID_ADDRESS		(0x1FF0F420)
#elif defined (STM32L0) || defeined (STM32L1)
#define ID_ADDRESS		(0x1FF80050)
#endif
// -------------------------------------------------------------------------
static bool McuInitialized = false;
static volatile bool SystemWakeupTimeCalibrated = false;
// -------------------------------------------------------------------------
void BoardCriticalSectionBegin(uint32_t* mask)
{
	*mask = __get_PRIMASK();
	__disable_irq();
}
// -------------------------------------------------------------------------
void BoardCriticalSectionEnd(uint32_t* mask)
{
	__set_PRIMASK(*mask);
}
// -------------------------------------------------------------------------
void BoardInitMcu(void)
{
	McuInitialized = false;
	if (McuInitialized == false)
	{
		HAL_Init();
		SystemClockConfig();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_PWR_CLK_ENABLE();
	}
	else
	{
		SystemClockConfig();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		__HAL_RCC_PWR_CLK_ENABLE();
	}
}
// -------------------------------------------------------------------------
void BoardResetMcu(void)
{
	CRITICAL_SECTION_BEGIN();
	NVIC_SystemReset();
}
// -------------------------------------------------------------------------
void BoardDeInitMcu(void)
{
}
// -------------------------------------------------------------------------
void BoardGetUniqueId(uint8_t* id)
{
	uint8_t* id_add = (uint8_t*)ID_ADDRESS;
	for (int i = 0; i < 8; ++i)
	{
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
		id[i] = id_add[i];
	}
}
// -------------------------------------------------------------------------
void BoardJumpTobootloader()
{
	void(*SysMemBootJump)(void);
	volatile uint32_t addr = 0x1FFF0000;
	HAL_RCC_DeInit();
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	__disable_irq();
	//__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

	SysMemBootJump = (void(*)(void))(*((uint32_t*)(addr + 4)));
	__set_MSP(*(uint32_t*)addr);
	SysMemBootJump();
}
// -------------------------------------------------------------------------
static void BoardUnusedIoInit(void)
{
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGStandbyMode();
}
// -------------------------------------------------------------------------
void SystemClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
	}
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
// -------------------------------------------------------------------------
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
// -------------------------------------------------------------------------
void NMI_Handler(void)
{
	BoardResetMcu();
}
void HardFault_Handler(void)
{
	BoardResetMcu();
}
void SVC_Handler(void)
{
}
void PendSV_Handler(void)
{
}
void MemManage_Handler(void)
{
}
// -------------------------------------------------------------------------
#ifdef USE_FULL_ASSERT
/*
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %lu\r\n", file, line) */

	printf("Wrong parameters value: file %s on line %lu\r\n", (const char*)file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif
// -------------------------------------------------------------------------
