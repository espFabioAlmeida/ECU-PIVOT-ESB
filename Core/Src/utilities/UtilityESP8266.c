///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityESP8266
//AUTOR:      Fábio Almeida
//CRIADO:     03/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "string.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TEMPO_PADRAO_DADOS 10
/*==============================================================================
AGUARDA WIFI
==============================================================================*/
uint8_t aguardaWiFi(uint16_t tempo, uint8_t isRequest) {
	contadorTimeoutWiFi = tempo;
	contadorTimeoutDadosWiFi = 0;
	//apagaSoqueteBuffer();

	if(isRequest) {
		while(contadorTimeoutWiFi) {
			if(soqueteDataIn == '}') {
				contadorTimeoutWiFi = 0;
				contadorTimeoutDadosWiFi = 1;
			}

			if(contadorTimeoutDadosWiFi >= TEMPO_PADRAO_DADOS) {
				contadorTimeoutWiFi = 0;
				contadorTimeoutDadosWiFi = 0;
			}
		}
	}
	else {
		while(contadorTimeoutWiFi) {
			if(soqueteDataIn == '\n') {
				if(indexOf(bufferEnvioSoquete, "OK") >= 0 || indexOf(bufferEnvioSoquete, "ERROR") >= 0) {
					contadorTimeoutWiFi = 0;
				}
			}
		}
	}

	if(contadorSoqueteBuffer) {
		HAL_UART_Transmit(&huart3, &bufferSoquete, strlen(bufferSoquete), 500);
		if(indexOf(bufferSoquete, "200 OK") >= 0) {
			HAL_UART_Transmit(&huart3, "RECEBIDO 200\n", 13, 100);
			return true;
		}
		if(indexOf(bufferSoquete, "201 Created") >= 0) {
			HAL_UART_Transmit(&huart3, "RECEBIDO 201\n", 13, 100);
			return true;
		}
	}
	apagaSoqueteBuffer();
	return false;
}
/*==============================================================================
REPORTE WIFI
==============================================================================*/
uint8_t reporteWiFi() {
	uint8_t sucesso = false;
	char request[TAMANHO_MAXIMO_REPORTE + 512];
	char jsonLengthChar[10];
	memset(request, 0x00, TAMANHO_MAXIMO_REPORTE + 512);
	memset(jsonLengthChar, 0x00, 10);

	montaReporteService();
	sprintf(jsonLengthChar, "%d", strlen(reporteService));

	strcat(request, "POST /");
	strcat(request, ENDPOINT_POST_REPORTE);
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, SERVICE_HOST);
	strcat(request, "\r\n");
	strcat(request, "Content-Type: application/json\r\n");
	strcat(request, SERVICE_TOKEN);
	strcat(request, "\r\n");
	strcat(request, "Content-Length: ");
	strcat(request, jsonLengthChar);
	strcat(request, "\r\n");

	strcat(request, "\r\n");
	strcat(request, reporteService);
	strcat(request, "\r\n");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSTART=\"TCP\",\"");
	strcat(bufferEnvioSoquete, SERVICE_HOST);
	strcat(bufferEnvioSoquete, "\"\,80\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(100, false);
	escreveStringService("Reporte 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSEND=");
	sprintfSoquete(strlen(request), 0);
	strcat(bufferEnvioSoquete, "\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(50, false);
	escreveStringService("Reporte 50%");

	HAL_UART_Transmit(&huart4, request, strlen(request), 1000);
	if(aguardaWiFi(100, true)) {
		sucesso = true;
		apagaSoqueteBuffer();
	}
	escreveStringService("Reporte 90%");

	//HAL_UART_Transmit(&huart4, "AT+CIPCLOSE\r\n", 13, 500);
	//aguardaWiFi(50);
	escreveStringService("Reporte 99%");

	HAL_UART_Transmit(&huart4, "+++", 3, 200);
	return sucesso;
}
/*==============================================================================
CONFIG WIFI
==============================================================================*/
uint8_t leituraConfigWiFi() {
	uint8_t sucesso = false;
	char request[512];
	char serialNumberChar[10];
	memset(request, 0x00, 512);
	memset(serialNumberChar, 0x00, 10);
	sprintf(serialNumberChar, "%d", numeroSerial);

	strcat(request, "GET /");
	strcat(request, ENDPOINT_GET_CONFIG);
	strcat(request, serialNumberChar);
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, SERVICE_HOST);
	strcat(request, "\r\n");
	strcat(request, "Cache-Control: no-cache\r\n");
	strcat(request, SERVICE_TOKEN);
	strcat(request, "\r\n");
	strcat(request, "\r\n");

	/*limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSTART=\"TCP\",\"");
	strcat(bufferEnvioSoquete, SERVICE_HOST);
	strcat(bufferEnvioSoquete, "\"\,80\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(100, false);
	escreveStringService("Config 40%");*/

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSEND=");
	sprintfSoquete(strlen(request), 0);
	strcat(bufferEnvioSoquete, "\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(50, false);
	escreveStringService("Config 50%");

	HAL_UART_Transmit(&huart4, request, strlen(request), 1000);
	if(aguardaWiFi(100, true)) {
		sucesso = true;
		leituraConfiguracaoService();
		apagaSoqueteBuffer();
	}
	escreveStringService("Config 90%");

	//HAL_UART_Transmit(&huart4, "AT+CIPCLOSE\r\n", 13, 500);
	//aguardaWiFi(50);
	escreveStringService("Config 99%");

	HAL_UART_Transmit(&huart4, "+++", 3, 200);
	return sucesso;
}
/*==============================================================================
ACIONAMENTO WIFI
==============================================================================*/
uint8_t leituraAcionamentoWiFi() {
	uint8_t sucesso = false;
	char request[512];
	char serialNumberChar[10];
	memset(request, 0x00, 512);
	memset(serialNumberChar, 0x00, 10);
	sprintf(serialNumberChar, "%d", numeroSerial);

	strcat(request, "GET /");
	strcat(request, ENDPOINT_GET_ACIONAMENTO);
	strcat(request, serialNumberChar);
	strcat(request, " HTTP/1.1\r\n");
	strcat(request, "Host: ");
	strcat(request, SERVICE_HOST);
	strcat(request, "\r\n");
	strcat(request, "Cache-Control: no-cache\r\n");
	strcat(request, SERVICE_TOKEN);
	strcat(request, "\r\n");
	strcat(request, "\r\n");

	/*limpaBufferEnvioSoquete(
	strcat(bufferEnvioSoquete, "AT+CIPSTART=\"TCP\",\"");
	strcat(bufferEnvioSoquete, SERVICE_HOST);
	strcat(bufferEnvioSoquete, "\"\,80\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(100, false);
	escreveStringService("Acionamento 40%");*/

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSEND=");
	sprintfSoquete(strlen(request), 0);
	strcat(bufferEnvioSoquete, "\r\n");
	HAL_UART_Transmit(&huart4, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaWiFi(50, false);
	escreveStringService("Acionamento 50%");

	HAL_UART_Transmit(&huart4, request, strlen(request), 1000);
	if(aguardaWiFi(100, true)) {
		sucesso = true;
		leituraAcionamentoService();
		apagaSoqueteBuffer();
	}
	escreveStringService("Acionamento 90%");

	HAL_UART_Transmit(&huart4, "AT+CIPCLOSE\r\n", 13, 500);
	aguardaWiFi(50, false);
	escreveStringService("Acionamento 99%");

	HAL_UART_Transmit(&huart4, "+++", 3, 200);
	return sucesso;
}
/*==============================================================================
REINICIA MODULO WIFI
==============================================================================*/
void reiniciaModuloWiFi() {
	HAL_UART_Transmit(&huart4, "AT+RESTORE\r\n", 12, 100);
	contadorTimeoutWiFi = 35;

	while(contadorTimeoutWiFi) {
	}

	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 100);
	apagaSoqueteBuffer();

	HAL_UART_Transmit(&huart4, "AT+CWMODE_DEF=1\r\n", 17, 100);
	contadorTimeoutWiFi = 10;

	while(contadorTimeoutWiFi) {
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);
	apagaSoqueteBuffer();
}
/*==============================================================================
CONECTAR EM AP
==============================================================================*/
uint8_t conectarAPWiFi() {
	limpaBufferEnvioSoquete();

	strcat(bufferEnvioSoquete, "AT+CWJAP_DEF=\"");
	strcat(bufferEnvioSoquete, wifiSSID);
	strcat(bufferEnvioSoquete, "\",\"");
	strcat(bufferEnvioSoquete, wifiSenha);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart4, bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);

	contadorTimeoutWiFi = 100;

	while(contadorTimeoutWiFi) {
	}

	if(contadorSoqueteBuffer) {
		HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);
		if(indexOf(bufferSoquete, "WIFI CONNECTED") >= 0) {
			apagaSoqueteBuffer();
			HAL_UART_Transmit(&huart3, "WIFI conectada", 14, 100);
			return true;
		}
	}

	apagaSoqueteBuffer();
	return false;
}
/*==============================================================================
WIFI COMO DHCP
==============================================================================*/
void wiFiHabilitaDhcp() {
	HAL_UART_Transmit(&huart4, "AT+CWDHCP_DEF=1,1\r\n", 19, 100);

	contadorTimeoutWiFi = 50;
	while(contadorTimeoutWiFi) {
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 100);
	apagaSoqueteBuffer();
}
/*==============================================================================
WIFI CONFIGURA IP FIXO
==============================================================================*/
void wiFiFixaIp() {
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CIPSTA_DEF=\"");
	for(uint8_t i = 0; i < 4; i ++) {
		sprintfSoquete(wifiIp[i], 0);
		if(i < 3) {
			strcat(bufferEnvioSoquete, ".");
		}
	}
	strcat(bufferEnvioSoquete, "\",\"");

	for(uint8_t i = 0; i < 4; i ++) {
		sprintfSoquete(wifiGateway[i], 0);
		if(i < 3) {
			strcat(bufferEnvioSoquete, ".");
		}
	}
	strcat(bufferEnvioSoquete, "\",\"");

	for(uint8_t i = 0; i < 4; i ++) {
		sprintfSoquete(wifiMask[i], 0);
		if(i < 3) {
			strcat(bufferEnvioSoquete, ".");
		}
	}
	strcat(bufferEnvioSoquete, "\"\r\n");

	HAL_UART_Transmit(&huart4, bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);

	contadorTimeoutWiFi = 50;
	while(contadorTimeoutWiFi) {
	}

	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 100);
	apagaSoqueteBuffer();
}
/*==============================================================================
WIFI LER IP DINÂMICO
==============================================================================*/
void wiFiLerIpDinamico() {
	wifiIpDinamico[0] = 0;
	wifiIpDinamico[1] = 0;
	wifiIpDinamico[2] = 0;
	wifiIpDinamico[3] = 0;

	HAL_UART_Transmit(&huart4, "AT+CIFSR\r\n", 10, 100);

	contadorTimeoutWiFi = 50;
	while(contadorTimeoutWiFi) {
	}

	if(contadorSoqueteBuffer) {
		signed int offset = indexOf(bufferSoquete, "+CIFSR:STAIP");
		HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 100);

		if(offset >= 0) {
			uint16_t offsetBuffer = offset + 14;
			for(uint8_t i = 0; i < 4; offsetBuffer ++) {
				if(bufferSoquete[offsetBuffer] == '.' || bufferSoquete[offsetBuffer] == '\"') {
					i ++;
				}
				else {
					wifiIpDinamico[i] *= 10;
					wifiIpDinamico[i] += charToByte(bufferSoquete[offsetBuffer]);
				}
			}
		}
	}

	apagaSoqueteBuffer();
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
