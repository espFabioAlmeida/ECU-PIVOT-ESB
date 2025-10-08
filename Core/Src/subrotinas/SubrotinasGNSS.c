///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasGNSS
//AUTOR:      Fábio Almeida
//CRIADO:     02/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
/*==============================================================================
ZERA POSICAO GNSS
==============================================================================*/
void zeraPosicaoGNSS() {
	posicaoAtualGPS.latitudeGraus = 0;
	posicaoAtualGPS.latitudeMinutos = 0;
	posicaoAtualGPS.latitudeSegundos = 0;
	posicaoAtualGPS.hemisferio = 'S';
	posicaoAtualGPS.longitudeGraus = 0;
	posicaoAtualGPS.longitudeMinutos = 0;
	posicaoAtualGPS.longitudeSegundos = 0;
	posicaoAtualGPS.indicadorWE = 'W';
}
/*==============================================================================
RECEBE POSICAO GNSS
==============================================================================*/
void recebePosicaoGNSS(signed int offset, char buffer[]) {
	CoordenadasTypeDef posicaoRecebida;

	posicaoRecebida.latitudeGraus = charToByte(buffer[offset + 21]) * 10;
	posicaoRecebida.latitudeGraus += charToByte(buffer[offset + 22]);
	posicaoRecebida.latitudeMinutos = charToByte(buffer[offset + 23]) * 10;
	posicaoRecebida.latitudeMinutos += charToByte(buffer[offset + 24]);

	posicaoRecebida.latitudeSegundos = 0;
	for(uint8_t i = 0; i < 5; i ++) {
		posicaoRecebida.latitudeSegundos *= 10;
		posicaoRecebida.latitudeSegundos += charToByte(buffer[offset + 26 + i]);
	}
	posicaoRecebida.hemisferio = buffer[offset + 32];

	posicaoRecebida.longitudeGraus = charToByte(buffer[offset + 34]) * 100;
	posicaoRecebida.longitudeGraus += charToByte(buffer[offset + 35]) * 10;
	posicaoRecebida.longitudeGraus += charToByte(buffer[offset + 36]);
	posicaoRecebida.longitudeMinutos = charToByte(buffer[offset + 37]) * 10;
	posicaoRecebida.longitudeMinutos += charToByte(buffer[offset + 38]);

	posicaoRecebida.longitudeSegundos = 0;
	for(uint8_t i = 0; i < 5; i ++) {
		posicaoRecebida.longitudeSegundos *= 10;
		posicaoRecebida.longitudeSegundos += charToByte(buffer[offset + 40 + i]);
	}
	posicaoRecebida.indicadorWE = buffer[offset + 46];

	if(buffer[offset + 48] == '1') {
		salvaLogBarricadaGPS();
		flagBarricadaGPS = true;
	}
	else {
		flagBarricadaGPS = false;
	}

	//Debug
	/*
	posicaoRecebida.latitudeGraus = 26;
	posicaoRecebida.latitudeMinutos = 52;
	posicaoRecebida.latitudeSegundos = 49429;
	posicaoRecebida.hemisferio = 'S';
	posicaoRecebida.longitudeGraus = 49;
	posicaoRecebida.longitudeMinutos = 4;
	posicaoRecebida.longitudeSegundos = 30907;
	posicaoRecebida.indicadorWE = 'W';
	*/

	if(!posicaoRecebida.latitudeGraus && !posicaoRecebida.latitudeMinutos && !posicaoRecebida.latitudeSegundos &&
			!posicaoRecebida.longitudeGraus && !posicaoRecebida.longitudeMinutos && !posicaoRecebida.longitudeSegundos) {
		//Ignora posição zerada, matém última válida
		return;
	}

	posicaoAtualGPS = posicaoRecebida;
	flagLeituraGNSSRecente = true;
	contadorTimeoutGNSSRecente = 0;
	writeEepromPosicaoAtualGPS();
}
/*==============================================================================
BUSCA POSICAO OBSTACULO OU HOME
==============================================================================*/
void buscaPosicaoObstaculo() {
	contadorAguardaPosicaoObstaculo = TEMPO_AGUARDA_POSICAO_OBSTACULO;
	solicitaGNSS();
	while(contadorAguardaPosicaoObstaculo) {
		if(comunicacaoGNSS == GNSS_485) {
			//HAL_UART_Receive_IT(&huart4, &rs485DataIn, 1);
			if(flagPacoteGNSSRS485) {
				flagPacoteGNSSRS485 = false;
				contadorAguardaPosicaoObstaculo = 0;
				recebeGNSSRS485();
			}
		}
		else {
			//HAL_UART_Receive_IT(&huart2, &loraDataIn, 1);
			if(flagPacoteGNSSLoRa) {
				flagPacoteGNSSLoRa = false;
				contadorAguardaPosicaoObstaculo = 0;
				recebeGNSSLoRa();
			}
		}
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
