// -------------------------------------------------------------------------
// Time Base Driver
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include "config.h"
// -------------------------------------------------------------------------
static TIM_HandleTypeDef        TimHandle;
// -------------------------------------------------------------------------
void TIM6_DAC_IRQHandler(void);
void PeriodElapsedCallback(TIM_HandleTypeDef* htim);
// -------------------------------------------------------------------------
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock, uwAPB1Prescaler;
    uint32_t              uwPrescalerValue;
    uint32_t              pFLatency;

    if (TickPriority < (1UL << __NVIC_PRIO_BITS))
    {
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0U);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        uwTickPrio = TickPriority;
    }
    else
    {
        return HAL_ERROR;
    }

    __HAL_RCC_TIM6_CLK_ENABLE();
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    TimHandle.Instance = TIM6;
    TimHandle.PeriodElapsedCallback = PeriodElapsedCallback;
    TimHandle.Init.Period = (1000000U / 1000U) - 1U;
    TimHandle.Init.Prescaler = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    if (HAL_TIM_Base_Init(&TimHandle) == HAL_OK)
    {
        return HAL_TIM_Base_Start_IT(&TimHandle);
    }
    return HAL_ERROR;
}
// -------------------------------------------------------------------------
void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}
// -------------------------------------------------------------------------
void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}
// -------------------------------------------------------------------------
void PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    UNUSED(htim);
    HAL_IncTick();
}
// -------------------------------------------------------------------------
void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle);
}
// -------------------------------------------------------------------------
