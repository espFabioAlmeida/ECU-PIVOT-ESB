///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityLoRaGateway
//AUTOR:      Fábio Almeida
//CRIADO:     21/01/2025
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
/*==============================================================================
AGUARDA LoRa
==============================================================================*/
void aguardaLoRaGateway(uint16_t tempo) {
	contadorTimeoutLoRaGateway = tempo;
	while(contadorTimeoutLoRaGateway) {
	}
}
/*==============================================================================
ENVIA POSICAO ATUAL
==============================================================================*/
void enviaPosicaoAtualGateway() {
	char hemisferio[2], indicadorWE[2];
	hemisferio[0] = posicaoAtualGPS.hemisferio;
	hemisferio[1] = 0x00;
	indicadorWE[0] = posicaoAtualGPS.indicadorWE;
	indicadorWE[1] = 0x00;

	limpaBufferEnvioSoquete();

	bufferEnvioSoquete[0] = make8(enderecoLoRaGateway, 1);
	bufferEnvioSoquete[1] = make8(enderecoLoRaGateway, 0);
	bufferEnvioSoquete[2] = canalLoRaGateway;

	strcat(bufferEnvioSoquete, "$,01,");
	sprintfSoquete(posicaoAtualGPS.latitudeGraus, 2);
	sprintfSoquete(posicaoAtualGPS.latitudeMinutos, 2);
	strcat(bufferEnvioSoquete, ".");
	sprintfSoquete(posicaoAtualGPS.latitudeSegundos, 5);
	strcat(bufferEnvioSoquete, ",");
	strcat(bufferEnvioSoquete, hemisferio);
	strcat(bufferEnvioSoquete, ",");
	sprintfSoquete(posicaoAtualGPS.longitudeGraus, 3);
	sprintfSoquete(posicaoAtualGPS.longitudeMinutos, 2);
	strcat(bufferEnvioSoquete, ".");
	sprintfSoquete(posicaoAtualGPS.longitudeSegundos, 5);
	strcat(bufferEnvioSoquete, ",");
	strcat(bufferEnvioSoquete, indicadorWE);
	strcat(bufferEnvioSoquete, ",");
	sprintfSoquete(qualidadeSinalLora, 0);
	strcat(bufferEnvioSoquete, ",\r\n");

	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
}
/*==============================================================================
ENVIA DATA OPERACAO TEMPO PRESS. PRESSAO STATUS PORTA E SERIAL EQUIPAMENTO
==============================================================================*/
void enviaStatusGerais1Gateway() {
	limpaBufferEnvioSoquete();

	bufferEnvioSoquete[0] = make8(enderecoLoRaGateway, 1);
	bufferEnvioSoquete[1] = make8(enderecoLoRaGateway, 0);
	bufferEnvioSoquete[2] = canalLoRaGateway;

	strcat(bufferEnvioSoquete, "$,02,");
	sprintfSoquete(sTime.Hours , 2);
	sprintfSoquete(sTime.Minutes , 2);
	sprintfSoquete(sTime.Seconds , 2);
	sprintfSoquete(sDate.Date, 2);
	sprintfSoquete(sDate.Month, 2);
	sprintfSoquete(sDate.Year, 2);
	strcat(bufferEnvioSoquete, ",");

	sprintfSoquete(modoOperacao, 0);
	sprintfSoquete(sentidoMotor, 0);
	sprintfSoquete(flagRetornoHome, 0);
	sprintfSoquete(contadorBombaPrincipal, 3);
	sprintfSoquete(pressao, 3);
	sprintfSoquete(flagPortaAberta, 0);
	strcat(bufferEnvioSoquete, ",");

	sprintfSoquete(numeroSerial, 0);
	strcat(bufferEnvioSoquete, ",\r\n");

	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
}
/*==============================================================================
ENVIA ALARMES HORIMETRO HORARIO DE PONTA E OBSTACULO ATINGIDO
==============================================================================*/
void enviaStatusGerais2Gateway() {
	uint8_t reporteBarricada = false;

	if(flagBarricada || flagBarricadaGPS) {
		reporteBarricada = true;
	}
	limpaBufferEnvioSoquete();

	bufferEnvioSoquete[0] = make8(enderecoLoRaGateway, 1);
	bufferEnvioSoquete[1] = make8(enderecoLoRaGateway, 0);
	bufferEnvioSoquete[2] = canalLoRaGateway;

	strcat(bufferEnvioSoquete, "$,03,");
	sprintfSoquete(flagEmergencia, 0);
	sprintfSoquete(flagFalhaSeguranca, 0);
	sprintfSoquete(flagFalhaPressao, 0);
	sprintfSoquete(flagFaltaFase, 0);
	sprintfSoquete(reporteBarricada, 0);
	strcat(bufferEnvioSoquete, ",");

	sprintfSoquete(horimetro.horas, 0);
	strcat(bufferEnvioSoquete, ":");
	sprintfSoquete(horimetro.minutos, 2);
	strcat(bufferEnvioSoquete, ",");
	sprintfSoquete(flagHorarioPonta, 0);
	sprintfSoquete(flagObstaculoEncontrado, 0);
	strcat(bufferEnvioSoquete, ",\r\n");

	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
}
/*==============================================================================
ENVIA POSICAO INICIO OPERACAO
==============================================================================*/
void enviaPosicaoInicioOperacaoGateway() {
	char hemisferio[2], indicadorWE[2];
	hemisferio[0] = posicaoInicioOperacao.hemisferio;
	hemisferio[1] = 0x00;
	indicadorWE[0] = posicaoInicioOperacao.indicadorWE;
	indicadorWE[1] = 0x00;

	limpaBufferEnvioSoquete();

	bufferEnvioSoquete[0] = make8(enderecoLoRaGateway, 1);
	bufferEnvioSoquete[1] = make8(enderecoLoRaGateway, 0);
	bufferEnvioSoquete[2] = canalLoRaGateway;

	strcat(bufferEnvioSoquete, "$,04,");
	sprintfSoquete(posicaoInicioOperacao.latitudeGraus, 2);
	sprintfSoquete(posicaoInicioOperacao.latitudeMinutos, 2);
	strcat(bufferEnvioSoquete, ".");
	sprintfSoquete(posicaoInicioOperacao.latitudeSegundos, 5);
	strcat(bufferEnvioSoquete, ",");
	strcat(bufferEnvioSoquete, hemisferio);
	strcat(bufferEnvioSoquete, ",");
	sprintfSoquete(posicaoInicioOperacao.longitudeGraus, 3);
	sprintfSoquete(posicaoInicioOperacao.longitudeMinutos, 2);
	strcat(bufferEnvioSoquete, ".");
	sprintfSoquete(posicaoInicioOperacao.longitudeSegundos, 5);
	strcat(bufferEnvioSoquete, ",");
	strcat(bufferEnvioSoquete, indicadorWE);
	strcat(bufferEnvioSoquete, ",");
	sprintfSoquete(flagPosicaoInicioOperacaoConfiavel, 0);
	strcat(bufferEnvioSoquete, ",\r\n");

	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
}
/*==============================================================================
ENVIA SOLICITACAO DE LEITURA DE ACIONAMENTO
==============================================================================*/
void enviaSolicitacaoAcionamentoGateway() {
	limpaBufferEnvioSoquete();

	bufferEnvioSoquete[0] = make8(enderecoLoRaGateway, 1);
	bufferEnvioSoquete[1] = make8(enderecoLoRaGateway, 0);
	bufferEnvioSoquete[2] = canalLoRaGateway;

	strcat(bufferEnvioSoquete, "$,05,\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
}
/*==============================================================================
ENVIA SOLICITACAO DE LEITURA DE ACIONAMENTO
==============================================================================*/
void recebeAcionamentoGateway() {
	uint32_t idRecebido = 0;
	signed int offset = indexOf(bufferSoquete, "$,05,");

	if(offset >= 0) {
		if(bufferSoquete[offset + 5] == '1') { //pacote valido
			uint8_t dados[6];
			uint16_t dadoLamina = 0;
			for(uint16_t i = offset + 7; i < TAMANHO_BUFFER_SOQUETE; i ++) {
				if(isNumber(bufferSoquete[i])) {
					idRecebido *= 10;
					idRecebido += charToByte(bufferSoquete[i]);
				}
				else {
					offset = i + 1;
					i = TAMANHO_BUFFER_SOQUETE;
				}
			}

			if(idRecebido == ultimoIdAcionamento) {
				HAL_UART_Transmit(&huart3, "Acionamento ja processado\r\n", 27, 100);
				apagaSoqueteBuffer();
				return;
			}

			ultimoIdAcionamento = idRecebido;
			writeEepromIdAcionamento();

			for(uint8_t i = 0; i < 6; i ++) {
				dados[i] = charToByte(bufferSoquete[offset + i]);
			}

			for(uint16_t i = offset + 7; i < TAMANHO_BUFFER_SOQUETE; i ++) {
				if(isNumber(bufferSoquete[i])) {
					dadoLamina *= 10;
					dadoLamina += charToByte(bufferSoquete[i]);
				}
				else {
					offset = i + 1;
					i = TAMANHO_BUFFER_SOQUETE;
				}
			}

			modoOperacaoRemoto = dados[0];
			sentidoRemoto = dados[1];
			flagRetornoHome = dados[2];
			flagSelecaoPivot = dados[3];
			flagFertiIrrigacao = dados[4];
			flagOverrideBombaBooster = dados[5];

			if(dadoLamina> 0 && dadoLamina <= VALOR_MAXIMO_LAMINA_MM) {
				if(flagOperacao && dadoLamina != laminaDagua) {
					flagTrocouLaminaDagua = true;
					trocaLaminaDagua = dadoLamina;
				}
			}

			limpaReporteService();
			strcat(reporteService, "Operacao: ");
			sprintfReporteService(modoOperacaoRemoto, 0);
			strcat(reporteService, "\r\n");

			strcat(reporteService, "Sentido: ");
			sprintfReporteService(sentidoRemoto, 0);
			strcat(reporteService, "\r\n");

			strcat(reporteService, "Home: ");
			sprintfReporteService(flagRetornoHome, 0);
			strcat(reporteService, "\r\n");

			strcat(reporteService, "Pivot: ");
			sprintfReporteService(flagSelecaoPivot, 0);
			strcat(reporteService, "\r\n");

			if(flagTrocouLaminaDagua) {
				strcat(reporteService, "Lamina Dagua: ");
				sprintfReporteService(trocaLaminaDagua, 0);
				strcat(reporteService, "\r\n");
			}

			strcat(reporteService, "Ferti: ");
			sprintfReporteService(flagFertiIrrigacao, 0);
			strcat(reporteService, "\r\n");

			strcat(reporteService, "Bomba Override: ");
			sprintfReporteService(flagOverrideBombaBooster, 0);
			strcat(reporteService, "\r\n");

			HAL_UART_Transmit(&huart3, &reporteService, strlen(reporteService), 500);
			writeEepromFertiIrrigacao();
		}
		else { //pacote nao valido
			HAL_UART_Transmit(&huart3, "Pacote invalido\r\n", 17, 100);
		}
	}

	apagaSoqueteBuffer();
}
/*==============================================================================
CONFIGURA LoRa
==============================================================================*/
uint8_t configuraLoRaGateway() {
	off(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin); //M0 e M1 (inverso, utiliza um transistor NPN)
	HAL_Delay(100);

	if(debounceInverso(IOT_PSM_IND_GPIO_Port, IOT_PSM_IND_Pin)) {
		uint8_t sucesso = false;
		uint8_t ponteiro = 0;
		limpaBufferEnvioSoquete();
		bufferEnvioSoquete[0] = 0xC0; //Salvar
		bufferEnvioSoquete[1] = make8(enderecoLoRaPivotGateway, 1);
		bufferEnvioSoquete[2] = make8(enderecoLoRaPivotGateway, 0);
		bufferEnvioSoquete[3] = 0x1A; //8N1, 9600bps, 2.4k
		bufferEnvioSoquete[4] = canalLoRaGateway;
		bufferEnvioSoquete[5] = 0xC4; //opções
		HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, 6, 100);
		aguardaLoRaGateway(20);

		for(uint16_t i = 0; i < TAMANHO_BUFFER_SOQUETE; i ++) {
			if(bufferSoquete[i] == 0xC0) {
				i = TAMANHO_BUFFER_SOQUETE;
			}
			else {
				ponteiro ++;
			}
		}

		sucesso = true;
		for(uint8_t i = 0; i < 6; i ++) {
			if(bufferSoquete[i + ponteiro] != bufferEnvioSoquete[i]) {
				sucesso = false;
			}
		}

		apagaSoqueteBuffer();
		on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
		HAL_Delay(100);
		return sucesso;
	}

	on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	HAL_Delay(100);

	return false;
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
