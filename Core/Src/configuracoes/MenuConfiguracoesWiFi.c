///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuConfiguracoesWiFi
//AUTOR:      Fábio Almeida
//CIRADO:     19/11/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_WIFI 4
/*=============================================================================
MENU SSID
==============================================================================*/
void submenuWiFiSSID() {
	uint8_t flagSubmenu = true, display = 1;
	char valor[QUANTIDADE_CHAR_WIFI_SSID + 1];
	memset(valor, 0x00, QUANTIDADE_CHAR_WIFI_SSID + 1);
	memcpy(valor, wifiSSID, QUANTIDADE_CHAR_WIFI_SSID + 1);

	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			valor[display - 1] ++;
			if(valor[display - 1] > '}') {
				valor[display - 1] = ' ';
			}
			if(valor[display - 1] < ' ') {
				valor[display - 1] = ' ';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(valor, QUANTIDADE_CHAR_WIFI_SSID);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			valor[display - 1] --;
			if(valor[display - 1] > '}') {
				valor[display - 1] = '}';
			}
			if(valor[display - 1] < ' ') {
				valor[display - 1] = '}';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(valor, QUANTIDADE_CHAR_WIFI_SSID);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < QUANTIDADE_CHAR_WIFI_SSID) {
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
			for(uint8_t i = QUANTIDADE_CHAR_WIFI_SSID - 1; i < QUANTIDADE_CHAR_WIFI_SSID; i --) {
				if(valor[i] == ' ') {
					valor[i] = 0x00;
				}
				else {
					if(valor[i] == 0xFF) {
						valor[i] = 0x00;
					}
					else if(valor[i] > 0x00) {
						i = 0xFF;
					}
				}
			}
			memset(wifiSSID, 0x00, QUANTIDADE_CHAR_WIFI_SSID + 1);
			memcpy(wifiSSID, valor, QUANTIDADE_CHAR_WIFI_SSID + 1);
			writeEepromWiFiSSID();
			flagSubmenu = false;
		}
	}
	lcdCursor(false);
}
/*=============================================================================
MENU SENHA
==============================================================================*/
void submenuWiFiSenha() {
	uint8_t flagSubmenu = true, display = 1;
	char valor[QUANTIDADE_CHAR_WIFI_SENHA + 1];
	memset(valor, 0x00, QUANTIDADE_CHAR_WIFI_SENHA + 1);
	memcpy(valor, wifiSenha, QUANTIDADE_CHAR_WIFI_SENHA + 1);

	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			valor[display - 1] ++;
			if(valor[display - 1] > '}') {
				valor[display - 1] = ' ';
			}
			if(valor[display - 1] < ' ') {
				valor[display - 1] = ' ';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(valor, QUANTIDADE_CHAR_WIFI_SENHA);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			valor[display - 1] --;
			if(valor[display - 1] > '}') {
				valor[display - 1] = '}';
			}
			if(valor[display - 1] < ' ') {
				valor[display - 1] = '}';
			}
			lcdPosLc(2, 1);
			lcdEscreveString(valor, QUANTIDADE_CHAR_WIFI_SENHA);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < QUANTIDADE_CHAR_WIFI_SENHA) {
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
			for(uint8_t i = QUANTIDADE_CHAR_WIFI_SSID - 1; i < QUANTIDADE_CHAR_WIFI_SSID; i --) {
				if(valor[i] == ' ') {
					valor[i] = 0x00;
				}
				else {
					if(valor[i] == 0xFF) {
						valor[i] = 0x00;
					}
					else if(valor[i] > 0x00) {
						i = 0xFF;
					}
				}
			}
			memset(wifiSenha, 0x00, QUANTIDADE_CHAR_WIFI_SENHA + 1);
			memcpy(wifiSenha, valor, QUANTIDADE_CHAR_WIFI_SENHA + 1);
			writeEepromWiFiSenha();
			flagSubmenu = false;
		}
	}
	lcdCursor(false);
}
/*=============================================================================
MENU ENVIAR PARA O MODULO WIFI
==============================================================================*/
void submenuEnviarParaWiFi() {
	if(placaSoquete != SOQUETE_WIFI) {
		telaVerifiquePlacaSoquete();
		return;
	}

	telaAguarde();
	if(conectarAPWiFi()) {
		flagBuzzer = true;
		telaWiFiConectado();
	}
	else {
		flagBuzzer = true;
		telaWiFiNaoConectado();
	}
}
/*=============================================================================
MENU REINICIAR MODULO WIFI
==============================================================================*/
void submenuReiniciarWiFi() {
	if(placaSoquete != SOQUETE_WIFI) {
		telaVerifiquePlacaSoquete();
		return;
	}
	telaAguarde();
	reiniciaModuloWiFi();
}
/*=============================================================================
MENU PRINCIPAL DE CONFIGURAÇÕES WIFI
==============================================================================*/
void submenuConfiguracoesWiFi() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaConfiguracoesWiFi(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_WIFI) {
				submenu = 0;
			}
			telaConfiguracoesWiFi(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_WIFI) {
				submenu = QUANTIDADE_MENU_WIFI;
			}
			telaConfiguracoesWiFi(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1: submenuWiFiSSID(); break;
				case 2: submenuWiFiSenha(); break;
				case 3: submenuEnviarParaWiFi(); break;
				case 4: submenuReiniciarWiFi(); break;
			}
			telaConfiguracoesWiFi(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
