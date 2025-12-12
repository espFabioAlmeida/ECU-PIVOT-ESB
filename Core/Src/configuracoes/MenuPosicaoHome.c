///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuPosicaoHome
//AUTOR:      Fábio Almeida
//CIRADO:     23/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
COSNTANTES DO ARQUIVO
==============================================================================*/
#define TEMPO_PRESS_AND_HOLD	25
/*=============================================================================
MENU AJUSTA POSIÇÃO HOME
==============================================================================*/
void submenuAjustaPosicaoHome() {
	uint8_t flagSubmenu = true;
	uint8_t display = 4, linha = 1;
	uint8_t latitudeGrausDeze = 0, latitudeGrausUnid = 0, latitudeMinutosDeze = 0, latitudeMinutosUnid = 0;
	uint8_t longitudeGrausCent = 0, longitudeGrausDeze = 0, longitudeGrausUnid = 0, longitudeMinutosDeze = 0, longitudeMinutosUnid = 0;
	uint16_t latitudeSegundosDezeMilh = 0, latitudeSegundosMilh = 0, latitudeSegundosCent = 0, latitudeSegundosDeze = 0, latitudeSegundosUnid = 0;
	uint16_t longitudeSegundosDezeMilh = 0, longitudeSegundosMilh = 0, longitudeSegundosCent = 0, longitudeSegundosDeze = 0, longitudeSegundosUnid = 0;
	char hemisferio = posicaoHome.hemisferio, indicadorWE = posicaoHome.indicadorWE;
	telaEdicaoPosicaoHome();

	latitudeGrausDeze = posicaoHome.latitudeGraus / 10;
	latitudeGrausUnid = posicaoHome.latitudeGraus % 10;
	latitudeMinutosDeze = posicaoHome.latitudeMinutos / 10;
	latitudeMinutosUnid = posicaoHome.latitudeMinutos % 10;

	latitudeSegundosDezeMilh = posicaoHome.latitudeSegundos / 10000;
	latitudeSegundosMilh = posicaoHome.latitudeSegundos / 1000;
	latitudeSegundosCent = posicaoHome.latitudeSegundos / 100;
	latitudeSegundosDeze = posicaoHome.latitudeSegundos / 10;
	latitudeSegundosUnid = posicaoHome.latitudeSegundos % 10;

	longitudeGrausCent = posicaoHome.longitudeGraus / 100;
	longitudeGrausDeze = posicaoHome.longitudeGraus / 10;
	longitudeGrausUnid = posicaoHome.longitudeGraus % 10;
	longitudeMinutosDeze = posicaoHome.longitudeMinutos / 10;
	longitudeMinutosUnid = posicaoHome.longitudeMinutos % 10;

	longitudeSegundosDezeMilh = posicaoHome.longitudeSegundos / 10000;
	longitudeSegundosMilh = posicaoHome.longitudeSegundos / 1000;
	longitudeSegundosCent = posicaoHome.longitudeSegundos / 100;
	longitudeSegundosDeze = posicaoHome.longitudeSegundos / 10;
	longitudeSegundosUnid = posicaoHome.longitudeSegundos % 10;

	if(longitudeGrausDeze >= 10) {
		longitudeGrausDeze %= 10;
	}

	if(latitudeSegundosMilh >= 10) {
		latitudeSegundosMilh %= 10;
	}

	if(latitudeSegundosCent >= 10) {
		latitudeSegundosCent %= 10;
	}

	if(latitudeSegundosDeze >= 10) {
		latitudeSegundosDeze %= 10;
	}

	if(longitudeSegundosMilh >= 10) {
		longitudeSegundosMilh %= 10;
	}

	if(longitudeSegundosCent >= 10) {
		longitudeSegundosCent %= 10;
	}

	if(longitudeSegundosDeze >= 10) {
		longitudeSegundosDeze %= 10;
	}

	while(flagSubmenu) {
		if(botaoUp()) {
			if(linha == 1) {
				switch(display) {
					case 4: latitudeGrausDeze = incrementaDecrementa(latitudeGrausDeze, 0, 9, true); break;
					case 5: latitudeGrausUnid = incrementaDecrementa(latitudeGrausUnid, 0, 9, true); break;

					case 7: latitudeMinutosDeze = incrementaDecrementa(latitudeMinutosDeze, 0, 5, true); break;
					case 8: latitudeMinutosUnid = incrementaDecrementa(latitudeMinutosUnid, 0, 9, true); break;

					case 10: latitudeSegundosDezeMilh = incrementaDecrementa(latitudeSegundosDezeMilh, 0, 5, true); break;
					case 11: latitudeSegundosMilh = incrementaDecrementa(latitudeSegundosMilh, 0, 9, true); break;
					case 12: latitudeSegundosCent = incrementaDecrementa(latitudeSegundosCent, 0, 9, true); break;
					case 13: latitudeSegundosDeze = incrementaDecrementa(latitudeSegundosDeze, 0, 9, true); break;
					case 14: latitudeSegundosUnid = incrementaDecrementa(latitudeSegundosUnid, 0, 9, true); break;
					case 15:
						if(hemisferio == 'S'){
							hemisferio = 'N';
						}
						else {
							hemisferio = 'S';
						}
						break;
				}
			}
			else {
				switch(display) {
					case 4: longitudeGrausCent = incrementaDecrementa(longitudeGrausCent, 0, 1, true); break;
					case 5: longitudeGrausDeze = incrementaDecrementa(longitudeGrausDeze, 0, 9, true); break;
					case 6: longitudeGrausUnid = incrementaDecrementa(longitudeGrausUnid, 0, 9, true); break;

					case 8: longitudeMinutosDeze = incrementaDecrementa(longitudeMinutosDeze, 0, 5, true); break;
					case 9: longitudeMinutosUnid = incrementaDecrementa(longitudeMinutosUnid, 0, 9, true); break;

					case 11: longitudeSegundosDezeMilh = incrementaDecrementa(longitudeSegundosDezeMilh, 0, 5, true); break;
					case 12: longitudeSegundosMilh = incrementaDecrementa(longitudeSegundosMilh, 0, 9, true); break;
					case 13: longitudeSegundosCent = incrementaDecrementa(longitudeSegundosCent, 0, 9, true); break;
					case 14: longitudeSegundosDeze = incrementaDecrementa(longitudeSegundosDeze, 0, 9, true); break;
					case 15: longitudeSegundosUnid = incrementaDecrementa(longitudeSegundosUnid, 0, 9, true); break;
					case 16:
						if(indicadorWE == 'W'){
							indicadorWE = 'E';
						}
						else {
							indicadorWE = 'W';
						}
						break;
				}
			}

			escreveDado(latitudeGrausDeze, 1, 4, 1);
			escreveDado(latitudeGrausUnid, 0, 0, 1);
			escreveDado(latitudeMinutosDeze, 1, 7, 1);
			escreveDado(latitudeMinutosUnid, 0, 0, 1);
			escreveDado(latitudeSegundosDezeMilh, 1, 10, 1);
			escreveDado(latitudeSegundosMilh, 0, 0, 1);
			escreveDado(latitudeSegundosCent, 0, 0, 1);
			escreveDado(latitudeSegundosDeze, 0, 0, 1);
			escreveDado(latitudeSegundosUnid, 0, 0, 1);
			lcdEscreveChar(hemisferio);

			escreveDado(longitudeGrausCent, 2, 4, 1);
			escreveDado(longitudeGrausDeze, 0, 0, 1);
			escreveDado(longitudeGrausUnid, 0, 0, 1);
			escreveDado(longitudeMinutosDeze, 2, 8, 1);
			escreveDado(longitudeMinutosUnid, 0, 0, 1);
			escreveDado(longitudeSegundosDezeMilh, 2, 11, 1);
			escreveDado(longitudeSegundosMilh, 0, 0, 1);
			escreveDado(longitudeSegundosCent, 0, 0, 1);
			escreveDado(longitudeSegundosDeze, 0, 0, 1);
			escreveDado(longitudeSegundosUnid, 0, 0, 1);
			lcdEscreveChar(indicadorWE);
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			if(linha == 1) {
				switch(display) {
					case 4: latitudeGrausDeze = incrementaDecrementa(latitudeGrausDeze, 0, 9, false); break;
					case 5: latitudeGrausUnid = incrementaDecrementa(latitudeGrausUnid, 0, 9, false); break;

					case 7: latitudeMinutosDeze = incrementaDecrementa(latitudeMinutosDeze, 0, 5, false); break;
					case 8: latitudeMinutosUnid = incrementaDecrementa(latitudeMinutosUnid, 0, 9, false); break;

					case 10: latitudeSegundosDezeMilh = incrementaDecrementa(latitudeSegundosDezeMilh, 0, 5, false); break;
					case 11: latitudeSegundosMilh = incrementaDecrementa(latitudeSegundosMilh, 0, 9, false); break;
					case 12: latitudeSegundosCent = incrementaDecrementa(latitudeSegundosCent, 0, 9, false); break;
					case 13: latitudeSegundosDeze = incrementaDecrementa(latitudeSegundosDeze, 0, 9, false); break;
					case 14: latitudeSegundosUnid = incrementaDecrementa(latitudeSegundosUnid, 0, 9, false); break;
					case 15:
						if(hemisferio == 'S'){
							hemisferio = 'N';
						}
						else {
							hemisferio = 'S';
						}
						break;
				}
			}
			else {
				switch(display) {
					case 4: longitudeGrausCent = incrementaDecrementa(longitudeGrausCent, 0, 1, false); break;
					case 5: longitudeGrausDeze = incrementaDecrementa(longitudeGrausDeze, 0, 9, false); break;
					case 6: longitudeGrausUnid = incrementaDecrementa(longitudeGrausUnid, 0, 9, false); break;

					case 8: longitudeMinutosDeze = incrementaDecrementa(longitudeMinutosDeze, 0, 5, false); break;
					case 9: longitudeMinutosUnid = incrementaDecrementa(longitudeMinutosUnid, 0, 9, false); break;

					case 11: longitudeSegundosDezeMilh = incrementaDecrementa(longitudeSegundosDezeMilh, 0, 5, false); break;
					case 12: longitudeSegundosMilh = incrementaDecrementa(longitudeSegundosMilh, 0, 9, false); break;
					case 13: longitudeSegundosCent = incrementaDecrementa(longitudeSegundosCent, 0, 9, false); break;
					case 14: longitudeSegundosDeze = incrementaDecrementa(longitudeSegundosDeze, 0, 9, false); break;
					case 15: longitudeSegundosUnid = incrementaDecrementa(longitudeSegundosUnid, 0, 9, false); break;
					case 16:
						if(indicadorWE == 'W'){
							indicadorWE = 'E';
						}
						else {
							indicadorWE = 'W';
						}
						break;
				}
			}

			escreveDado(latitudeGrausDeze, 1, 4, 1);
			escreveDado(latitudeGrausUnid, 0, 0, 1);
			escreveDado(latitudeMinutosDeze, 1, 7, 1);
			escreveDado(latitudeMinutosUnid, 0, 0, 1);
			escreveDado(latitudeSegundosDezeMilh, 1, 10, 1);
			escreveDado(latitudeSegundosMilh, 0, 0, 1);
			escreveDado(latitudeSegundosCent, 0, 0, 1);
			escreveDado(latitudeSegundosDeze, 0, 0, 1);
			escreveDado(latitudeSegundosUnid, 0, 0, 1);
			lcdEscreveChar(hemisferio);

			escreveDado(longitudeGrausCent, 2, 4, 1);
			escreveDado(longitudeGrausDeze, 0, 0, 1);
			escreveDado(longitudeGrausUnid, 0, 0, 1);
			escreveDado(longitudeMinutosDeze, 2, 8, 1);
			escreveDado(longitudeMinutosUnid, 0, 0, 1);
			escreveDado(longitudeSegundosDezeMilh, 2, 11, 1);
			escreveDado(longitudeSegundosMilh, 0, 0, 1);
			escreveDado(longitudeSegundosCent, 0, 0, 1);
			escreveDado(longitudeSegundosDeze, 0, 0, 1);
			escreveDado(longitudeSegundosUnid, 0, 0, 1);
			lcdEscreveChar(indicadorWE);
			lcdPosLc(linha, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(linha == 1) {
				if(display > 4) {
					display --;

					if(display == 6 || display == 9) {
						display --;
					}
					lcdPosLc(linha, display);
				}
			}
			else {
				display --;
				switch(display) {
					case 3: display = 15; linha = 1; break;
					case 7:
					case 10: display --; break;
				}
				lcdPosLc(linha, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(linha == 1) {
				display ++;
				switch(display) {
					case 6:
					case 9: display ++; break;
					case 16: display = 4; linha = 2; break;;
				}
				lcdPosLc(linha, display);
			}
			else {
				if(display < 16) {
					display ++;
				}

				if(display == 7 || display == 10) {
					display ++;
				}
				lcdPosLc(linha, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			uint8_t pressAndHold = 0;
			while(botaoOk()) {
				pressAndHold ++;
				if(pressAndHold == TEMPO_PRESS_AND_HOLD) {
					flagBuzzer = true;
				}
				if(pressAndHold > TEMPO_PRESS_AND_HOLD) {
					pressAndHold = TEMPO_PRESS_AND_HOLD + 1;
				}
			}
			if(pressAndHold < TEMPO_PRESS_AND_HOLD) {
				flagSubmenu = false;
				posicaoHome.latitudeGraus = latitudeGrausDeze * 10 + latitudeGrausUnid;
				posicaoHome.latitudeMinutos = latitudeMinutosDeze * 10 + latitudeMinutosUnid;
				posicaoHome.latitudeSegundos = latitudeSegundosDezeMilh * 10000 + latitudeSegundosMilh * 1000
						+ latitudeSegundosCent * 100 + latitudeSegundosDeze * 10 + latitudeSegundosUnid;
				posicaoHome.hemisferio = hemisferio;

				posicaoHome.longitudeGraus = longitudeGrausCent * 100 + longitudeGrausDeze * 10 + longitudeGrausUnid;
				posicaoHome.longitudeMinutos = longitudeMinutosDeze * 10 + longitudeMinutosUnid;
				posicaoHome.longitudeSegundos = longitudeSegundosDezeMilh * 10000 + longitudeSegundosMilh * 1000
						+ longitudeSegundosCent * 100 + longitudeSegundosDeze * 10 + longitudeSegundosUnid;
				posicaoHome.indicadorWE = indicadorWE;

				if(posicaoHome.latitudeGraus > 90) {
					posicaoHome.latitudeGraus = 90;
				}

				if(posicaoHome.longitudeGraus > 180) {
					posicaoHome.longitudeGraus = 180;
				}
				writeEepromPosicaoHome();
			}
			else {
				lcdCursor(false);
				telaAguarde();
				buscaPosicaoObstaculo();
				hemisferio = posicaoAtualGPS.hemisferio;
				indicadorWE = posicaoAtualGPS.indicadorWE;

				latitudeGrausDeze = posicaoAtualGPS.latitudeGraus / 10;
				latitudeGrausUnid = posicaoAtualGPS.latitudeGraus % 10;
				latitudeMinutosDeze = posicaoAtualGPS.latitudeMinutos / 10;
				latitudeMinutosUnid = posicaoAtualGPS.latitudeMinutos % 10;

				latitudeSegundosDezeMilh = posicaoAtualGPS.latitudeSegundos / 10000;
				latitudeSegundosMilh = posicaoAtualGPS.latitudeSegundos / 1000;
				latitudeSegundosCent = posicaoAtualGPS.latitudeSegundos / 100;
				latitudeSegundosDeze = posicaoAtualGPS.latitudeSegundos / 10;
				latitudeSegundosUnid = posicaoAtualGPS.latitudeSegundos % 10;

				longitudeGrausCent = posicaoAtualGPS.longitudeGraus / 100;
				longitudeGrausDeze = posicaoAtualGPS.longitudeGraus / 10;
				longitudeGrausUnid = posicaoAtualGPS.longitudeGraus % 10;
				longitudeMinutosDeze = posicaoAtualGPS.longitudeMinutos / 10;
				longitudeMinutosUnid = posicaoAtualGPS.longitudeMinutos % 10;

				longitudeSegundosDezeMilh = posicaoAtualGPS.longitudeSegundos / 10000;
				longitudeSegundosMilh = posicaoAtualGPS.longitudeSegundos / 1000;
				longitudeSegundosCent = posicaoAtualGPS.longitudeSegundos / 100;
				longitudeSegundosDeze = posicaoAtualGPS.longitudeSegundos / 10;
				longitudeSegundosUnid = posicaoAtualGPS.longitudeSegundos % 10;

				if(longitudeGrausDeze >= 10) {
					longitudeGrausDeze %= 10;
				}

				if(latitudeSegundosMilh >= 10) {
					latitudeSegundosMilh %= 10;
				}

				if(latitudeSegundosCent >= 10) {
					latitudeSegundosCent %= 10;
				}

				if(latitudeSegundosDeze >= 10) {
					latitudeSegundosDeze %= 10;
				}

				if(longitudeSegundosMilh >= 10) {
					longitudeSegundosMilh %= 10;
				}

				if(longitudeSegundosCent >= 10) {
					longitudeSegundosCent %= 10;
				}

				if(longitudeSegundosDeze >= 10) {
					longitudeSegundosDeze %= 10;
				}

				telaEdicaoPosicaoHome();
				linha = 1;
				display = 4;
				escreveDado(latitudeGrausDeze, 1, 4, 1);
				escreveDado(latitudeGrausUnid, 0, 0, 1);
				escreveDado(latitudeMinutosDeze, 1, 7, 1);
				escreveDado(latitudeMinutosUnid, 0, 0, 1);
				escreveDado(latitudeSegundosDezeMilh, 1, 10, 1);
				escreveDado(latitudeSegundosMilh, 0, 0, 1);
				escreveDado(latitudeSegundosCent, 0, 0, 1);
				escreveDado(latitudeSegundosDeze, 0, 0, 1);
				escreveDado(latitudeSegundosUnid, 0, 0, 1);
				lcdEscreveChar(hemisferio);

				escreveDado(longitudeGrausCent, 2, 4, 1);
				escreveDado(longitudeGrausDeze, 0, 0, 1);
				escreveDado(longitudeGrausUnid, 0, 0, 1);
				escreveDado(longitudeMinutosDeze, 2, 8, 1);
				escreveDado(longitudeMinutosUnid, 0, 0, 1);
				escreveDado(longitudeSegundosDezeMilh, 2, 11, 1);
				escreveDado(longitudeSegundosMilh, 0, 0, 1);
				escreveDado(longitudeSegundosCent, 0, 0, 1);
				escreveDado(longitudeSegundosDeze, 0, 0, 1);
				escreveDado(longitudeSegundosUnid, 0, 0, 1);
				lcdEscreveChar(indicadorWE);
				lcdPosLc(linha, display);
			}
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
