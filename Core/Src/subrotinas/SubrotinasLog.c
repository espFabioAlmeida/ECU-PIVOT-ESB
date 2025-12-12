///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasLog
//AUTOR:      Fábio Almeida
//CIRADO:     13/02/2025
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "codigosLogs.h"
/*=============================================================================
SALVA LOG
==============================================================================*/
void salvaLog(uint8_t codigo) {
	LogTypeDef log;

	log.hora = sTime.Hours;
	log.minuto = sTime.Minutes;
	log.segundo = sTime.Seconds;
	log.dia = sDate.Date;
	log.mes = sDate.Month;
	log.ano = sDate.Year;
	log.codigo = codigo;

	writeLog(log);
}
/*=============================================================================
SALVA LOG IHM
==============================================================================*/
void salvaLogIHM() {
	if(sentidoMotor != MOTOR_DESLIGADO) {
		salvaLog(LOG_PARADO_IHM);
	}
}
/*=============================================================================
SALVA LOG CHAVE
==============================================================================*/
void salvaLogChave(uint8_t sentidoAvanco) {

	if(sentidoAvanco) {
		if(flagManualDireita) {
			if(modoOperacao == MODO_DESLOCAMENTO) {
				salvaLog(LOG_DESLOCAMENTO_AVANCO_SW);
			}
			else {
				if(sentidoMotor == MOTOR_DESLIGADO) {
					salvaLog(LOG_IRRIGACAO_AVANCO_SW);
				}
			}
		}
		else {
			if(modoOperacao == MODO_DESLOCAMENTO) {
				salvaLog(LOG_PARADO_SW);
			}
		}
		return;
	}

	if(flagManualEsquerda) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_REVERSO_SW);
		}
		else {
			if(sentidoMotor == MOTOR_DESLIGADO) {
				salvaLog(LOG_IRRIGACAO_REVERSO_SW);
			}
		}
	}
	else {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_PARADO_SW);
		}
	}
}
/*=============================================================================
SALVA LOG CHAVE TROCA DE OPERAÇÃO
==============================================================================*/
void salvaLogChaveTrocaOperacao() {
	if(sentidoMotor != MOTOR_DESLIGADO) {
		salvaLog(LOG_PARADO_SW);
		return;
	}

	if(!flagIrrigacaoAguardandoInicio) { //No caso de IRRIGRAÇÃO aguardando pressão (MOTOR DESLIGADO)
		salvaLog(LOG_PARADO_SW);
	}
}
/*=============================================================================
SALVA LOG APP
==============================================================================*/
void salvaLogApp(uint8_t trocouOperacao, uint8_t sentidoAvanco) {
	if(sentidoAvanco) {
		if(trocouOperacao) {
			if(modoOperacao == MODO_DESLOCAMENTO) {
				salvaLog(LOG_DESLOCAMENTO_AVANCO_APP);
				return;
			}
			salvaLog(LOG_IRRIGACAO_AVANCO_APP);
			return;
		}

		if(sentidoMotor != MOTOR_DIREITA) {
			if(modoOperacao == MODO_DESLOCAMENTO) {
				salvaLog(LOG_DESLOCAMENTO_AVANCO_APP);
				return;
			}
			salvaLog(LOG_IRRIGACAO_AVANCO_APP);
		}
		return;
	}

	if(trocouOperacao) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_REVERSO_APP);
			return;
		}
		salvaLog(LOG_IRRIGACAO_REVERSO_APP);
		return;
	}

	if(sentidoMotor != MOTOR_ESQUERDA) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_REVERSO_APP);
			return;
		}
		salvaLog(LOG_IRRIGACAO_REVERSO_APP);
	}
}
/*=============================================================================
SALVA LOG APP DESLIGAOU
==============================================================================*/
void salvaLogAppDesligar() {
	if(sentidoMotor != MOTOR_DESLIGADO) {
		salvaLog(LOG_PARADO_APP);
	}
}
/*=============================================================================
SALVA LOG EEPROM
==============================================================================*/
void salvaLogEeprom() {
	if(sentidoMotor == MOTOR_DIREITA) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_AVANCO_EEPROM);
			return;
		}
		salvaLog(LOG_IRRIGACAO_AVANCO_EEPROM);
		return;
	}

	if(sentidoMotor == MOTOR_ESQUERDA) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_REVERSO_EEPROM);
			return;
		}
		salvaLog(LOG_IRRIGACAO_REVERSO_EEPROM);
	}
}
/*=============================================================================
SALVA LOG HORARIO PONTA
==============================================================================*/
void salvaLogHorarioPonta() {
	if(sentidoMotor == MOTOR_DIREITA) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_AVANCO_PONTA);
			return;
		}
		salvaLog(LOG_IRRIGACAO_AVANCO_PONTA);
		return;
	}

	if(sentidoMotor == MOTOR_ESQUERDA) {
		if(modoOperacao == MODO_DESLOCAMENTO) {
			salvaLog(LOG_DESLOCAMENTO_REVERSO_PONTA);
			return;
		}
		salvaLog(LOG_IRRIGACAO_REVERSO_PONTA);
	}
}
/*=============================================================================
SALVA EMERGENCIA
==============================================================================*/
void salvaLogEmergencia() {
	if(!flagEmergencia) {
		salvaLog(LOG_PARADO_EMERGENCIA);
	}
}
/*=============================================================================
SALVA BARRICADA GPS
==============================================================================*/
void salvaLogBarricadaGPS() {
	if(!flagBarricadaGPS) {
		salvaLog(LOG_PARADO_BARRICADA_GPS);
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
