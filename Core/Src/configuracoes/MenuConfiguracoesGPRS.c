///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuConfiguracoesFTP
//AUTOR:      Fábio Almeida
//CIRADO:     17/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_GPRS 2
/*=============================================================================
APN GPRS
==============================================================================*/
void submenuAPNGPRS() {
	uint8_t flagSubmenu = true, display = 1;
	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			gprsAPN[display - 1] ++;
			if(gprsAPN[display - 1] > '}') {
				gprsAPN[display - 1] = ' ';
			}
			if(gprsAPN[display - 1] < ' ') {
				gprsAPN[display - 1] = ' ';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(gprsAPN, QUANTIDADE_CHAR_GPRS_APN);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			gprsAPN[display - 1] --;
			if(gprsAPN[display - 1] > '}') {
				gprsAPN[display - 1] = '}';
			}
			if(gprsAPN[display - 1] < ' ') {
				gprsAPN[display - 1] = '}';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(gprsAPN, QUANTIDADE_CHAR_GPRS_APN);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < QUANTIDADE_CHAR_GPRS_APN) {
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
			writeEepromGPRSAPN();
		}
	}
	lcdCursor(false);
}
/*=============================================================================
MENU REINICIAR MODULO GPRS
==============================================================================*/
void submenuReiniciarGPRS() {
	telaAguarde();
	resetGPRS();
}
/*=============================================================================
MENU PRINCIPAL DE CONFIGURAÇÕES GPRS
==============================================================================*/
void submenuConfiguracoesGPRS() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaConfiguracoesGPRS(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_GPRS) {
				submenu = 0;
			}
			telaConfiguracoesGPRS(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_GPRS) {
				submenu = QUANTIDADE_MENU_GPRS;
			}
			telaConfiguracoesGPRS(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1: submenuAPNGPRS(); break;
				case 2: submenuReiniciarGPRS(); break;
			}
			telaConfiguracoesGPRS(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
