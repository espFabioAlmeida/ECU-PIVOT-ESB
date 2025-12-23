///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    Subrotinas Operacao
//AUTOR:      Fábio Almeida
//CIRADO:     05/02/2021
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "codigosLogs.h"
/*=============================================================================
REINICIA VARIAVEIS DE PROCESSO
==============================================================================*/
void reiniciaVeriaveisProcesso() {
	flagManualEsquerda = false;
	flagManualDireita = false;
	flagRetornoHome = false;
	sentidoMotor = MOTOR_DESLIGADO;
	agendaHorarioAcionado = 99;

	contadorLaminaDagua = setPointContadorLaminaDagua;
	contadorLaminaDaguaDesligado = setPointContadorLaminaDaguaDesligado;
	contadorBombaPrincipal = tempoPressurizacaoSegundos;
	contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA;
	flagIrrigacaoAguardandoInicio = true;
	flagIrrigacaoAguardandoPressao = true;
	flagAcionamentoContatoAuxiliar = false;
	flagAcionamentoPercentimetro = false;
	flagAcionamentoBombaPrincipal = false;
	flagContaHorimetro = false;
	flagOperacao = false;
	flagBombaBooster = false;
	flagTrocouLaminaDagua = false;
}
/*=============================================================================
GRAVA POSICAO INICIAL PROCESSO
==============================================================================*/
void gravaPosicaoInicialProcesso() {
	if(flagAcionamentoPelaEeprom) {
		flagAcionamentoPelaEeprom = false;
		return;
	}
	posicaoInicioOperacao = posicaoAtualGPS;
	flagPosicaoInicioOperacaoConfiavel = flagLeituraGNSSRecente;
	writeEepromPosicaoInicioOperacao();
}
/*=============================================================================
VERIFICA PRESSAO
==============================================================================*/
uint8_t verificaPressao() {
	if(pressao > alarmePressao) {
		return true;
	}

	if(alarmePressao >= histereseAlarmePressao) { //histerese menor que o setpoint (o correto)
		if(pressao <  alarmePressao - histereseAlarmePressao) {
			return false;
		}
	}
	else { //ignora a histerese caso for maior que o proprio setpoint
		if(pressao < alarmePressao) {
			return false;
		}
	}

	if(flagIrrigacaoAguardandoPressao) {
		return false;
	}

	return true;
}
/*=============================================================================
CALCULA CONTADOR LAMINA DAGUA
==============================================================================*/
void calculaLaminaDagua() {
	uint32_t calculo = 0;

	if(laminaDagua < tabelaLaminaDagua70) {
		calculo = map(laminaDagua, 0, tabelaLaminaDagua70, 1000, 700);
	}
	else if(laminaDagua < tabelaLaminaDagua50) {
		calculo = map(laminaDagua, tabelaLaminaDagua70, tabelaLaminaDagua50, 700, 500);
	}
	else if(laminaDagua < tabelaLaminaDagua40) {
		calculo = map(laminaDagua, tabelaLaminaDagua50, tabelaLaminaDagua40, 500, 400);
	}
	else if(laminaDagua < tabelaLaminaDagua30) {
		calculo = map(laminaDagua, tabelaLaminaDagua40, tabelaLaminaDagua30, 400, 300);
	}
	else if(laminaDagua < tabelaLaminaDagua20) {
		calculo = map(laminaDagua, tabelaLaminaDagua30, tabelaLaminaDagua20, 300, 200);
	}
	else if(laminaDagua < tabelaLaminaDagua12) {
		calculo = map(laminaDagua, tabelaLaminaDagua20, tabelaLaminaDagua12, 200, 120);
	}
	else {
		calculo = map(laminaDagua, tabelaLaminaDagua12, tabelaLaminaDagua05, 120, 50);
	}


	if(calculo > 1000) { //estourou
		calculo = 0;
	}

	calculo = map(calculo, 0, 1000, 0, tempoBaseLaminaDagua); //0-100% / 0-tempo base em 100ms (600 = 60s)

	setPointContadorLaminaDagua = calculo;
	setPointContadorLaminaDaguaDesligado = tempoBaseLaminaDagua - setPointContadorLaminaDagua;
	contadorLaminaDagua = setPointContadorLaminaDagua;
	contadorLaminaDaguaDesligado = setPointContadorLaminaDaguaDesligado;
}
/*=============================================================================
OPERACAO DESLOCAMENTO A SECO
==============================================================================*/
void operacaoDeslocamentoSeco() {
	static uint8_t gravaPosicaoInicial = true;
	if(flagFimCursoDireita) {
		if(flagManualDireita) {
			salvaLog(LOG_PARADO_FIM_CURSO);
			sentidoMotor = MOTOR_DESLIGADO;
			flagAcionamentoContatoAuxiliar = false;
		}
		flagManualDireita = false;
	}
	if(flagFimCursoEsquerda) {
		if(flagManualEsquerda) {
			salvaLog(LOG_PARADO_FIM_CURSO);
			sentidoMotor = MOTOR_DESLIGADO;
			flagAcionamentoContatoAuxiliar = false;
		}
		flagManualEsquerda = false;
	}

	if(flagRetornoHome) {
		if(flagHomeEncontrado || contadorErroGNSS >= QUANTIDADE_ERRO_GNSS) {
			if(contadorErroGNSS >= QUANTIDADE_ERRO_GNSS) {
				flagInfoLCDGNSS = true;
				salvaLog(LOG_PARADO_FALTA_GNSS);
			}
			else {
				salvaLog(LOG_PARADO_HOME);
			}
			flagRetornoHome = false;
			sentidoMotor = MOTOR_DESLIGADO;
			flagAcionamentoContatoAuxiliar = false;
			flagManualDireita = false;
			flagManualEsquerda = false;
		}
	}

	if(flagManualDireita) {
		flagAcionamentoContatoAuxiliar = true;
		sentidoMotor = MOTOR_DIREITA;
		flagAcionamentoPercentimetro = true;
		flagFalhaPressao = false;
		flagContaHorimetro = true;
		flagOperacao = true;
		if(gravaPosicaoInicial) {
			gravaPosicaoInicialProcesso();
		}
		gravaPosicaoInicial = false;
	}

	if(flagManualEsquerda) {
		flagAcionamentoContatoAuxiliar = true;
		sentidoMotor = MOTOR_ESQUERDA;
		flagAcionamentoPercentimetro = true;
		flagFalhaPressao = false;
		flagContaHorimetro = true;
		flagOperacao = true;
		if(gravaPosicaoInicial) {
			gravaPosicaoInicialProcesso();
		}
		gravaPosicaoInicial = false;
	}

	if(!flagManualDireita && !flagManualEsquerda) {
		gravaPosicaoInicial = true;
		reiniciaVeriaveisProcesso();
	}
	else {
		if(contadorVerificaSeguranca == TEMPO_VERIFICA_SEGURANCA) {
			contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA - 1;
		}
	}
}
/*=============================================================================
OPERACAO IRRIGAÇÃO
==============================================================================*/
void operacaoIrrigacao() {
	static uint8_t ultimoSentido = MOTOR_DESLIGADO;
	static uint8_t flagInicioMovimento = true;
	static uint8_t gravaPosicaoInicial = true;
	if(flagIrrigacaoAguardandoInicio) {
		contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA;
		flagAcionamentoContatoAuxiliar = false;
		flagBombaBooster = false;
		if(!flagFimCursoDireita || cicloIrrigacao == IRRIGACAO_CONTINUO) {
			if(flagManualDireita) {
				ultimoSentido = MOTOR_DIREITA;
				sentidoMotor = MOTOR_DESLIGADO;
				flagIrrigacaoAguardandoInicio = false;
				flagAcionamentoPercentimetro = false;
				flagAcionamentoBombaPrincipal = true;
				contadorBombaPrincipal = tempoPressurizacaoSegundos - 1;
			}
		}
		else {
			if(flagManualDireita) {
				salvaLog(LOG_PARADO_FIM_CURSO);
				sentidoMotor = MOTOR_DESLIGADO;
			}
			flagManualDireita = false;
		}

		if(!flagFimCursoEsquerda || cicloIrrigacao == IRRIGACAO_CONTINUO) {
			if(flagManualEsquerda) {
				ultimoSentido = MOTOR_ESQUERDA;
				sentidoMotor = MOTOR_DESLIGADO;
				flagIrrigacaoAguardandoInicio = false;
				flagAcionamentoPercentimetro = false;
				flagAcionamentoBombaPrincipal = true;
				contadorBombaPrincipal = tempoPressurizacaoSegundos - 1;
			}
		}
		else {
			if(flagManualEsquerda) {
				salvaLog(LOG_PARADO_FIM_CURSO);
				sentidoMotor = MOTOR_DESLIGADO;
			}
			flagManualEsquerda = false;
		}

		gravaPosicaoInicial = true;
		return;
	}

	if(gravaPosicaoInicial) {
		gravaPosicaoInicialProcesso();
		gravaPosicaoInicial = false;
	}

	flagAcionamentoContatoAuxiliar = true;
	flagManualDireita = false;
	flagManualEsquerda = false;
	flagFalhaPressao = false;
	flagContaHorimetro = true;
	flagOperacao = true;

	if(flagIrrigacaoAguardandoPressao) {
		flagBombaBooster = false;
		if(verificaPressao() || !contadorBombaPrincipal) {
			flagIrrigacaoAguardandoPressao = false;
			contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA - 1;
			flagInicioMovimento = true;
		}
		return;
	}
	else {
		if(!verificaPressao()) {
			if(contadorBombaPrincipal) {
				flagIrrigacaoAguardandoPressao = true;
				sentidoMotor = MOTOR_DESLIGADO;
				flagAcionamentoPercentimetro = false;
				contadorVerificaSeguranca = TEMPO_VERIFICA_SEGURANCA;
				return;
			}
			flagInfoLCDPressao = true;
			flagFalhaPressao = true;
			reiniciaVeriaveisProcesso();
			salvaLog(LOG_PARADO_FALTA_PRESSAO);
			return;
		}
		if(flagInicioMovimento) {
			flagInicioMovimento = false;
			sentidoMotor = ultimoSentido;
			contadorLaminaDagua = setPointContadorLaminaDagua - 1;
			flagAcionamentoPercentimetro = true;
		}
	}

	if(!flagLeituraGNSSRecente || flagObstaculoEncontrado) {
		flagBombaBooster = false;
	}
	else {
		flagBombaBooster = true;
	}

	if(!contadorLaminaDagua) {
		if(flagTrocouLaminaDagua) {
			flagTrocouLaminaDagua = false;
			laminaDagua = trocaLaminaDagua;
			writeEepromLamina();
		}
		contadorLaminaDagua = setPointContadorLaminaDagua;
		contadorLaminaDaguaDesligado = setPointContadorLaminaDaguaDesligado - 1;
		flagAcionamentoPercentimetro = false;
	}

	if(!contadorLaminaDaguaDesligado) {
		contadorLaminaDaguaDesligado = setPointContadorLaminaDaguaDesligado;
		contadorLaminaDagua = setPointContadorLaminaDagua - 1;
		flagAcionamentoPercentimetro = true;
	}

	if(flagFimCursoDireita && sentidoMotor == MOTOR_DIREITA) {
		sentidoMotor = MOTOR_DESLIGADO;
		if(cicloIrrigacao == IRRIGACAO_1_CICLO) {
			reiniciaVeriaveisProcesso();
			salvaLog(LOG_PARADO_FIM_CURSO);
		}
		else {
			salvaLog(LOG_TROCOU_SENTIDO_FIM_CURSO_REVERSO);
			sentidoMotor = MOTOR_ESQUERDA;
			ultimoSentido = MOTOR_ESQUERDA;
		}
	}

	if(flagFimCursoEsquerda && sentidoMotor == MOTOR_ESQUERDA) {
		sentidoMotor = MOTOR_DESLIGADO;
		if(cicloIrrigacao == IRRIGACAO_1_CICLO) {
			reiniciaVeriaveisProcesso();
			salvaLog(LOG_PARADO_FIM_CURSO);
		}
		else {
			salvaLog(LOG_TROCOU_SENTIDO_FIM_CURSO_AVANCO);
			sentidoMotor = MOTOR_DIREITA;
			ultimoSentido = MOTOR_DIREITA;
			contadorLaminaDagua = setPointContadorLaminaDagua - 1;
		}
	}
}
/*=============================================================================
CENTRAL DA OPERACAO
==============================================================================*/
void operacao() {
	static uint8_t ultimoAcionamentoAgenda = 99;
	static uint8_t flagReentradaFalhaSeguranca = false;
	if(flagEmergencia) {
		modoOperacao = MODO_DESLOCAMENTO;
		modoOperacaoRemoto = MODO_DESLOCAMENTO;
		sentidoRemoto = REMOTO_SEM_COMANDO;
		reiniciaVeriaveisProcesso();
		return;
	}

	if(flagFaltaFase || flagBarricada || flagBarricadaGPS) {
		modoOperacao = MODO_DESLOCAMENTO;
		modoOperacaoRemoto = MODO_DESLOCAMENTO;
		sentidoRemoto = REMOTO_SEM_COMANDO;
		reiniciaVeriaveisProcesso();
		return;
	}

	if(flagFalhaSeguranca && !contadorVerificaSeguranca) {
		if(!flagReentradaFalhaSeguranca) {
			salvaLog(LOG_PARADO_FALHA_SEGURANCA);
		}
		flagReentradaFalhaSeguranca = true;
		modoOperacao = MODO_DESLOCAMENTO;
		modoOperacaoRemoto = MODO_DESLOCAMENTO;
		sentidoRemoto = REMOTO_SEM_COMANDO;
		flagInfoLCDSeguranca = true;
		reiniciaVeriaveisProcesso();
		return;
	}
	else {
		flagReentradaFalhaSeguranca = false;
	}

	if(flagHorarioPonta) {
		if(!flagSaiuHorarioPonta) {
			modoOperacaoHorarioPonta = modoOperacao;
			sentidoHorarioPonta = sentidoMotor;
			flagRetornoHomeHorarioPonta = flagRetornoHome;
			flagSaiuHorarioPonta = true;
			writeEepromDadosHorarioPonta();
			salvaLog(LOG_PARADO_PONTA);
		}
		modoOperacao = MODO_DESLOCAMENTO;
		modoOperacaoRemoto = MODO_DESLOCAMENTO;
		sentidoRemoto = REMOTO_SEM_COMANDO;

		reiniciaVeriaveisProcesso();
		return;
	}
	else {
		if(flagSaiuHorarioPonta) {
			flagSaiuHorarioPonta = false;
			modoOperacao = modoOperacaoHorarioPonta; //volta o que estava
			sentidoMotor = sentidoHorarioPonta;
			flagRetornoHome = flagRetornoHomeHorarioPonta;
			writeEepromDadosHorarioPonta();
			salvaLogHorarioPonta();

			if(sentidoHorarioPonta == REMOTO_DIREITA) {
				flagManualEsquerda = false;
				flagManualDireita = true;
			}
			else if(sentidoHorarioPonta == REMOTO_ESQUERDA) {
				flagManualDireita = false;
				flagManualEsquerda = true;
			}
		}
	}

	verificaAgendaAcionamento();

	if(sentidoRemoto != REMOTO_SEM_COMANDO) { //Comando Remoto
		uint8_t flagTrocouOperacao = false;
		if(modoOperacaoRemoto != modoOperacao) {
			flagTrocouOperacao = true;
			reiniciaVeriaveisProcesso();
			modoOperacao = modoOperacaoRemoto;
		}

		if(sentidoRemoto == REMOTO_DESLIGADO) {
			salvaLogAppDesligar();
			flagManualDireita = false;
			flagManualEsquerda = false;
			reiniciaVeriaveisProcesso();
		}
		else if(sentidoRemoto == REMOTO_DIREITA) {
			salvaLogApp(flagTrocouOperacao, true);
			flagManualEsquerda = false;
			flagManualDireita = true;
		}
		else if(sentidoRemoto == REMOTO_ESQUERDA) {
			salvaLogApp(flagTrocouOperacao, false);
			flagManualDireita = false;
			flagManualEsquerda = true;
		}

		flagInfoLCDPressao = false;
		flagInfoLCDSeguranca = false;
		flagInfoLCDGNSS = false;

		sentidoRemoto = REMOTO_SEM_COMANDO;
	}

	if(ultimoAcionamentoAgenda != agendaHorarioAcionado) {
		ultimoAcionamentoAgenda = agendaHorarioAcionado;

		if(!ultimoAcionamentoAgenda) {
			flagManualDireita = false;
			flagManualEsquerda = false;
			reiniciaVeriaveisProcesso();
			salvaLog(LOG_PARADO_AGENDA);
		}
		else {
			if(ultimoAcionamentoAgenda <= QUANTIDADE_AGENDA_ACIONAMENTO) {
				if(modoOperacao == MODO_DESLOCAMENTO) {
					reiniciaVeriaveisProcesso();
					agendaHorarioAcionado = ultimoAcionamentoAgenda; //evita que seja reiniciada
				}
				modoOperacao = MODO_IRRIGACAO;
				if(acionamentoAgenda[ultimoAcionamentoAgenda - 1] == MOTOR_DIREITA) {
					flagManualEsquerda = false;
					flagManualDireita = true;
					salvaLog(LOG_IRRIGACAO_AVANCO_AGENDA);
				}
				else if(acionamentoAgenda[ultimoAcionamentoAgenda - 1] == MOTOR_ESQUERDA) {
					flagManualEsquerda = true;
					flagManualDireita = false;
					salvaLog(LOG_IRRIGACAO_REVERSO_AGENDA);
				}
			}
		}
	}

	if(modoOperacao == MODO_DESLOCAMENTO) {
		operacaoDeslocamentoSeco();
		return;
	}

	operacaoIrrigacao();
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
