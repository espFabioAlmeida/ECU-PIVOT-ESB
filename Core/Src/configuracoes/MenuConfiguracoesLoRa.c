///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuConfiguracoesLoRa
//AUTOR:      Fábio Almeida
//CIRADO:     15/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_LORA 6
/*=============================================================================
VALIDA ENDERECO E CANAL
==============================================================================*/
uint8_t validacaoLoRaHexa(uint8_t valor) {
	if(!valor) {
		return false;
	}

	if(valor == 0x0A) {
		return false;
	}

	return true;
}
/*=============================================================================
ENDERECO PIVOT
==============================================================================*/
void submenuEnderecoLoRaPivot() {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t hi = make8(enderecoLoRaPivot, 1), lo = make8(enderecoLoRaPivot, 0);
	uint8_t nibbleSuperiorHi = hi & 0xF0, nibbleInferiorHi = hi & 0x0F;
	uint8_t nibbleSuperiorLo = lo & 0xF0, nibbleInferiorLo = 0; //sempre zero no pivot

	nibbleSuperiorHi >>= 4;
	nibbleSuperiorLo >>= 4;

	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1: nibbleSuperiorHi = incrementaDecrementa(nibbleSuperiorHi, 0, 0x0F, true); break;
				case 2: nibbleInferiorHi = incrementaDecrementa(nibbleInferiorHi, 0, 0x0F, true); break;
				case 3: nibbleSuperiorLo = incrementaDecrementa(nibbleSuperiorLo, 0, 0x0F, true); break;
			}
			hi = nibbleSuperiorHi;
			hi <<= 4;
			hi |= nibbleInferiorHi;
			lo = nibbleSuperiorLo;
			lo <<= 4;
			lo |= nibbleInferiorLo;

			escreveDadoHexadecimal(hi, 2, 1, 1);
			escreveDadoHexadecimal(lo, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1: nibbleSuperiorHi = incrementaDecrementa(nibbleSuperiorHi, 0, 0x0F, false); break;
				case 2: nibbleInferiorHi = incrementaDecrementa(nibbleInferiorHi, 0, 0x0F, false); break;
				case 3: nibbleSuperiorLo = incrementaDecrementa(nibbleSuperiorLo, 0, 0x0F, false); break;
			}
			hi = nibbleSuperiorHi;
			hi <<= 4;
			hi |= nibbleInferiorHi;
			lo = nibbleSuperiorLo;
			lo <<= 4;
			lo |= nibbleInferiorLo;

			escreveDadoHexadecimal(hi, 2, 1, 1);
			escreveDadoHexadecimal(lo, 0, 0, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 3) {
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

			if(validacaoLoRaHexa(hi) && validacaoLoRaHexa(lo)) {
				enderecoLoRaPivot = make16(hi, lo);

				if(!enderecoLoRaPivot) { //0 é broadcast
					enderecoLoRaPivot = 0x0010;
				}

				enderecoLoRaGNSS = enderecoLoRaPivot + 1;
				writeEepromEnderecoLoRa();
			}
			else {
				lcdCursor(false);
				telaErro();
			}
		}
	}
	lcdCursor(false);
}
/*=============================================================================
CANAL LoRa
==============================================================================*/
void submenuCanalLoRa() {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t valor = canalLoRa;
	uint8_t nibbleSuperior = canalLoRa & 0xF0, nibbleInferior = canalLoRa & 0x0F;

	nibbleSuperior >>= 4;

	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1: nibbleSuperior = incrementaDecrementa(nibbleSuperior, 0, 0x01, true); break;
				case 2: nibbleInferior = incrementaDecrementa(nibbleInferior, 0, 0x0F, true); break;
			}
			valor = nibbleSuperior;
			valor <<= 4;
			valor |= nibbleInferior;

			escreveDadoHexadecimal(valor, 2, 1, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1: nibbleSuperior = incrementaDecrementa(nibbleSuperior, 0, 0x01, false); break;
				case 2: nibbleInferior = incrementaDecrementa(nibbleInferior, 0, 0x0F, false); break;
			}
			valor = nibbleSuperior;
			valor <<= 4;
			valor |= nibbleInferior;

			escreveDadoHexadecimal(valor, 2, 1, 1);
			lcdPosLc(2, display);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 2) {
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
			if(validacaoLoRaHexa(valor)) {
				canalLoRa = valor;
				writeEepromCanalLoRa();
			}
			else {
				lcdCursor(false);
				telaErro();
			}
		}
	}
	lcdCursor(false);
}
/*=============================================================================
MENU ENVIAR PARA O MODULO LoRa
==============================================================================*/
void submenuEnviarParaLoRa() {
	telaAguarde();
	if(configuraLoRa()) {
		telaSucesso();
		return;
	}
	telaErro();
}
/*=============================================================================
MENU ENVIAR PARA O MODULO LoRa da ECU GPS
==============================================================================*/
void submenuEnviarParaEcuGps() {
	telaAguarde();
	if(configuraLoRaGNSS()) {
		uint8_t flagSubmenu = true;
		telaSucesso();
		telaApresentaConfigLoRaGNSS();
		while(flagSubmenu) {
			if(botaoOk()) {
				flagSubmenu = false;
				while(botaoOk());
			}
		}
		return;
	}
	telaErro();
}
/*=============================================================================
MENU SOLICITAR PARA O MODULO LoRa da ECU GPS
==============================================================================*/
void submenuSolicitarConfigParaEcuGps() {
	telaAguarde();
	if(solicitaConfiguracoesLoRaGNSS()) {
		uint8_t flagSubmenu = true;
		telaSucesso();
		telaApresentaConfigLoRaGNSS();
		while(flagSubmenu) {
			if(botaoOk()) {
				flagSubmenu = false;
				while(botaoOk());
			}
		}
		return;
	}
	telaErro();
}
/*=============================================================================
MENU PRINCIPAL DE CONFIGURAÇÕES FTP
==============================================================================*/
void submenuConfiguracoesLoRa() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaConfiguracoesLoRa(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_LORA) {
				submenu = 0;
			}
			telaConfiguracoesLoRa(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_LORA) {
				submenu = QUANTIDADE_MENU_LORA;
			}
			telaConfiguracoesLoRa(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1: submenuEnderecoLoRaPivot(); break;
				case 2: break; //endereco lora gnss, apenas visualização
				case 3: submenuCanalLoRa(); break;
				case 4: submenuEnviarParaLoRa(); break;
				case 5: submenuEnviarParaEcuGps(); break;
				case 6: submenuSolicitarConfigParaEcuGps(); break;
			}
			telaConfiguracoesLoRa(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
