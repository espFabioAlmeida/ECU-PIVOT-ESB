///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTempoBasePercentimetro
//AUTOR:      Fábio Almeida
//CIRADO:     05/11/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
TEMPO BASE PERCENTIMETRO
==============================================================================*/
void submenuTempoBasePercentimetro() {
	uint8_t flagSubmenu = true, display = 1;
	uint16_t tempo = tempoBaseLaminaDagua / 10;
	uint8_t deze = tempo / 10, unid = tempo % 10;

	telaAjusteTempoPercentimetro(tempo);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1: deze = incrementaDecrementa(deze, 0, 6, true); break;
				case 2: unid = incrementaDecrementa(unid, 0, 9, true); break;
			}
			escreveDado(deze, 2, 1, 1);
			escreveDado(unid, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1: deze = incrementaDecrementa(deze, 0, 6, false); break;
				case 2: unid = incrementaDecrementa(unid, 0, 9, false); break;
			}
			escreveDado(deze, 2, 1, 1);
			escreveDado(unid, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 2) {
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
			tempo = deze * 10 + unid;
			if(tempo > TEMPO_MAXIMO_PERCENTIMETRO) {
				tempo = TEMPO_MAXIMO_PERCENTIMETRO;
			}
			if(tempo < TEMPO_MINIMO_PERCENTIMETRO) {
				tempo = TEMPO_MINIMO_PERCENTIMETRO;
			}
			tempoBaseLaminaDagua = tempo * 10;
			writeEepromTempoBaseLaminaDagua();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
