///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAlarmePressao
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE ALARME PRESSAO
==============================================================================*/
void submenuAjusteAlarmePressao() {
	uint8_t flagSubmenu = true, display = 9, linha = 1;
	uint8_t cent = alarmePressao / 100, deze = alarmePressao / 10, unid = alarmePressao % 10;
	uint8_t dezeHisterese = histereseAlarmePressao / 10, unidHisterese = histereseAlarmePressao % 10;

	if(cent >= 10) {
		cent %= 10;
	}
	if(deze >= 10) {
		deze %= 10;
	}

	telaAjusteAlarmePressao();

	while(flagSubmenu) {
		if(botaoUp()) {
			if(linha == 1) {
				switch(display) {
					case 9: cent = incrementaDecrementa(cent, 0, 1, true); break;
					case 10: deze = incrementaDecrementa(deze, 0, 9, true); break;
					case 12: unid = incrementaDecrementa(unid, 0, 9, true); break;
				}
			}
			else {
				switch(display) {
					case 10: dezeHisterese = incrementaDecrementa(dezeHisterese, 0, 5, true); break;
					case 12: unidHisterese = incrementaDecrementa(unidHisterese, 0, 9, true); break;
				}
			}

			escreveDado(cent, 1, 9, 1);
			escreveDado(deze, 0, 0, 1);
			escreveDado(unid, 1, 12, 1);
			escreveDado(dezeHisterese, 2, 10, 1);
			escreveDado(unidHisterese, 2, 12, 1);
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			if(linha == 1) {
				switch(display) {
					case 9: cent = incrementaDecrementa(cent, 0, 1, false); break;
					case 10: deze = incrementaDecrementa(deze, 0, 9, false); break;
					case 12: unid = incrementaDecrementa(unid, 0, 9, false); break;
				}
			}
			else {
				switch(display) {
					case 10: dezeHisterese = incrementaDecrementa(dezeHisterese, 0, 5, false); break;
					case 12: unidHisterese = incrementaDecrementa(unidHisterese, 0, 9, false); break;
				}
			}

			escreveDado(cent, 1, 9, 1);
			escreveDado(deze, 0, 0, 1);
			escreveDado(unid, 1, 12, 1);
			escreveDado(dezeHisterese, 2, 10, 1);
			escreveDado(unidHisterese, 2, 12, 1);
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(linha == 1) {
				switch(display) {
					case 9: display = 10; break;
					case 10: display = 12; break;
					case 12: display = 10; linha = 2; break;
				}
			}
			else {
				if(display == 10) {
					display = 12;
				}
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(linha == 1) {
				switch(display) {
					case 10: display = 9; break;
					case 12: display = 10; break;
				}
			}
			else {
				switch(display) {
					case 10: display = 12; linha = 1; break;
					case 12: display = 10; break;
				}
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			alarmePressao = cent * 100 + deze * 10 + unid;

			if(tipoSensorPressao == SENSOR_10BAR) {
				if(alarmePressao > 100) {
					alarmePressao = 100;
				}
			}
			if(tipoSensorPressao == SENSOR_16BAR) {
				if(alarmePressao > 160) {
					alarmePressao = 160;
				}
			}

			histereseAlarmePressao = dezeHisterese * 10 + unidHisterese;

			writeEepromAlarmePressao();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
