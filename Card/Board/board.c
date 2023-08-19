// -------------------------------------------------------------------------
// Board Driver
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include "General_Config.h"
#include "config.h"
#include "utilities.h"
#include "board.h"
// -------------------------------------------------------------------------
#define ID1 ( 0x1FF80050 )
#define ID2 ( 0x1FF80054 )
#define ID3 ( 0x1FF80064 )
// -------------------------------------------------------------------------
void BoardCriticalSectionBegin(uint32_t* mask)
{
#ifdef FREERTOS_ENABLE
	//vPortEnterCritical();
#else
	* mask = __get_PRIMASK();
	__disable_irq();
#endif
	* mask = __get_PRIMASK();
	__disable_irq();
}
// -------------------------------------------------------------------------
void BoardCriticalSectionEnd(uint32_t* mask)
{
#ifdef FREERTOS_ENABLE
	//vPortExitCritical();
#else
	__set_PRIMASK(*mask);
#endif
	__set_PRIMASK(*mask);
}
// -------------------------------------------------------------------------
void BoardInitMcu(void)
{
	BoardMpuConfig();
	HAL_Init();
	BoardClockConfig();
	//BoardEnableTick();

	*((uint32_t*)0x51008108) = 1; /* AXI SRAM issue in cut 1.2 */
	__HAL_RCC_CRC_CLK_ENABLE();
	__HAL_RCC_D2SRAM1_CLK_ENABLE();
	__HAL_RCC_D2SRAM2_CLK_ENABLE();
	__HAL_RCC_D2SRAM3_CLK_ENABLE();
	SCB->CACR |= 1 << 2;
}
// -------------------------------------------------------------------------
void BoardResetMcu(void)
{
	CRITICAL_SECTION_BEGIN();
	NVIC_SystemReset();
}
// -------------------------------------------------------------------------
void BoardGetUniqueId(uint8_t* id)
{
	for (int i = 0; i < 8; ++i)
	{
		id[i] = ((*(uint32_t*)MCU_ID_ADDRESS + i));
	}
}
// -------------------------------------------------------------------------
uint32_t BoardGetRandomSeed(void)
{
	return ((*(uint32_t*)MCU_ID_ADDRESS) ^ (*(uint32_t*)MCU_ID_ADDRESS + 0x04) ^ (*(uint32_t*)ID3 + 0x04));
}
// -------------------------------------------------------------------------
void SysTick_Handler_OLD(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef FREERTOS_ENABLE
	osSystickHandler();
#endif
}
// -------------------------------------------------------------------------
void BoardClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
	HAL_PWR_EnableBkUpAccess();

	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 192;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
		| RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
	}
	//__HAL_RCC_CSI_ENABLE() ;

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_UART7 | 
		RCC_PERIPHCLK_SPI1 | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_SPI5 |
		RCC_PERIPHCLK_SPI6 | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_I2C4 | RCC_PERIPHCLK_RNG;
	PeriphClkInitStruct.PLL2.PLL2M = 5;
	PeriphClkInitStruct.PLL2.PLL2N = 192;
	PeriphClkInitStruct.PLL2.PLL2P = 8;
	PeriphClkInitStruct.PLL2.PLL2Q = 8;
	PeriphClkInitStruct.PLL2.PLL2R = 8;
	PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
	PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
	PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
	PeriphClkInitStruct.PLL3.PLL3M = 25;
	PeriphClkInitStruct.PLL3.PLL3N = 384;
	PeriphClkInitStruct.PLL3.PLL3P = 10;
	PeriphClkInitStruct.PLL3.PLL3Q = 10;
	PeriphClkInitStruct.PLL3.PLL3R = 10;
	PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
	PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
	PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
	PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
	PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
	PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
	PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_PLL2;
	PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_PLL2;
	PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	}

#ifdef BOARD_USE_INTOSC
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLL1QCLK, RCC_MCODIV_10);
#endif

	__HAL_RCC_SYSCFG_CLK_ENABLE();
}
// -------------------------------------------------------------------------
void BoardMpuConfig()
{
	HAL_MPU_Disable();

	MPU_Region_InitTypeDef MPU_InitStruct;

	// --- Configure the MPU attributes as Device not cacheable for ETH DMA descriptors 
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30040000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	// --- Configure the MPU attributes as Normal Non Cacheable for LwIP RAM heap which contains the Tx buffers 
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30044000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

	/* Enable I&D caches */
	SCB_EnableICache();
	SCB_EnableDCache();
}
// -------------------------------------------------------------------------
void BoardEnableTick()
{
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
#ifdef FREERTOS_ENABLE
	HAL_NVIC_SetPriority(SysTick_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
#else
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
#endif
}
// -------------------------------------------------------------------------
#ifdef FREERTOS_ENABLE
void vApplicationStackOverflowHook(xTaskHandle xTask, signed portCHAR* pcTaskName)
{
	while (1) ;
}

void vApplicationMallocFailedHook(xTaskHandle xTask, signed portCHAR* pcTaskName)
{
	while (1) ;
}

void vApplicationIdleHook(void)
{
}
#endif
// -------------------------------------------------------------------------
