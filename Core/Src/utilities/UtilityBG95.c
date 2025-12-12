///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityBG95
//AUTOR:      Fábio Almeida
//CRIADO:     02/12/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "string.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TIMEOUT_BG95 30
#define TIMEOUT_DUPLO_BG95 60
#define TIMEOUT_EXTENDIDO_BG95 120
#define TIMEOUT_CONEXAO_BG95 610
/*==============================================================================
AGUARDA OK OU ERROR
==============================================================================*/
uint8_t aguardaBG95(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutBG95 = tempo;
	while(contadorTimeoutBG95) {
		//HAL_UART_Receive_IT(&huart3, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			if(indexOf(bufferSoquete, "OK") >= 0) {
				contadorTimeoutBG95 = 0;
				sucesso = true;
			}

			if(indexOf(bufferSoquete, "ERROR") >= 0) {
				contadorTimeoutBG95 = 0;
			}

			if(indexOf(bufferSoquete, "+CME ERROR") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+CME ERROR") + 12;
				uint16_t codigoErro = 0;
				contadorTimeoutBG95 = 0;
				contadorErroBG95 ++;

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoErro *= 10;
						codigoErro += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				if(codigoErro == 703) {
					contadorErroBG95 = QUANTIDADE_ERRO_BG95;
				}
			}
		}
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);

	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
AGUARDA CONNECT
==============================================================================*/
uint8_t aguardaConnectBG95(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutBG95 = tempo;
	while(contadorTimeoutBG95) {
		//HAL_UART_Receive_IT(&huart3, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			if(indexOf(bufferSoquete, "OK") >= 0) {
				contadorTimeoutBG95 = 0;
				sucesso = true;
			}

			if(indexOf(bufferSoquete, "CONNECT") >= 0) {
				contadorTimeoutBG95 = 0;
				sucesso = true;
			}

			if(indexOf(bufferSoquete, "ERROR") >= 0) {
				contadorTimeoutBG95 = 0;
			}

			if(indexOf(bufferSoquete, "+CME ERROR") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+CME ERROR") + 12;
				uint16_t codigoErro = 0;
				contadorTimeoutBG95 = 0;
				contadorErroBG95 ++;

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoErro *= 10;
						codigoErro += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				if(codigoErro == 703) {
					contadorErroBG95 = QUANTIDADE_ERRO_BG95;
				}
			}
		}
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);

	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
AGUARDA POST
==============================================================================*/
uint8_t aguardaPostBG95(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutBG95 = tempo;
	while(contadorTimeoutBG95) {
		//HAL_UART_Receive_IT(&huart3, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			uint16_t codigoRecebido = 0;

			if(indexOf(bufferSoquete, "+QHTTPPOST:") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+QHTTPPOST:");

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(bufferSoquete[i] == ',') {
						offset = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoRecebido *= 10;
						codigoRecebido += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				contadorTimeoutBG95 = 0;

				if(codigoRecebido == 200 || codigoRecebido == 201) {
					sucesso = true;
				}
				else {
					contadorErroBG95 ++;
				}
			}
			if(indexOf(bufferSoquete, "+CME ERROR") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+CME ERROR") + 12;
				uint16_t codigoErro = 0;
				contadorTimeoutBG95 = 0;
				contadorErroBG95 ++;

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoErro *= 10;
						codigoErro += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				if(codigoErro == 703) {
					contadorErroBG95 = QUANTIDADE_ERRO_BG95;
				}
			}
		}
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);

	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
AGUARDA GET
==============================================================================*/
uint8_t aguardaGetBG95(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutBG95 = tempo;
	while(contadorTimeoutBG95) {
		//HAL_UART_Receive_IT(&huart3, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			uint16_t codigoRecebido = 0;

			if(indexOf(bufferSoquete, "+QHTTPGET:") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+QHTTPGET:");

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(bufferSoquete[i] == ',') {
						offset = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoRecebido *= 10;
						codigoRecebido += charToByte(bufferSoquete[i]);
					}
					else {
						offset = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				contadorTimeoutBG95 = 0;

				if(codigoRecebido == 200 || codigoRecebido == 201) {
					sucesso = true;
				}
				else {
					contadorErroBG95 ++;
				}
			}
			if(indexOf(bufferSoquete, "+CME ERROR") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+CME ERROR") + 12;
				uint16_t codigoErro = 0;
				contadorTimeoutBG95 = 0;
				contadorErroBG95 ++;

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoErro *= 10;
						codigoErro += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				if(codigoErro == 703) {
					contadorErroBG95 = QUANTIDADE_ERRO_BG95;
				}
			}
		}
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);

	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
AGUARDA READ
==============================================================================*/
uint8_t aguardaReadBG95(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutBG95 = tempo;
	while(contadorTimeoutBG95) {
		//HAL_UART_Receive_IT(&huart3, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			uint16_t codigoRecebido = 0;

			if(indexOf(bufferSoquete, "+QHTTPREAD:") >= 0) {
				sucesso = true;
			}
			if(indexOf(bufferSoquete, "+CME ERROR") >= 0) {
				uint16_t offset = indexOf(bufferSoquete, "+CME ERROR") + 12;
				uint16_t codigoErro = 0;
				contadorTimeoutBG95 = 0;
				contadorErroBG95 ++;

				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						codigoErro *= 10;
						codigoErro += charToByte(bufferSoquete[i]);
					}
					else {
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				if(codigoErro == 703) {
					contadorErroBG95 = QUANTIDADE_ERRO_BG95;
				}
			}
		}
	}
	HAL_UART_Transmit(&huart3, bufferSoquete, strlen(bufferSoquete), 200);

	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
ACIONA 4G
==============================================================================*/
void aciona4G() {
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"contextid\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Ligando 4G 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QICSGP=1,1,\"");
	strcat(bufferEnvioSoquete, gprsAPN);
	strcat(bufferEnvioSoquete, "\",\"\",\"\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Ligando 4G 20%");

	HAL_UART_Transmit(&huart6, "AT+QIACT=1\r\n", 12, 100);
	if(aguardaBG95(TIMEOUT_EXTENDIDO_BG95, false)) {
		operadoraConectada = 'C';
		escreveStringService("Ligando 4G 90%");
	}
	else {
		HAL_UART_Transmit(&huart6, "AT+QIACT?\r\n", 11, 100);
		aguardaBG95(TIMEOUT_BG95, true);
		signed int offset = indexOf(bufferSoquete, "+QIACT");

		if(offset >= 0) {
			uint8_t ip4G[4];
			for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
				if(bufferSoquete[i] == '\"') {
					offset = i + 1;
					i = TAMANHO_BUFFER_SOQUETE;
				}
				else if(i == TAMANHO_BUFFER_SOQUETE - 1) {
					operadoraConectada = 'N';
					contadorErroBG95 ++;
					apagaSoqueteBuffer();
					return;
				}
			}

			for(uint8_t octeto = 0; octeto < 4; octeto ++) {
				ip4G[octeto] = 0;
				for(uint16_t i = offset; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(isNumber(bufferSoquete[i])) {
						ip4G[octeto] *= 10;
						ip4G[octeto] += charToByte(bufferSoquete[i]);
					}
					else {
						offset = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}
				char ipChar[4];
				memset(ipChar, 0x00, 4);
				sprintf(ipChar, "%d", ip4G[octeto]);
				HAL_UART_Transmit(&huart3, &ipChar, strlen(ipChar), 100);
				if(octeto < 3) {
					HAL_UART_Transmit(&huart3, ".", 1, 100);
				}
				else {
					HAL_UART_Transmit(&huart3, "\r\n", 2, 100);
				}
			}

			if(!ip4G[0] && !ip4G[1] && !ip4G[2] && !ip4G[3]) {
				operadoraConectada = 'N';
				contadorErroBG95 ++;
			}
			else {
				operadoraConectada = 'C';
			}
		}
		else {
			operadoraConectada = 'N';
			contadorErroBG95 ++;
		}

		apagaSoqueteBuffer();
		return;
	}
}
/*==============================================================================
REPORTE VIA 4G
==============================================================================*/
uint8_t reporte4G() {
	char address[100];
	memset(address, 0x00, 100);
	strcat(address, SERVICE_ADDRESS);
	strcat(address, ENDPOINT_POST_REPORTE);
	montaReporteService();

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"contenttype\",4\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 15%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"contextid\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 30%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"custom_header\",\"");
	strcat(bufferEnvioSoquete, SERVICE_TOKEN);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	/* --Tentativa de colocar https no bg95, sem sucesso por enquanto
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"sslctxid\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"sslversion\",1,1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"ciphersuite\",1,0xFFFF\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"seclevel\",1,2\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QFUPL=\"cacert.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QFUPL=\"clientcert.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QFUPL=\"clientkey.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"cacert\",1,\"cacert.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"clientcert\",1,\"clientcert.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QSSLCFG=\"clientkey\",1,\"clientkey.pem\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Reportando 45%");
	*/

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPURL=");
	sprintfSoquete(strlen(address), 0);
	strcat(bufferEnvioSoquete, ",60\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	//HAL_UART_Transmit(&huart3, &address, strlen(address), 200);
	//HAL_UART_Transmit(&huart3, "\r\n", 2, 200);

	if(aguardaConnectBG95(TIMEOUT_BG95, false)) {
		escreveStringService("Reportando 55%");
		HAL_UART_Transmit(&huart6, &address, strlen(address), 200);
		if(aguardaConnectBG95(TIMEOUT_CONEXAO_BG95, false)) {
			escreveStringService("Reportando 65%");
			limpaBufferEnvioSoquete();
			strcat(bufferEnvioSoquete, "AT+QHTTPPOST=");
			sprintfSoquete(strlen(reporteService), 0);
			strcat(bufferEnvioSoquete, ",60\r\n");
			HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
			if(aguardaConnectBG95(TIMEOUT_CONEXAO_BG95, false)) {
				escreveStringService("Reportando 80%");
				HAL_UART_Transmit(&huart6, &reporteService, strlen(reporteService), 500);
				if(aguardaPostBG95(TIMEOUT_CONEXAO_BG95, false)) {
					escreveStringService("Reporte OK");
					return true;
					/* Não é necessário ler o que foi enviado no 201
					limpaBufferEnvioSoquete();
					strcat(bufferEnvioSoquete, "AT+QHTTPREAD=80\r\n");
					HAL_UART_Transmit(&huart3, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
					if(aguardaConnectBG95(TIMEOUT_DUPLO_BG95, false)) {
						aguardaReadBG95(TIMEOUT_DUPLO_BG95, false);
					}*/
				}
			}
		}
	}

	return false;
}
/*==============================================================================
LEITURA DE CONFIG VIA 4G
==============================================================================*/
void leituraConfig4G() {
	char address[100];
	char numeroSerialChar[10];
	memset(numeroSerialChar, 0x00, 10);
	sprintf(numeroSerialChar, "%d", numeroSerial);
	memset(address, 0x00, 100);
	strcat(address, SERVICE_ADDRESS);
	strcat(address, ENDPOINT_GET_CONFIG);
	strcat(address, numeroSerialChar);

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"contextid\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Verf. Config 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"custom_header\",\"");
	strcat(bufferEnvioSoquete, SERVICE_TOKEN);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Verf. Config 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPURL=");
	sprintfSoquete(strlen(address), 0);
	strcat(bufferEnvioSoquete, ",60\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);

	if(aguardaConnectBG95(TIMEOUT_BG95, false)) {
		escreveStringService("Verf. Config 60%");
		HAL_UART_Transmit(&huart6, &address, strlen(address), 200);
		if(aguardaConnectBG95(TIMEOUT_CONEXAO_BG95, false)) {
			escreveStringService("Verf. Config 70%");
			limpaBufferEnvioSoquete();
			strcat(bufferEnvioSoquete, "AT+QHTTPGET=60\r\n");
			HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
			if(aguardaBG95(TIMEOUT_CONEXAO_BG95, false)) {
				escreveStringService("Verf. Config 80%");
				if(aguardaGetBG95(TIMEOUT_CONEXAO_BG95, true)) {
					escreveStringService("Verf. Config 90%");
					limpaBufferEnvioSoquete();
					strcat(bufferEnvioSoquete, "AT+QHTTPREAD=80\r\n");
					HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
					if(aguardaReadBG95(TIMEOUT_DUPLO_BG95, true)) {
						escreveStringService("Config. OK");
						leituraConfiguracaoService();
						flagServiceLeituraConfig = false;
					}
				}
			}
		}
	}
}
/*==============================================================================
LEITURA DE ACIONAMENTO VIA 4G
==============================================================================*/
void leituraAcionamento4G() {
	char address[100];
	char numeroSerialChar[10];
	memset(numeroSerialChar, 0x00, 10);
	sprintf(numeroSerialChar, "%d", numeroSerial);
	memset(address, 0x00, 100);
	strcat(address, SERVICE_ADDRESS);
	strcat(address, ENDPOINT_GET_ACIONAMENTO);
	strcat(address, numeroSerialChar);

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"contextid\",1\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Verf. Acion. 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPCFG=\"custom_header\",\"");
	strcat(bufferEnvioSoquete, SERVICE_TOKEN);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);
	escreveStringService("Verf. Acion. 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QHTTPURL=");
	sprintfSoquete(strlen(address), 0);
	strcat(bufferEnvioSoquete, ",60\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);

	if(aguardaConnectBG95(TIMEOUT_BG95, false)) {
		escreveStringService("Verf. Acion. 60%");
		HAL_UART_Transmit(&huart6, &address, strlen(address), 200);
		if(aguardaConnectBG95(TIMEOUT_CONEXAO_BG95, false)) {
			escreveStringService("Verf. Acion. 70%");
			limpaBufferEnvioSoquete();
			strcat(bufferEnvioSoquete, "AT+QHTTPGET=60\r\n");
			HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
			if(aguardaBG95(TIMEOUT_CONEXAO_BG95, false)) {
				escreveStringService("Verf. Acion. 80%");
				if(aguardaGetBG95(TIMEOUT_CONEXAO_BG95, true)) {
					escreveStringService("Verf. Acion. 90%");
					limpaBufferEnvioSoquete();
					strcat(bufferEnvioSoquete, "AT+QHTTPREAD=80\r\n");
					HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
					if(aguardaReadBG95(TIMEOUT_DUPLO_BG95, true)) {
						escreveStringService("Acion. OK");
						leituraAcionamentoService();
					}
				}
			}
		}
	}
}
/*==============================================================================
VERIFICA A QUALIDADE DO SINAL DO BG95
==============================================================================*/
void verificaQualidadeSinalBG95() {
	signed int offset = 0;
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+QINISTAT\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, false);

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CSQ\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaBG95(TIMEOUT_BG95, true);

	offset = indexOf(bufferSoquete, "+CSQ:");

	if(offset >= 0) {
		uint8_t valorSinal = 0;

		for(uint16_t i = offset + 6; i < TAMANHO_BUFFER_SOQUETE; i ++) {
			if(bufferSoquete[i] == ',') {
				i = TAMANHO_BUFFER_SOQUETE;
			}
			else {
				valorSinal *= 10;
				valorSinal += charToByte(bufferSoquete[i]);
			}
		}

		apagaSoqueteBuffer();

		if(valorSinal > 31) {
			percentualSinalOperadora = 0;
			return;
		}

		percentualSinalOperadora = map(valorSinal, 0, 31, 0, 99);
		return;
	}

	percentualSinalOperadora = 0;
	apagaSoqueteBuffer();
}
/*==============================================================================
REINICIA BG95
==============================================================================*/
void resetBG95() {
	/*on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	HAL_Delay(1200);
	off(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	HAL_Delay(1200);
	on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	HAL_Delay(1200);
	off(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);*/
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
