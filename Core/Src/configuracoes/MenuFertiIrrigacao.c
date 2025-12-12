///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuFertiIrrigacao
//AUTOR:      Fábio Almeida
//CIRADO:     07/06/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE FERTI IRRIGACAO
==============================================================================*/
void submenuFertiIrrigacao() {
	uint8_t flagSubmenu = true, unid = flagFertiIrrigacao;

	telaFertiIrrigacao(unid);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			if(unid == true) {
				unid = false;
			}
			else {
				unid = true;
			}
			telaFertiIrrigacao(unid);
			lcdPosLc(2, 1);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			flagFertiIrrigacao = unid;
			writeEepromFertiIrrigacao();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
