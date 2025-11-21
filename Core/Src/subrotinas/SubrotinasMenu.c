///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasMenu
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANATES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU 26
/*=============================================================================
SUBMENU SENHA
==============================================================================*/
uint8_t submenuSenha() {
	uint16_t milh = 0, cent = 0, deze = 0, unid = 0;
	uint8_t flagSubmenu = true, display = 1;
	telaSenha();
	while(botaoOk());

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
			uint16_t entraSenha = milh * 1000 + cent * 100 + deze * 10 + unid;
			while(botaoOk());
			lcdCursor(0);
			if(entraSenha == senha || entraSenha == SENHA_MESTRE) {
				return true;
			}
			return false;
		}

	}

	return false;
}
/*=============================================================================
MENU PRINCIPAL
==============================================================================*/
void menuPrincipal() {
	uint8_t menu = 1;
	flagMenu = true;

	if(flagServiceOcupado) {
		telaAguardaFTP();
		while(botaoOk());
		while(flagServiceOcupado) {
			if(botaoOk()) {
				telaOperacao();
				flagMenu = false;
				while(botaoOk());
			}
			osDelay(1);
		}
	}

	if(submenuSenha()) {
		telaSenhaCorreta();
		telaMenu(menu);
		while(flagMenu) {
			if(botaoUp()) {
				menu ++;
				if(menu > QUANTIDADE_MENU) {
					menu = 0;
				}
				telaMenu(menu);
				HAL_Delay(TEMPO_DUPLO_CLIQUE);
			}

			if(botaoDown()) {
				menu --;
				if(menu > QUANTIDADE_MENU) {
					menu = QUANTIDADE_MENU;
				}
				telaMenu(menu);
				HAL_Delay(TEMPO_DUPLO_CLIQUE);
			}

			if(botaoOk()) {
				while(botaoOk());
				switch(menu) {
					case 0: flagMenu = false; break;
					case 1: submenuAjusteHorario(); break;
					case 2: submenuAjusteLamina(); break;
					case 3: submenuCicloIrrigacao(); break;
					case 4: submenuFertiIrrigacao(); break;
					case 5: submenuAjusteAlarmePressao(); break;
					case 6: submenuTempoPressurizacao(); break;
					case 7: submenuAgendaAcionamento(); break;
					case 8: submenuAjusteHorarioPonta(); break;
					case 9: submenuSelecaoObstaculo(); break;
					case 10: submenuAjustaPosicaoHome(); break;
					case 11: submenuConfiguracoesGPRS(); break;
					case 12: submenuConfiguracoesWiFi(); break;
					case 13: submenuConfiguracoesIp(); break;
					case 14: submenuConfiguracoesLoRaGateway(); break;
					case 15: submenuConfiguracoesLoRa(); break;
					case 16: submenuComunicacaoGNSS(); break;
					case 17: break; //somente apresentação (Horimetro)
					case 18: submenuZerarHorimetro(); break;
					case 19: submenuTabelaPercentimetro(); break;
					case 20: submenuTempoBasePercentimetro(); break;
					case 21: submenuPlacaSoqueteInternet(); break;
					case 22: submenuTipoSensorPressao(); break;
					case 23: submenuTrocaSenha(); break;
					case 24: submenuSelecionaLog(); break;
					case 25: submenuCarregaParametrosFabrica(); break;
					case 26: submenuTrocaNumeroSerial(); break;
				}
				telaMenu(menu);
			}
		}
		telaOperacao();
		return;
	}
	telaSenhaIncorreta();
	telaOperacao();
	flagMenu = false;
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
