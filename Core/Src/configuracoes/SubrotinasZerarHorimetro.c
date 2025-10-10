///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuZerarHorimetro
//AUTOR:      Fábio Almeida
//CIRADO:     31/07/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
ZERAR HORIMETRO
==============================================================================*/
void submenuZerarHorimetro() {
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
				horimetro.horas = 0;
				horimetro.minutos = 0;
				horimetro.segundos = 0;
				writeEepromHorimetro();
			}
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
