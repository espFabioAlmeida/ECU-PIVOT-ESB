///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasEntradasAnalogicas
//AUTOR:      Fábio Almeida
//CIRADO:     25/04/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define ADC_MIN_PRESSAO 819
#define GAP_ADC_PRESSAO 3277 //4096-819
/*=============================================================================
LEITURA TRANSDUTOR DE PRESSAO
==============================================================================*/
void leituraTransdutorPressao() {
	static uint16_t bufferPressao[4];
	uint32_t calculo = 0;

	for(uint8_t i = 0; i < 3; i ++) {
		bufferPressao[i] = bufferPressao[i + 1];
	}
	bufferPressao[3] = valorAdc[1];

	for(uint8_t i = 0; i < 4; i ++) {
		calculo += bufferPressao[i];
	}
	calculo /= 4;

	if(calculo < 819) {
		pressao = 0;
		return;
	}

	calculo -= ADC_MIN_PRESSAO;
	calculo *= (MAXIMO_PRESSAO * 10);
	calculo /= GAP_ADC_PRESSAO;

	pressao = calculo;
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
