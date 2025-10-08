/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global.h"
#include "string.h"
#include "codigosLogs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
const char SERVICE_TOKEN[160] = "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE3MjY3ODc2ODAsImV4cCI6NDg4MjU0NzY4MH0.HAkiJpJuatu9opYnIijl3HBSTZjp9GCYPEYsGmvLF14";
const char SERVICE_ADDRESS[45] = "http://pivots-907b8.rj.r.appspot.com/";
const char SERVICE_HOST[32] = "pivots-907b8.rj.r.appspot.com";
const char ENDPOINT_POST_REPORTE[25] = "api/pivots/report";
const char ENDPOINT_GET_ACIONAMENTO[25] = "api/pivots/drive/";
const char ENDPOINT_GET_CONFIG[25] = "api/pivots/config/";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart8_rx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart6_rx;

osThreadId mainTaskHandle;
osThreadId ftpTaskHandle;
/* USER CODE BEGIN PV */
RTC_DateTypeDef sDate;

RTC_TimeTypeDef
	sTime,
	horarioInicioPonta,
	horarioFimPonta;

uint8_t
	flagMenu = false,
	flagEntraMenu = false,
	flagOperacao = false,
	flagBuzzer = false,
	flagLedCOM = false,
	flagAtualizaLCD = false,
	flagReadRTC = false,
	flagServiceOcupado = false,
	flagServiceLeituraConfig = false,
	flagReportaDesligou = false,
	flagContaHorimetro = false,
	flagSalvaHorimetro = false,
	flagVerificaObstaculos = false,

	flagRetornoHome = false,
	flagRetornoHomeHorarioPonta = false,
	flagHomeEncontrado = false,
	flagEmergencia = false,
	flagFalhaSeguranca = false,
	flagFaltaFase = false,
	flagBarricada = false,
	flagBarricadaGPS = false,
	flagFalhaPressao = false,
	flagObstaculoEncontrado = false,
	flagPortaAberta = false,
	flagHorarioPonta = false,
	flagSaiuHorarioPonta = false,
	flagManualDireita = false,
	flagManualEsquerda = false,
	flagAcionamentoPelaEeprom = false,
	flagBombaBooster = false,
	flagOverrideBombaBooster = false,
	flagTrocouLaminaDagua = false,
	flagSelecaoPivot = false,
	flagIrrigacaoAguardandoInicio = false,
	flagIrrigacaoAguardandoPressao = false,
	flagFertiIrrigacao = false,
	flagAcionamentoPercentimetro = false,
	flagAcionamentoBombaPrincipal = false,
	flagAcionamentoContatoAuxiliar = false,

	flagLeituraGNSS = false,
	flagLeituraGNSSRecente = false,
	flagPosicaoInicioOperacaoConfiavel = false,
	flagPacoteRS485 = false,
	flagPacoteGNSSRS485 = false,
	flagPacoteGNSSLoRa = false,
	flagSucessoLoRa = false,

	flagWiFiDhcp = true,

	flagSalvaOperacao = false,

	flagInfoLCDSeguranca = false,
	flagInfoLCDPressao = false,
	flagInfoLCDGNSS = false,

	flagFimCursoDireita = false,
	flagFimCursoEsquerda = false;

uint8_t
	modoOperacao = MODO_DESLOCAMENTO,
	modoOperacaoRemoto = MODO_DESLOCAMENTO,
	modoOperacaoHorarioPonta = MODO_DESLOCAMENTO,
	cicloIrrigacao = IRRIGACAO_1_CICLO,
	sentidoMotor = MOTOR_DESLIGADO,
	sentidoRemoto = REMOTO_SEM_COMANDO,
	sentidoHorarioPonta = MOTOR_DESLIGADO,
	agendaHorarioAcionado = 99,
	alarmePressao = 0,
	histereseAlarmePressao = 0,
	tempoPressurizacao = 5,
	placaSoquete = SOQUETE_GPRS,

	contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA,
	contadorReversaoMotor = TEMPO_REVERSAO_MOTOR,
	contadorPulsoReleOn = TEMPO_PULSO_RELE_ON,
	contadorErroBG95 = 0,
	contadorErroGPRS = 0,
	contadorErroGNSS = 0,
	contadorTimeoutGNSS = 0,
	contadorTimeoutGNSSRecente = 0,
	contadorTimeoutConfiguraGNSS = 0,
	contadorTimeoutWiFi = 0,
	contadorTimeoutDadosWiFi = 0,
	comunicacaoGNSS = GNSS_485,
	contadorIniciaVerificacaoHorarioPonta = 0,
	contadorRS485Buffer = 0,
	contadorLoRaBuffer = 0,
	contadorAguardaPosicaoObstaculo = 0,
	pressao = 0,

	timeoutRS485 = TIMEOUT_PADRAO_RS485,

	contadorEntraMenu = 0,
	contadorReporteService = 0,

	percentualSinalOperadora = 0, //entre 0-99%
	qualidadeSinalLora = 0, //entre 0-5

	canalLoRa = 0x1F,
	canalLoRaGateway = 0x10;

char
	soqueteDataIn = ' ',
	rs485DataIn = ' ',
	loraDataIn = ' ',

	operadoraConectada = 'N';

uint16_t
	senha = 0,
	tempoPressurizacaoSegundos = 0,
	contadorSoqueteBuffer = 0,
	contadorTimeoutBG95 = 0,
	contadorTimeoutGPRS = 0,
	contadorTimeoutLoRa = 0,
	contadorTimeoutLoRaGateway = 0,
	contadorBombaPrincipal = 0,

	tempoBaseLaminaDagua = 200,
	tabelaLaminaDagua05 = 7000,
	tabelaLaminaDagua12 = 5000,
	tabelaLaminaDagua20 = 4000,
	tabelaLaminaDagua30 = 3000,
	tabelaLaminaDagua40 = 2000,
	tabelaLaminaDagua50 = 1200,
	tabelaLaminaDagua70 = 500,

	laminaDagua = 100,
	trocaLaminaDagua = 100,
	contadorLaminaDagua = 0,
	contadorLaminaDaguaDesligado = 0,
	setPointContadorLaminaDagua = 0,
	setPointContadorLaminaDaguaDesligado = 0,

	enderecoLoRaPivot = 0x0110,
	enderecoLoRaGNSS = 0x0111,

	enderecoLoRaPivotGateway = 0x0110,
	enderecoLoRaGateway = 0x0112,

	contadorTempoInfoSinais = 0,

	posicaoMemoriaLog = 0;

uint32_t
	numeroSerial = 0,

	ultimoIdConfig = 0,
	ultimoIdAcionamento = 0;

CoordenadasTypeDef
	posicaoAtualGPS,
	posicaoInicioOperacao,
	posicaoHome;

HorimetroTypeDef
	horimetro;

LogTypeDef
	logLido;

char
	mensagemFTPLCD[16],

	bufferEnvioLoRa[TAMANHO_BUFFER_LORA],
	bufferLoRa[TAMANHO_BUFFER_LORA],
	bufferRS485[TAMANHO_BUFFER_RS485],
	bufferSoquete[TAMANHO_BUFFER_SOQUETE],
	bufferEnvioSoquete[TAMANHO_BUFFER_SOQUETE],
	reporteService[TAMANHO_MAXIMO_REPORTE],

	enderecoLoRaRecebidoPivot[5],
	enderecoLoRaRecebidoGNSS[5],
	canalLoRaRecebido[5],

	wifiSSID[QUANTIDADE_CHAR_WIFI_SSID + 1],
	wifiSenha[QUANTIDADE_CHAR_WIFI_SENHA + 1],

	gprsAPN[QUANTIDADE_CHAR_GPRS_APN];

uint8_t
	flagObstaculoAtivado[QUANTIDADE_OBSTACULOS],
	statusEntradasDigitais[QUANTIDADE_ENTRADAS_DIGITAIS],
	statusReles[QUANTIDADE_RELES];

uint8_t
	acionamentoAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],
	diaDaSemanaAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],

	wifiIpDinamico[4],
	wifiIp[4],
	wifiGateway[4],
	wifiDNS[4],
	wifiMask[4];

uint16_t
	valorAdc[2],
	raioAtuacaoObstaculo[QUANTIDADE_OBSTACULOS];

RTC_TimeTypeDef
	horarioLigarAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],
	horarioDesligarAgenda[QUANTIDADE_AGENDA_ACIONAMENTO];

CoordenadasTypeDef
	posicoesObstaculos[QUANTIDADE_OBSTACULOS];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_UART8_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART3_UART_Init(void);
void MainTask(void const * argument);
void FTPTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if(hadc == &hadc1) {
		//leituraTransdutorPressao();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {



}

void delayMicro(uint32_t tempo) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while(__HAL_TIM_GET_COUNTER(&htim2) < tempo) {
	}
}

void reiniciaWatchDog() {
	HAL_IWDG_Refresh(&hiwdg);
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
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_UART8_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of mainTask */
  osThreadDef(mainTask, MainTask, osPriorityNormal, 0, 512);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* definition and creation of ftpTask */
  osThreadDef(ftpTask, FTPTask, osPriorityIdle, 0, 1024);
  ftpTaskHandle = osThreadCreate(osThread(ftpTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  return; //não permite reiniciar a data/hora
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 90-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 900-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10-1;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
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
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 9600;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

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
  huart1.Init.BaudRate = 9600;
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RELE1_Pin|RELE2_Pin|RELE3_Pin|RELE4_Pin
                          |LED_CPU_Pin|RELE5_Pin|RELE6_Pin|RELE7_Pin
                          |RELE8_Pin|RELE10_Pin|RELE9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_COM_Pin|LCD_RS_Pin|LCD_ENABLE_Pin|BUZZER_Pin
                          |LCD_BKL_Pin|SOQUETE_OUT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LCD_D7_Pin|LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin
                          |SOQUETE_OUT1_Pin|LORA_M1_Pin|LORA_M0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(IOT_PON_TRIG_GPIO_Port, IOT_PON_TRIG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RELE1_Pin RELE2_Pin RELE3_Pin RELE4_Pin
                           LED_CPU_Pin RELE5_Pin RELE6_Pin RELE7_Pin
                           RELE8_Pin RELE10_Pin RELE9_Pin */
  GPIO_InitStruct.Pin = RELE1_Pin|RELE2_Pin|RELE3_Pin|RELE4_Pin
                          |LED_CPU_Pin|RELE5_Pin|RELE6_Pin|RELE7_Pin
                          |RELE8_Pin|RELE10_Pin|RELE9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_DOWN_Pin SW_OK_Pin IN1_Pin IN2_Pin */
  GPIO_InitStruct.Pin = SW_DOWN_Pin|SW_OK_Pin|IN1_Pin|IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_COM_Pin LCD_RS_Pin LCD_ENABLE_Pin BUZZER_Pin
                           LCD_BKL_Pin SOQUETE_OUT2_Pin */
  GPIO_InitStruct.Pin = LED_COM_Pin|LCD_RS_Pin|LCD_ENABLE_Pin|BUZZER_Pin
                          |LCD_BKL_Pin|SOQUETE_OUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_UP_Pin SW_LEFT_Pin */
  GPIO_InitStruct.Pin = SW_UP_Pin|SW_LEFT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D7_Pin LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin
                           SOQUETE_OUT1_Pin LORA_M1_Pin LORA_M0_Pin */
  GPIO_InitStruct.Pin = LCD_D7_Pin|LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin
                          |SOQUETE_OUT1_Pin|LORA_M1_Pin|LORA_M0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : IN3_Pin IN4_Pin IN5_Pin IN6_Pin */
  GPIO_InitStruct.Pin = IN3_Pin|IN4_Pin|IN5_Pin|IN6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : IOT_PON_TRIG_Pin */
  GPIO_InitStruct.Pin = IOT_PON_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IOT_PON_TRIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IOT_PSM_IND_Pin LORA_AUX_Pin */
  GPIO_InitStruct.Pin = IOT_PSM_IND_Pin|LORA_AUX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SW_RIGHT_Pin */
  GPIO_InitStruct.Pin = SW_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_RIGHT_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_MainTask */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MainTask */
void MainTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  mainTask();
	  osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_FTPTask */
/**
* @brief Function implementing the ftpTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FTPTask */
void FTPTask(void const * argument)
{
  /* USER CODE BEGIN FTPTask */
  /* Infinite loop */
  for(;;)
  {
	  serviceTask();
	  osDelay(1);
  }
  /* USER CODE END FTPTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  if(htim == &htim3) {
  	  schedulerEngine();
  }

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
  while (1)
  {
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
