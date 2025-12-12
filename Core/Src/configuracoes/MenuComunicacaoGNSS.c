///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuComunicacaoGNSS
//AUTOR:      Fábio Almeida
//CIRADO:     15/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
SELECIONAR COMUNICACAO GNSS
==============================================================================*/
void submenuComunicacaoGNSS() {
	uint8_t flagSubmenu = true, valor = comunicacaoGNSS;

	telaComunicacaoECUGPS(valor);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			if(valor == GNSS_485) {
				valor = GNSS_RF;
			}
			else {
				valor = GNSS_485;
			}
			telaComunicacaoECUGPS(valor);
			lcdPosLc(2, 1);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			comunicacaoGNSS = valor;
			writeEepromComunicacaoGNSS();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
