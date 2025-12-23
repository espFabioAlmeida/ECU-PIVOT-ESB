///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasEntradasDigitais
//AUTOR:      Fábio Almeida
//CIRADO:     25/04/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "codigosLogs.h"
/*=============================================================================
LEITURA FIM DE CURSO
==============================================================================*/
void leituraFimCurso() {
	if(flagFimCursoDireita) {
		if(debounceInverso(IN1_GPIO_Port, IN1_Pin)) {
			flagFimCursoDireita = false;
		}
	}
	else {
		if(debounce(IN1_GPIO_Port, IN1_Pin)) {
			flagFimCursoDireita = true;
		}
	}

	if(flagFimCursoEsquerda) {
		if(debounceInverso(IN2_GPIO_Port, IN2_Pin)) {
			flagFimCursoEsquerda = false;
		}
	}
	else {
		if(debounce(IN2_GPIO_Port, IN2_Pin)) {
			flagFimCursoEsquerda = true;
		}
	}

	statusEntradasDigitais[0] = flagFimCursoDireita;
	statusEntradasDigitais[1] = flagFimCursoEsquerda;
}
/*=============================================================================
LEITURA SEGURANCA
==============================================================================*/
void leituraSeguranca() {
	static uint8_t flagEntradaSeguranca1 = false, flagEntradaSeguranca2 = false;

	if(flagEntradaSeguranca1) {
		if(debounce(IN3_GPIO_Port, IN3_Pin)) {
			flagEntradaSeguranca1 = false;
		}
	}
	else {
		if(debounceInverso(IN3_GPIO_Port, IN3_Pin)) {
			flagEntradaSeguranca1 = true;
		}
	}

	if(flagEntradaSeguranca2) {
		if(debounce(IN4_GPIO_Port, IN4_Pin)) {
			flagEntradaSeguranca2 = false;
		}
	}
	else {
		if(debounceInverso(IN4_GPIO_Port, IN4_Pin)) {
			flagEntradaSeguranca2 = true;
		}
	}

	if(flagEntradaSeguranca1 || flagEntradaSeguranca2) {
		flagFalhaSeguranca = true;
	}
	else {
		flagFalhaSeguranca = false;
	}

	statusEntradasDigitais[2] = !flagEntradaSeguranca1;
	statusEntradasDigitais[3] = !flagEntradaSeguranca2;
}
/*=============================================================================
LEITURA ACIONAMENTO MANUAL
==============================================================================*/
void leituraAcionamentoManual() {
	static uint8_t flagWhileManualDireita = false;

	if(flagWhileManualDireita) {
		if(input(IN6_GPIO_Port, IN6_Pin)) {
			flagWhileManualDireita = false;
		}
	}
	else {
		if(debounce(IN6_GPIO_Port, IN6_Pin)) {
			flagWhileManualDireita = true;
			flagManualDireita = !flagManualDireita;
			salvaLogChave(true);
			flagManualEsquerda = false;
			flagInfoLCDPressao = false;
			flagInfoLCDSeguranca = false;
			flagInfoLCDGNSS = false;
		}
	}

	/* --Somente há manual para a direita
	if(flagWhileManualEsquerda) {
		if(input(IN10_GPIO_Port, IN10_Pin)) {
			flagWhileManualEsquerda = false;
		}
	}
	else {
		if(debounce(IN10_GPIO_Port, IN10_Pin)) {
			flagWhileManualEsquerda = true;
			flagManualEsquerda = !flagManualEsquerda;
			salvaLogChave(false);
			flagManualDireita = false;
			flagInfoLCDPressao = false;
			flagInfoLCDSeguranca = false;
			flagInfoLCDGNSS = false;
		}
	}
	*/

	statusEntradasDigitais[5] = flagWhileManualDireita;
}
/*=============================================================================
LEITURA TROCA DE OPERACAO
==============================================================================*/
void leituraTrocaModoOperacao() {
	static uint8_t flagWhileTrocaModoOperacao = false;

	if(flagWhileTrocaModoOperacao) {
		if(input(IN5_GPIO_Port, IN5_Pin)) {
			flagWhileTrocaModoOperacao = false;
		}
	}
	else {
		if(debounce(IN5_GPIO_Port, IN5_Pin)) {
			flagWhileTrocaModoOperacao = true;
			if(modoOperacao == MODO_IRRIGACAO) {
				modoOperacao = MODO_DESLOCAMENTO;
			}
			else {
				modoOperacao = MODO_IRRIGACAO;
			}

			salvaLogChaveTrocaOperacao();
			flagFalhaPressao = false;
			flagInfoLCDPressao = false;
			flagInfoLCDSeguranca = false;
			flagInfoLCDGNSS = false;
			flagOverrideBombaBooster = false;
			writeEepromBombaBooster();
			reiniciaVeriaveisProcesso();
		}
	}

	statusEntradasDigitais[4] = flagWhileTrocaModoOperacao;
}
/*=============================================================================
LEITURA DAS ENTRADAS DIGITAIS
==============================================================================*/
void entradasDigitais() {
	leituraFimCurso();
	leituraSeguranca();
	leituraAcionamentoManual();
	leituraTrocaModoOperacao();
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
