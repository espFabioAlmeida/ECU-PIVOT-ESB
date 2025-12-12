///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTempoPressurizacao
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
TEMPO PRESSURIZAÇÃO
==============================================================================*/
void submenuTempoPressurizacao() {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t deze = tempoPressurizacao / 10, unid = tempoPressurizacao % 10;

	telaAjusteTempoPressurizacao();

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1: deze = incrementaDecrementa(deze, 0, 1, true); break;
				case 2: unid = incrementaDecrementa(unid, 0, 9, true); break;
			}
			escreveDado(deze, 2, 1, 1);
			escreveDado(unid, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1: deze = incrementaDecrementa(deze, 0, 1, false); break;
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
			tempoPressurizacao = deze * 10 + unid;
			if(tempoPressurizacao > TEMPO_MAXIMO_PRESSURIZACAO) {
				tempoPressurizacao = TEMPO_MAXIMO_PRESSURIZACAO;
			}
			if(tempoPressurizacao < 1) {
				tempoPressurizacao = 1;
			}
			writeEepromTempoPressurizacao();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
