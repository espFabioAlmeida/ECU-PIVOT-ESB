///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTrocaSenha
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE DENSIDADE
==============================================================================*/
void submenuTrocaSenha() {
	uint8_t flagSubmenu = true, display = 1;
	uint16_t milh = 0, cent = 0, deze = 0, unid = 0;

	telaTrocaSenha();
	lcdPosLc(2, 1);
	lcdCursor(true);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1:
					milh = incrementaDecrementa(milh, 0, 9, true);
					escreveDado(milh, 0, 0, 1);
					break;
				case 2:
					cent = incrementaDecrementa(cent, 0, 9, true);
					escreveDado(cent, 0, 0, 1);
					break;
				case 3:
					deze = incrementaDecrementa(deze, 0, 9, true);
					escreveDado(deze, 0, 0, 1);
					break;
				case 4:
					unid = incrementaDecrementa(unid, 0, 9, true);
					escreveDado(unid, 0, 0, 1);
					break;
			}
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1:
					milh = incrementaDecrementa(milh, 0, 9, false);
					escreveDado(milh, 0, 0, 1);
					break;
				case 2:
					cent = incrementaDecrementa(cent, 0, 9, false);
					escreveDado(cent, 0, 0, 1);
					break;
				case 3:
					deze = incrementaDecrementa(deze, 0, 9, false);
					escreveDado(deze, 0, 0, 1);
					break;
				case 4:
					unid = incrementaDecrementa(unid, 0, 9, false);
					escreveDado(unid, 0, 0, 1);
					break;
			}
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 4) {
				display ++;
				lcdEscreve("*");
				switch(display) {
					case 1: escreveDado(milh, 0, 0, 1); break;
					case 2: escreveDado(cent, 0, 0, 1); break;
					case 3: escreveDado(deze, 0, 0, 1); break;
					case 4: escreveDado(unid, 0, 0, 1); break;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(display > 1) {
				display --;
				lcdEscreve("*");
				lcdPosLc(2, display);
				switch(display) {
					case 1: escreveDado(milh, 0, 0, 1); break;
					case 2: escreveDado(cent, 0, 0, 1); break;
					case 3: escreveDado(deze, 0, 0, 1); break;
					case 4: escreveDado(unid, 0, 0, 1); break;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			senha = milh * 1000 + cent * 100 + deze * 10 + unid;
			writeEepromSenha();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
