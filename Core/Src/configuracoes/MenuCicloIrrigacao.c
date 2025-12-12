///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuCicloIrrigacao
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CICLO IRRIGACAO
==============================================================================*/
void submenuCicloIrrigacao() {
	uint8_t flagSubmenu = true;
	uint8_t unid = cicloIrrigacao;

	telaCicloIrrigacao(unid);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			if(unid == IRRIGACAO_1_CICLO) {
				unid = IRRIGACAO_CONTINUO;
			}
			else {
				unid = IRRIGACAO_1_CICLO;
			}
			telaCicloIrrigacao(unid);
			lcdPosLc(2, 1);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			cicloIrrigacao = unid;
			writeEepromCicloIrrigacao();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
