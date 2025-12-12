///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuCarregaParametrosFabrica
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CARREGA PARAMETROS DE FABRICA
==============================================================================*/
void submenuCarregaParametrosFabrica() {
	uint8_t flagSubmenu = true;
	uint8_t unid = false;

	telaConfirmacao(unid);

	while(flagSubmenu) {
		if(botaoLeft()) {
			unid = 1;
			telaConfirmacao(unid);
			lcdPosLc(2, 1);
			while(botaoLeft());
		}

		if(botaoRight()) {
			unid = 0;
			telaConfirmacao(unid);
			lcdPosLc(2, 1);
			while(botaoRight());
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			if(unid) {
				telaAguarde();
				apagaEeprom();
				HAL_Delay(1000);
			}
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
