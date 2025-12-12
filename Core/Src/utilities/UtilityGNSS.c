///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityGNSS
//AUTOR:      Fábio Almeida
//CRIADO:     02/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TIMEOUT_GNSS 65
#define TIMEOUT_CONFIGURA_GNSS 30
/*==============================================================================
SOLICITA DADOS GNSS PELA LORA
==============================================================================*/
void solicitaGNSSLoRa() {
	limpaBufferEnvioLoRa();
	bufferEnvioLoRa[0] = make8(enderecoLoRaGNSS, 1);
	bufferEnvioLoRa[1] = make8(enderecoLoRaGNSS, 0);
	bufferEnvioLoRa[2] = canalLoRa;

	strcat(bufferEnvioLoRa, "$,030,1,\r\n");

	HAL_UART_Transmit(&huart8, &bufferEnvioLoRa, strlen(bufferEnvioLoRa), 100);

	if(flagSucessoLoRa) { //Verifica se o pacote retorou com sucesso
		if(qualidadeSinalLora < 5) {
			qualidadeSinalLora ++;
		}
	}
	else {
		if(qualidadeSinalLora) {
			qualidadeSinalLora --;
		}
	}

	flagSucessoLoRa = false; //reseta para verificar novamente
	contadorTimeoutGNSS = TIMEOUT_GNSS;
}
/*==============================================================================
SOLICITA DADOS GNSS PELA RS485
==============================================================================*/
void solicitaGNSSRS485() {
	HAL_UART_Transmit(&huart1, "$,030,1,\r\n", 10, 100);
}
/*==============================================================================
SOLICITA DADOS GNSS
==============================================================================*/
void solicitaGNSS() {

	if(comunicacaoGNSS == GNSS_485) {
		solicitaGNSSRS485();
	}
	else {
		solicitaGNSSLoRa();
	}

	contadorErroGNSS ++;
	if(contadorErroGNSS > QUANTIDADE_ERRO_GNSS) {
		contadorErroGNSS = QUANTIDADE_ERRO_GNSS;
	}
}
/*==============================================================================
RECEBE CONFIGURACAO LORA RS485
==============================================================================*/
uint8_t recebeConfigGNSSRS485(uint8_t comando) {
	signed int offset = -1;

	if(comando == 2) {
		offset = lastIndexOf(bufferRS485, "$,030,2,");

		if(bufferRS485[offset + 16] == 0x0D || bufferRS485[offset + 16] == 0x0A) { //Leu apenas o echo
			apagaRS485Buffer();
			return false;
		}
	}
	else if(comando == 3) {
		offset = lastIndexOf(bufferRS485, "$,030,3,");

		if(bufferRS485[offset + 8] == 0x0D || bufferRS485[offset + 8] == 0x0A) { //Leu apenas o echo
			apagaRS485Buffer();
			return false;
		}
	}
	else {
		apagaRS485Buffer();
		return false;
	}

	if(offset >= 0) {
		flagLedCOM = true;

		for(uint8_t i = 0; i < 5; i ++) {
			enderecoLoRaRecebidoPivot[i] = 0x00;
			enderecoLoRaRecebidoGNSS[i] = 0x00;
			canalLoRaRecebido[i] = 0x00;
		}

		offset += 8;

		for(uint8_t i = 0; i < 4; i ++) {
			enderecoLoRaRecebidoPivot[i] = bufferRS485[offset + i];
			enderecoLoRaRecebidoGNSS[i] = bufferRS485[offset + i + 8];
		}

		for(uint8_t i = 0; i < 2; i ++) {
			canalLoRaRecebido[i] = bufferRS485[offset + i + 5];
		}

		apagaRS485Buffer();
		return true;
	}

	apagaRS485Buffer();
	return false;
}
/*==============================================================================
CONFIGURA LoRa DO GNSS VIA RS485
==============================================================================*/
uint8_t configuraLoRaGNSS() {
	char dadoHexa[5];
	uint8_t hi = make8(enderecoLoRaPivot, 1), lo = make8(enderecoLoRaPivot, 0);
	uint8_t nibbleSuperiorHi = hi & 0xF0, nibbleInferiorHi = hi & 0x0F;
	uint8_t nibbleSuperiorLo = lo & 0xF0, nibbleInferiorLo = 0; //sempre zero no pivot
	nibbleSuperiorHi >>= 4;
	nibbleSuperiorLo >>= 4;

	dadoHexa[0] = serializer(nibbleSuperiorHi);
	dadoHexa[1] = serializer(nibbleInferiorHi);
	dadoHexa[2] = serializer(nibbleSuperiorLo);
	dadoHexa[3] = serializer(nibbleInferiorLo);
	dadoHexa[4] = 0x00;

	limpaBufferEnvioLoRa();
	strcat(bufferEnvioLoRa, "$,030,2,");
	strcat(bufferEnvioLoRa, dadoHexa);
	strcat(bufferEnvioLoRa, ",");

	nibbleSuperiorLo = canalLoRa & 0xF0;
	nibbleInferiorLo = canalLoRa & 0x0F;
	nibbleSuperiorLo >>= 4;

	dadoHexa[0] = serializer(nibbleSuperiorLo);
	dadoHexa[1] = serializer(nibbleInferiorLo);
	dadoHexa[2] = 0x00;
	dadoHexa[3] = 0x00;
	dadoHexa[4] = 0x00;
	strcat(bufferEnvioLoRa, dadoHexa);
	strcat(bufferEnvioLoRa, ",\r\n");

	HAL_UART_Transmit(&huart1, bufferEnvioLoRa, strlen(bufferEnvioLoRa), 100);

	contadorTimeoutConfiguraGNSS = TIMEOUT_CONFIGURA_GNSS;

	while(contadorTimeoutConfiguraGNSS) {
	}

	if(flagPacoteGNSSRS485) {
		flagPacoteGNSSRS485 = false;
		if(recebeConfigGNSSRS485(2)) {
			contadorTimeoutConfiguraGNSS = 0;
			return true;
		}
	}

	return false;
}
/*==============================================================================
SOLICITA CONFIGURAÇÕES LoRa DO GNSS VIA RS485
==============================================================================*/
uint8_t solicitaConfiguracoesLoRaGNSS() {
	HAL_UART_Transmit(&huart1, "$,030,3,\r\n", 10, 100);

	contadorTimeoutConfiguraGNSS = TIMEOUT_CONFIGURA_GNSS;

	while(contadorTimeoutConfiguraGNSS) {
	}

	if(flagPacoteGNSSRS485) {
		flagPacoteGNSSRS485 = false;
		if(recebeConfigGNSSRS485(3)) {
			contadorTimeoutConfiguraGNSS = 0;
			return true;
		}
	}

	return false;
}
/*==============================================================================
RECEBE DADOS GNSS PELA RS485
==============================================================================*/
void recebeGNSSRS485() {
	signed int offset = lastIndexOf(bufferRS485, "$,030,1,");

	if(offset >= 0) {
		contadorErroGNSS = 0;
		flagLedCOM = true;
		recebePosicaoGNSS(offset, bufferRS485);
	}

	apagaRS485Buffer();
}
/*==============================================================================
RECEBE DADOS GNSS PELA LORA
==============================================================================*/
void recebeGNSSLoRa() {
	signed int offset = indexOf(bufferLoRa, "$,030,1,");

	if(offset >= 0) {
		flagSucessoLoRa = true;
		contadorErroGNSS = 0;
		flagLedCOM = true;
		recebePosicaoGNSS(offset, bufferLoRa);
		contadorTimeoutGNSS = 0;
	}

	apagaLoRaBuffer();
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
