///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityLoRa
//AUTOR:      Fábio Almeida
//CRIADO:     15/08/2024
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
void aguardaLoRa(uint16_t tempo) {
	contadorTimeoutLoRa = tempo;
	while(contadorTimeoutLoRa) {
		//HAL_UART_Receive_IT(&huart8, &loraDataIn, 1);
	}
	//HAL_UART_Transmit(&huart3, &bufferLoRa, strlen(bufferLoRa), 100);
}
/*=============================================================================
LIMPA BUFFER ENVIO GPRS
==============================================================================*/
void limpaBufferEnvioLoRa() {
	for(uint8_t i = 0; i < TAMANHO_BUFFER_LORA; i ++) {
		bufferEnvioLoRa[i] = 0x00;
	}
}
/*=============================================================================
SPRINTF GPRS
==============================================================================*/
void sprintfLoRa(uint32_t valor, uint8_t qntdDig) {
	//Enviar 0 em qntdDig para ignorar "zeros" a esquerda
	char dataChar[9];
	sprintf(dataChar, "%u", valor);

	switch(qntdDig) {
		case 9:
			if(valor < 100000000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 8:
			if(valor < 10000000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 7:
			if(valor < 1000000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 6:
			if(valor < 100000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 5:
			if(valor < 10000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 4:
			if(valor < 1000) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 3:
			if(valor < 100) {
				strcat(bufferEnvioLoRa, "0");
			}
		case 2:
			if(valor < 10) {
				strcat(bufferEnvioLoRa, "0");
			}
	}

	strcat(bufferEnvioLoRa, dataChar);
}
/*==============================================================================
CONFIGURA LoRa
==============================================================================*/
uint8_t configuraLoRa() {
	on(LORA_M0_GPIO_Port, LORA_M0_Pin);
	on(LORA_M1_GPIO_Port, LORA_M1_Pin);
	HAL_Delay(100);

	if(debounceInverso(LORA_AUX_GPIO_Port, LORA_AUX_Pin)) {
		uint8_t sucesso = false;
		uint8_t ponteiro = 0;
		limpaBufferEnvioLoRa();
		bufferEnvioLoRa[0] = 0xC0; //Salvar
		bufferEnvioLoRa[1] = make8(enderecoLoRaPivot, 1);
		bufferEnvioLoRa[2] = make8(enderecoLoRaPivot, 0);
		bufferEnvioLoRa[3] = 0x1A; //8N1, 9600bps, 2.4k
		bufferEnvioLoRa[4] = canalLoRa;
		bufferEnvioLoRa[5] = 0xC4; //opções
		HAL_UART_Transmit(&huart8, &bufferEnvioLoRa, 6, 100);
		aguardaLoRa(20);

		for(uint8_t i = 0; i < TAMANHO_BUFFER_LORA; i ++) {
			if(bufferLoRa[i] == 0xC0) {
				i = TAMANHO_BUFFER_LORA;
			}
			else {
				ponteiro ++;
			}
		}

		sucesso = true;
		for(uint8_t i = 0; i < 6; i ++) {
			if(bufferLoRa[i + ponteiro] != bufferEnvioLoRa[i]) {
				sucesso = false;
			}
		}

		apagaLoRaBuffer();
		off(LORA_M0_GPIO_Port, LORA_M0_Pin);
		off(LORA_M1_GPIO_Port, LORA_M1_Pin);
		HAL_Delay(100);
		return sucesso;
	}

	off(LORA_M0_GPIO_Port, LORA_M0_Pin);
	off(LORA_M1_GPIO_Port, LORA_M1_Pin);
	HAL_Delay(100);

	return false;
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
