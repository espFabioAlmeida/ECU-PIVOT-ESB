///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAlarmePressao
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TEMPO_PRESS_AND_HOLD 25
/*=============================================================================
VISUALIZA LOG
==============================================================================*/
uint16_t submenuVisualizaLog(uint16_t registro) {
	uint8_t flagSubmenu = true;

	telaVisualizaLog();
	while(flagSubmenu) {
		if(botaoUp()) {
			registro ++;
			if(registro >= QUANTIDADE_REGISTROS_LOG) {
				registro = 0;
			}
			readLog(registro);
			telaVisualizaLog();
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			registro --;
			if(registro >= QUANTIDADE_REGISTROS_LOG) {
				registro = QUANTIDADE_REGISTROS_LOG - 1;
			}

			readLog(registro);
			telaVisualizaLog();
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			flagSubmenu = false;
		}
	}

	return registro;
}
/*=============================================================================
SELECIONA LOG
==============================================================================*/
void submenuSelecionaLog() {
	uint8_t flagSubmenu = true;
	uint8_t contaPressAndHold = 0;
	uint16_t passoRegistro = 1, registro = 0;
	readLog(registro);

	telaSelecionaLog(registro + 1, passoRegistro);

	while(flagSubmenu) {
		if(botaoUp()) {
			registro += passoRegistro;
			if(registro >= QUANTIDADE_REGISTROS_LOG) {
				registro = 0;
			}

			readLog(registro);
			telaSelecionaLog(registro + 1, passoRegistro);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			registro -= passoRegistro;
			if(registro >= QUANTIDADE_REGISTROS_LOG) {
				registro = QUANTIDADE_REGISTROS_LOG - 1;
			}

			readLog(registro);
			telaSelecionaLog(registro + 1, passoRegistro);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(passoRegistro > 1) {
				passoRegistro /= 10;
				telaSelecionaLog(registro + 1, passoRegistro);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(passoRegistro < 1000) {
				passoRegistro *= 10;
				telaSelecionaLog(registro + 1, passoRegistro);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			contaPressAndHold ++;
			if(contaPressAndHold >= TEMPO_PRESS_AND_HOLD) {
				contaPressAndHold = 0;
				flagSubmenu = false;
				flagBuzzer = true;
				while(botaoOk());
			}
		}
		else {
			if(contaPressAndHold) {
				registro = submenuVisualizaLog(registro);
				telaSelecionaLog(registro + 1, passoRegistro);
			}
			contaPressAndHold = 0;
		}
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
