///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuConfiguracoesIp
//AUTOR:      Fábio Almeida
//CIRADO:     17/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
COSNTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_IP_WIFI 7

enum SELECAO_IP {
	IP_LOCAL = 1,
	SERVIDOR_DNS,
	GATEWAY,
	MASK
};
/*=============================================================================
MENU DHCP
==============================================================================*/
void submenuWiFiDHCP() {
	uint8_t flagSubmenu = true, flagEscolha = flagWiFiDhcp;
	lcdPosLc(2, 1);
	lcdCursor(true);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			while(botaoUp() || botaoDown());
			lcdPosLc(2, 1);
			if(flagEscolha) {
				flagEscolha = false;
				lcdEscreve("Desabilitado");
			}
			else {
				flagEscolha = true;
				lcdEscreve("Habilitado  ");
			}
			lcdPosLc(2, 1);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			flagWiFiDhcp = flagEscolha;
			writeEepromWiFiDhcp();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
MENU LER O IP DINAMICO
==============================================================================*/
void submenuLerIpDinamico() {
	uint8_t flagSubmenu = true;

	if(placaSoquete != SOQUETE_WIFI) {
		telaVerifiquePlacaSoquete();
		return;
	}

	telaAguarde();
	wiFiLerIpDinamico();
	telaIpDinamico();
	while(flagSubmenu) {
		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
		}
	}
}
/*=============================================================================
MENU WIFI IP
==============================================================================*/
void submenuWiFiIP(uint8_t offset) {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t octeto[4];
	uint16_t cent = 0, deze = 0, unid = 0;

	switch(offset) {
		case IP_LOCAL:
			for(uint8_t i = 0; i < 4; i ++) {
				octeto[i] = wifiIp[i];
			}
			break;
		case SERVIDOR_DNS:
			for(uint8_t i = 0; i < 4; i ++) {
				octeto[i] = wifiDNS[i];
			}
			break;
		case GATEWAY:
			for(uint8_t i = 0; i < 4; i ++) {
				octeto[i] = wifiGateway[i];
			}
			break;
		case MASK:
			for(uint8_t i = 0; i < 4; i ++) {
				octeto[i] = wifiMask[i];
			}
			break;
	}
	cent = octeto[0] / 100;
	deze = octeto[0] / 10;
	unid = octeto[0] % 10;

	if(cent >= 10) {
		cent %= 10;
	}
	if(deze >= 10) {
		deze %= 10;
	}

	lcdPosLc(2, display);
	lcdCursor(true);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1:
				case 5:
				case 9:
				case 13:
					cent = incrementaDecrementa(cent, 0, 2, true);
					break;
				case 2:
				case 6:
				case 10:
				case 14:
					deze = incrementaDecrementa(deze, 0, 9, true);
					break;
				case 3:
				case 7:
				case 11:
				case 15:
					unid = incrementaDecrementa(unid, 0, 9, true);
					break;
			}
			if(cent == 2 && deze > 5) {
				deze = 0;
				unid = 0;
			}
			if(cent == 2 && deze == 5 && unid > 5) {
				unid = 0;
			}

			if(display < 4) {
				escreveDado(cent, 2, 1, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else if(display < 8) {
				escreveDado(cent, 2, 5, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else if(display < 12) {
				escreveDado(cent, 2, 9, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else {
				escreveDado(cent, 2, 13, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}

			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1:
				case 5:
				case 9:
				case 13:
					cent = incrementaDecrementa(cent, 0, 2, false);
					break;
				case 2:
				case 6:
				case 10:
				case 14:
					deze = incrementaDecrementa(deze, 0, 9, false);
					break;
				case 3:
				case 7:
				case 11:
				case 15:
					unid = incrementaDecrementa(unid, 0, 9, false);
					break;
			}
			if(cent == 2 && deze > 5) {
				deze = 5;
				unid = 5;
			}
			if(cent == 2 && deze == 5 && unid > 5) {
				unid = 5;
			}

			if(display < 4) {
				escreveDado(cent, 2, 1, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else if(display < 8) {
				escreveDado(cent, 2, 5, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else if(display < 12) {
				escreveDado(cent, 2, 9, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}
			else {
				escreveDado(cent, 2, 13, 1);
				escreveDado(deze, 0, 0, 1);
				escreveDado(unid, 0, 0, 1);
			}

			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			display ++;
			if(display == 4) {
				octeto[0] = cent * 100 + deze * 10 + unid;
				cent = octeto[1] / 100;
				deze = octeto[1] / 10;
				unid = octeto[1] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 5;
			}
			if(display == 8) {
				octeto[1] = cent * 100 + deze * 10 + unid;
				cent = octeto[2] / 100;
				deze = octeto[2] / 10;
				unid = octeto[2] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 9;
			}
			if(display == 12) {
				octeto[2] = cent * 100 + deze * 10 + unid;
				cent = octeto[3] / 100;
				deze = octeto[3] / 10;
				unid = octeto[3] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 13;
			}

			if(display > 15) {
				display = 15;
			}

			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			display --;
			if(display == 4) {
				octeto[1] = cent * 100 + deze * 10 + unid;
				cent = octeto[0] / 100;
				deze = octeto[0] / 10;
				unid = octeto[0] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 3;
			}
			if(display == 8) {
				octeto[2] = cent * 100 + deze * 10 + unid;
				cent = octeto[1] / 100;
				deze = octeto[1] / 10;
				unid = octeto[1] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 7;
			}
			if(display == 12) {
				octeto[3] = cent * 100 + deze * 10 + unid;
				cent = octeto[2] / 100;
				deze = octeto[2] / 10;
				unid = octeto[2] % 10;
				if(cent >= 10) {
					cent %= 10;
				}
				if(deze >= 10) {
					deze %= 10;
				}
				display = 11;
			}

			if(!display) {
				display = 1;
			}

			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());

			switch(display) {
				case 1:
				case 2:
				case 3: octeto[0] = cent * 100 + deze * 10 + unid; break;

				case 5:
				case 6:
				case 7: octeto[1] = cent * 100 + deze * 10 + unid; break;

				case 9:
				case 10:
				case 11: octeto[2] = cent * 100 + deze * 10 + unid; break;

				case 13:
				case 14:
				case 15: octeto[3] = cent * 100 + deze * 10 + unid; break;
			}

			switch(offset) {
				case IP_LOCAL:
					for(uint8_t i = 0; i < 4; i ++) {
						wifiIp[i] = octeto[i];
					}
					writeEepromWiFiIp();
					break;
				case SERVIDOR_DNS:
					for(uint8_t i = 0; i < 4; i ++) {
						wifiDNS[i] = octeto[i];
					}
					writeEepromWiFiDNS();
					break;
				case GATEWAY:
					for(uint8_t i = 0; i < 4; i ++) {
						wifiGateway[i] = octeto[i];
					}
					writeEepromWiFiGateway();
					break;
				case MASK:
					for(uint8_t i = 0; i < 4; i ++) {
						wifiMask[i] = octeto[i];
					}
					writeEepromWiFiMask();
					break;
			}
			flagSubmenu = false;
		}
	}

	lcdCursor(false);
}
/*=============================================================================
MENU ENVIAR IP PARA O MODULO WIFI
==============================================================================*/
void submenuEnviarIpParaWiFi() {
	if(placaSoquete != SOQUETE_WIFI) {
		telaVerifiquePlacaSoquete();
		return;
	}

	telaAguarde();
	if(flagWiFiDhcp) {
		wiFiHabilitaDhcp();
		return;
	}
	wiFiFixaIp();
}
/*=============================================================================
MENU PRINCIPAL DE CONFIGURAÇÕES IP
==============================================================================*/
void submenuConfiguracoesIp() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaConfiguracoesIpWiFi(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_IP_WIFI) {
				submenu = 0;
			}
			telaConfiguracoesIpWiFi(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_IP_WIFI) {
				submenu = QUANTIDADE_MENU_IP_WIFI;
			}
			telaConfiguracoesIpWiFi(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1: submenuWiFiDHCP(); break;
				case 2: submenuLerIpDinamico(); break;
				case 3: submenuWiFiIP(IP_LOCAL); break;
				case 4: submenuWiFiIP(SERVIDOR_DNS);break;
				case 5: submenuWiFiIP(GATEWAY); break;
				case 6: submenuWiFiIP(MASK); break;
				case 7: submenuEnviarIpParaWiFi(); break;
			}
			telaConfiguracoesIpWiFi(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
