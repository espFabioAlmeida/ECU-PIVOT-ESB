///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MainTask
//AUTOR:      Fábio Almeida
//CRIADO:     25/07/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
MAIN TASK
==============================================================================*/
void mainTask() {
	static uint8_t whileBotaoRight = false, whileBotaoLeft = false;
	if(flagEntraMenu) {
		if(contadorEntraMenu >= TIMEOUT_ENTRA_MENU) {
		  salvaLogIHM();
		  flagEntraMenu = false;
		  modoOperacao = MODO_DESLOCAMENTO;
		  sentidoMotor = MOTOR_DESLIGADO;
		  desligaTodasSaidas();
		  writeEepromOperacao();
		  menuPrincipal();
		}
		if(input(SW_OK_GPIO_Port, SW_OK_Pin)) {
		  flagEntraMenu = false;
		}
	}
	else {
		if(botaoOk()) {
		  flagEntraMenu = true;
		  flagInfoLCDPressao = false;
		  flagInfoLCDSeguranca = false;
		  flagInfoLCDGNSS = false;
		}
	}

	if(whileBotaoRight) {
		if(input(SW_RIGHT_GPIO_Port, SW_RIGHT_Pin)) {
			whileBotaoRight = false;
		}
	}
	else {
		if(botaoRight()) {
			contadorTempoInfoSinais = TEMPO_INFO_SINAIS;
			whileBotaoRight = true;
			flagAtualizaLCD = true;
		}
	}


	if(whileBotaoLeft) {
		if(input(SW_LEFT_GPIO_Port, SW_LEFT_Pin)) {
			whileBotaoLeft = false;
		}
	}
	else {
		if(botaoLeft()) {
			contadorTempoInfoSinais = 0;
			whileBotaoRight = true;
			flagAtualizaLCD = true;
		}
	}

	if(flagAtualizaLCD) {
		flagAtualizaLCD = false;
		telaOperacao();
	}

	if(flagReadRTC) {
		flagReadRTC = false;
		readRTC();
	}

	if(flagSalvaHorimetro) {
		flagSalvaHorimetro = false;
		writeEepromHorimetro();
	}

	if(flagLeituraGNSS) {
		flagLeituraGNSS = false;
		solicitaGNSS();
	}

	if(flagPacoteGNSSRS485) {
		flagPacoteGNSSRS485 = false;
		recebeGNSSRS485();
	}

	if(flagPacoteGNSSLoRa) {
		flagPacoteGNSSLoRa = false;
		recebeGNSSLoRa();
	}

	if(flagVerificaObstaculos) {
		flagVerificaObstaculos = false;
		verificaObstaculos();
		verificaPosicaoHome();
	}

	if(flagSalvaOperacao) {
		flagSalvaOperacao = false;
		writeEepromOperacao();
	}

	verificaHorarioPonta();
	entradasDigitais();
	operacao();
	acionamentoSaidas();

	//HAL_UART_Receive_IT(&huart4, &rs485DataIn, 1);
	//HAL_UART_Receive_IT(&huart2, &loraDataIn, 1);
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
