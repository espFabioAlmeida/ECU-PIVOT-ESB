///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    Subrotinas LCD
//AUTOR:      Fábio Almeida
//CIRADO:     05/02/2021
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "codigosLogs.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define TEMPO_TELA_INICIAL 2000
/*=============================================================================
TELA INICIAL
==============================================================================*/
void escreveStringService(char valor[]) {
	uint8_t tamanho = 0;
	for(uint8_t i = 0; i < 16; i ++) {
		mensagemFTPLCD[i] = 0x00;
	}

	strcat(mensagemFTPLCD, valor);

	tamanho = strlen(mensagemFTPLCD);

	if(tamanho >= 16) {
		return;
	}

	for(; tamanho < 16; tamanho ++) {
		strcat(mensagemFTPLCD, " ");
	}
}
/*=============================================================================
TELA INICIAL
==============================================================================*/
void telaInicial() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("AUTOMASENSOR");
	lcdPosLc(2, 1);
	lcdEscreve("FeA");
	HAL_Delay(TEMPO_TELA_INICIAL);
	flagBuzzer = true;

	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Ecu Pivot ESB");
	lcdPosLc(2, 1);
	lcdEscreve("V");
	escreveDado(VERSAO_FIRMWARE, 0, 0, 2);
	lcdEscreve(".");
	escreveDado(SUBVERSAO_FIRMWARE, 0, 0, 1);
	lcdEscreve(" NOV/25");
	HAL_Delay(TEMPO_TELA_INICIAL);
	flagBuzzer = true;
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Iniciando");
	lcdPosLc(2, 1);
	lcdEscreve("Sistema");
}
/*=============================================================================
TELA OPERACAO
==============================================================================*/
void telaOperacao() {
	lcdPosLc(1, 1);

	if(contadorTempoInfoSinais) {
		lcdEscreveString(mensagemFTPLCD, 16);
		lcdPosLc(2, 1);
		lcdEscreve("GSM: ");
		lcdEscreveChar(operadoraConectada);
		escreveDado(percentualSinalOperadora, 0, 0, 2);
		lcdEscreve("% Rad.:");
		escreveDado(qualidadeSinalLora, 0, 0, 1);
		return;
	}

	if(flagEmergencia) {
		lcdEscreve("Equipamento em  ");
		lcdPosLc(2, 1);
		lcdEscreve("EMERGENCIA!!!   ");
		return;
	}

	if(flagFaltaFase) {
		lcdEscreve("Equipamento em  ");
		lcdPosLc(2, 1);
		lcdEscreve("Falha:Falta Fase");
		return;
	}

	if(flagBarricada) {
		lcdEscreve("Equipamento em  ");
		lcdPosLc(2, 1);
		lcdEscreve("Falha: Barricada");
		return;
	}

	if(flagBarricadaGPS) {
		lcdEscreve("Equipamento em  ");
		lcdPosLc(2, 1);
		lcdEscreve("Falha: Barr. GPS");
		return;
	}

	if(flagInfoLCDSeguranca) {
		lcdEscreve("Parou por falha ");
		lcdPosLc(2, 1);
		lcdEscreve("na SEGURANCA!   ");
		return;
	}

	if(flagInfoLCDPressao) {
		lcdEscreve("Parou por falta ");
		lcdPosLc(2, 1);
		lcdEscreve("de PRESSAO!     ");
		return;
	}

	if(flagInfoLCDGNSS) {
		lcdEscreve("Parou por falta ");
		lcdPosLc(2, 1);
		lcdEscreve("de sinal do GNSS");
		return;
	}

	if(flagHorarioPonta) {
		lcdEscreve("Equipamento em  ");
		lcdPosLc(2, 1);
		lcdEscreve("Horario de Ponta");
		return;
	}

	if(modoOperacao == MODO_IRRIGACAO) {
		if(flagIrrigacaoAguardandoInicio) {
			lcdEscreve("Modo Irrigacao  ");
			lcdPosLc(2, 1);
			lcdEscreve("   .  mm Standby");
		}
		else {
			if(flagIrrigacaoAguardandoPressao) {
				lcdEscreve("Pressuriza.    s");
				escreveDado(contadorBombaPrincipal, 1, 13, 3);
			}
			else {
				if(sentidoMotor == MOTOR_DESLIGADO) {
					lcdEscreve("Modo Irrigacao  ");
				}
				else if(sentidoMotor == MOTOR_DIREITA) {
					lcdEscreve("Irrigacao Avanco");
				}
				else {
					lcdEscreve("Irrigacao Rever.");
				}
			}

			lcdPosLc(2, 1);
			lcdEscreve("   .  mm   . bar");
			escreveDado(pressao / 10, 2, 10, 2);
			escreveDado(pressao % 10, 2, 13, 1);
		}

		escreveDado(laminaDagua / 100, 2, 1, 3);
		escreveDado(laminaDagua % 100, 2, 5, 2);
		return;
	}

	lcdEscreve("Deslocam. a Seco");
	lcdPosLc(2, 1);
	if(sentidoMotor == MOTOR_DESLIGADO) {
		limpaLinha(2);
		return;
	}
	else if(sentidoMotor == MOTOR_DIREITA) {
		lcdEscreve("Avanco ");
		if(flagRetornoHome) {
			lcdEscreve("p/Home   ");
		}
		else {
			lcdEscreve("         ");
		}
		return;
	}
	lcdEscreve("Reverso ");
	if(flagRetornoHome) {
		lcdEscreve("p/Home  ");
		return;
	}
	lcdEscreve("        ");
}
/*=============================================================================
TELA SENHA
==============================================================================*/
void telaSenha() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Senha ?");
	lcdPosLc(2, 1);
	lcdEscreve("0***");
	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA AGUARDA FTP
==============================================================================*/
void telaAguardaFTP() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Finalizando Proc");
	lcdPosLc(2, 1);
	lcdEscreve("Aguarde ....");
}
/*=============================================================================
TELA SENHA CORRETA
==============================================================================*/
void telaSenhaCorreta() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Senha Correta");
	HAL_Delay(1500);
}
/*=============================================================================
TELA SENHA INCORRETA
==============================================================================*/
void telaSenhaIncorreta() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Senha Incorreta");
	HAL_Delay(1500);
}
/*=============================================================================
TELA CONFIRMACAO
==============================================================================*/
void telaConfirmacao(uint8_t confirmacao) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Tem certeza ?");
	lcdPosLc(2, 1);
	if(confirmacao) {
		lcdEscreve("[X]Sim [ ]Nao");
		return;
	}
	lcdEscreve("[ ]Sim [X]Nao");
}
/*=============================================================================
TELA AGUARDE
==============================================================================*/
void telaAguarde() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Aguarde ....");
	HAL_Delay(1500);
}
/*=============================================================================
TELA SUCESSO
==============================================================================*/
void telaSucesso() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Sucesso");
	HAL_Delay(1500);
}
/*=============================================================================
TELA ERRO
==============================================================================*/
void telaErro() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Erro");
	HAL_Delay(1500);
}
/*=============================================================================
TELA WIFI CONECTADO
==============================================================================*/
void telaWiFiConectado() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("WiFi Conectado");
	HAL_Delay(1500);
}
/*=============================================================================
TELA WIFI CONECTADO
==============================================================================*/
void telaWiFiNaoConectado() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("WiFi NAO");
	lcdPosLc(2, 1);
	lcdEscreve("Conectado");
	HAL_Delay(1500);
}
/*=============================================================================
TELA VERIFIQUE PLACA SOQUETE
==============================================================================*/
void telaVerifiquePlacaSoquete() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("ERRO! Verifique");
	lcdPosLc(2, 1);
	lcdEscreve("a Placa Soquete");
	HAL_Delay(1500);
}
/*=============================================================================
TELA HORARIO
==============================================================================*/
void telaAjusteHorario() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("  :  :");
	lcdPosLc(2, 1);
	lcdEscreve("  /  /");

	escreveDado(sTime.Hours, 1, 1, 2);
	escreveDado(sTime.Minutes, 1, 4, 2);
	escreveDado(sTime.Seconds, 1, 7, 2);

	escreveDado(sDate.Date, 2, 1, 2);
	escreveDado(sDate.Month, 2, 4, 2);
	escreveDado(sDate.Year, 2, 7, 2);

	lcdPosLc(1, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA DIA DA SEMANA
==============================================================================*/
void telaAjusteDiaSemana(uint8_t dia) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Dia da Semana");
	lcdPosLc(2, 1);

	switch(dia) {
		case RTC_WEEKDAY_MONDAY: lcdEscreve("Segunda-feira"); break;
		case RTC_WEEKDAY_TUESDAY: lcdEscreve("Terca-feira"); break;
		case RTC_WEEKDAY_WEDNESDAY: lcdEscreve("Quarta-feira"); break;
		case RTC_WEEKDAY_THURSDAY: lcdEscreve("Quinta-feira"); break;
		case RTC_WEEKDAY_FRIDAY: lcdEscreve("Sexta-feira"); break;
		case RTC_WEEKDAY_SATURDAY: lcdEscreve("Sabado"); break;
		case RTC_WEEKDAY_SUNDAY: lcdEscreve("Domingo"); break;
	}

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA HORARIO DE PONTA
==============================================================================*/
void telaAjusteHorarioPonta() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Inicio:   :");
	lcdPosLc(2, 1);
	lcdEscreve("Fim:   :");

	escreveDado(horarioInicioPonta.Hours, 1, 9, 2);
	escreveDado(horarioInicioPonta.Minutes, 1, 12, 2);

	escreveDado(horarioFimPonta.Hours, 2, 6, 2);
	escreveDado(horarioFimPonta.Minutes, 2, 9, 2);

	lcdPosLc(1, 9);
	lcdCursor(1);
}
/*=============================================================================
TELA SELECAO AGENDA ACIONAMENTO
==============================================================================*/
void telaSelecaoAgendaAcionamento() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Sel. o Horario");
	lcdPosLc(2, 1);
	lcdEscreve("1");

	lcdPosLc(2, 1);
	lcdCursor(true);
}
/*=============================================================================
TELA AJUSTE HORARIO AGENDA ACIONAMENTO
==============================================================================*/
void telAjusteHorarioAgenda(uint8_t horarioFim) {
	limpaLcd();
	lcdPosLc(1, 1);
	horarioFim ? lcdEscreve("Horario Desligar") : lcdEscreve("Horario Ligar");
	lcdPosLc(2, 3);
	lcdEscreve(":");

	lcdPosLc(2, 1);
	lcdCursor(true);
}
/*=============================================================================
TELA SELECIONA O DIA DA SEMANA
==============================================================================*/
void telaSelecioneDiaSemana() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Selecione os");
	lcdPosLc(2, 1);
	lcdEscreve("Dias da Semana");
	HAL_Delay(1500);
}
/*=============================================================================
TELA DIA DA SEMANA AGENDA ACIONAMENTO
==============================================================================*/
void telaSelecaoDiaSemanaAgenda(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("S T Q Q S S D");

	for(uint8_t i = 0; i < 7; i ++) {
		lcdPosLc(2, i * 2 + 1);
		if(bitRead(valor, i)) {
			lcdEscreve("X");
		}
		else {
			lcdEscreve(" ");
		}
	}
}
/*=============================================================================
TELA OPERACAO AGENDA ACIONAMENTO
==============================================================================*/
void telaOperacaoAgenda(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Operacao");
	lcdPosLc(2, 1);

	switch(valor) {
		case MOTOR_DESLIGADO: lcdEscreve("Desligado"); break;
		case MOTOR_DIREITA: lcdEscreve("Irrigacao Avanco"); break;
		case MOTOR_ESQUERDA: lcdEscreve("Irrigacao Rever."); break;
	}

	lcdCursor(true);
	lcdPosLc(2, 1);
}
/*=============================================================================
TELA AJUSTE LAMINA DAGUA
==============================================================================*/
void telaAjusteLaminaDagua() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Ajuste Lamina");
	escreveDado(laminaDagua / 100, 2, 1, 3);
	lcdEscreve(".");
	escreveDado(laminaDagua % 100, 2, 5, 2);
	lcdEscreve("mm");

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA CICLO IRRIGACAO
==============================================================================*/
void telaCicloIrrigacao(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Ciclo Irrigacao");
	lcdPosLc(2, 1);
	if(valor == IRRIGACAO_1_CICLO) {
		lcdEscreve("1 Ciclo");
	}
	else {
		lcdEscreve("Continuo");
	}
	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA FERTI IRRIGACAO
==============================================================================*/
void telaFertiIrrigacao(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Ferti Irrigacao");
	lcdPosLc(2, 1);
	if(valor == true) {
		lcdEscreve("Ativado");
	}
	else {
		lcdEscreve("Desativado");
	}
	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA AJUSTE LAMINA DAGUA
==============================================================================*/
void telaAjusteAlarmePressao() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Alarme:   . bar ");
	lcdPosLc(2, 1);
	lcdEscreve("Hister.:  . bar ");

	escreveDado(alarmePressao / 10, 1, 9, 2);
	escreveDado(alarmePressao % 10, 1, 12, 1);
	escreveDado(histereseAlarmePressao / 10, 2, 10, 1);
	escreveDado(histereseAlarmePressao % 10, 2, 12, 1);

	lcdPosLc(1, 9);
	lcdCursor(1);
}
/*=============================================================================
TELA TEMPO PRESSURIZAÇÃO
==============================================================================*/
void telaAjusteTempoPressurizacao() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Tempo Press.");
	escreveDado(tempoPressurizacao, 2, 1, 2);
	lcdEscreve("min");

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA SELECAO OBSTACULOS
==============================================================================*/
void telaSelecaoObstaculos(uint8_t obstaculo, uint8_t ativado) {
	limpaLcd();
	lcdPosLc(1, 1);

	if(!obstaculo) {
		lcdEscreve("Voltar");
		return;
	}

	lcdEscreve("Obstaculo ");
	escreveDado(obstaculo, 0, 0, 2);
	lcdEscreve(":");
	lcdPosLc(2, 1);

	if(ativado) {
		lcdEscreve("Ativado");
		return;
	}
	lcdEscreve("Desativado");
}
/*=============================================================================
TELA EDICAO OBSTACULOS
==============================================================================*/
void telaEdicaoObstaculos(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("La:     '       ");
	lcdPosLc(2, 1);
	lcdEscreve("Lo:      '      ");
	escreveSimbolo(LCD_GRAUS, 1, 6);
	escreveSimbolo(LCD_GRAUS, 2, 7);

	escreveDado(posicoesObstaculos[offset].latitudeGraus, 1, 4, 2);
	escreveDado(posicoesObstaculos[offset].latitudeMinutos, 1, 7, 2);
	escreveDado(posicoesObstaculos[offset].latitudeSegundos, 1, 10, 5);
	lcdEscreveChar(posicoesObstaculos[offset].hemisferio);

	escreveDado(posicoesObstaculos[offset].longitudeGraus, 2, 4, 3);
	escreveDado(posicoesObstaculos[offset].longitudeMinutos, 2, 8, 2);
	escreveDado(posicoesObstaculos[offset].longitudeSegundos, 2, 11, 5);
	lcdEscreveChar(posicoesObstaculos[offset].indicadorWE);

	lcdPosLc(1, 4);
	lcdCursor(true);
}
/*=============================================================================
TELA EDICAO RAIO OBSTACULOS
==============================================================================*/
void telaEdicaoRaioObstaculos(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Raio de Atuacao");
	escreveDado(raioAtuacaoObstaculo[offset] / 10, 2, 1, 2);
	lcdEscreve(".");
	escreveDado(raioAtuacaoObstaculo[offset] % 10, 0, 0, 1);
	lcdEscreve("m");

	lcdPosLc(2, 1);
	lcdCursor(true);
}
/*=============================================================================
TELA EDICAO POSICAO HOME
==============================================================================*/
void telaEdicaoPosicaoHome() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("La:     '       ");
	lcdPosLc(2, 1);
	lcdEscreve("Lo:      '      ");
	escreveSimbolo(LCD_GRAUS, 1, 6);
	escreveSimbolo(LCD_GRAUS, 2, 7);

	escreveDado(posicaoHome.latitudeGraus, 1, 4, 2);
	escreveDado(posicaoHome.latitudeMinutos, 1, 7, 2);
	escreveDado(posicaoHome.latitudeSegundos, 1, 10, 5);
	lcdEscreveChar(posicaoHome.hemisferio);

	escreveDado(posicaoHome.longitudeGraus, 2, 4, 3);
	escreveDado(posicaoHome.longitudeMinutos, 2, 8, 2);
	escreveDado(posicaoHome.longitudeSegundos, 2, 11, 5);
	lcdEscreveChar(posicaoHome.indicadorWE);

	lcdPosLc(1, 4);
	lcdCursor(true);
}
/*=============================================================================
TELA IP DINAMICO
==============================================================================*/
void telaIpDinamico() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Ip Dinamico Lido");
	escreveDado(wifiIpDinamico[0], 2, 1, 3);
	lcdEscreve(".");
	escreveDado(wifiIpDinamico[1], 0, 0, 3);
	lcdEscreve(".");
	escreveDado(wifiIpDinamico[2], 0, 0, 3);
	lcdEscreve(".");
	escreveDado(wifiIpDinamico[3], 0, 0, 3);
}
/*=============================================================================
TELA PLACA SOQUETE
==============================================================================*/
void telaPlacaSoquete(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Soquete Internet");
	lcdPosLc(2, 1);
	switch(valor) {
		case SOQUETE_GPRS: lcdEscreve("GPRS"); break;
		case SOQUETE_WIFI: lcdEscreve("WiFI"); break;
		case SOQUETE_4G: lcdEscreve("4G/Nb-IoT"); break;
		case SOQUETE_RADIO: lcdEscreve("Radio/Gateway"); break;
	}
	lcdPosLc(2, 1);
	lcdCursor(true);
}
/*=============================================================================
TELA CONFIGURACOES GPRS
==============================================================================*/
void telaConfiguracoesGPRS(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			break;

		case 1:
			lcdEscreve("APN GPRS");
			lcdPosLc(2, 1);
			lcdEscreveString(gprsAPN, QUANTIDADE_CHAR_GPRS_APN);
			break;

		case 2:
			lcdEscreve("Reiniciar");
			lcdPosLc(2, 1);
			lcdEscreve("Modulo GPRS");
			break;
	}
}
/*=============================================================================
TELA CONFIGURACOES WIFI
==============================================================================*/
void telaConfiguracoesWiFi(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			break;

		case 1:
			lcdEscreve("WiFi SSID");
			lcdPosLc(2, 1);
			lcdEscreveString(wifiSSID, QUANTIDADE_CHAR_WIFI_SSID);
			break;

		case 2:
			lcdEscreve("WiFi Senha");
			lcdPosLc(2, 1);
			lcdEscreveString(wifiSenha, QUANTIDADE_CHAR_WIFI_SENHA);
			break;

		case 3:
			lcdEscreve("Enviar para o ");
			lcdPosLc(2, 1);
			lcdEscreve("Modulo WiFi");
			break;

		case 4:
			lcdEscreve("Reset");
			lcdPosLc(2, 1);
			lcdEscreve("Modulo WiFi");
			break;
	}
}
/*=============================================================================
TELA CONFIGURACOES IP WIFI
==============================================================================*/
void telaConfiguracoesIpWiFi(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			break;

		case 1:
			lcdEscreve("Ip Dinamico");
			lcdPosLc(2, 1);
			if(flagWiFiDhcp) {
				lcdEscreve("Habilitado");
			}
			else {
				lcdEscreve("Desabilitado");
			}
			break;

		case 2:
			lcdEscreve("Ler o IP");
			lcdPosLc(2, 1);
			lcdEscreve("Dinamico");
			break;

		case 3:
			lcdEscreve("IP Local");
			lcdPosLc(2, 1);
			for(uint8_t i = 0; i < 4; i ++) {
				escreveDado(wifiIp[i], 0, 0, 3);
				if(i < 3) {
					lcdEscreve(".");
				}
			}
			break;

		case 4:
			lcdEscreve("Servidor DNS");
			lcdPosLc(2, 1);
			for(uint8_t i = 0; i < 4; i ++) {
				escreveDado(wifiDNS[i], 0, 0, 3);
				if(i < 3) {
					lcdEscreve(".");
				}
			}
			break;

		case 5:
			lcdEscreve("Gateway");
			lcdPosLc(2, 1);
			for(uint8_t i = 0; i < 4; i ++) {
				escreveDado(wifiGateway[i], 0, 0, 3);
				if(i < 3) {
					lcdEscreve(".");
				}
			}
			break;

		case 6:
			lcdEscreve("Mascara Subrede");
			lcdPosLc(2, 1);
			for(uint8_t i = 0; i < 4; i ++) {
				escreveDado(wifiMask[i], 0, 0, 3);
				if(i < 3) {
					lcdEscreve(".");
				}
			}
			break;

		case 7:
			lcdEscreve("Enviar para o");
			lcdPosLc(2, 1);
			lcdEscreve("Modulo WiFi");
			break;
	}
}
/*=============================================================================
TELA CONFIGURACOES LoRa
==============================================================================*/
void telaConfiguracoesLoRa(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			break;

		case 1:
			lcdEscreve("Endereco Pivot");
			escreveDadoHexadecimal(enderecoLoRaPivot, 2, 1, 2);
			lcdEscreve("H");
			break;

		case 2:
			lcdEscreve("Endereco GNSS");
			escreveDadoHexadecimal(enderecoLoRaGNSS, 2, 1, 2);
			lcdEscreve("H");
			break;

		case 3:
			lcdEscreve("Canal");
			escreveDadoHexadecimal(canalLoRa, 2, 1, 1);
			lcdEscreve("H");
			break;

		case 4:
			lcdEscreve("Enviar para o");
			lcdPosLc(2, 1);
			lcdEscreve("Radio da Pivot");
			break;

		case 5:
			lcdEscreve("Enviar para o");
			lcdPosLc(2, 1);
			lcdEscreve("Radio da ECU-GPS");
			break;

		case 6:
			lcdEscreve("Ler endereco do");
			lcdPosLc(2, 1);
			lcdEscreve("Radio da ECU-GPS");
			break;
	}
}
/*=============================================================================
TELA CONFIGURACOES LoRa/GATEWAY
==============================================================================*/
void telaConfiguracoesLoRaGateway(uint8_t offset) {
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			break;

		case 1:
			lcdEscreve("Endereco Pivot");
			escreveDadoHexadecimal(enderecoLoRaPivotGateway, 2, 1, 2);
			lcdEscreve("H");
			break;

		case 2:
			lcdEscreve("Endereco Gateway");
			escreveDadoHexadecimal(enderecoLoRaGateway, 2, 1, 2);
			lcdEscreve("H");
			break;

		case 3:
			lcdEscreve("Canal");
			escreveDadoHexadecimal(canalLoRaGateway, 2, 1, 1);
			lcdEscreve("H");
			break;

		case 4:
			lcdEscreve("Enviar para o");
			lcdPosLc(2, 1);
			lcdEscreve("Radio da Pivot");
			break;
	}
}
/*=============================================================================
TELA APRESENTA CONFIG LoRa GNSS
==============================================================================*/
void telaApresentaConfigLoRaGNSS() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Pivot:    H");
	lcdPosLc(2, 1);
	lcdEscreve("GPS:    H CH:  H");

	lcdPosLc(1, 7);
	lcdEscreveString(enderecoLoRaRecebidoPivot, 4);
	lcdPosLc(2, 5);
	lcdEscreveString(enderecoLoRaRecebidoGNSS, 4);
	lcdPosLc(2, 14);
	lcdEscreveString(canalLoRaRecebido, 2);
}
/*=============================================================================
TELA COMUNICACAO ECU GPS
==============================================================================*/
void telaComunicacaoECUGPS(uint8_t valor) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Comunicacao GNSS");
	lcdPosLc(2, 1);
	if(valor == GNSS_485) {
		lcdEscreve("RS85");
	}
	else {
		lcdEscreve("Radio");
	}

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA TROCA NUMERO SERIAL
==============================================================================*/
void telaTrocaNumeroSerial() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Numero Serial");
	escreveDado(numeroSerial, 2, 1, 9);

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA TABELA PERCENTIMETRO
==============================================================================*/
void telaTabelaPercentimetro(uint8_t offset) {
	uint16_t valor = 0;
	limpaLcd();
	lcdPosLc(1, 1);

	switch(offset) {
		case 0:
			lcdEscreve("Voltar");
			return;
			break;

		case 1:
			lcdEscreve("Percent. em 5%");
			valor = tabelaLaminaDagua05;
			break;

		case 2:
			lcdEscreve("Percent. em 12%");
			valor = tabelaLaminaDagua12;
			break;

		case 3:
			lcdEscreve("Percent. em 20%");
			valor = tabelaLaminaDagua20;
			break;

		case 4:
			lcdEscreve("Percent. em 30%");
			valor = tabelaLaminaDagua30;
			break;

		case 5:
			lcdEscreve("Percent. em 40%");
			valor = tabelaLaminaDagua40;
			break;

		case 6:
			lcdEscreve("Percent. em 50%");
			valor = tabelaLaminaDagua50;
			break;

		case 7:
			lcdEscreve("Percent. em 70%");
			valor = tabelaLaminaDagua70;
			break;
	}

	escreveDado(valor / 100, 2, 1, 3);
	lcdEscreve(".");
	escreveDado(valor % 100, 0, 0, 2);
	lcdEscreve("mm");
}
/*=============================================================================
TELA TEMPO BASE PERCENTIMETRO
==============================================================================*/
void telaAjusteTempoPercentimetro(uint8_t tempo) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Tempo Base Perc.");
	escreveDado(tempo, 2, 1, 2);
	lcdEscreve("s");

	lcdPosLc(2, 1);
	lcdCursor(1);
}
/*=============================================================================
TELA TROCA SENHA
==============================================================================*/
void telaTrocaSenha() {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("Nova Senha");
	lcdPosLc(2, 1);
	lcdEscreve("0***");
}
/*=============================================================================
TELA VISUALIZA LOG
==============================================================================*/
void telaVisualizaLog() {
	limpaLcd();
	escreveDado(logLido.dia, 1, 1, 2);
	lcdEscreve("/");
	escreveDado(logLido.mes, 0, 0, 2);
	lcdEscreve("   ");
	escreveDado(logLido.hora, 0, 0, 2);
	lcdEscreve(":");
	escreveDado(logLido.minuto, 0, 0, 2);
	lcdEscreve(":");
	escreveDado(logLido.segundo, 0, 0, 2);

	lcdPosLc(2, 1);
	switch(logLido.codigo) {
		case LOG_SISTEMA_INICIADO: lcdEscreve("Sistema Iniciado"); break;

		case LOG_IRRIGACAO_AVANCO_SW: lcdEscreve("Irrig. Avanco SW"); break;
		case LOG_IRRIGACAO_REVERSO_SW: lcdEscreve("Irrig. Rever. SW"); break;
		case LOG_DESLOCAMENTO_AVANCO_SW: lcdEscreve("Desl. Avanco SW"); break;
		case LOG_DESLOCAMENTO_REVERSO_SW: lcdEscreve("Desl. Reverso SW"); break;
		case LOG_PARADO_SW: lcdEscreve("Desligou por SW"); break;

		case LOG_IRRIGACAO_AVANCO_APP: lcdEscreve("Irrig. Avanco Ap"); break;
		case LOG_IRRIGACAO_REVERSO_APP: lcdEscreve("Irrig. Rever. Ap"); break;
		case LOG_DESLOCAMENTO_AVANCO_APP: lcdEscreve("Desl. Avanco App"); break;
		case LOG_DESLOCAMENTO_REVERSO_APP: lcdEscreve("Desl. Reverso Ap"); break;
		case LOG_PARADO_APP: lcdEscreve("Desligou por App"); break;

		case LOG_IRRIGACAO_AVANCO_EEPROM: lcdEscreve("Irrig. Avanco Me"); break;
		case LOG_IRRIGACAO_REVERSO_EEPROM: lcdEscreve("Irrig. Rever. Me"); break;
		case LOG_DESLOCAMENTO_AVANCO_EEPROM: lcdEscreve("Desl. Avanco Mem"); break;
		case LOG_DESLOCAMENTO_REVERSO_EEPROM: lcdEscreve("Desl. Reverso Me"); break;

		case LOG_IRRIGACAO_AVANCO_AGENDA: lcdEscreve("Irrig. Avanco Ag"); break;
		case LOG_IRRIGACAO_REVERSO_AGENDA: lcdEscreve("Irrig. Rever. Ag"); break;
		case LOG_DESLOCAMENTO_AVANCO_AGENDA: lcdEscreve("Desl. Avanco Age"); break;
		case LOG_DESLOCAMENTO_REVERSO_AGENDA: lcdEscreve("Desl. Reverso Ag"); break;
		case LOG_PARADO_AGENDA: lcdEscreve("Desligou Agenda"); break;

		case LOG_IRRIGACAO_AVANCO_PONTA: lcdEscreve("Irrig. Avanco Po"); break;
		case LOG_IRRIGACAO_REVERSO_PONTA: lcdEscreve("Irrig. Rever. Po"); break;
		case LOG_DESLOCAMENTO_AVANCO_PONTA: lcdEscreve("Desl. Avanco Pon"); break;
		case LOG_DESLOCAMENTO_REVERSO_PONTA: lcdEscreve("Desl. Reverso Po"); break;
		case LOG_PARADO_PONTA: lcdEscreve("Desligou Hr Pon."); break;

		case LOG_PARADO_FIM_CURSO: lcdEscreve("Parou Fim Curso"); break;
		case LOG_PARADO_HOME: lcdEscreve("Parou por Home"); break;
		case LOG_TROCOU_SENTIDO_FIM_CURSO_AVANCO: lcdEscreve("Irrig. Avanco FC"); break;
		case LOG_TROCOU_SENTIDO_FIM_CURSO_REVERSO: lcdEscreve("Irrig. Rever. FC"); break;

		case LOG_PARADO_EMERGENCIA: lcdEscreve("Emerge. acionada"); break;
		case LOG_PARADO_FALHA_SEGURANCA: lcdEscreve("Parou Seguranca"); break;
		case LOG_PARADO_BARRICADA: lcdEscreve("Barricada encon."); break;
		case LOG_PARADO_BARRICADA_GPS: lcdEscreve("Barricada GPS"); break;
		case LOG_PARADO_FALTA_FASE: lcdEscreve("Falta de Fase"); break;
		case LOG_PARADO_FALTA_PRESSAO: lcdEscreve("Parou Falta Pres"); break;
		case LOG_PARADO_FALTA_GNSS: lcdEscreve("Parou Falta GNSS"); break;

		case LOG_PARADO_IHM: lcdEscreve("Parou via IHM"); break;

		default: lcdEscreve("Log Vazio"); break;
	}
}
/*=============================================================================
TELA SELECIONA LOG
==============================================================================*/
void telaSelecionaLog(uint16_t registro, uint16_t passoRegistro) {
	limpaLcd();
	escreveDado(registro, 1, 1, 4);
	lcdEscreve(")");
	switch(passoRegistro) {
		case 1:
			escreveSimbolo(LCD_SETA_DIREITA, 1, 15);
			lcdEscreve("1");
			break;

		case 10:
			escreveSimbolo(LCD_SETA_DIREITA, 1, 14);
			lcdEscreve("10");
			break;

		case 100:
			escreveSimbolo(LCD_SETA_DIREITA, 1, 13);
			lcdEscreve("100");
			break;

		case 1000:
			escreveSimbolo(LCD_SETA_DIREITA, 1, 12);
			lcdEscreve("1000");
			break;
	}

	escreveDado(logLido.dia, 2, 1, 2);
	lcdEscreve("/");
	escreveDado(logLido.mes, 0, 0, 2);
	lcdEscreve("/");
	escreveDado(logLido.ano, 0, 0, 2);
	lcdEscreve("   ");
	escreveDado(logLido.hora, 0, 0, 2);
	lcdEscreve(":");
	escreveDado(logLido.minuto, 0, 0, 2);
}
/*=============================================================================
TELA MENU
==============================================================================*/
void telaMenu(uint8_t menu) {
	limpaLcd();
	lcdPosLc(1, 1);
	lcdEscreve("F");
	escreveDado(menu, 0, 0, 2);

	switch(menu) {
		case 0:
			lcdEscreve(": Sair");
			break;

		case 1:
			lcdEscreve(": Ajuste");
			lcdPosLc(2, 1);
			lcdEscreve("Data e Hora");
			break;

		case 2:
			lcdEscreve(": Ajuste");
			lcdPosLc(2, 1);
			lcdEscreve("Lamina D'agua");
			break;

		case 3:
			lcdEscreve(": Ciclo da");
			lcdPosLc(2, 1);
			lcdEscreve("Irrigacao");
			break;

		case 4:
			lcdEscreve(": Ferti");
			lcdPosLc(2, 1);
			lcdEscreve("Irrigacao");
			break;

		case 5:
			lcdEscreve(": Alarme de");
			lcdPosLc(2, 1);
			lcdEscreve("Pressao");
			break;

		case 6:
			lcdEscreve(": Tempo de");
			lcdPosLc(2, 1);
			lcdEscreve("Pressurizacao");
			break;

		case 7:
			lcdEscreve(": Agenda de");
			lcdPosLc(2, 1);
			lcdEscreve("Acionamento");
			break;

		case 8:
			lcdEscreve(": Ajuste");
			lcdPosLc(2, 1);
			lcdEscreve("Horario de Ponta");
			break;

		case 9:
			lcdEscreve(": Cadastro de");
			lcdPosLc(2, 1);
			lcdEscreve("Obstaculos");
			break;

		case 10:
			lcdEscreve(": Posicao");
			lcdPosLc(2, 1);
			lcdEscreve("Home");
			break;

		case 11:
			lcdEscreve(": Config.");
			lcdPosLc(2, 1);
			lcdEscreve("GPRS");
			break;

		case 12:
			lcdEscreve(": Config.");
			lcdPosLc(2, 1);
			lcdEscreve("Rede WiFi");
			break;

		case 13:
			lcdEscreve(": Config.");
			lcdPosLc(2, 1);
			lcdEscreve("IP WiFi");
			break;

		case 14:
			lcdEscreve(": Config.");
			lcdPosLc(2, 1);
			lcdEscreve("Radio/Gateway");
			break;

		case 15:
			lcdEscreve(": Config.");
			lcdPosLc(2, 1);
			lcdEscreve("Radio/ECU-GPS");
			break;

		case 16:
			lcdEscreve(": Comunicacao");
			lcdPosLc(2, 1);
			lcdEscreve("ECU-GPS");
			break;

		case 17:
			lcdEscreve(": Horimetro");
			escreveDado(horimetro.horas, 2, 1, 9);
			lcdEscreve(":");
			escreveDado(horimetro.minutos, 0, 0, 2);
			break;

		case 18:
			lcdEscreve(": Zerar o");
			lcdPosLc(2, 1);
			lcdEscreve("Horimetro");
			break;

		case 19:
			lcdEscreve(": Tabela do");
			lcdPosLc(2, 1);
			lcdEscreve("Percentimetro");
			break;

		case 20:
			lcdEscreve(": Tempo base");
			lcdPosLc(2, 1);
			lcdEscreve("do Percentimetro");
			break;

		case 21:
			lcdEscreve(": Placa");
			lcdPosLc(2, 1);
			lcdEscreve("Soquete Internet");
			break;

		case 22:
			lcdEscreve(": Troca a");
			lcdPosLc(2, 1);
			lcdEscreve("Senha");
			break;

		case 23:
			lcdEscreve(": Log do");
			lcdPosLc(2, 1);
			lcdEscreve("Sistema");
			break;

		case 24:
			lcdEscreve(": Carrega os");
			lcdPosLc(2, 1);
			lcdEscreve("Param. Fabrica");
			break;

		case 25:
			lcdEscreve(": Troca o");
			lcdPosLc(2, 1);
			lcdEscreve("Numero Serial");
			break;
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
