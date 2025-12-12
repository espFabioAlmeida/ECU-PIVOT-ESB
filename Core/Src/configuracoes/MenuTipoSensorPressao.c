///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuTipoSensorPressao
//AUTOR:      Fábio Almeida
//CIRADO:     13/11/2025
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
TEMPO PRESSURIZAÇÃO
==============================================================================*/
void submenuTipoSensorPressao() {
	uint8_t flagSubmenu = true, tipo = tipoSensorPressao;

	telaAjusteTipoSensorPressao(tipo);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			if(tipo == SENSOR_10BAR) {
				tipo = SENSOR_16BAR;
			}
			else {
				tipo = SENSOR_10BAR;
			}
			telaAjusteTipoSensorPressao(tipo);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
			tipoSensorPressao = tipo;
			writeEepromTipoSensorPressao();
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
