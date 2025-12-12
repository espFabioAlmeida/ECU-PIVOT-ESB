///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTrocaNumeroSerial
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
TROCA NUMERO SERIAL
==============================================================================*/
void submenuTrocaNumeroSerial() {
	uint8_t flagSubmenu = true, display = 1;
	uint32_t casaDecimal[9];

	casaDecimal[8] = numeroSerial / 100000000;
	casaDecimal[7] = numeroSerial / 10000000;
	casaDecimal[6] = numeroSerial / 1000000;
	casaDecimal[5] = numeroSerial / 100000;
	casaDecimal[4] = numeroSerial / 10000;
	casaDecimal[3] = numeroSerial / 1000;
	casaDecimal[2] = numeroSerial / 100;
	casaDecimal[1] = numeroSerial / 10;
	casaDecimal[0] = numeroSerial % 10;

	for(uint8_t i = 1; i < 9; i ++) {
		if(casaDecimal[i] > 9) {
			casaDecimal[i] %= 10;
		}
	}

	telaTrocaNumeroSerial();

	while(flagSubmenu) {
		if(botaoUp()) {
			casaDecimal[9 - display] = incrementaDecrementa(casaDecimal[9 - display], 0, 9, true);
			escreveDado(casaDecimal[9 - display], 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			casaDecimal[9 - display] = incrementaDecrementa(casaDecimal[9 - display], 0, 9, false);
			escreveDado(casaDecimal[9 - display], 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 9) {
				display ++;
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(display > 1) {
				display --;
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			numeroSerial = 0;
			for(uint8_t i = 0; i < 9; i ++) {
				numeroSerial *= 10;
				numeroSerial += casaDecimal[8 - i];
			}
			writeEepromNumeroSerial();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
