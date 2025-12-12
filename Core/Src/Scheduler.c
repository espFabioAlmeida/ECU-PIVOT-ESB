///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    Scheduler
//AUTOR:      Fábio Almeida
//CRIADO:     04/08/2023
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
# include "main.h"
# include "global.h"
/*==============================================================================
TAREFAS 100us
==============================================================================*/
void tarefas100us() {

}
/*==============================================================================
TAREFAS 1ms
==============================================================================*/
void tarefas1ms() {

}
/*==============================================================================
TAREFAS 10ms
==============================================================================*/
void tarefas10ms() {
	static uint8_t contadorPacoteRS485 = 0;

	if(flagPacoteRS485) {
		contadorPacoteRS485 ++;
		if(contadorPacoteRS485 >= timeoutRS485) {
			contadorPacoteRS485 = 0;
			flagPacoteRS485 = false;
			flagPacoteGNSSRS485 = true;
		}
	}
}
/*==============================================================================
TAREFAS 100ms
==============================================================================*/
void tarefas100ms() {
	static uint8_t contadorLedCPU = 0;
	reiniciaWatchDog();

	flagReadRTC = true;

	if(flagBuzzer) {
		flagBuzzer = false;
		on(BUZZER_GPIO_Port, BUZZER_Pin);
	}
	else {
		off(BUZZER_GPIO_Port, BUZZER_Pin);
	}

	if(flagLedCOM) {
		flagLedCOM = false;
		off(LED_COM_GPIO_Port, LED_COM_Pin);
	}
	else {
		on(LED_COM_GPIO_Port, LED_COM_Pin);
	}

	if(flagEntraMenu) {
		contadorEntraMenu ++;
	}
	else {
		contadorEntraMenu = 0;
	}

	if(contadorLaminaDagua < setPointContadorLaminaDagua) {
		if(contadorLaminaDagua) {
			contadorLaminaDagua --;
		}
	}

	if(contadorLaminaDaguaDesligado < setPointContadorLaminaDaguaDesligado) {
		if(contadorLaminaDaguaDesligado) {
			contadorLaminaDaguaDesligado --;
		}
	}

	if(contadorVerificaSeguranca < TEMPO_VERIFICA_SEGURANCA) {
		if(contadorVerificaSeguranca) {
			contadorVerificaSeguranca --;
		}
	}

	if(contadorReversaoMotor < TEMPO_REVERSAO_MOTOR) {
		if(contadorReversaoMotor) {
			contadorReversaoMotor --;
		}
	}

	if(contadorPulsoReleOn < TEMPO_PULSO_RELE_ON) {
		if(contadorPulsoReleOn) {
			contadorPulsoReleOn --;
		}
	}

	if(contadorTimeoutGPRS) {
		contadorTimeoutGPRS --;
	}

	if(contadorTimeoutLoRa) {
		contadorTimeoutLoRa --;
	}

	if(contadorTimeoutLoRaGateway) {
		contadorTimeoutLoRaGateway --;
	}

	if(contadorAguardaPosicaoObstaculo) {
		contadorAguardaPosicaoObstaculo --;
	}

	if(contadorTempoInfoSinais) {
		contadorTempoInfoSinais --;
	}

	if(contadorTimeoutGNSS) {
		contadorTimeoutGNSS --;
	}

	if(contadorTimeoutConfiguraGNSS) {
		contadorTimeoutConfiguraGNSS --;
	}

	if(contadorTimeoutWiFi) {
		contadorTimeoutWiFi --;
	}

	if(contadorTimeoutDadosWiFi) {
		contadorTimeoutDadosWiFi ++;
	}

	if(contadorTimeoutBG95) {
		contadorTimeoutBG95 --;
	}

	contadorLedCPU ++;
	if(contadorLedCPU >= 10) {
		contadorLedCPU = 0;
	}

	toggle(LED_CPU_GPIO_Port, LED_CPU_Pin);
}
/*==============================================================================
TAREFAS 1s
==============================================================================*/
void tarefas1s() {
	flagAtualizaLCD = true;
	flagSalvaOperacao = true;

	if(contadorBombaPrincipal < tempoPressurizacaoSegundos) {
		if(contadorBombaPrincipal) {
			contadorBombaPrincipal --;
		}
	}

	if(!flagMenu) {
		if(contadorReporteService < TEMPO_SERVICE) {
			contadorReporteService ++;
		}
	}
	else {
		contadorReporteService = 0;
	}

	if(flagContaHorimetro) {
		horimetro.segundos ++;
		if(horimetro.segundos >= 60) {
			horimetro.segundos = 0;
			horimetro.minutos ++;
			if(horimetro. minutos >= 60) {
				horimetro.minutos = 0;
				horimetro.horas ++;
				if(horimetro.horas > TEMPO_MAXIMO_HORIMETRO) {
					horimetro.horas = 0;
				}
			}
			flagSalvaHorimetro = true;
		}
	}

	if(contadorIniciaVerificacaoHorarioPonta < TEMPO_VERIFICA_HORARIO_PONTA) {
		contadorIniciaVerificacaoHorarioPonta ++;
	}

	flagVerificaObstaculos = true;

	if(!contadorTimeoutGNSS) {
		flagLeituraGNSS = true;
	}

	if(flagLeituraGNSSRecente) {
		contadorTimeoutGNSSRecente ++;
		if(contadorTimeoutGNSSRecente >= TIMEOUT_GNSS_RECENTE) {
			contadorTimeoutGNSSRecente = 0;
			flagLeituraGNSSRecente = false;
		}
	}
}
/*==============================================================================
SCHEDULER
==============================================================================*/
void schedulerEngine() {
	static uint8_t conta100us = 0, conta1ms = 0, conta10ms = 0, conta100ms = 0;
	//Essa rotina deve ser chamada através de um timer configurado em 100us

	tarefas100us();
	conta100us ++;

	if(conta100us >= 10) {
		conta100us = 0;
		tarefas1ms();
		conta1ms ++;

		if(conta1ms >= 10) {
			conta1ms = 0;
			tarefas10ms();
			conta10ms ++;

			if(conta10ms >= 10) {
				conta10ms = 0;
				tarefas100ms();
				conta100ms ++;

				if(conta100ms >= 10) {
					conta100ms = 0;
					tarefas1s();
				}
			}
		}
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
