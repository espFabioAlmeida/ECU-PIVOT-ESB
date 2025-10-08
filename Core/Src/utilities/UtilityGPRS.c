///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityGPRS
//AUTOR:      Fábio Almeida
//CRIADO:     26/06/2024
//OBSERVAÇÕES:
//Servidor padrão: 208.115.238.110
//Login padrão: automase
//Senha padrão: lD8CkGaY07
//APN padrão: emnify
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TIMEOUT_ACTION 320
#define TIMEOUT_GPRS 30
#define TIMEOUT_DUPLO_GPRS 60
#define TIMEOUT_EXTENDIDO_GPRS 120
/*==============================================================================
AGUARDA OK OU ERROR
==============================================================================*/
uint8_t aguardaGPRS(uint16_t tempo, uint8_t preservaBuffer) {
	uint8_t sucesso = false;
	contadorTimeoutGPRS = tempo;
	while(contadorTimeoutGPRS) {
		//HAL_UART_Receive_IT(&huart6, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			if(indexOf(bufferSoquete, "OK") >= 0) {
				contadorTimeoutGPRS = 0;
				sucesso = true;
			}
			if(indexOf(bufferSoquete, "ERROR") >= 0) {
				contadorTimeoutGPRS = 0;
			}
		}
	}
	if(preservaBuffer == false) {
		apagaSoqueteBuffer();
	}

	return sucesso;
}
/*==============================================================================
AGUARDA RETORNO DO HTTP ACTION
==============================================================================*/
uint16_t aguardaGPRSAction() {
	uint8_t comandoRecebido = false;
	uint16_t quantidadeDados = 0;
	contadorTimeoutGPRS = TIMEOUT_ACTION;
	while(contadorTimeoutGPRS) {
		//HAL_UART_Receive_IT(&huart6, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			signed int index = indexOf(bufferSoquete, "ACTION:");
			if(index >= 0) {
				uint8_t codigoHTTP = 0;
				for(uint16_t i = index; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(bufferSoquete[i] == ',') {
						index = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
				}

				for(uint16_t i = index; i < TAMANHO_BUFFER_SOQUETE; i ++) {
					if(bufferSoquete[i] == ',') {
						index = i + 1;
						i = TAMANHO_BUFFER_SOQUETE;
					}
					else {
						codigoHTTP *= 10;
						codigoHTTP += charToByte(bufferSoquete[i]);
					}
				}
				if(codigoHTTP == 200 || codigoHTTP == 201) {
					contadorErroGPRS = 0;
					for(uint16_t i = index; i < TAMANHO_BUFFER_SOQUETE; i ++) {
						if(bufferSoquete[i] == '\r' || bufferSoquete[i] == '\n') {
							i = TAMANHO_BUFFER_SOQUETE;
						}
						else {
							quantidadeDados *= 10;
							quantidadeDados += charToByte(bufferSoquete[i]);
						}
					}
				}
				else {
					contadorErroGPRS ++;
					if(codigoHTTP >= 600) {
						contadorErroGPRS = QUANTIDADE_ERRO_GPRS;
					}
				}
				contadorTimeoutGPRS = 0;
				comandoRecebido = true;
			}
		}
	}

	if(comandoRecebido == false) {
		contadorErroGPRS ++;
	}

	apagaSoqueteBuffer();
	return quantidadeDados;
}
/*==============================================================================
AGUARDA RETORNO DA INSTRUÇÃO DOWNLOAD
==============================================================================*/
uint8_t aguardaGPRSDownload(uint16_t tempo) {
	uint8_t sucesso = false;
	contadorTimeoutGPRS = tempo;
	while(contadorTimeoutGPRS) {
		//HAL_UART_Receive_IT(&huart6, &soqueteDataIn, 1);
		if(soqueteDataIn == '\n') {
			if(indexOf(bufferSoquete, "DOWNLOAD") >= 0) {
				sucesso = true;
				contadorTimeoutGPRS = 0;
			}
			else if(indexOf(bufferSoquete, "ERROR") >= 0) {
				sucesso = false;
				contadorTimeoutGPRS = 0;
			}
		}
	}
	apagaSoqueteBuffer();
	return sucesso;
}
/*==============================================================================
ACIONA GPRS
==============================================================================*/
void acionaGPRS() {
	HAL_UART_Transmit(&huart6, "AT+CGATT=1\r\n", 12, 100); //Liga o GPRS
	if(aguardaGPRS(TIMEOUT_EXTENDIDO_GPRS, false)) {
		operadoraConectada = 'C';
	}
	else {
		operadoraConectada = 'N';
		contadorErroGPRS ++;
		return;
	}
	escreveStringService("Ligando GPRS 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+SAPBR=3,1,APN,"); //Configura APN
	strcat(bufferEnvioSoquete, gprsAPN);
	strcat(bufferEnvioSoquete, "\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Ligando GPRS 60%");

	HAL_UART_Transmit(&huart6, "AT+SAPBR=1,1\r\n", 14, 100); //Habilita IP 1
	aguardaGPRS(TIMEOUT_EXTENDIDO_GPRS, false);
	escreveStringService("Ligando GPRS 90%");

	HAL_UART_Transmit(&huart6, "AT+SAPBR=2,1\r\n", 14, 100); //Verifica IP 1
	aguardaGPRS(TIMEOUT_EXTENDIDO_GPRS, false);
}
/*==============================================================================
REPORTE VIA GPRS
==============================================================================*/
uint8_t reporteGPRS() {
	HAL_UART_Transmit(&huart6, "AT+HTTPINIT\r\n", 13, 200); //Iniciar HTTP
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 10%");

	HAL_UART_Transmit(&huart6, "AT+HTTPPARA=\"CID\",1\r\n", 21, 200); //Configura HTTP para o IP1
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"URL\",\""); //Configura Endereço
	strcat(bufferEnvioSoquete, SERVICE_ADDRESS);
	strcat(bufferEnvioSoquete, ENDPOINT_POST_REPORTE);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 30%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"USERDATA\",\""); //Configura Token
	strcat(bufferEnvioSoquete, SERVICE_TOKEN);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 800);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 50%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"TIMEOUT\",\"30\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 55%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPDATA=0\r\n"); //Apaga o buffer de dados para POST
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 60%");

	montaReporteService();
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPDATA="); //Prepara para enviar novos dados
	sprintfSoquete(strlen(reporteService), 0);
	strcat(bufferEnvioSoquete, ",1200\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
	escreveStringService("Reportando 80%");

	if(aguardaGPRSDownload(TIMEOUT_EXTENDIDO_GPRS)) { //Aguarda fim da transmissão dos dados
		escreveStringService("Reportando 90%");
		HAL_UART_Transmit(&huart6, &reporteService, strlen(reporteService), 800);
		aguardaGPRS(TIMEOUT_DUPLO_GPRS, false);

		HAL_UART_Transmit(&huart6, "AT+HTTPACTION=1\r\n", 17, 200); //Efetua o POST
		aguardaGPRSAction();
		escreveStringService("Reporte OK");
		return true;
	}

	return false;
}
/*==============================================================================
LEITURA CONFIGURACOES VIA GPRS
==============================================================================*/
void leituraConfigGPRS() {
	uint16_t quantidadeDados = 0;
	HAL_UART_Transmit(&huart6, "AT+HTTPINIT\r\n", 13, 200); //Inicia HTTP
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Config 10%");

	HAL_UART_Transmit(&huart6, "AT+HTTPPARA=\"CID\",1\r\n", 21, 200); //Configura HTTP para o IP 1
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Config 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"URL\",\""); //Configura Endereço
	strcat(bufferEnvioSoquete, SERVICE_ADDRESS);
	strcat(bufferEnvioSoquete, ENDPOINT_GET_CONFIG);
	sprintfSoquete(numeroSerial, 0);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Config 30%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Config 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"TIMEOUT\",\"30\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Config 45%");

	HAL_UART_Transmit(&huart6, "AT+HTTPACTION=0\r\n", 17, 200); //Envia o GET
	aguardaGPRS(TIMEOUT_DUPLO_GPRS, false);
	escreveStringService("Verf. Config 50%");

	quantidadeDados = aguardaGPRSAction(); //Aguarda receber o pacote
	escreveStringService("Verf. Config 80%");

	if(quantidadeDados) {
		escreveStringService("Verf. Config 90%");
		limpaBufferEnvioSoquete();
		strcat(bufferEnvioSoquete, "AT+HTTPREAD\r\n"); //Lê os dados recebido
		HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
		aguardaGPRS(TIMEOUT_GPRS, true);
		escreveStringService("Config. OK");
		leituraConfiguracaoService();
		flagServiceLeituraConfig = false;
	}
}
/*==============================================================================
LEITURA ACIONAMENTO VIA GPRS
==============================================================================*/
void leituraAcionamentoGPRS() {
	uint16_t quantidadeDados = 0;
	HAL_UART_Transmit(&huart6, "AT+HTTPINIT\r\n", 13, 200); //Inicia HTTP
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Acion. 10%");

	HAL_UART_Transmit(&huart6, "AT+HTTPPARA=\"CID\",1\r\n", 21, 200); //Configura HTTP para o IP 1
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Acion. 20%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"URL\",\""); //Configura Endereço
	strcat(bufferEnvioSoquete, SERVICE_ADDRESS);
	strcat(bufferEnvioSoquete, ENDPOINT_GET_ACIONAMENTO);
	sprintfSoquete(numeroSerial, 0);
	strcat(bufferEnvioSoquete, "\"\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Acion. 30%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Verf. Acion. 40%");

	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+HTTPPARA=\"TIMEOUT\",\"30\"\r\n"); //Configura tipo de arquivo
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 500);
	aguardaGPRS(TIMEOUT_GPRS, false);
	escreveStringService("Reportando 45%");

	HAL_UART_Transmit(&huart6, "AT+HTTPACTION=0\r\n", 17, 200); //Envia o GET
	aguardaGPRS(TIMEOUT_DUPLO_GPRS, false);
	escreveStringService("Verf. Acion. 50%");

	quantidadeDados = aguardaGPRSAction(); //Aguarda receber o pacote
	escreveStringService("Verf. Acion. 80%");

	if(quantidadeDados) {
		escreveStringService("Verf. Acion. 90%");
		limpaBufferEnvioSoquete();
		strcat(bufferEnvioSoquete, "AT+HTTPREAD\r\n"); //Lê os dados recebido
		HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 300);
		aguardaGPRS(TIMEOUT_GPRS, true);
		escreveStringService("Acion. OK");
		leituraAcionamentoService();
	}
}
/*==============================================================================
VERIFICA A QUALIDADE DO SINAL DO GPRS
==============================================================================*/
void verificaQualidadeSinalGPRS() {
	signed int offset = 0;
	limpaBufferEnvioSoquete();
	strcat(bufferEnvioSoquete, "AT+CSQ\r\n");
	HAL_UART_Transmit(&huart6, &bufferEnvioSoquete, strlen(bufferEnvioSoquete), 200);
	aguardaGPRS(TIMEOUT_GPRS, true);

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

		switch(valorSinal) {
			case 0: valorSinal = 115; break;
			case 1: valorSinal = 111; break;
			case 31: valorSinal = 52;
			default: valorSinal = map(valorSinal, 2, 30, 110, 54);
		}

		percentualSinalOperadora = map(valorSinal, 115, 52, 0, 99);
		return;
	}

	percentualSinalOperadora = 0;
	apagaSoqueteBuffer();
}
/*==============================================================================
REINICIA GPRS
==============================================================================*/
void resetGPRS() {
	on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	HAL_Delay(1200);
	off(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
