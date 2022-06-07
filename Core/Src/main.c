/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
 SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
void MX_USART1_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//memory_map_t MemoryMap;
uint32_t u32TicksMain;
uint8_t ptu8data[32];
uint8_t deb1;

//Time before reset to be able to answer the frame
uint32_t u32TicksReset;

void appProtocoloSim(void);
void vars_Init(void);

void appLED_Init(void);
void appLED_Motor(void);

void APP_Accions(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	  char buf_arr[5] = {'a','b','c','d','e'};
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
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  //MemoryMap.var.cabecera_fw.board_definition.u8BoardType = 1;



  //We initialize the variables: sourcesStatus, of the protocol (MemoryMap)
  vars_Init();

  // initialize the system
  SYS_Init();

  //Initialize alarm control
  alarmsControl_init();

  // initialize the protocol
  PROTSLV_Init();

  //Initialization of the LED engine
  LED_Init();

  u32TicksMain = TICKS_GetTicks();
  //while(TICKS_DiffTicks(u32TicksMain) < 200); //wait to ensure the correct detection of the source, without this wait when the equipment started up the 2 PCB's remained as PCB1
  // We identify the PCB and the connected sources
  CTR_identif();

  // Initialize the standby module



  standbyInit();

  appSPI_init();

  appLED_Init();

  CTR_fuentes_ON_OFF_init();

  u32TicksMain = TICKS_GetTicks();

  (void)AD5676R_init();

  prtGreenLED(0);
  prtRedLED(1);

  prtGreenLED(1);
  prtRedLED(0);

  prtGreenLED(1);
  prtRedLED(1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  RS485_TRANSMIT_ENABLE(0);
//	  HAL_UART_Transmit(&huart4, buf_arr, 5, 100);
//
//		HAL_Delay(200);


      APP_Accions();
      if (fuentesStatus.alarmsOn == 1) {
          alarmsControl_motor();


      }

      //We call the LED application engine
      appLED_Motor(); //internal led of the board, identifies if it is PCB1 or 2

		//We call the engine of led 1
      LED_Motor(LED1);
		//We call the engine of led 2
      LED_Motor(LED2);

		//We call the SPI engine
      standbyMotor();

		//We call the protocol engine, slave
      PROTSLV_Motor();

      // if (prtEN_Apl1n == 0) //debug peltiers 1
      if ((standby.state == PCB_STATE_ON) || (standby.state == PCB_STATE_WAIT_FOR_STANDBY)) //faltaaa: or PCB_STATE_WAIT_FOR_STANDBY, to be able to turn them off smoothly iii TODO
      {
          appSPI_motor();
      }

      CTR_fuentes_ON_OFF_motor();

		// CTR_sources_ON_OFF();

     // appProtocolSim();
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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_9B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_EVEN;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|R_LED_Pin|G_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_TEMP_2ERR_Pin|LED_AC2_ERR_Pin|LED_FAN_2ERR_Pin|EN_24V_DIG_Pin
                          |EN_APL_5_Pin|EN_APL_6_Pin|EN_APL_7_Pin|EN_APL_8_Pin
                          |EN_APL_1_Pin|EN_APL_2_Pin|EN_APL_3_Pin|EN_APL_4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_TEMP_1ERR_Pin|LED_AC1_ERR_Pin|LED_FAN1_ERR_Pin|CS_DAC_FAN_Pin
                          |CS_DAC_FANC5_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PS2_EN_Pin|EN_485_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DAC_GPIO_Port, CS_DAC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LOAD_DAC_Pin|EN_24V_FAN_Pin|EN_PS_GLOBAL_Pin|LED_Y_Pin
                          |LED_ALARM_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, MEM_WP_Pin|LED_G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ALARMA_OUT_GPIO_Port, ALARMA_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PS1_FAN_FAIL_Pin */
  GPIO_InitStruct.Pin = PS1_FAN_FAIL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PS1_FAN_FAIL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE3 LED_TEMP_2ERR_Pin LED_AC2_ERR_Pin LED_FAN_2ERR_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3|LED_TEMP_2ERR_Pin|LED_AC2_ERR_Pin|LED_FAN_2ERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PS1_TEMP_ALARM_Pin PS1_AC_FAIL_Pin PS1_A1_Pin PS1_A0_Pin
                           PG_PLT_4_Pin PG_PLT_3_Pin */
  GPIO_InitStruct.Pin = PS1_TEMP_ALARM_Pin|PS1_AC_FAIL_Pin|PS1_A1_Pin|PS1_A0_Pin
                          |PG_PLT_4_Pin|PG_PLT_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ON_OFF_Pin */
  GPIO_InitStruct.Pin = ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ON_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_TEMP_1ERR_Pin LED_AC1_ERR_Pin LED_FAN1_ERR_Pin */
  GPIO_InitStruct.Pin = LED_TEMP_1ERR_Pin|LED_AC1_ERR_Pin|LED_FAN1_ERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : FAN_PS_EXIST_Pin */
  GPIO_InitStruct.Pin = FAN_PS_EXIST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(FAN_PS_EXIST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PS2_AC_FAIL_Pin PS2_FAN_FAIL_Pin */
  GPIO_InitStruct.Pin = PS2_AC_FAIL_Pin|PS2_FAN_FAIL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PS2_TEMP_ALARM_Pin */
  GPIO_InitStruct.Pin = PS2_TEMP_ALARM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PS2_TEMP_ALARM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PS2_EN_Pin CS_DAC_Pin */
  GPIO_InitStruct.Pin = PS2_EN_Pin|CS_DAC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_DAC_FAN_Pin CS_DAC_FANC5_Pin */
  GPIO_InitStruct.Pin = CS_DAC_FAN_Pin|CS_DAC_FANC5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LOAD_DAC_Pin EN_24V_FAN_Pin EN_PS_GLOBAL_Pin */
  GPIO_InitStruct.Pin = LOAD_DAC_Pin|EN_24V_FAN_Pin|EN_PS_GLOBAL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_24V_DIG_Pin EN_APL_5_Pin EN_APL_6_Pin EN_APL_7_Pin
                           EN_APL_8_Pin EN_APL_1_Pin EN_APL_2_Pin EN_APL_3_Pin
                           EN_APL_4_Pin */
  GPIO_InitStruct.Pin = EN_24V_DIG_Pin|EN_APL_5_Pin|EN_APL_6_Pin|EN_APL_7_Pin
                          |EN_APL_8_Pin|EN_APL_1_Pin|EN_APL_2_Pin|EN_APL_3_Pin
                          |EN_APL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_Pin ALARMA__IN_Pin */
  GPIO_InitStruct.Pin = BTN_Pin|ALARMA__IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEM_WP_Pin */
  GPIO_InitStruct.Pin = MEM_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MEM_WP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PS2_A1_Pin PS2_A0_Pin PG_PLT_8_Pin PG_PLT_7_Pin
                           PG_PLT_6_Pin PG_PLT_5_Pin PG_24_FAN_Pin SEL_2_Pin
                           SEL_1_Pin */
  GPIO_InitStruct.Pin = PS2_A1_Pin|PS2_A0_Pin|PG_PLT_8_Pin|PG_PLT_7_Pin
                          |PG_PLT_6_Pin|PG_PLT_5_Pin|PG_24_FAN_Pin|SEL_2_Pin
                          |SEL_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PG_PLT_2_Pin PG_PLT_1_Pin */
  GPIO_InitStruct.Pin = PG_PLT_2_Pin|PG_PLT_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : EN_485_Pin */
  GPIO_InitStruct.Pin = EN_485_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EN_485_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_G_Pin */
  GPIO_InitStruct.Pin = LED_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_G_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Y_Pin LED_ALARM_Pin */
  GPIO_InitStruct.Pin = LED_Y_Pin|LED_ALARM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ALARMA_OUT_Pin */
  GPIO_InitStruct.Pin = ALARMA_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ALARMA_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : R_LED_Pin G_LED_Pin */
  GPIO_InitStruct.Pin = R_LED_Pin|G_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void vars_Init(void) {

    uint8_t i;

    //fuentesStatus
    fuentesStatus.alarmsOn = 0;
    //   fuentesStatus.tempFail1 = 0;
    //   fuentesStatus.tempFail2 = 0;
    for (i = 0; i < FNT_NUM_TEMP_FAIL; i++) {
        fuentesStatus.atempFail1[i] = 0;
        fuentesStatus.atempFail2[i] = 0;
    }

    //MemoryMap
    MemoryMap.var.cabecera_fw.board_alarms.alarms = 0;
    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_OFF;
    MemoryMap.var.cabecera_fw.u16accions = 0; //Accion idle
    MemoryMap.var.cabecera_fw.u16accioactual = 0;
    MemoryMap.var.fuentes.estadoModulos = 0;
    MemoryMap.var.fuentes.powerGoods = 0;
    MemoryMap.var.fuentes.mainStatus = MAIN_STATE_OFF;
    //4  MemoryMap.var.fuentes.modulos_ON_OFF = 0b00000000; //modules off
    MemoryMap.var.fuentes.alarma = FNT_MAIN_ALARM_NONE;


    for (i = 0; i < FNT_NUM_MODULES; i++) {
        MemoryMap.var.fuentes.aModulos_ON_OFF[i] = 0;
    }

    for (i = 0; i < MAX_NUM_SPI_CH; i++) {
        MemoryMap.var.fuentes.aVoltagePercent[i].fvalue = 0;
    }
    //  Alarms initialized on interrupts and control
}

typedef struct {
    uint8_t u8Motor;
    uint32_t u32Timer;
} appLed_t;

appLed_t appLed;

void appLED_Init(void) {
    appLed.u8Motor = 0;
}

void appLED_Motor(void) {

    switch (appLed.u8Motor) {
        case 0:
            //We capture refresh time between each protocol simulation
            appLed.u32Timer = TICKS_GetTicks();
            //We advance to the next state
            appLed.u8Motor++;
            break;

        case 1:
            //We check if the refresh time has expired
            if (TICKS_DiffTicks(appLed.u32Timer) > 1000) {

                if (identif.IdPCB == PCB1) {
                    LED_Configure(LED2, 1, 100, 100, 0); //Green LED: single flash

                } else if (identif.IdPCB == PCB2) {
                    LED_Configure(LED2, 2, 100, 100, 0); //Green LED: double flash
                }


                //Capturamos tiempo de refresco entre cada parpadeo del led
                appLed.u32Timer = TICKS_GetTicks();
                //Avanzamos al siguiente estado
                appLed.u8Motor = 0;
            }
            break;

    }
}

typedef struct {
    uint8_t u8Motor;
    uint32_t u32Timer;
} protocoloSim_t;

protocoloSim_t protocoloSim;

uint8_t debControlM;

void appProtocoloSim(void) {

    switch (protocoloSim.u8Motor) {
        case 0:
            //Capturamos tiempo de refresco entre cada simulacion de protocolo
            protocoloSim.u32Timer = TICKS_GetTicks();
            //Avanzamos al siguiente estado
            protocoloSim.u8Motor++;
            break;

        case 1:
            //Miramos si se ha cumplido el tiempo de refresco
            if (TICKS_DiffTicks(protocoloSim.u32Timer) > 2000) {
                MemoryMap.var.fuentes.mainStatus = MAIN_STATE_READY_FOR_ON;
                //4   MemoryMap.var.fuentes.modulos_ON_OFF = 0b00000010;

                MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED2] = 1;


                //     debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & FIXED1;
                //    debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & 0b00000001;


                //Capturamos tiempo de refresco entre cada simulacion de protocolo
                protocoloSim.u32Timer = TICKS_GetTicks();
                //Avanzamos al siguiente estado
                protocoloSim.u8Motor++;
            }
            break;

        case 2:
            //Miramos si se ha cumplido el tiempo de refresco
            if (TICKS_DiffTicks(protocoloSim.u32Timer) > 2000) {
                MemoryMap.var.fuentes.aVoltagePercent[0].fvalue = 20;
                MemoryMap.var.fuentes.aVoltagePercent[1].fvalue = 40;
                MemoryMap.var.fuentes.aVoltagePercent[2].fvalue = 60;
                MemoryMap.var.fuentes.aVoltagePercent[3].fvalue = 80;

               //4 MemoryMap.var.fuentes.modulos_ON_OFF = 0b00001111;

                MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED1] = 1;
                MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED2] = 1;
                MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE1] = 1;
                MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE2] = 1;


                //     debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & FIXED1;
                //    debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & 0b00000001;

                //MemoryMap.var.fuentes.alarma=FNT_MAIN_ALARM_OFF;

                //Capturamos tiempo de refresco entre cada simulacion de protocolo
                protocoloSim.u32Timer = TICKS_GetTicks();
                //Avanzamos al siguiente estado
                protocoloSim.u8Motor++;
            }
            break;

        case 3:
            //Miramos si se ha cumplido el tiempo de refresco
            if (TICKS_DiffTicks(protocoloSim.u32Timer) > 2000) {
                //    MemoryMap.var.fuentes.modulos_ON_OFF = 0b00000000;
                //     debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & FIXED1;
                //    debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & 0b00000001;

                MemoryMap.var.fuentes.aVoltagePercent[0].fvalue = 20;
                MemoryMap.var.fuentes.aVoltagePercent[1].fvalue = 20;
                MemoryMap.var.fuentes.aVoltagePercent[2].fvalue = 20;
                MemoryMap.var.fuentes.aVoltagePercent[3].fvalue = 20;


                //We capture refresh time between each protocol simulation
                protocoloSim.u32Timer = TICKS_GetTicks();
                //We advance to the next state
                protocoloSim.u8Motor++;
            }
            break;

        case 4:

            //Miramos si se ha cumplido el tiempo de refresco
            if (TICKS_DiffTicks(protocoloSim.u32Timer) > 5000 && MemoryMap.var.cabecera_fw.board_status.status == PCB_STATE_WAIT_FOR_STANDBY) {
                MemoryMap.var.fuentes.mainStatus = MAIN_STATE_READY_FOR_STANDBY;
                //     debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & FIXED1;
                //    debControlM=MemoryMap.var.fuentes.modulos_ON_OFF & 0b00000001;


                //We capture refresh time between each protocol simulation
                protocoloSim.u32Timer = TICKS_GetTicks();
                //We advance to the next state
                protocoloSim.u8Motor = 0;
            }
            break;

        case 5:

            break;

        default:
            break;

    }

}


void APP_Accions(void) {
    switch (MemoryMap.var.cabecera_fw.u16accions) {
        case 0:
            /*accion idle*/
            break;
        case 1:
            /* accion de reset */
            // if (MemoryMap.var.cabecera_fw.u16accioactual == 0) {
            MemoryMap.var.cabecera_fw.u16accioactual = MemoryMap.var.cabecera_fw.u16accions;
            //  LED_Configure(LED_VERMELL, 100, 80, 20);
            //  LED_Configure(LED_GROC, 100, 80, 20);
            //Tiempo para poder responder a la trama
            u32TicksReset = TICKS_GetTicks();
            MemoryMap.var.cabecera_fw.u16accions = 0;
            break;
        case 2:
            /* accion guardar config */
            //            if(MemoryMap.u8accioactual == 0)
            //            {
            //                MIFARE_ForceScan();
            //                MemoryMap.u8accioactual = MemoryMap.u8accions;
            //            }
            //            MemoryMap.u8accions = 0;
            break;

        case 3:
            /* accion standby*/


            break;


            //        case 3:
            //            /* accion cerrar EV */
            //            //Comprueba que no haya ninguna accion ejecutandose
            //            if (MemoryMap.var.cabecera_fw.u16accioactual == 0) {
            //                //Cerramos la electrovalvula
            //                evCanviaEstat(1);
            //                //Terminamos la accion
            //                MemoryMap.var.cabecera_fw.u16accions = 0;
            //
            //            }
            //            break;
            //        case 4:
            //            /* accion abrir EV */
            //            ///Comprueba que no haya ninguna accion ejecutandose
            //            if (MemoryMap.var.cabecera_fw.u16accioactual == 0) {
            //                //Abrimos la electrovalvula
            //                evCanviaEstat(0);
            //                //Terminamos la accion
            //                MemoryMap.var.cabecera_fw.u16accions = 0;
            //
            //            }
            //            break;
            //
            //        case 5:
            //            /* accion abrir EV temporizada */
            //            //Comprueba que no haya ninguna accion ejecutandose
            //            if (MemoryMap.var.cabecera_fw.u16accioactual == 0) {
            //                evOnTemp(MemoryMap.var.complementos.tiempoEV_ON.u32value); //faltaa: cambiar por variable memoryMap
            //                //Terminamos la accion
            //                MemoryMap.var.cabecera_fw.u16accions = 0;
            //
            //            }
            //
            //
            //            break;

        default:
            MemoryMap.var.cabecera_fw.u16accions = 0;
            break;
    }

    switch (MemoryMap.var.cabecera_fw.u16accioactual) {
        case 0:
            /*accion idle*/
            break;
        case 1:
            /* accion de reset */
//            if (TICKS_DiffTicks(u32TicksReset) > 500) {
//                Reset();
//            }
            break;
        case 2:
            /* accion guardar config */

            //            if(MIFARE_GetScanStatus() == 0)
            //            {
            //                MemoryMap.u8accioactual = 0;
            //            }
            //            else
            //            {
            //
            //            }
            break;

        case 3:
            /* action close EV */
            break;

        case 4:
            /* action open EV */
            break;

        case 5:
            /* accion abrir EV temporizada */
            break;
        default:
            MemoryMap.var.cabecera_fw.u16accioactual = 0;
            break;
    }
}






/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
