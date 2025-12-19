/*
 * global.h
 *
 *  Created on: Aug 23, 2023
 *      Author: User
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#define VERSAO_FIRMWARE 2
#define SUBVERSAO_FIRMWARE 1

enum BOOL {
	false,
	true
};

#define NEW_LINE	0x0A

#define SENHA_MESTRE 1120

extern const char SERVICE_TOKEN[160];
extern const char SERVICE_ADDRESS[65];
extern const char SERVICE_HOST[55];
extern const char ENDPOINT_POST_REPORTE[25];
extern const char ENDPOINT_GET_ACIONAMENTO[25];
extern const char ENDPOINT_GET_CONFIG[25];

#define QUANTIDADE_CHAR_GPRS_APN	16
#define QUANTIDADE_OBSTACULOS 8
#define QUANTIDADE_ENTRADAS_DIGITAIS 6
#define QUANTIDADE_RELES 10
#define QUANTIDADE_ERRO_BG95 10
#define QUANTIDADE_ERRO_GPRS 10
#define QUANTIDADE_ERRO_GNSS 5
#define QUANTIDADE_CHAR_WIFI_SSID 16
#define QUANTIDADE_CHAR_WIFI_SENHA 16
#define QUANTIDADE_AGENDA_ACIONAMENTO 2
#define QUANTIDADE_REGISTROS_LOG 9000

#define TAMANHO_BUFFER_RS485 96
#define TAMANHO_BUFFER_LORA 64
#define TAMANHO_BUFFER_SOQUETE 2048
#define TAMANHO_MAXIMO_REPORTE 1024

#define	TIMEOUT_ENTRA_MENU 30
#define TIMEOUT_TROCA_MODO_OPERACAO 30
#define TIMEOUT_PADRAO_RS485 10
#define TIMEOUT_GNSS_RECENTE 30

#define TEMPO_MAXIMO_PRESSURIZACAO 15
#define TEMPO_MAXIMO_HORIMETRO 999999999
#define TEMPO_MAXIMO_PERCENTIMETRO	60
#define TEMPO_MINIMO_PERCENTIMETRO	5
#define TEMPO_DUPLO_CLIQUE 100
#define TEMPO_VERIFICA_SEGURANCA 20
#define TEMPO_REVERSAO_MOTOR 15
#define TEMPO_PULSO_RELE_ON 30
#define TEMPO_SERVICE 10
#define TEMPO_VERIFICA_HORARIO_PONTA 10
#define TEMPO_AGUARDA_POSICAO_OBSTACULO 30
#define TEMPO_INFO_SINAIS 600

#define VALOR_MAXIMO_LAMINA_MM	65000

enum SIMBOLOS_DISPLAYS {
    LCD_DOT_TODO_ACESO,
    LCD_SETA_DIREITA,
    LCD_SETA_ESQUERDA,
    LCD_GRAUS,
    LCD_MICRO,
    LCD_RAIZ_QUADRADA,
    LCD_OHM,
    LCD_SIMBOLO_PI,
    LCD_DIVISAO
};

enum MODOS_OPERACAO {
	MODO_IRRIGACAO,
	MODO_DESLOCAMENTO
};

enum CICLOS_IRRIGACAO {
	IRRIGACAO_1_CICLO,
	IRRIGACAO_CONTINUO
};

enum SENTIDO_MOTOR {
	MOTOR_DESLIGADO,
	MOTOR_DIREITA,
	MOTOR_ESQUERDA
};

enum SENTIDO_REMOTO {
	REMOTO_DESLIGADO,
	REMOTO_DIREITA,
	REMOTO_ESQUERDA,
	REMOTO_SEM_COMANDO = 99
};

enum COMUNICACAO_GNSS {
	GNSS_485,
	GNSS_RF
};

enum PLACA_SOQUETE {
	SOQUETE_GPRS,
	SOQUETE_WIFI,
	SOQUETE_4G,
	SOQUETE_RADIO
};

enum TIPO_SENSOR_PRESSAO {
	SENSOR_10BAR,
	SENSOR_16BAR
};

typedef struct {
	uint8_t modoOperacao;
	uint8_t sentidoOperacao;
	uint8_t retornoHome;
	uint8_t selecaoPivot;
	uint16_t valorLamina;
	uint8_t fertiIrrigacao;
	uint8_t overrideBombaBooster;
} PacoteAcionamentoTypeDef;

typedef struct {
	uint16_t latitudeGraus;
	uint16_t latitudeMinutos;
	uint32_t latitudeSegundos;
	uint16_t longitudeGraus;
	uint16_t longitudeMinutos;
	uint32_t longitudeSegundos;
	char hemisferio; //N ou  S
	char indicadorWE; //W ou E
} CoordenadasTypeDef;

typedef struct {
	uint32_t horas;
	uint8_t minutos;
	uint8_t segundos;
} HorimetroTypeDef;

typedef struct {
	uint8_t hora;
	uint8_t minuto;
	uint8_t segundo;
	uint8_t dia;
	uint8_t mes;
	uint8_t ano;
	uint8_t codigo;
} LogTypeDef;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern I2C_HandleTypeDef hi2c1;

extern RTC_DateTypeDef sDate;

extern RTC_HandleTypeDef hrtc;

extern RTC_TimeTypeDef
	sTime,
	horarioInicioPonta,
	horarioFimPonta;

extern uint8_t
	flagMenu,
	flagEntraMenu,
	flagOperacao,
	flagBuzzer,
	flagLedCOM,
	flagAtualizaLCD,
	flagReadRTC,
	flagServiceOcupado,
	flagServiceLeituraConfig,
	flagReportaDesligou,
	flagContaHorimetro,
	flagSalvaHorimetro,
	flagVerificaObstaculos,

	flagRetornoHome,
	flagRetornoHomeHorarioPonta,
	flagHomeEncontrado,
	flagEmergencia,
	flagFalhaSeguranca,
	flagFaltaFase,
	flagBarricada,
	flagBarricadaGPS,
	flagFalhaPressao,
	flagObstaculoEncontrado,
	flagHorarioPonta,
	flagSaiuHorarioPonta,
	flagPortaAberta,
	flagManualDireita,
	flagManualEsquerda,
	flagAcionamentoPelaEeprom,
	flagBombaBooster,
	flagOverrideBombaBooster,
	flagTrocouLaminaDagua,
	flagSelecaoPivot,
	flagIrrigacaoAguardandoInicio,
	flagIrrigacaoAguardandoPressao,
	flagFertiIrrigacao,
	flagAcionamentoPercentimetro,
	flagAcionamentoBombaPrincipal,
	flagAcionamentoContatoAuxiliar,

	flagLeituraGNSS,
	flagLeituraGNSSRecente,
	flagPosicaoInicioOperacaoConfiavel,
	flagPacoteRS485,
	flagPacoteGNSSRS485,
	flagPacoteGNSSLoRa,
	flagSucessoLoRa,

	flagWiFiDhcp,

	flagSalvaOperacao,

	flagInfoLCDSeguranca,
	flagInfoLCDPressao,
	flagInfoLCDGNSS,

	flagFimCursoDireita,
	flagFimCursoEsquerda;

extern uint8_t
	modoOperacao,
	modoOperacaoRemoto,
	modoOperacaoHorarioPonta,
	cicloIrrigacao,
	sentidoMotor,
	sentidoRemoto,
	sentidoHorarioPonta,
	agendaHorarioAcionado,
	alarmePressao,
	histereseAlarmePressao,
	tipoSensorPressao,
	tempoPressurizacao,
	placaSoquete,

	contadorVerificaSeguranca,
	contadorReversaoMotor,
	contadorPulsoReleOn,
	contadorErroBG95,
	contadorErroGPRS,
	contadorErroGNSS,
	contadorTimeoutGNSS,
	contadorTimeoutGNSSRecente,
	contadorTimeoutConfiguraGNSS,
	contadorTimeoutWiFi,
	contadorTimeoutDadosWiFi,
	comunicacaoGNSS,
	contadorIniciaVerificacaoHorarioPonta,
	contadorRS485Buffer,
	contadorLoRaBuffer,
	contadorAguardaPosicaoObstaculo,
	pressao,

	timeoutRS485,

	contadorEntraMenu,
	contadorReporteService,

	percentualSinalOperadora,
	qualidadeSinalLora,

	canalLoRa,
	canalLoRaGateway;

extern char
	soqueteDataIn,
	rs485DataIn,
	loraDataIn,

	operadoraConectada;

extern uint16_t
	senha,
	tempoPressurizacaoSegundos,
	contadorSoqueteBuffer,
	contadorTimeoutBG95,
	contadorTimeoutGPRS,
	contadorTimeoutLoRa,
	contadorTimeoutLoRaGateway,
	contadorBombaPrincipal,

	contadorTempoInfoSinais,

	tempoBaseLaminaDagua,
	tabelaLaminaDagua05,
	tabelaLaminaDagua12,
	tabelaLaminaDagua20,
	tabelaLaminaDagua30,
	tabelaLaminaDagua40,
	tabelaLaminaDagua50,
	tabelaLaminaDagua70,

	laminaDagua,
	trocaLaminaDagua,
	contadorLaminaDagua,
	contadorLaminaDaguaDesligado,
	setPointContadorLaminaDagua,
	setPointContadorLaminaDaguaDesligado,

	enderecoLoRaPivot,
	enderecoLoRaGNSS,

	enderecoLoRaPivotGateway,
	enderecoLoRaGateway,

	posicaoMemoriaLog;

extern uint32_t
	numeroSerial,

	ultimoIdConfig,
	ultimoIdAcionamento;

extern HorimetroTypeDef
	horimetro;

extern CoordenadasTypeDef
	posicaoAtualGPS,
	posicaoInicioOperacao,
	posicaoHome;

extern LogTypeDef
	logLido;

extern char
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

extern uint8_t
	flagObstaculoAtivado[QUANTIDADE_OBSTACULOS],
	statusEntradasDigitais[QUANTIDADE_ENTRADAS_DIGITAIS],
	statusReles[QUANTIDADE_RELES];

extern uint8_t
	acionamentoAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],
	diaDaSemanaAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],

	wifiIpDinamico[4],
	wifiIp[4],
	wifiGateway[4],
	wifiDNS[4],
	wifiMask[4];

extern uint16_t
	valorAdc[2],
	raioAtuacaoObstaculo[QUANTIDADE_OBSTACULOS];

extern RTC_TimeTypeDef
	horarioLigarAgenda[QUANTIDADE_AGENDA_ACIONAMENTO],
	horarioDesligarAgenda[QUANTIDADE_AGENDA_ACIONAMENTO];

extern CoordenadasTypeDef
	posicoesObstaculos[QUANTIDADE_OBSTACULOS];
#endif /* INC_GLOBAL_H_ */
