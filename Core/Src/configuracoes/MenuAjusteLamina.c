///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAjusteDensidade
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE DENSIDADE
==============================================================================*/
void submenuAjusteLamina() {
	uint8_t flagSubmenu = true, display = 1;
	uint16_t dmilh = laminaDagua / 10000, milh = laminaDagua / 1000, cent = laminaDagua / 100,
			deze = laminaDagua / 10, unid = laminaDagua % 10;
	if(dmilh >= 10) {
		dmilh %= 10;
	}
	if(milh >= 10) {
		milh %= 10;
	}
	if(cent >= 10) {
		cent %= 10;
	}
	if(deze >= 10) {
		deze %= 10;
	}

	telaAjusteLaminaDagua();

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1: dmilh = incrementaDecrementa(dmilh, 0, 6, true); break;
				case 2: milh = incrementaDecrementa(milh, 0, 9, true); break;
				case 3: cent = incrementaDecrementa(cent, 0, 9, true); break;
				case 5: deze = incrementaDecrementa(deze, 0, 9, true); break;
				case 6: unid = incrementaDecrementa(unid, 0, 9, true); break;
			}
			escreveDado(dmilh, 2, 1, 1);
			escreveDado(milh, 0, 0, 1);
			escreveDado(cent, 0, 0, 1);
			escreveDado(deze, 2, 5, 1);
			escreveDado(unid, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1: dmilh = incrementaDecrementa(dmilh, 0, 6, false); break;
				case 2: milh = incrementaDecrementa(milh, 0, 9, false); break;
				case 3: cent = incrementaDecrementa(cent, 0, 9, false); break;
				case 5: deze = incrementaDecrementa(deze, 0, 9, false); break;
				case 6: unid = incrementaDecrementa(unid, 0, 9, false); break;
			}
			escreveDado(dmilh, 2, 1, 1);
			escreveDado(milh, 0, 0, 1);
			escreveDado(cent, 0, 0, 1);
			escreveDado(deze, 2, 5, 1);
			escreveDado(unid, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 6) {
				display ++;
				if(display == 4) {
					display = 5;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(display > 1) {
				display --;
				if(display == 4) {
					display = 3;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			uint32_t valor = dmilh * 10000 + milh * 1000 + cent * 100 + deze * 10 + unid;
			while(botaoOk());
			flagSubmenu = false;

			if(valor > VALOR_MAXIMO_LAMINA_MM) {
				valor = VALOR_MAXIMO_LAMINA_MM;
			}
			laminaDagua = valor;

			if(!laminaDagua) {
				laminaDagua = 100;
			}

			writeEepromLamina();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
