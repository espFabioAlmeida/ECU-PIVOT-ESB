///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    FTPTask
//AUTOR:      Fábio Almeida
//CRIADO:     25/07/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
# include "main.h"
# include "global.h"
# include "string.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_OPERACOES_REPORTE 30
/*==============================================================================
SERVICE GPRS
==============================================================================*/
void serviceGPRS() {
	static uint8_t contadorOperacoesReporte = 0;
	if(contadorReporteService == TEMPO_SERVICE) {
		static uint8_t contStartup = 0;
		if(contStartup < 1) {
			contStartup ++;
			flagServiceOcupado = false;
			return;
		}
		flagServiceOcupado = true;

		if(flagOperacao || flagReportaDesligou) {
			contadorOperacoesReporte = QUANTIDADE_OPERACOES_REPORTE;
		}
		else {
			contadorOperacoesReporte ++;
			if(contadorOperacoesReporte >= QUANTIDADE_OPERACOES_REPORTE) {
				contadorOperacoesReporte = QUANTIDADE_OPERACOES_REPORTE;
			}
		}

		HAL_UART_Transmit(&huart3, "Iniciando GPRS\r\n", 16, 100);
		verificaQualidadeSinalGPRS();
		escreveStringService("Ligando GPRS 00%");
		acionaGPRS();
		if(operadoraConectada == 'C') {
			escreveStringService("Reportando 00%");
			if(contadorOperacoesReporte >= QUANTIDADE_OPERACOES_REPORTE) {
				if(reporteGPRS()) {
					contadorOperacoesReporte = 0;
					flagReportaDesligou = false;
				}
			}
		}
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "GPRS Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		if(!flagOperacao && operadoraConectada == 'C' && flagServiceLeituraConfig) {
			escreveStringService("Verf. Config 00%");
			leituraConfigGPRS();
		}
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "GPRS Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		if(operadoraConectada == 'C') {
			escreveStringService("Verf. Acion. 00%");
			leituraAcionamentoGPRS();
		}
		HAL_UART_Transmit(&huart3, "Fim GPRS\r\n", 10, 100);
		flagServiceOcupado = false;
		contadorReporteService = 0;
		escreveStringService("Servico Standby");
	}

	if(contadorErroGPRS >= QUANTIDADE_ERRO_GPRS) {
		flagServiceOcupado = true;
		contadorErroGPRS = 0;
		contadorReporteService = 0;
		HAL_UART_Transmit(&huart3, "Reset GPRS\r\n", 12, 100);
		escreveStringService("Reset GPRS");
		resetGPRS();
		escreveStringService("Servico Standby");
		flagServiceOcupado = false;
	}
}
/*==============================================================================
SERVICE WIFI
==============================================================================*/
void serviceWiFi() {
	char stringReporte[17];
	uint8_t sucessoReporte = false, sucessoConfig = false, sucessoAcionamento = false;
	if(contadorReporteService == TEMPO_SERVICE) {
		static uint8_t contStartup = 0;
		if(contStartup < 1) {
			contStartup ++;
			flagServiceOcupado = false;
			return;
		}

		HAL_UART_Transmit(&huart3, "Iniciando Servico WiFi\r\n", 24, 100);
		flagServiceOcupado = true;
		escreveStringService("Reporte 00%");

		sucessoReporte = reporteWiFi();
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "WiFi Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		if(!flagOperacao) {
			escreveStringService("Config 00%");
			sucessoConfig = leituraConfigWiFi();
		}
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "WiFi Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		escreveStringService("Acionamento 00%");
		sucessoAcionamento = leituraAcionamentoWiFi();

		flagServiceOcupado = false;
		contadorReporteService = 0;
		HAL_UART_Transmit(&huart3, "Fim Servico WiFi\r\n", 18, 100);
		memset(stringReporte, 0x00, 17);
		strcat(stringReporte, "Standby ");
		if(sucessoReporte) {
			strcat(stringReporte, "OK ");
		}
		else {
			strcat(stringReporte, "ER ");
		}
		if(sucessoConfig) {
			strcat(stringReporte, "OK ");
		}
		else {
			strcat(stringReporte, "ER ");
		}
		if(sucessoAcionamento) {
			strcat(stringReporte, "OK");
		}
		else {
			strcat(stringReporte, "ER");
		}

		escreveStringService(&stringReporte);

		setPin(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin, !sucessoReporte);
	}
}
/*==============================================================================
SERVICE 4G
==============================================================================*/
void service4G() {
	static uint8_t contadorOperacoesReporte = 0;
	static uint8_t contStartup = 0;
	static uint8_t tempoStartup = 1;

	if(contadorReporteService == TEMPO_SERVICE) {
		if(contStartup < tempoStartup) {
			contadorReporteService = 0;
			contStartup ++;
			flagServiceOcupado = false;
			return;
		}

		flagServiceOcupado = true;

		if(flagOperacao || flagReportaDesligou) {
			contadorOperacoesReporte = QUANTIDADE_OPERACOES_REPORTE;
		}
		else {
			contadorOperacoesReporte ++;
			if(contadorOperacoesReporte >= QUANTIDADE_OPERACOES_REPORTE) {
				contadorOperacoesReporte = QUANTIDADE_OPERACOES_REPORTE;
			}
		}

		HAL_UART_Transmit(&huart3, "Iniciando BG95\r\n", 16, 100);
		verificaQualidadeSinalBG95();
		escreveStringService("Ligando 4G 00%");
		aciona4G();

		if(operadoraConectada == 'C') {
			escreveStringService("Reportando 00%");
			if(contadorOperacoesReporte >= QUANTIDADE_OPERACOES_REPORTE) {
				if(reporte4G()) {
					contadorOperacoesReporte = 0;
					flagReportaDesligou = false;
				}
			}
		}
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "BG95 Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		if(!flagOperacao && operadoraConectada == 'C' && flagServiceLeituraConfig) {
			escreveStringService("Verf. Config 00%");
			leituraConfig4G();
		}
		if(flagMenu) {
			HAL_UART_Transmit(&huart3, "BG95 Interrompido\r\n", 19, 100);
			flagServiceOcupado = false;
			return;
		}

		if(operadoraConectada == 'C') {
			escreveStringService("Verf. Acion. 00%");
			leituraAcionamento4G();
		}

		HAL_UART_Transmit(&huart3, "Fim BG95\r\n", 10, 100);
		flagServiceOcupado = false;
		contadorReporteService = 0;
		escreveStringService("Servico Standby");
	}

	if(contadorErroBG95 >= QUANTIDADE_ERRO_BG95) {
		contadorErroBG95 = 0;
		contadorReporteService = 0;
		HAL_UART_Transmit(&huart3, "Aguardando BG95\r\n", 17, 100);
		escreveStringService("Aguardando 4G");
		contStartup = 0;
		tempoStartup = 4;
	}
}
/*==============================================================================
SERVICE RADIO
==============================================================================*/
void serviceRadio() {
	static uint8_t seletorComando = 0;
	if(contadorReporteService == TEMPO_SERVICE) {
		contadorReporteService = 0;
		flagServiceOcupado = true;

		switch(seletorComando) {
			case 0:
				HAL_UART_Transmit(&huart3, "Enviando Posicao Atual\r\n", 24, 100);
				escreveStringService("-> Posicao Atual");
				enviaPosicaoAtualGateway();
				break;

			case 1:
				HAL_UART_Transmit(&huart3, "Enviando Status 1\r\n", 19, 100);
				escreveStringService("-> Status 1");
				enviaStatusGerais1Gateway();
				break;

			case 2:
				HAL_UART_Transmit(&huart3, "Enviando Status 2\r\n", 19, 100);
				escreveStringService("-> Status 2");
				enviaStatusGerais2Gateway();
				break;

			case 3:
				HAL_UART_Transmit(&huart3, "Enviando Posicao Inicial\r\n", 26, 100);
				escreveStringService("-> Posic. Inicio");
				enviaPosicaoInicioOperacaoGateway();
				break;

			case 4:
				HAL_UART_Transmit(&huart3, "Solicitando acioanmento\r\n", 25, 100);
				escreveStringService("-> Sol. Acionam.");
				enviaSolicitacaoAcionamentoGateway();
				break; //solicita acionamento

			case 5:
				HAL_UART_Transmit(&huart3, "Standby\r\n", 9, 100);
				escreveStringService("Gateway Standby");
				break; //recebe acionamento
		}

		seletorComando ++;
		if(seletorComando > 5) {
			seletorComando = 0;
		}
	}

	if(soqueteDataIn == 0x0A) {
		recebeAcionamentoGateway();
	}

	flagServiceOcupado = false;
}
/*==============================================================================
SERVICE TASk
==============================================================================*/
void serviceTask() {
	switch(placaSoquete) {
		case SOQUETE_GPRS: serviceGPRS(); break;
		case SOQUETE_WIFI: serviceWiFi(); break;
		case SOQUETE_4G: service4G(); break;
		case SOQUETE_RADIO: serviceRadio(); break;
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
