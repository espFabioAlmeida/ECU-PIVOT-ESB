///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAjusteHorarioPonta
//AUTOR:      Fábio Almeida
//CIRADO:     25/07/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE HORARIO PONTA
==============================================================================*/
void submenuAjusteHorarioPonta() {
	uint8_t flagSubmenu = true, display = 9, linha = 1;
	uint8_t dezeHoraInicio, unidHoraInicio, dezeMinutoInicio, unidMinutoInicio;
	uint8_t dezeHoraFim, unidHoraFim, dezeMinutoFim, unidMinutoFim;

	dezeHoraInicio = horarioInicioPonta.Hours / 10;
	unidHoraInicio = horarioInicioPonta.Hours % 10;
	dezeMinutoInicio = horarioInicioPonta.Minutes / 10;
	unidMinutoInicio = horarioInicioPonta.Minutes % 10;

	dezeHoraFim = horarioFimPonta.Hours / 10;
	unidHoraFim = horarioFimPonta.Hours % 10;
	dezeMinutoFim = horarioFimPonta.Minutes / 10;
	unidMinutoFim = horarioFimPonta.Minutes % 10;

	telaAjusteHorarioPonta();

	while(flagSubmenu) {
		if(botaoUp()) {
			if(linha == 1) {
				switch(display) {
					case 9:
						dezeHoraInicio = incrementaDecrementa(dezeHoraInicio, 0, 2, true);
						if(dezeHoraInicio == 2 && unidHoraInicio > 3) {
							unidHoraInicio = 3;
						}
						break;

					case 10:
						unidHoraInicio = incrementaDecrementa(unidHoraInicio, 0, 9, true);
						if(dezeHoraInicio == 2 && unidHoraInicio > 3) {
							unidHoraInicio = 0;
						}
						break;

					case 12: dezeMinutoInicio = incrementaDecrementa(dezeMinutoInicio, 0, 5, true); break;
					case 13: unidMinutoInicio = incrementaDecrementa(unidMinutoInicio, 0, 9, true); break;
				}
			}
			if(linha == 2) {
				switch(display) {
					case 6:
						dezeHoraFim = incrementaDecrementa(dezeHoraFim, 0, 2, true);
						if(dezeHoraFim == 2 && unidHoraFim > 3) {
							unidHoraFim = 3;
						}
						break;

					case 7:
						unidHoraFim = incrementaDecrementa(unidHoraFim, 0, 9, true);
						if(dezeHoraFim == 2 && unidHoraFim > 3) {
							unidHoraFim = 0;
						}
						break;

					case 9: dezeMinutoFim = incrementaDecrementa(dezeMinutoFim, 0, 5, true); break;
					case 10: unidMinutoFim = incrementaDecrementa(unidMinutoFim, 0, 9, true); break;
				}
			}
			escreveDado(dezeHoraInicio, 1, 9, 1);
			escreveDado(unidHoraInicio, 0, 0, 1);
			escreveDado(dezeMinutoInicio, 1, 12, 1);
			escreveDado(unidMinutoInicio, 0, 0, 1);

			escreveDado(dezeHoraFim, 2, 6, 1);
			escreveDado(unidHoraFim, 0, 0, 1);
			escreveDado(dezeMinutoFim, 2, 9, 1);
			escreveDado(unidMinutoFim, 0, 0, 1);

			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			if(linha == 1) {
				switch(display) {
					case 9:
						dezeHoraInicio = incrementaDecrementa(dezeHoraInicio, 0, 2, false);
						if(dezeHoraInicio == 2 && unidHoraInicio > 3) {
							unidHoraInicio = 3;
						}
						break;

					case 10:
						unidHoraInicio = incrementaDecrementa(unidHoraInicio, 0, 9, false);
						if(dezeHoraInicio == 2 && unidHoraInicio > 3) {
							unidHoraInicio = 3;
						}
						break;

					case 12: dezeMinutoInicio = incrementaDecrementa(dezeMinutoInicio, 0, 5, false); break;
					case 13: unidMinutoInicio = incrementaDecrementa(unidMinutoInicio, 0, 9, false); break;
				}
			}
			if(linha == 2) {
				switch(display) {
					case 6:
						dezeHoraFim = incrementaDecrementa(dezeHoraFim, 0, 2, false);
						if(dezeHoraFim == 2 && unidHoraFim > 3) {
							unidHoraFim = 3;
						}
						break;

					case 7:
						unidHoraFim = incrementaDecrementa(unidHoraFim, 0, 9, false);
						if(dezeHoraFim == 2 && unidHoraFim > 3) {
							unidHoraFim = 3;
						}
						break;

					case 9: dezeMinutoFim = incrementaDecrementa(dezeMinutoFim, 0, 5, false); break;
					case 10: unidMinutoFim = incrementaDecrementa(unidMinutoFim, 0, 9, false); break;
				}
			}
			escreveDado(dezeHoraInicio, 1, 9, 1);
			escreveDado(unidHoraInicio, 0, 0, 1);
			escreveDado(dezeMinutoInicio, 1, 12, 1);
			escreveDado(unidMinutoInicio, 0, 0, 1);

			escreveDado(dezeHoraFim, 2, 6, 1);
			escreveDado(unidHoraFim, 0, 0, 1);
			escreveDado(dezeMinutoFim, 2, 9, 1);
			escreveDado(unidMinutoFim, 0, 0, 1);

			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			display ++;
			if(linha == 1) {
				switch(display) {
					case 11: display = 12; break;
					case 14: display = 6; linha = 2; break;
				}
			}
			else {
				switch(display) {
					case 8: display = 9; break;
					case 11: display = 10; break;
				}
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			display --;
			if(linha == 1) {
				switch(display) {
					case 8: display = 9; break;
					case 11: display = 10; break;
				}
			}
			else {
				switch(display) {
					case 5: display = 13; linha = 1; break;
					case 8: display = 7; break;
				}
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;

			horarioInicioPonta.Hours = dezeHoraInicio * 10 + unidHoraInicio;
			horarioInicioPonta.Minutes = dezeMinutoInicio * 10 + unidMinutoInicio;
			horarioFimPonta.Hours = dezeHoraFim * 10 + unidHoraFim;
			horarioFimPonta.Minutes = dezeMinutoFim * 10 + unidMinutoFim;
			writeEepromHorarioPonta();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
