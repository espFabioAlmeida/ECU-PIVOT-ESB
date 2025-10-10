///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAjusteHorario
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE HORARIO
==============================================================================*/
void submenuAjusteHorario() {
	uint8_t flagSubmenu = true, display = 1, linha = 1;
	uint8_t dezeDia, unidDia, dezeMes, unidMes, dezeAno, unidAno;
	uint8_t dezeHora, unidHora, dezeMinuto, unidMinuto, dezeSegundo, unidSegundo;
	uint8_t diaSemana = RTC_WEEKDAY_MONDAY;

	readRTC();

	dezeDia = sDate.Date / 10;
	unidDia = sDate.Date % 10;
	dezeMes = sDate.Month / 10;
	unidMes = sDate.Month % 10;
	dezeAno = sDate.Year / 10;
	unidAno = sDate.Year % 10;
	diaSemana = sDate.WeekDay;

	dezeHora = sTime.Hours / 10;
	unidHora = sTime.Hours % 10;
	dezeMinuto = sTime.Minutes / 10;
	unidMinuto = sTime.Minutes % 10;
	dezeSegundo = sTime.Seconds / 10;
	unidSegundo = sTime.Seconds % 10;


	telaAjusteHorario();

	while(flagSubmenu) {
		if(botaoUp()) {
			if(linha == 1) {
				switch(display) {
					case 1:
						dezeHora = incrementaDecrementa(dezeHora, 0, 2, true);
						if(dezeHora == 2 && unidHora > 3) {
							unidHora = 3;
						}
						break;

					case 2:
						unidHora = incrementaDecrementa(unidHora, 0, 9, true);
						if(dezeHora == 2 && unidHora > 3) {
							unidHora = 0;
						}
						break;

					case 4: dezeMinuto = incrementaDecrementa(dezeMinuto, 0, 5, true); break;
					case 5: unidMinuto = incrementaDecrementa(unidMinuto, 0, 9, true); break;

					case 7: dezeSegundo = incrementaDecrementa(dezeSegundo, 0, 5, true); break;
					case 8: unidSegundo = incrementaDecrementa(unidSegundo, 0, 9, true); break;
				}
			}
			if(linha == 2) {
				switch(display) {
					case 1:
						dezeDia = incrementaDecrementa(dezeDia, 0, 3, true);
						if(dezeDia == 3 && unidDia > 1) {
							unidDia = 1;
						}
						if(!dezeDia && !unidDia) {
							unidDia = 1;
						}
						break;

					case 2:
						unidDia = incrementaDecrementa(unidDia, 0, 9, true);
						if(dezeDia == 3 && unidDia > 1) {
							unidDia = 0;
						}
						if(!dezeDia && !unidDia) {
							unidDia = 1;
						}
						break;

					case 4:
						dezeMes = incrementaDecrementa(dezeMes, 0, 1, true);
						if(dezeMes == 1 && unidMes > 2) {
							unidMes = 2;
						}
						if(!dezeMes && !unidMes) {
							unidMes = 1;
						}
						break;

					case 5:
						unidMes = incrementaDecrementa(unidMes, 0, 9, true);
						if(dezeMes == 1 && unidMes > 2) {
							unidMes = 0;
						}
						if(!dezeMes && !unidMes) {
							unidMes = 1;
						}
						break;

					case 7: dezeAno = incrementaDecrementa(dezeAno, 0, 9, true); break;
					case 8: unidAno = incrementaDecrementa(unidAno, 0, 9, true); break;
				}
			}
			escreveDado(dezeHora, 1, 1, 1);
			escreveDado(unidHora, 0, 0, 1);
			escreveDado(dezeMinuto, 1, 4, 1);
			escreveDado(unidMinuto, 0, 0, 1);
			escreveDado(dezeSegundo, 1, 7, 1);
			escreveDado(unidSegundo, 0, 0, 1);

			escreveDado(dezeDia, 2, 1, 1);
			escreveDado(unidDia, 0, 0, 1);
			escreveDado(dezeMes, 2, 4, 1);
			escreveDado(unidMes, 0, 0, 1);
			escreveDado(dezeAno, 2, 7, 1);
			escreveDado(unidAno, 0, 0, 1);

			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			if(linha == 1) {
				switch(display) {
					case 1:
						dezeHora = incrementaDecrementa(dezeHora, 0, 2, false);
						if(dezeHora == 2 && unidHora > 3) {
							unidHora = 3;
						}
						break;

					case 2:
						unidHora = incrementaDecrementa(unidHora, 0, 9, false);
						if(dezeHora == 2 && unidHora > 3) {
							unidHora = 3;
						}
						break;

					case 4: dezeMinuto = incrementaDecrementa(dezeMinuto, 0, 5, false); break;
					case 5: unidMinuto = incrementaDecrementa(unidMinuto, 0, 9, false); break;

					case 7: dezeSegundo = incrementaDecrementa(dezeSegundo, 0, 5, false); break;
					case 8: unidSegundo = incrementaDecrementa(unidSegundo, 0, 9, false); break;
				}
			}
			if(linha == 2) {
				switch(display) {
					case 1:
						dezeDia = incrementaDecrementa(dezeDia, 0, 3, false);
						if(dezeDia == 3 && unidDia > 1) {
							unidDia = 1;
						}
						if(!dezeDia && !unidDia) {
							unidDia = 1;
						}
						break;

					case 2:
						unidDia = incrementaDecrementa(unidDia, 0, 9, false);
						if(dezeDia == 3 && unidDia > 1) {
							unidDia = 1;
						}
						if(!dezeDia && !unidDia) {
							unidDia = 9;
						}
						break;

					case 4:
						dezeMes = incrementaDecrementa(dezeMes, 0, 1, false);
						if(dezeMes == 1 && unidMes > 2) {
							unidMes = 2;
						}
						if(!dezeMes && !unidMes) {
							unidMes = 1;
						}
						break;

					case 5:
						unidMes = incrementaDecrementa(unidMes, 0, 9, false);
						if(dezeMes == 1 && unidMes > 2) {
							unidMes = 2;
						}
						if(!dezeMes && !unidMes) {
							unidMes = 9;
						}
						break;

					case 7: dezeAno = incrementaDecrementa(dezeAno, 0, 9, false); break;
					case 8: unidAno = incrementaDecrementa(unidAno, 0, 9, false); break;
				}
			}
			escreveDado(dezeHora, 1, 1, 1);
			escreveDado(unidHora, 0, 0, 1);
			escreveDado(dezeMinuto, 1, 4, 1);
			escreveDado(unidMinuto, 0, 0, 1);
			escreveDado(dezeSegundo, 1, 7, 1);
			escreveDado(unidSegundo, 0, 0, 1);

			escreveDado(dezeDia, 2, 1, 1);
			escreveDado(unidDia, 0, 0, 1);
			escreveDado(dezeMes, 2, 4, 1);
			escreveDado(unidMes, 0, 0, 1);
			escreveDado(dezeAno, 2, 7, 1);
			escreveDado(unidAno, 0, 0, 1);

			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			display ++;
			switch(display) {
				case 3: display = 4; break;
				case 6: display = 7; break;
				case 9:
					if(linha == 1) {
						linha = 2;
						display = 1;
					}
					else {
						display = 8;
					}
					break;
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			display --;
			switch(display) {
				case 0:
					if(linha == 1) {
						display = 1;
					}
					else {
						display = 8;
						linha = 1;
					}
					break;
				case 3: display = 2; break;
				case 6: display = 5; break;
			}
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
		}
	}

	flagSubmenu = true;
	telaAjusteDiaSemana(diaSemana);

	while(flagSubmenu) {
		if(botaoUp()) {
			diaSemana ++;
			if(diaSemana > RTC_WEEKDAY_SUNDAY) {
				diaSemana = RTC_WEEKDAY_MONDAY;
			}
			telaAjusteDiaSemana(diaSemana);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			diaSemana --;
			if(diaSemana < RTC_WEEKDAY_MONDAY) {
				diaSemana = RTC_WEEKDAY_SUNDAY;
			}
			telaAjusteDiaSemana(diaSemana);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			writeRTC(dezeHora * 10 + unidHora, dezeMinuto * 10 + unidMinuto, dezeSegundo * 10 + unidSegundo,
					dezeDia * 10 + unidDia, diaSemana, dezeMes * 10 + unidMes, dezeAno * 10 + unidAno);
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
