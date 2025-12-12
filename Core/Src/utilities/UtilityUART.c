///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityUART
//AUTOR:      Fábio Almeida
//CRIADO:     03/01/2024
//OBSERVAÇÕES:
//USART1 - RS485
//USART3 - DEBUG
//UART4 - WIFI
//UART6 - PLACA SOQUETE (GPRS/4G/LORA GATEWAY)
//UART8 - LORA
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
/*=============================================================================
CONFIGURA VELOCIDADE UART6
==============================================================================*/
void configuraVelocidadeUART6() {
	HAL_UART_Transmit(&huart3, "Configurando UART Soquete\r\n", 27, 200);

	if(placaSoquete == SOQUETE_RADIO) {
		on(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	}
	else {
		off(SOQUETE_OUT1_GPIO_Port, SOQUETE_OUT1_Pin);
	}

	if(placaSoquete == SOQUETE_4G) {
		huart6.Instance = USART6;
		huart6.Init.BaudRate = 115200;
		if(HAL_UART_Init(&huart6) != HAL_OK) {
			Error_Handler();
		}
		HAL_UART_Transmit(&huart3, "Soquete 115200\r\n", 16, 100);
		return;
	}

	huart6.Instance = USART6;
	huart6.Init.BaudRate = 9600;
	if(HAL_UART_Init(&huart6) != HAL_OK) {
		Error_Handler();
	}
	HAL_UART_Transmit(&huart3, "Soquete 9600\r\n", 14, 100);
}
/*=============================================================================
LIMPA BUFFER ENVIO GPRS
==============================================================================*/
void limpaBufferEnvioSoquete() {
	for(uint16_t i = 0; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		bufferEnvioSoquete[i] = 0x00;
	}
}
/*=============================================================================
SPRINTF GPRS
==============================================================================*/
void sprintfSoquete(uint32_t valor, uint8_t qntdDig) {
	//Enviar 0 em qntdDig para ignorar "zeros" a esquerda
	char dataChar[9];
	sprintf(dataChar, "%u", valor);

	switch(qntdDig) {
		case 9:
			if(valor < 100000000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 8:
			if(valor < 10000000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 7:
			if(valor < 1000000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 6:
			if(valor < 100000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 5:
			if(valor < 10000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 4:
			if(valor < 1000) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 3:
			if(valor < 100) {
				strcat(bufferEnvioSoquete, "0");
			}
		case 2:
			if(valor < 10) {
				strcat(bufferEnvioSoquete, "0");
			}
	}

	strcat(bufferEnvioSoquete, dataChar);
}
/*==============================================================================
APAGA BUFFER LORA
==============================================================================*/
void apagaLoRaBuffer() {
	contadorLoRaBuffer = 0;
	for(uint8_t i = 0; i < TAMANHO_BUFFER_LORA; i ++) {
		bufferLoRa[i] = 0;
	}
}
/*==============================================================================
APAGA BUFFER SOQUETE
==============================================================================*/
void apagaSoqueteBuffer() {
	soqueteDataIn = 0x00;
	contadorSoqueteBuffer = 0;
	for(uint16_t i = 0; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		bufferSoquete[i] = 0;
	}
}
/*==============================================================================
APAGA BUFFER RS485
==============================================================================*/
void apagaRS485Buffer() {
	rs485DataIn = 0x00;
	contadorRS485Buffer = 0;
	for(uint8_t i = 0; i < TAMANHO_BUFFER_RS485; i ++) {
		bufferRS485[i] = 0;
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
