/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STM32World <lth@stm23world.com>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>

#include "wasm3.h"
#include "m3_env.h"

#include "ws2812.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim13;
DMA_HandleTypeDef hdma_tim3_ch3;

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tickTask */
osThreadId_t tickTaskHandle;
const osThreadAttr_t tickTask_attributes = {
  .name = "tickTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for statusTask */
osThreadId_t statusTaskHandle;
const osThreadAttr_t statusTask_attributes = {
  .name = "statusTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ws2812Task */
osThreadId_t ws2812TaskHandle;
const osThreadAttr_t ws2812Task_attributes = {
  .name = "ws2812Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tickQueue */
osMessageQueueId_t tickQueueHandle;
const osMessageQueueAttr_t tickQueue_attributes = {
  .name = "tickQueue"
};
/* Definitions for ws2812Queue */
osMessageQueueId_t ws2812QueueHandle;
const osMessageQueueAttr_t ws2812Queue_attributes = {
  .name = "ws2812Queue"
};
/* Definitions for printMutex */
osMutexId_t printMutexHandle;
const osMutexAttr_t printMutex_attributes = {
  .name = "printMutex"
};
/* Definitions for ledMutex */
osMutexId_t ledMutexHandle;
const osMutexAttr_t ledMutex_attributes = {
  .name = "ledMutex"
};
/* Definitions for ledSemaphore */
osSemaphoreId_t ledSemaphoreHandle;
const osSemaphoreAttr_t ledSemaphore_attributes = {
  .name = "ledSemaphore"
};
/* USER CODE BEGIN PV */

// Counter updated from high freq timer
volatile unsigned long ulHighFrequencyTimerTicks;

ws2812_handleTypeDef ws2812; // The WS2812 panel handler

// Pre-allocate all the heap for FreeRTOS.  FreeRTOS will do this by itself if this define is
// not set.
#ifdef configAPPLICATION_ALLOCATED_HEAP
uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section(".ccmram"))); // Put in ccmram
#endif

uint8_t wasm_stack[16 * 1024]; // 16KB for the Wasm stack/internal use

unsigned char test_wasm[] = { 0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
		0x01, 0x07, 0x01, 0x60, 0x02, 0x7f, 0x7f, 0x01, 0x7f, 0x03, 0x02, 0x01,
		0x00, 0x07, 0x07, 0x01, 0x03, 0x61, 0x64, 0x64, 0x00, 0x00, 0x0a, 0x09,
		0x01, 0x07, 0x00, 0x20, 0x00, 0x20, 0x01, 0x6a, 0x0b };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM13_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void *argument);
void startLedTask(void *argument);
void startTickTask(void *argument);
void startStatusTask(void *argument);
void startWs2812Task(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Send printf to uart1
int __io_putchar(int ch) {
	if (ch == '\n') {
		HAL_UART_Transmit(&huart1, (uint8_t*) "\r", 1, HAL_MAX_DELAY);
	}
	if (HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY)
			!= HAL_OK) {
		return -1;
	}
	return ch;
}

void configureTimerForRunTimeStats(void) {
	ulHighFrequencyTimerTicks = 0;
	HAL_TIM_Base_Start_IT(&htim13);
}

unsigned long getRunTimeCounterValue(void) {
	return ulHighFrequencyTimerTicks;
}

// Done sending first half of the DMA buffer - this can now safely be updated
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM3) {
        ws2812_update_buffer(&ws2812, &ws2812.dma_buffer[0]);
    }

}

// Done sending the second half of the DMA buffer - this can now be safely updated
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM3) {
        ws2812_update_buffer(&ws2812, &ws2812.dma_buffer[BUFFER_SIZE]);
    }

}

void run_wasm(void) {
	IM3Environment env = m3_NewEnvironment();
	/* We create a runtime using our CCM RAM stack */
	IM3Runtime runtime = m3_NewRuntime(env, sizeof(wasm_stack), NULL);

	// In Wasm3, you can manually point the runtime to use your specific buffer
	// For simplicity, m3_NewRuntime allocates from the system heap unless
	// you override the allocator, but defining the stack in CCM is a great start.

	IM3Module module;
	m3_ParseModule(env, &module, test_wasm, sizeof(test_wasm));
	m3_LoadModule(runtime, module);

	IM3Function f;
	m3_FindFunction(&f, runtime, "add");

	// Call the function: add(10, 20)
	const char *i_argv[] = { "10", "20" };
	m3_CallArgv(f, 2, i_argv);

	// Get the result
	int32_t result = 0;
	m3_GetResultsV(f, &result);

	// result is now 30!
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM13_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

	printf("\n\n\n\nStarting WASM demo\n");

    printf("Firing up ws2812\n");
    ws2812_init(&ws2812, &htim3, TIM_CHANNEL_3, 512);
    zeroLedValues(&ws2812);

    setLedValues(&ws2812, 0, 10, 0, 10);

	//run_wasm();

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of printMutex */
  printMutexHandle = osMutexNew(&printMutex_attributes);

  /* creation of ledMutex */
  ledMutexHandle = osMutexNew(&ledMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of ledSemaphore */
  ledSemaphoreHandle = osSemaphoreNew(1, 1, &ledSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of tickQueue */
  tickQueueHandle = osMessageQueueNew (3, sizeof(uint32_t), &tickQueue_attributes);

  /* creation of ws2812Queue */
  ws2812QueueHandle = osMessageQueueNew (2, sizeof(uint16_t), &ws2812Queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(startLedTask, NULL, &ledTask_attributes);

  /* creation of tickTask */
  tickTaskHandle = osThreadNew(startTickTask, NULL, &tickTask_attributes);

  /* creation of statusTask */
  statusTaskHandle = osThreadNew(startStatusTask, NULL, &statusTask_attributes);

  /* creation of ws2812Task */
  ws2812TaskHandle = osThreadNew(startWs2812Task, NULL, &ws2812Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	uint32_t now = 0, next_blink = 500, next_tick = 1000;

	while (1) {

		now = uwTick;

		if (now >= next_blink) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			next_blink = now + 500;
		}

		if (now >= next_tick) {
			printf("Tick %lu\n", now / 1000);
			next_tick = now + 1000;
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = LED_CNT;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 84 - 1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 100 - 1;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 2000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */

	uint32_t loop_cnt = 0;

	/* Infinite loop */
	for (;;) {
		osDelay(100);

		if (loop_cnt % 10 == 0) {

			uint32_t tick = osKernelGetTickCount();

			osMessageQueuePut(tickQueueHandle, &tick, 0, osWaitForever);

		}

		if (loop_cnt % 5 == 0) {

			osSemaphoreRelease(ledSemaphoreHandle);

		}

		++loop_cnt;
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_startLedTask */
/**
 * @brief Function implementing the ledTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startLedTask */
void startLedTask(void *argument)
{
  /* USER CODE BEGIN startLedTask */

	osStatus_t ret;

	/* Infinite loop */
	for (;;) {

		ret = osSemaphoreAcquire(ledSemaphoreHandle, osWaitForever);

		if (!ret) {

			osMutexWait(ledMutexHandle, osWaitForever);

			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

			osMutexRelease(ledMutexHandle);

		}

	}
  /* USER CODE END startLedTask */
}

/* USER CODE BEGIN Header_startTickTask */
/**
 * @brief Function implementing the tickTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTickTask */
void startTickTask(void *argument)
{
  /* USER CODE BEGIN startTickTask */
	osStatus_t ret;

	/* Infinite loop */
	for (;;) {

		uint32_t tick;

		ret = osMessageQueueGet(tickQueueHandle, &tick, NULL, osWaitForever);

		if (ret == osOK) {

			osMutexWait(printMutexHandle, osWaitForever);

			printf("Tick %lu \n", tick / 1000);

			osMutexRelease(printMutexHandle);

		}

	}
  /* USER CODE END startTickTask */
}

/* USER CODE BEGIN Header_startStatusTask */
/**
 * @brief Function implementing the statusTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startStatusTask */
void startStatusTask(void *argument)
{
  /* USER CODE BEGIN startStatusTask */
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;
	float runtime_percentage;

	/* Infinite loop */
	for (;;) {

		osDelay(10000);

		uxArraySize = uxTaskGetNumberOfTasks();
		pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t)); // a little bit scary!

		osMutexWait(printMutexHandle, osWaitForever);

		if (pxTaskStatusArray != NULL) {

			uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize,
					&ulTotalRunTime);

			printf("Task count = %lu\n", uxArraySize);
			printf("No       Name          P  S   Usage       Count      HW\n");

			for (x = 0; x < uxArraySize; x++) {

				runtime_percentage = (float) (100
						* (float) pxTaskStatusArray[x].ulRunTimeCounter
						/ (float) ulTotalRunTime);

				printf("Task %2lu: %-12s %2lu %2d %8.4f (%12lu) %5i\n", x,
						pxTaskStatusArray[x].pcTaskName,
						pxTaskStatusArray[x].uxCurrentPriority,
						pxTaskStatusArray[x].eCurrentState, runtime_percentage,
						pxTaskStatusArray[x].ulRunTimeCounter,
						pxTaskStatusArray[x].usStackHighWaterMark);

			}

			vPortFree(pxTaskStatusArray);

		} else {
			printf("Unable to allocate stack space\n");
		}

		osMutexRelease(printMutexHandle);

	}
  /* USER CODE END startStatusTask */
}

/* USER CODE BEGIN Header_startWs2812Task */
/**
* @brief Function implementing the ws2812Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startWs2812Task */
void startWs2812Task(void *argument)
{
  /* USER CODE BEGIN startWs2812Task */
  /* Infinite loop */
  for(;;) {
    osDelay(1);
  }
  /* USER CODE END startWs2812Task */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
