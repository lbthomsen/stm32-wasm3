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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>

#include "tusb.h"

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

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

// Counter updated from high freq timer

ws2812_handleTypeDef ws2812; // The WS2812 panel handler


uint8_t wasm_stack[2 * 1024] __attribute__((aligned(8))); // 2KB for the Wasm stack/internal use

uint8_t wasm_buffer[32 * 1024] __attribute__((aligned(4)));
uint32_t wasm_file_size = 0;
uint32_t _dfu_start_address = 0;
volatile bool wasm_ready = false;
volatile bool upload_in_progress = false;
volatile bool wasm_should_stop = false;

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
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */
void startWasmTask(void *argument);

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

//--------------------------------------------------------------------+
// DFU Callbacks
//--------------------------------------------------------------------+

void tud_dfu_download_cb(uint8_t alt, uint16_t block_num, uint8_t const *data,
        uint16_t length) {
    (void) alt;

    // --- DfuSe Special Command Handling (Block 0) ---
    if (block_num == 0) {

        if (length > 0) {

            if (!upload_in_progress) {
            //    uint16_t wasm_cmd = 1;
            //    osMessageQueuePut(wasmCtlQueueHandle, &wasm_cmd, 0,
            //            osWaitForever);
            //    upload_in_progress = true;
            }

            if (data[0] == 0x21 && length >= 5) {
                _dfu_start_address = data[1] | (data[2] << 8) | (data[3] << 16)
                        | (data[4] << 24);
            }
            // Pass DFU_STATUS_OK to acknowledge the command
            tud_dfu_finish_flashing(DFU_STATUS_OK);
        }
        return;
    }

    // --- Normal Data Download ---
    if (block_num >= 2) {
        uint32_t offset = (block_num - 2) * CFG_TUD_DFU_XFER_BUFSIZE;

        if (offset + length <= sizeof(wasm_buffer)) {
            memcpy(wasm_buffer + offset, data, length);
            wasm_file_size = offset + length;

            // Success!
            tud_dfu_finish_flashing(DFU_STATUS_OK);
        } else {
            // Buffer overflow - tell the host something went wrong
            tud_dfu_finish_flashing(DFU_STATUS_ERR_ADDRESS);
        }
    }
}

void tud_dfu_get_status_cb(uint8_t alt, uint8_t *status) {
    (void) alt;
    // Let TinyUSB fill status[4] (state) automatically
    status[0] = DFU_STATUS_OK;
    status[1] = 10; // bwPollTimeout (10ms)
    status[2] = 0;
    status[3] = 0;
}

uint32_t tud_dfu_get_timeout_cb(uint8_t alt, uint8_t state) {
    (void) alt;
    // Return a non-zero timeout when busy to let the host poll
    if (state == DFU_DNBUSY)
        return 10;
    return 0;
}

void tud_dfu_manifest_cb(uint8_t alt) {
    (void) alt;
    wasm_ready = true;
    upload_in_progress = false;
    uint16_t wasm_cmd = 2;

    //osMessageQueuePut(wasmCtlQueueHandle, &wasm_cmd, 0, osWaitForever);
}

void tud_dfu_runtime_reboot_to_dfu_cb(void) {
}

m3ApiRawFunction(host_delay) {
    m3ApiGetArg(uint32_t, ms);

    // If the DFU task signaled a stop, don't delay—just die.
    if (wasm_should_stop) {
        m3ApiTrap(m3Err_trapAbort);
    }

    osDelay(ms);
    m3ApiSuccess();
}

m3ApiRawFunction(host_set_led)
{
    m3ApiGetArg(uint32_t, led);
    m3ApiGetArg(uint32_t, r);
    m3ApiGetArg(uint32_t, g);
    m3ApiGetArg(uint32_t, b);

    osMutexWait(ws2812MutexHandle, osWaitForever);
    setLedValues(&ws2812, led, r, g, b);
    osMutexRelease(ws2812MutexHandle);

    m3ApiSuccess();
}

m3ApiRawFunction(host_zero_led_values)
{
    osMutexWait(ws2812MutexHandle, osWaitForever);
    zeroLedValues(&ws2812);
    osMutexRelease(ws2812MutexHandle);

    m3ApiSuccess();
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
    MX_USB_OTG_FS_PCD_Init();
    /* USER CODE BEGIN 2 */

    printf("\n\n\n\nStarting WASM demo\n");

    printf("Firing up ws2812\n");
    ws2812_init(&ws2812, &htim3, TIM_CHANNEL_3, 512);
    zeroLedValues(&ws2812);

    //setLedValues(&ws2812, 0, 10, 0, 10);

    //run_wasm();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {

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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

    TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef sConfigOC = { 0 };

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
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void)
{

    /* USER CODE BEGIN USB_OTG_FS_Init 0 */

    /* USER CODE END USB_OTG_FS_Init 0 */

    /* USER CODE BEGIN USB_OTG_FS_Init 1 */

    /* USER CODE END USB_OTG_FS_Init 1 */
    hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
    hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
            {
        Error_Handler();
    }
    /* USER CODE BEGIN USB_OTG_FS_Init 2 */

    /* USER CODE END USB_OTG_FS_Init 2 */

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
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
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
void startWasmTask(void *argument) {
    M3Result result = m3Err_none;

    osDelay(50); // Settlement delay

    if (osMutexAcquire(printMutexHandle, 100) == osOK) {
        printf("Wasm: Starting... (%lu bytes)\n", wasm_file_size);
        osMutexRelease(printMutexHandle);
    }

    // 1. Validate Header and Size
    if (wasm_file_size >= 8 && wasm_buffer[0] == 0x00 && wasm_buffer[1] == 0x61) {

        IM3Environment env = m3_NewEnvironment();
        if (env) {
            IM3Runtime runtime = m3_NewRuntime(env, 1024, NULL);
            if (runtime) {
                IM3Module module;
                result = m3_ParseModule(env, &module, wasm_buffer, wasm_file_size);

                if (result == m3Err_none) {
                    m3_LoadModule(runtime, module);
                    m3_LinkRawFunction(module, "env", "host_delay", "v(i)", host_delay);
                    m3_LinkRawFunction(module, "env", "host_set_led", "v(iiii)", host_set_led);
                    m3_LinkRawFunction(module, "env", "host_zero_led_values", "v()", host_zero_led_values);

                    IM3Function f;
                    result = m3_FindFunction(&f, runtime, "demo");
                    if (result == m3Err_none) {
                        if (osMutexAcquire(printMutexHandle, 100) == osOK) {
                            printf("Wasm: demo() found. Entering loop...\n");
                            osMutexRelease(printMutexHandle);
                        }
                        // Execution blocks here in the demo loop
                        result = m3_CallArgv(f, 0, NULL);
                    } else {
                        if (osMutexAcquire(printMutexHandle, 100) == osOK) {
                            printf("Wasm: Error - 'demo' not found: %s\n", result);
                            osMutexRelease(printMutexHandle);
                        }
                    }
                } else {
                    if (osMutexAcquire(printMutexHandle, 100) == osOK) {
                        printf("Wasm: Parse Error: %s\n", result);
                        osMutexRelease(printMutexHandle);
                    }
                }

                // Cleanup Runtime
                m3_FreeRuntime(runtime);
            }
            // Cleanup Environment
            m3_FreeEnvironment(env);
        }
    } else {
        if (osMutexAcquire(printMutexHandle, 100) == osOK) {
            printf("Wasm: Invalid Header or empty buffer\n");
            osMutexRelease(printMutexHandle);
        }
    }

    // 3. Final Report
    if (osMutexAcquire(printMutexHandle, 100) == osOK) {
        if (result && result != m3Err_trapAbort) {
            printf("Wasm: Finished with error: %s\n", result);
        } else {
            printf("Wasm: Task Cleanly Finished\n");
        }
        osMutexRelease(printMutexHandle);
    }

    wasmTaskHandle = NULL;
    vTaskDelete(NULL);
}
/* USER CODE END 4 */

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
