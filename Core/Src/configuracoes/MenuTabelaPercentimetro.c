///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTabelaPercentimetro
//AUTOR:      Fábio Almeida
//CIRADO:     05/11/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_TABELA_PERCENTIMETRO 7
/*=============================================================================
AJUSTA VALOR TABELA
==============================================================================*/
void submenuAjustaValorTabelaPercentimetro(uint8_t offset) {
	uint8_t flagSubmenu = true, display = 1;
	uint16_t dmilh = 0, milh = 0, cent = 0, deze = 0, unid = 0;
	uint32_t valor = 0;

	switch(offset) {
		case 1: valor = tabelaLaminaDagua05; break;
		case 2: valor = tabelaLaminaDagua12; break;
		case 3: valor = tabelaLaminaDagua20; break;
		case 4: valor = tabelaLaminaDagua30; break;
		case 5: valor = tabelaLaminaDagua40; break;
		case 6: valor = tabelaLaminaDagua50; break;
		case 7: valor = tabelaLaminaDagua70; break;
		default: return;
	}

	dmilh = valor / 10000;
	milh = valor / 1000;
	cent = valor / 100;
	deze = valor / 10;
	unid = valor % 10;

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


	lcdCursor(true);
	lcdPosLc(2, display);

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
			while(botaoOk());
			flagSubmenu = false;
			valor = dmilh * 10000 + milh * 1000 + cent * 100 + deze * 10 + unid;
			if(valor > VALOR_MAXIMO_LAMINA_MM) {
				valor = VALOR_MAXIMO_LAMINA_MM;
			}

			switch(offset) {
				case 1: tabelaLaminaDagua05 = valor; break;
				case 2: tabelaLaminaDagua12 = valor; break;
				case 3: tabelaLaminaDagua20 = valor; break;
				case 4: tabelaLaminaDagua30 = valor; break;
				case 5: tabelaLaminaDagua40 = valor; break;
				case 6: tabelaLaminaDagua50 = valor; break;
				case 7: tabelaLaminaDagua70 = valor; break;
			}

			writeEepromTabelaLaminaDagua();
		}
	}
	lcdCursor(false);
}
/*=============================================================================
MENU PRINCIPAL DE CONFIGURAÇÕES FTP
==============================================================================*/
void submenuTabelaPercentimetro() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaTabelaPercentimetro(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_TABELA_PERCENTIMETRO) {
				submenu = 0;
			}
			telaTabelaPercentimetro(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_TABELA_PERCENTIMETRO) {
				submenu = QUANTIDADE_MENU_TABELA_PERCENTIMETRO;
			}
			telaTabelaPercentimetro(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					submenuAjustaValorTabelaPercentimetro(submenu);
					break;
			}
			telaTabelaPercentimetro(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
