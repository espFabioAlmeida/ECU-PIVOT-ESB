///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuConfiguracoesLoRaGateway
//AUTOR:      Fábio Almeida
//CIRADO:     21/01/2025
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define QUANTIDADE_MENU_LORA_GATEWAY 4
/*=============================================================================
ENDERECO PIVOT/GATEWAY
==============================================================================*/
void submenuEnderecoLoRaPivotGateway() {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t hi = make8(enderecoLoRaPivotGateway, 1), lo = make8(enderecoLoRaPivotGateway, 0);
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
				enderecoLoRaPivotGateway = make16(hi, lo);

				if(!enderecoLoRaPivotGateway) { //0 é broadcast
					enderecoLoRaPivotGateway = 0x0010;
				}

				enderecoLoRaGateway = enderecoLoRaPivotGateway + 2;
				writeEepromEnderecoLoRaGateway();
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
CANAL LoRa GATEWAY
==============================================================================*/
void submenuCanalLoRaGateway() {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t valor = canalLoRaGateway;
	uint8_t nibbleSuperior = canalLoRaGateway & 0xF0, nibbleInferior = canalLoRaGateway & 0x0F;

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
				canalLoRaGateway = valor;
				writeEepromCanalLoRaGatway();
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
MENU ENVIAR PARA O MODULO LoRa GATEWAY
==============================================================================*/
void submenuEnviarParaLoRaGateway() {
	if(placaSoquete != SOQUETE_RADIO) {
		telaVerifiquePlacaSoquete();
		return;
	}
	telaAguarde();
	if(configuraLoRaGateway()) {
		telaSucesso();
		return;
	}
	telaErro();
}
/*=============================================================================
CONFIGURAÇÕES LORA/GATEWAY
==============================================================================*/
void submenuConfiguracoesLoRaGateway() {
	uint8_t flagSubmenu = true, submenu = 1;

	telaConfiguracoesLoRaGateway(submenu);

	while(flagSubmenu) {
		if(botaoUp()) {
			submenu ++;
			if(submenu > QUANTIDADE_MENU_LORA_GATEWAY) {
				submenu = 0;
			}
			telaConfiguracoesLoRaGateway(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			submenu --;
			if(submenu > QUANTIDADE_MENU_LORA_GATEWAY) {
				submenu = QUANTIDADE_MENU_LORA_GATEWAY;
			}
			telaConfiguracoesLoRaGateway(submenu);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			switch(submenu) {
				case 0: flagSubmenu = false; break;
				case 1: submenuEnderecoLoRaPivotGateway(); break;
				case 2: break; //endereco lora gateway, apenas visualização
				case 3: submenuCanalLoRaGateway(); break;
				case 4: submenuEnviarParaLoRaGateway(); break;
			}
			telaConfiguracoesLoRaGateway(submenu);
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
