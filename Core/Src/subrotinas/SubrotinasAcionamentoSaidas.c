///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasAcionamentoSaidas
//AUTOR:      Fábio Almeida
//CIRADO:     25/04/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
DESLIGA TODAS AS SAIDAS
==============================================================================*/
void desligaTodasSaidas() {
	off(RELE1_GPIO_Port, RELE1_Pin);
	off(RELE2_GPIO_Port, RELE2_Pin);
	off(RELE3_GPIO_Port, RELE3_Pin);
	off(RELE4_GPIO_Port, RELE4_Pin);
	off(RELE5_GPIO_Port, RELE5_Pin);
	off(RELE6_GPIO_Port, RELE6_Pin);
	off(RELE7_GPIO_Port, RELE7_Pin);
	off(RELE8_GPIO_Port, RELE8_Pin);
	off(RELE9_GPIO_Port, RELE9_Pin);
	off(RELE10_GPIO_Port, RELE10_Pin);
	for(uint8_t i = 0; i < QUANTIDADE_RELES; i ++) {
		statusReles[i] = false;
	}
}
/*=============================================================================
ACIONAMENTO MOTOR
==============================================================================*/
void acionamentoSaidasMotor() {
	static uint8_t ultimoSentido = MOTOR_DESLIGADO;

	if(sentidoMotor == MOTOR_DESLIGADO) {
		off(RELE9_GPIO_Port, RELE9_Pin);
		on(RELE10_GPIO_Port, RELE10_Pin);
		off(RELE3_GPIO_Port, RELE3_Pin);
		off(RELE4_GPIO_Port, RELE4_Pin);
		contadorReversaoMotor = TEMPO_REVERSAO_MOTOR;
		contadorPulsoReleOn = TEMPO_PULSO_RELE_ON;
		ultimoSentido = MOTOR_DESLIGADO;
		statusReles[8] = false;
		statusReles[9] = true;
		statusReles[2] = false;
		statusReles[3] = false;
		return;
	}
	else if(sentidoMotor == MOTOR_DIREITA) {
		if(ultimoSentido == MOTOR_ESQUERDA) {
			off(RELE10_GPIO_Port, RELE10_Pin);
			off(RELE3_GPIO_Port, RELE3_Pin);
			off(RELE4_GPIO_Port, RELE4_Pin);
			statusReles[9] = false;
			statusReles[2] = false;
			statusReles[3] = false;
			if(contadorReversaoMotor == TEMPO_REVERSAO_MOTOR) {
				contadorReversaoMotor = TEMPO_REVERSAO_MOTOR - 1;
			}
			if(!contadorReversaoMotor) {
				on(RELE4_GPIO_Port, RELE4_Pin);
				statusReles[3] = false;
				ultimoSentido = MOTOR_DIREITA;
			}
			return;
		}
		if(contadorPulsoReleOn == TEMPO_PULSO_RELE_ON) {
			contadorPulsoReleOn --;
		}
		if(contadorPulsoReleOn) {
			on(RELE9_GPIO_Port, RELE9_Pin);
			statusReles[8] = true;
		}
		else {
			off(RELE9_GPIO_Port, RELE9_Pin);
			statusReles[8] = false;
		}
		off(RELE10_GPIO_Port, RELE10_Pin);
		off(RELE3_GPIO_Port, RELE3_Pin);
		on(RELE4_GPIO_Port, RELE4_Pin);
		statusReles[9] = false;
		statusReles[2] = false;
		statusReles[3] = true;
		contadorReversaoMotor = TEMPO_REVERSAO_MOTOR;
		ultimoSentido = MOTOR_DIREITA;
		return;
	}

	if(ultimoSentido == MOTOR_DIREITA) {
		off(RELE10_GPIO_Port, RELE10_Pin);
		off(RELE3_GPIO_Port, RELE3_Pin);
		off(RELE4_GPIO_Port, RELE4_Pin);
		statusReles[9] = false;
		statusReles[2] = false;
		statusReles[3] = false;
		if(contadorReversaoMotor == TEMPO_REVERSAO_MOTOR) {
			contadorReversaoMotor = TEMPO_REVERSAO_MOTOR - 1;
		}
		if(!contadorReversaoMotor) {
			on(RELE3_GPIO_Port, RELE3_Pin);
			statusReles[2] = true;
			ultimoSentido = MOTOR_ESQUERDA;
		}
		return;
	}

	if(contadorPulsoReleOn == TEMPO_PULSO_RELE_ON) {
		contadorPulsoReleOn --;
	}
	if(contadorPulsoReleOn) {
		on(RELE9_GPIO_Port, RELE9_Pin);
		statusReles[8] = true;
	}
	else {
		off(RELE9_GPIO_Port, RELE9_Pin);
		statusReles[8] = false;
	}

	off(RELE10_GPIO_Port, RELE10_Pin);
	on(RELE3_GPIO_Port, RELE3_Pin);
	off(RELE4_GPIO_Port, RELE4_Pin);
	statusReles[9] = false;
	statusReles[2] = true;
	statusReles[3] = false;
	contadorReversaoMotor = TEMPO_REVERSAO_MOTOR;
	ultimoSentido = MOTOR_ESQUERDA;
}
/*=============================================================================
ACIONAMENTO DAS SAIDAS
==============================================================================*/
void acionamentoSaidas() {
	setPin(RELE1_GPIO_Port, RELE1_Pin, flagAcionamentoContatoAuxiliar);
	statusReles[0] = flagAcionamentoContatoAuxiliar;

	acionamentoSaidasMotor();

	if(modoOperacao == MODO_IRRIGACAO) {
		if(flagIrrigacaoAguardandoPressao || flagIrrigacaoAguardandoInicio) {
			setPin(RELE6_GPIO_Port, RELE6_Pin, flagAcionamentoBombaPrincipal);
			setPin(RELE7_GPIO_Port, RELE7_Pin, flagAcionamentoBombaPrincipal);
			off(RELE8_GPIO_Port, RELE8_Pin);
			off(RELE2_GPIO_Port, RELE2_Pin);
			off(RELE5_GPIO_Port, RELE5_Pin);
			statusReles[5] = flagAcionamentoBombaPrincipal;
			statusReles[6] = flagAcionamentoBombaPrincipal;
			statusReles[7] = false;
			statusReles[1] = false;
			statusReles[4] = false;
			return;
		}
		setPin(RELE8_GPIO_Port, RELE8_Pin, flagFertiIrrigacao);
		if(flagOverrideBombaBooster) {
			on(RELE2_GPIO_Port, RELE2_Pin);
			statusReles[1] = true;
		}
		else {
			setPin(RELE2_GPIO_Port, RELE2_Pin, flagBombaBooster);
			statusReles[1] = flagBombaBooster;
		}
		setPin(RELE5_GPIO_Port, RELE5_Pin, flagAcionamentoPercentimetro);
		setPin(RELE6_GPIO_Port, RELE6_Pin, flagAcionamentoBombaPrincipal);
		setPin(RELE7_GPIO_Port, RELE7_Pin, flagAcionamentoBombaPrincipal);
		statusReles[7] = flagFertiIrrigacao;
		statusReles[4] = flagAcionamentoPercentimetro;
		statusReles[5] = flagAcionamentoBombaPrincipal;
		statusReles[6] = flagAcionamentoBombaPrincipal;
		return;
	}

	off(RELE8_GPIO_Port, RELE8_Pin);
	off(RELE2_GPIO_Port, RELE2_Pin);
	setPin(RELE5_GPIO_Port, RELE5_Pin, flagAcionamentoPercentimetro);
	off(RELE6_GPIO_Port, RELE6_Pin);
	off(RELE7_GPIO_Port, RELE7_Pin);

	statusReles[7] = false;
	statusReles[1] = false;
	statusReles[4] = flagAcionamentoPercentimetro;
	statusReles[5] = false;
	statusReles[6] = false;
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
