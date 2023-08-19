// -------------------------------------------------------------------------
// FreeRTOS Kernel
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#ifndef __FREERTOS_CONFIG_H__
#define __FREERTOS_CONFIG_H__
// -------------------------------------------------------------------------
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif
// -------------------------------------------------------------------------
#define configENABLE_FPU									1
#define configENABLE_MPU									1

#define configUSE_PREEMPTION								0
//#define configUSE_TIME_SLICING							0
#define configUSE_IDLE_HOOK									0
#define configUSE_TICK_HOOK									0
#define configCPU_CLOCK_HZ									( SystemCoreClock )
#define configTICK_RATE_HZ									( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES								( 32 )
#define configMINIMAL_STACK_SIZE							( ( uint16_t ) 1 * 1024  )
#define configTOTAL_HEAP_SIZE								( ( size_t ) ( 128 * 1024 ) )
#define configMAX_TASK_NAME_LEN								( 32 )
#define configUSE_TRACE_FACILITY							1
#define configUSE_16_BIT_TICKS								0
#define configIDLE_SHOULD_YIELD								1
#define configUSE_MUTEXES									1
#define configQUEUE_REGISTRY_SIZE							8
#define configCHECK_FOR_STACK_OVERFLOW						2
#define configUSE_RECURSIVE_MUTEXES							1
#define configUSE_MALLOC_FAILED_HOOK						1
#define configUSE_APPLICATION_TASK_TAG						1
#define configUSE_COUNTING_SEMAPHORES						1
#define configGENERATE_RUN_TIME_STATS						0
#define configAPPLICATION_ALLOCATED_HEAP					0

#define configUSE_CO_ROUTINES								0
#define configMAX_CO_ROUTINE_PRIORITIES						( 2 )

#define configUSE_TIMERS									1
#define configTIMER_TASK_PRIORITY							( 2 )
#define configTIMER_QUEUE_LENGTH							10
#define configTIMER_TASK_STACK_DEPTH						( configMINIMAL_STACK_SIZE * 2 )

#define INCLUDE_vTaskPrioritySet							1
#define INCLUDE_uxTaskPriorityGet							1
#define INCLUDE_vTaskDelete									1
#define INCLUDE_vTaskCleanUpResources						1
#define INCLUDE_vTaskSuspend								1
#define INCLUDE_vTaskDelayUntil								1
#define INCLUDE_vTaskDelay									1
#define INCLUDE_xQueueGetMutexHolder						1
#define INCLUDE_xTaskGetSchedulerState						1
#define INCLUDE_eTaskGetState								1

#define configMESSAGE_BUFFER_LENGTH_TYPE					size_t


#define configPRIO_BITS										4

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY             15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY        5
#define configKERNEL_INTERRUPT_PRIORITY                     ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY                ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configASSERT( x )                                   if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

#define vPortSVCHandler                                     SVC_Handler
#define xPortPendSVHandler                                  PendSV_Handler

#define traceTASK_SWITCHED_OUT()                            xTaskCallApplicationTaskHook( pxCurrentTCB, (void*)1 )
#define traceTASK_SWITCHED_IN()                             xTaskCallApplicationTaskHook( pxCurrentTCB, (void*)0 )

#define xPortSysTickHandler                                 SysTick_Handler
// -------------------------------------------------------------------------
#endif 
// -------------------------------------------------------------------------
