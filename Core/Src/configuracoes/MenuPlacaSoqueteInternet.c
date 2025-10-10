///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuPlacaSoqueteInternet
//AUTOR:      Fábio Almeida
//CIRADO:     19/11/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
PLACA SOQUETE INTERNET
==============================================================================*/
void submenuPlacaSoqueteInternet() {
	uint8_t flagSubmenu = true;
	uint8_t valor = placaSoquete;

	telaPlacaSoquete(valor);

	while(flagSubmenu) {
		if(botaoUp()) {
			valor ++;
			if(valor > SOQUETE_RADIO) {
				valor = SOQUETE_GPRS;
			}

			telaPlacaSoquete(valor);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			valor --;
			if(valor > SOQUETE_RADIO) {
				valor = SOQUETE_RADIO;
			}

			telaPlacaSoquete(valor);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}


		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			placaSoquete = valor;
			configuraVelocidadeUART3();
			writeEepromPlacaSoquete();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
