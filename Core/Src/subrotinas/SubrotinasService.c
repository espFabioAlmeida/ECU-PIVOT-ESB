///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasFTP
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
/*=============================================================================
LIMPA REPORTE SERVICE
==============================================================================*/
void limpaReporteService() {
	for(uint16_t i = 0; i < TAMANHO_MAXIMO_REPORTE; i ++) {
		reporteService[i] = 0x00;
	}
}
/*=============================================================================
SPRINTF REPORTE SERVICE
==============================================================================*/
void sprintfReporteService(uint32_t valor, uint8_t qntdDig) {
	//Enviar 0 em qntdDig para ignorar "zeros" a esquerda
	char dataChar[9];
	memset(dataChar, 0x00, 9);
	sprintf(dataChar, "%d", valor);

	switch(qntdDig) {
		case 9:
			if(valor < 100000000) {
				strcat(reporteService, "0");
			}
		case 8:
			if(valor < 10000000) {
				strcat(reporteService, "0");
			}
		case 7:
			if(valor < 1000000) {
				strcat(reporteService, "0");
			}
		case 6:
			if(valor < 100000) {
				strcat(reporteService, "0");
			}
		case 5:
			if(valor < 10000) {
				strcat(reporteService, "0");
			}
		case 4:
			if(valor < 1000) {
				strcat(reporteService, "0");
			}
		case 3:
			if(valor < 100) {
				strcat(reporteService, "0");
			}
		case 2:
			if(valor < 10) {
				strcat(reporteService, "0");
			}
	}

	strcat(reporteService, dataChar);
}
/*=============================================================================
MONTA REPORTE
==============================================================================*/
void montaReporteService() {
	uint8_t quantidadeAlarmes = 0;
	uint8_t reporteBarricada = false;
	char hemisferio[2], indicadorWE[2];

	if(flagBarricada || flagBarricadaGPS) {
		reporteBarricada = true;
	}

	limpaReporteService();
	strcat(reporteService, "{ \"pivotNumber\": \"");
	sprintfReporteService(numeroSerial, 0);
	strcat(reporteService, "\", ");
	strcat(reporteService, "\"reportData\": \"");
	hemisferio[0] = posicaoAtualGPS.hemisferio;
	hemisferio[1] = 0x00;
	indicadorWE[0] = posicaoAtualGPS.indicadorWE;
	indicadorWE[1] = 0x00;

	sprintfReporteService(posicaoAtualGPS.latitudeGraus, 2);
	sprintfReporteService(posicaoAtualGPS.latitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoAtualGPS.latitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, hemisferio);
	strcat(reporteService, ",");
	sprintfReporteService(posicaoAtualGPS.longitudeGraus, 3);
	sprintfReporteService(posicaoAtualGPS.longitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoAtualGPS.longitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, indicadorWE);
	strcat(reporteService, ",");
	sprintfReporteService(qualidadeSinalLora, 0);
	strcat(reporteService, ",");
	strcat(reporteService, " ");

	sprintfReporteService(sTime.Hours , 2);
	sprintfReporteService(sTime.Minutes , 2);
	sprintfReporteService(sTime.Seconds , 2);
	sprintfReporteService(sDate.Date, 2);
	sprintfReporteService(sDate.Month, 2);
	sprintfReporteService(sDate.Year, 2);
	strcat(reporteService, " ");

	sprintfReporteService(modoOperacao, 0);
	sprintfReporteService(sentidoMotor, 0);
	sprintfReporteService(flagRetornoHome, 0);
	strcat(reporteService, " ");

	sprintfReporteService(contadorBombaPrincipal, 3);
	strcat(reporteService, " ");

	sprintfReporteService(pressao, 3);
	strcat(reporteService, " ");

	sprintfReporteService(flagPortaAberta, 0);
	strcat(reporteService, " ");

	for(uint8_t i = 0; i < QUANTIDADE_ENTRADAS_DIGITAIS; i ++) {
		sprintfReporteService(statusEntradasDigitais[i], 0);
	}
	strcat(reporteService, " ");

	for(uint8_t i = 0; i < QUANTIDADE_RELES; i ++) {
		sprintfReporteService(statusReles[i], 0);
	}
	strcat(reporteService, " ");

	sprintfReporteService(flagEmergencia, 0);
	sprintfReporteService(flagFalhaSeguranca, 0);
	sprintfReporteService(flagFalhaPressao, 0);
	sprintfReporteService(flagFaltaFase, 0);
	sprintfReporteService(reporteBarricada, 0);
	strcat(reporteService, " ");

	sprintfReporteService(horimetro.horas, 0);
	strcat(reporteService, ":");
	sprintfReporteService(horimetro.minutos, 2);
	strcat(reporteService, " ");
	sprintfReporteService(flagHorarioPonta, 0);
	strcat(reporteService, " ");
	sprintfReporteService(flagObstaculoEncontrado, 0);
	strcat(reporteService, " ");

	hemisferio[0] = posicaoInicioOperacao.hemisferio;
	hemisferio[1] = 0x00;
	indicadorWE[0] = posicaoInicioOperacao.indicadorWE;
	indicadorWE[1] = 0x00;
	sprintfReporteService(posicaoInicioOperacao.latitudeGraus, 2);
	sprintfReporteService(posicaoInicioOperacao.latitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoInicioOperacao.latitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, hemisferio);
	strcat(reporteService, ",");
	sprintfReporteService(posicaoInicioOperacao.longitudeGraus, 3);
	sprintfReporteService(posicaoInicioOperacao.longitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoInicioOperacao.longitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, indicadorWE);
	strcat(reporteService, ",");
	sprintfReporteService(flagPosicaoInicioOperacaoConfiavel, 0);
	strcat(reporteService, ",");
	strcat(reporteService, " ");

	strcat(reporteService, " "); //Separa reporte de status das configurações salvas
	//Envia configurações salvas
	sprintfReporteService(laminaDagua, 0);
	strcat(reporteService, " ");
	sprintfReporteService(cicloIrrigacao, 0);
	strcat(reporteService, " ");
	sprintfReporteService(flagFertiIrrigacao, 0);
	strcat(reporteService, " ");
	sprintfReporteService(alarmePressao, 0);
	strcat(reporteService, ",");
	sprintfReporteService(histereseAlarmePressao, 0);
	strcat(reporteService, " ");
	sprintfReporteService(tempoPressurizacao, 0);
	strcat(reporteService, " ");
	sprintfReporteService(horarioInicioPonta.Hours, 2);
	strcat(reporteService, ":");
	sprintfReporteService(horarioInicioPonta.Minutes, 2);
	strcat(reporteService, ",");
	sprintfReporteService(horarioFimPonta.Hours, 2);
	strcat(reporteService, ":");
	sprintfReporteService(horarioFimPonta.Minutes, 2);
	strcat(reporteService, " ");
	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		sprintfReporteService(raioAtuacaoObstaculo[i], 0);
		strcat(reporteService, ",");
	}
	strcat(reporteService, " ");

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		hemisferio[0] = posicoesObstaculos[i].hemisferio;
		hemisferio[1] = 0x00;
		indicadorWE[0] = posicoesObstaculos[i].indicadorWE;
		indicadorWE[1] = 0x00;

		sprintfReporteService(flagObstaculoAtivado[i], 0);
		strcat(reporteService, ",");
		sprintfReporteService(posicoesObstaculos[i].latitudeGraus, 2);
		sprintfReporteService(posicoesObstaculos[i].latitudeMinutos, 2);
		strcat(reporteService, ".");
		sprintfReporteService(posicoesObstaculos[i].latitudeSegundos, 5);
		strcat(reporteService, ",");
		strcat(reporteService, hemisferio);
		strcat(reporteService, ",");
		sprintfReporteService(posicoesObstaculos[i].longitudeGraus, 3);
		sprintfReporteService(posicoesObstaculos[i].longitudeMinutos, 2);
		strcat(reporteService, ".");
		sprintfReporteService(posicoesObstaculos[i].longitudeSegundos, 5);
		strcat(reporteService, ",");
		strcat(reporteService, indicadorWE);
		strcat(reporteService, ", ");
	}

	hemisferio[0] = posicaoHome.hemisferio;
	hemisferio[1] = 0x00;
	indicadorWE[0] = posicaoHome.indicadorWE;
	indicadorWE[1] = 0x00;

	sprintfReporteService(posicaoHome.latitudeGraus, 2);
	sprintfReporteService(posicaoHome.latitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoHome.latitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, hemisferio);
	strcat(reporteService, ",");
	sprintfReporteService(posicaoHome.longitudeGraus, 3);
	sprintfReporteService(posicaoHome.longitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoHome.longitudeSegundos, 5);
	strcat(reporteService, ",");
	strcat(reporteService, indicadorWE);
	strcat(reporteService, ", ");

	sprintfReporteService(tabelaLaminaDagua05, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua12, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua20, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua30, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua40, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua50, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua70, 0);
	strcat(reporteService, ", ");
	sprintfReporteService(tempoBaseLaminaDagua, 0);
	strcat(reporteService, ", ");

	strcat(reporteService, "\", ");

	if(flagEmergencia) {
		quantidadeAlarmes ++;
	}

	if(flagFalhaSeguranca) {
		quantidadeAlarmes ++;
	}

	if(flagFalhaPressao) {
		quantidadeAlarmes ++;
	}

	if(flagFaltaFase) {
		quantidadeAlarmes ++;
	}

	if(reporteBarricada) {
		quantidadeAlarmes ++;
	}

	strcat(reporteService, "\"alarms\": ");
	sprintfReporteService(quantidadeAlarmes, 0);

	strcat(reporteService, " }");
}
/*=============================================================================
VERIFICA SE A QUANTIDADE DE BYTES INFORMADOS É IGUAL A LIDA
==============================================================================*/
uint8_t validaPacoteService() {
	uint16_t tamanhoPacote = 0, inicioPacote = 0, fimPacote = 0;
	signed int ponteiro = -1;

	if(placaSoquete == SOQUETE_GPRS) {
		ponteiro = indexOf(bufferSoquete, "HTTPREAD:");
		if(ponteiro < 0) {
			return false;
		}
		ponteiro += 10;
	}
	else if(placaSoquete == SOQUETE_WIFI) {
		ponteiro = indexOf(bufferSoquete, "Content-Length");
		if(ponteiro < 0) {
			return false;
		}
		ponteiro += 16;
	}
	else { //SOQUETE 4G
		uint8_t primeiraVirgula = false;
		ponteiro = indexOf(bufferSoquete, "+QHTTPGET:");
		if(ponteiro < 0) {
			return false;
		}

		for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
			if(bufferSoquete[i] == ',') {
				if(primeiraVirgula) {
					ponteiro = i + 1;
					i = TAMANHO_BUFFER_SOQUETE;
				}
				else {
					primeiraVirgula = true;
				}
			}
		}
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			tamanhoPacote *= 10;
			tamanhoPacote += charToByte(bufferSoquete[i]);
		}
		else {
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	ponteiro = indexOf(bufferSoquete, "{");

	if(ponteiro < 0) {
		return false;
	}

	inicioPacote = ponteiro;

	ponteiro = indexOf(bufferSoquete, "}");

	if(ponteiro < 0) {
		return false;
	}

	fimPacote = ponteiro;

	if(tamanhoPacote == (fimPacote - inicioPacote + 1)) {
		return true;
	}

	return false;
}
/*=============================================================================
LEITURA DA CONFIGURAÇÃO
==============================================================================*/
void leituraConfiguracaoService() {
	uint16_t dado = 0;
	uint32_t idRecebido = 0;
	signed int ponteiro = 0;

	if(!validaPacoteService()) {
		apagaSoqueteBuffer();
		return;
	}

	ponteiro = indexOf(bufferSoquete, "\"id\":");

	if(ponteiro < 0) {
		apagaSoqueteBuffer();
		return;
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			ponteiro = i;
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(bufferSoquete[i] == ',' || bufferSoquete[i] == '}') {
			i = TAMANHO_BUFFER_SOQUETE;
		}
		else {
			idRecebido *= 10;
			idRecebido += charToByte(bufferSoquete[i]);
		}
	}

	if(idRecebido == ultimoIdConfig) {
		apagaSoqueteBuffer();
		return;
	}

	ponteiro = indexOf(bufferSoquete, "\configData\":");

	if(ponteiro < 0) {
		apagaSoqueteBuffer();
		return;
	}

	laminaDagua = 0;
	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		laminaDagua *= 10;
		laminaDagua += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	cicloIrrigacao = charToBool(bufferSoquete[ponteiro]);
	ponteiro += 2;

	flagFertiIrrigacao = charToBool(bufferSoquete[ponteiro]);
	ponteiro += 2;

	alarmePressao = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		alarmePressao *= 10;
		alarmePressao += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	histereseAlarmePressao = 0;
	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		histereseAlarmePressao *= 10;
		histereseAlarmePressao += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	tempoPressurizacao = 0;
	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		tempoPressurizacao *= 10;
		tempoPressurizacao += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	horarioInicioPonta.Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioInicioPonta.Hours *= 10;
		horarioInicioPonta.Hours += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	horarioInicioPonta.Minutes = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		horarioInicioPonta.Minutes *= 10;
		horarioInicioPonta.Minutes += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	horarioFimPonta.Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioFimPonta.Hours *= 10;
		horarioFimPonta.Hours += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	horarioFimPonta.Minutes = 0;
	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		horarioFimPonta.Minutes *= 10;
		horarioFimPonta.Minutes += charToByte(bufferSoquete[ponteiro]);
	}
	ponteiro ++;

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		for(raioAtuacaoObstaculo[i] = 0; bufferSoquete[ponteiro] != ' ' && bufferSoquete[ponteiro] != ','; ponteiro ++) {
			raioAtuacaoObstaculo[i] *= 10;
			raioAtuacaoObstaculo[i] += charToByte(bufferSoquete[ponteiro]);
		}
		ponteiro ++;

		if(raioAtuacaoObstaculo[i] > 999) {
			raioAtuacaoObstaculo[i] = 10;
		}
		if(raioAtuacaoObstaculo[i] < 10) {
			raioAtuacaoObstaculo[i] = 10;
		}

		if(bufferSoquete[ponteiro] == ' ') {
			i = QUANTIDADE_OBSTACULOS;
		}
	}
	//ponteiro ++;

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		flagObstaculoAtivado[i] = charToBool(bufferSoquete[ponteiro]);
		ponteiro += 2;

		posicoesObstaculos[i].latitudeGraus = charToByte(bufferSoquete[ponteiro]) * 10;
		ponteiro ++;
		posicoesObstaculos[i].latitudeGraus += charToByte(bufferSoquete[ponteiro]);
		ponteiro ++;

		posicoesObstaculos[i].latitudeMinutos = charToByte(bufferSoquete[ponteiro]) * 10;
		ponteiro ++;
		posicoesObstaculos[i].latitudeMinutos += charToByte(bufferSoquete[ponteiro]);
		ponteiro += 2;

		posicoesObstaculos[i].latitudeSegundos = 0;
		for(uint8_t j = 0; j < 5; j ++) {
			posicoesObstaculos[i].latitudeSegundos *= 10;
			posicoesObstaculos[i].latitudeSegundos += charToByte(bufferSoquete[ponteiro]);
			ponteiro ++;
		}
		ponteiro ++;
		posicoesObstaculos[i].hemisferio = bufferSoquete[ponteiro];
		if(posicoesObstaculos[i].hemisferio != 'S' && posicoesObstaculos[i].hemisferio != 'N') {
			posicoesObstaculos[i].hemisferio = 'S';
		}
		ponteiro += 2;

		posicoesObstaculos[i].longitudeGraus = charToByte(bufferSoquete[ponteiro]) * 100;
		ponteiro ++;
		posicoesObstaculos[i].longitudeGraus += charToByte(bufferSoquete[ponteiro]) * 10;
		ponteiro ++;
		posicoesObstaculos[i].longitudeGraus += charToByte(bufferSoquete[ponteiro]);
		ponteiro ++;

		posicoesObstaculos[i].longitudeMinutos = charToByte(bufferSoquete[ponteiro]) * 10;
		ponteiro ++;
		posicoesObstaculos[i].longitudeMinutos += charToByte(bufferSoquete[ponteiro]);
		ponteiro += 2;

		posicoesObstaculos[i].longitudeSegundos = 0;
		for(uint8_t j = 0; j < 5; j ++) {
			posicoesObstaculos[i].longitudeSegundos *= 10;
			posicoesObstaculos[i].longitudeSegundos += charToByte(bufferSoquete[ponteiro]);
			ponteiro ++;
		}
		ponteiro ++;
		posicoesObstaculos[i].indicadorWE = bufferSoquete[ponteiro];
		if(posicoesObstaculos[i].indicadorWE != 'W' && posicoesObstaculos[i].indicadorWE != 'E') {
			posicoesObstaculos[i].indicadorWE = 'W';
		}
		ponteiro += 3;
	}

	posicaoHome.latitudeGraus = charToByte(bufferSoquete[ponteiro]) * 10;
	ponteiro ++;
	posicaoHome.latitudeGraus += charToByte(bufferSoquete[ponteiro]);
	ponteiro ++;

	posicaoHome.latitudeMinutos = charToByte(bufferSoquete[ponteiro]) * 10;
	ponteiro ++;
	posicaoHome.latitudeMinutos += charToByte(bufferSoquete[ponteiro]);
	ponteiro += 2;

	posicaoHome.latitudeSegundos = 0;
	for(uint8_t j = 0; j < 5; j ++) {
		posicaoHome.latitudeSegundos *= 10;
		posicaoHome.latitudeSegundos += charToByte(bufferSoquete[ponteiro]);
		ponteiro ++;
	}
	ponteiro ++;
	posicaoHome.hemisferio = bufferSoquete[ponteiro];
	if(posicaoHome.hemisferio != 'S' && posicaoHome.hemisferio != 'N') {
		posicaoHome.hemisferio = 'S';
	}
	ponteiro += 2;

	posicaoHome.longitudeGraus = charToByte(bufferSoquete[ponteiro]) * 100;
	ponteiro ++;
	posicaoHome.longitudeGraus += charToByte(bufferSoquete[ponteiro]) * 10;
	ponteiro ++;
	posicaoHome.longitudeGraus += charToByte(bufferSoquete[ponteiro]);
	ponteiro ++;

	posicaoHome.longitudeMinutos = charToByte(bufferSoquete[ponteiro]) * 10;
	ponteiro ++;
	posicaoHome.longitudeMinutos += charToByte(bufferSoquete[ponteiro]);
	ponteiro += 2;

	posicaoHome.longitudeSegundos = 0;
	for(uint8_t j = 0; j < 5; j ++) {
		posicaoHome.longitudeSegundos *= 10;
		posicaoHome.longitudeSegundos += charToByte(bufferSoquete[ponteiro]);
		ponteiro ++;
	}
	ponteiro ++;
	posicaoHome.indicadorWE = bufferSoquete[ponteiro];
	if(posicaoHome.indicadorWE != 'W' && posicaoHome.indicadorWE != 'E') {
		posicaoHome.indicadorWE = 'W';
	}

	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
	}

	for(uint8_t tabela = 0; tabela < 7; tabela ++) {
		ponteiro ++;
		dado = 0;
		for(; bufferSoquete[ponteiro] != ',' && bufferSoquete[ponteiro] != ' '; ponteiro ++) {
			if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
				return;
			}
			if(isNumber(bufferSoquete[ponteiro])) {
				dado *= 10;
				dado += charToByte(bufferSoquete[ponteiro]);
			}
		}
		if(dado <= VALOR_MAXIMO_LAMINA_MM) {
			switch(tabela) {
				case 0: tabelaLaminaDagua05 = dado; break;
				case 1: tabelaLaminaDagua12 = dado; break;
				case 2: tabelaLaminaDagua20 = dado; break;
				case 3: tabelaLaminaDagua30 = dado; break;
				case 4: tabelaLaminaDagua40 = dado; break;
				case 5: tabelaLaminaDagua50 = dado; break;
				case 6: tabelaLaminaDagua70 = dado; break;
			}
		}
	}

	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
	}

	ponteiro ++;
	dado = 0;
	for(; bufferSoquete[ponteiro] != ',' && bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
		if(isNumber(bufferSoquete[ponteiro])) {
			dado *= 10;
			dado += charToByte(bufferSoquete[ponteiro]);
		}
	}
	if(dado >= TEMPO_MINIMO_PERCENTIMETRO && dado <= TEMPO_MAXIMO_PERCENTIMETRO) {
		tempoBaseLaminaDagua = dado * 10;
	}

	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
	}

	ponteiro ++;
	horarioLigarAgenda[0].Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioLigarAgenda[0].Hours *= 10;
		horarioLigarAgenda[0].Hours += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioLigarAgenda[0].Minutes = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		horarioLigarAgenda[0].Minutes *= 10;
		horarioLigarAgenda[0].Minutes += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioDesligarAgenda[0].Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioDesligarAgenda[0].Hours *= 10;
		horarioDesligarAgenda[0].Hours += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioDesligarAgenda[0].Minutes = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		horarioDesligarAgenda[0].Minutes *= 10;
		horarioDesligarAgenda[0].Minutes += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioLigarAgenda[1].Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioLigarAgenda[1].Hours *= 10;
		horarioLigarAgenda[1].Hours += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioLigarAgenda[1].Minutes = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		horarioLigarAgenda[1].Minutes *= 10;
		horarioLigarAgenda[1].Minutes += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioDesligarAgenda[1].Hours = 0;
	for(; bufferSoquete[ponteiro] != ':'; ponteiro ++) {
		horarioDesligarAgenda[1].Hours *= 10;
		horarioDesligarAgenda[1].Hours += charToByte(bufferSoquete[ponteiro]);
	}

	ponteiro ++;
	horarioDesligarAgenda[1].Minutes = 0;
	for(; bufferSoquete[ponteiro] != ',' && bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		horarioDesligarAgenda[1].Minutes *= 10;
		horarioDesligarAgenda[1].Minutes += charToByte(bufferSoquete[ponteiro]);
	}

	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
	}

	ponteiro ++;
	dado = 0;
	for(; bufferSoquete[ponteiro] != ','; ponteiro ++) {
		dado *= 10;
		dado += charToByte(bufferSoquete[ponteiro]);
	}
	diaDaSemanaAgenda[0] = bitReset(dado, 7);

	ponteiro ++;
	dado = 0;
	for(; bufferSoquete[ponteiro] != ',' && bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		dado *= 10;
		dado += charToByte(bufferSoquete[ponteiro]);
	}
	diaDaSemanaAgenda[1] = bitReset(dado, 7);

	for(; bufferSoquete[ponteiro] != ' '; ponteiro ++) {
		if(ponteiro >= TAMANHO_BUFFER_SOQUETE) {
			return;
		}
	}

	ponteiro ++;
	acionamentoAgenda[0] = charToByte(bufferSoquete[ponteiro]);
	if(acionamentoAgenda[0] > MOTOR_ESQUERDA) {
		acionamentoAgenda[0] = MOTOR_DESLIGADO;
	}
	ponteiro += 2;
	acionamentoAgenda[1] = charToByte(bufferSoquete[ponteiro]);
	if(acionamentoAgenda[1] > MOTOR_ESQUERDA) {
		acionamentoAgenda[1] = MOTOR_DESLIGADO;
	}


	//Validação e preparação de variáveis auxiliares
	if(laminaDagua > VALOR_MAXIMO_LAMINA_MM) {
		laminaDagua = VALOR_MAXIMO_LAMINA_MM;
	}

	if(tipoSensorPressao == SENSOR_10BAR) {
		if(alarmePressao > 100) {
			alarmePressao = 100;
		}
	}
	if(tipoSensorPressao == SENSOR_16BAR) {
		if(alarmePressao > 160) {
			alarmePressao = 160;
		}
	}

	if(histereseAlarmePressao > 59) {
		histereseAlarmePressao = 59;
	}

	if(tempoPressurizacao > TEMPO_MAXIMO_PRESSURIZACAO) {
		tempoPressurizacao = TEMPO_MAXIMO_PRESSURIZACAO;
	}

	if(horarioInicioPonta.Hours > 23) {
		horarioInicioPonta.Hours = 0;
	}
	if(horarioInicioPonta.Minutes > 59) {
		horarioInicioPonta.Minutes = 0;
	}
	if(horarioFimPonta.Hours > 23) {
		horarioFimPonta.Hours = 0;
	}
	if(horarioFimPonta.Minutes > 59) {
		horarioFimPonta.Minutes = 0;
	}

	writeEepromLamina();
	writeEepromCicloIrrigacao();
	writeEepromFertiIrrigacao();
	writeEepromAlarmePressao();
	writeEepromTempoPressurizacao();
	writeEepromHorarioPonta();
	writeEepromPosicaoHome();
	writeEepromTabelaLaminaDagua();
	writeEepromTempoBaseLaminaDagua();
	writeEepromAgendaAcionamento(0);
	writeEepromAgendaAcionamento(1);

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		writeEepromObstaculo(i);
	}

	//DEBUG CONFIG
	limpaReporteService();
	sprintfReporteService(laminaDagua, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(cicloIrrigacao, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(flagFertiIrrigacao, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(alarmePressao, 0);
	strcat(reporteService, ",");
	sprintfReporteService(histereseAlarmePressao, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(tempoPressurizacao, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(horarioInicioPonta.Hours, 0);
	strcat(reporteService, ":");
	sprintfReporteService(horarioInicioPonta.Minutes, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(horarioFimPonta.Hours, 0);
	strcat(reporteService, ":");
	sprintfReporteService(horarioFimPonta.Minutes, 0);
	strcat(reporteService, "\n");

	for(uint8_t i = 0; i < QUANTIDADE_AGENDA_ACIONAMENTO; i ++) {
		sprintfReporteService(i, 0);
		strcat(reporteService, ") ");
		sprintfReporteService(horarioLigarAgenda[i].Hours, 2);
		strcat(reporteService, ":");
		sprintfReporteService(horarioLigarAgenda[i].Minutes, 2);
		strcat(reporteService, " ");
		sprintfReporteService(horarioDesligarAgenda[i].Hours, 2);
		strcat(reporteService, ":");
		sprintfReporteService(horarioDesligarAgenda[i].Minutes, 2);
		strcat(reporteService, " ");
		sprintfReporteService(diaDaSemanaAgenda[i], 0);
		strcat(reporteService, " ");
		sprintfReporteService(acionamentoAgenda[i], 0);
		strcat(reporteService, "\n");
	}

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		sprintfReporteService(raioAtuacaoObstaculo[i], 0);
		strcat(reporteService, ",");
	}
	strcat(reporteService, "\n");


	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		sprintfReporteService(i + 1, 0);
		strcat(reporteService, ") ");
		sprintfReporteService(flagObstaculoAtivado[i], 0);
		strcat(reporteService, " - ");

		sprintfReporteService(posicoesObstaculos[i].latitudeGraus, 2);
		sprintfReporteService(posicoesObstaculos[i].latitudeMinutos, 2);
		strcat(reporteService, ".");
		sprintfReporteService(posicoesObstaculos[i].latitudeSegundos, 5);
		if(posicoesObstaculos[i].hemisferio == 'S') {
			strcat(reporteService, ",S,");
		}
		else {
			strcat(reporteService, ",N,");
		}

		sprintfReporteService(posicoesObstaculos[i].longitudeGraus, 3);
		sprintfReporteService(posicoesObstaculos[i].longitudeMinutos, 2);
		strcat(reporteService, ".");
		sprintfReporteService(posicoesObstaculos[i].longitudeSegundos, 5);
		if(posicoesObstaculos[i].indicadorWE == 'W') {
			strcat(reporteService, ",W,");
		}
		else {
			strcat(reporteService, ",E,");
		}
		strcat(reporteService, "\r\n");
	}
	strcat(reporteService, "Home: ");

	sprintfReporteService(posicaoHome.latitudeGraus, 2);
	sprintfReporteService(posicaoHome.latitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoHome.latitudeSegundos, 5);
	if(posicaoHome.hemisferio == 'S') {
		strcat(reporteService, ",S,");
	}
	else {
		strcat(reporteService, ",N,");
	}

	sprintfReporteService(posicaoHome.longitudeGraus, 3);
	sprintfReporteService(posicaoHome.longitudeMinutos, 2);
	strcat(reporteService, ".");
	sprintfReporteService(posicaoHome.longitudeSegundos, 5);
	if(posicaoHome.indicadorWE == 'W') {
		strcat(reporteService, ",W,");
	}
	else {
		strcat(reporteService, ",E,");
	}
	strcat(reporteService, "\r\n");

	sprintfReporteService(tabelaLaminaDagua20, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua50, 0);
	strcat(reporteService, ",");
	sprintfReporteService(tabelaLaminaDagua70, 0);
	strcat(reporteService, "\n");
	sprintfReporteService(tempoBaseLaminaDagua, 0);
	strcat(reporteService, "\n");

	HAL_UART_Transmit(&huart1, &reporteService, strlen(reporteService), 500);
	apagaSoqueteBuffer();

	ultimoIdConfig = idRecebido;
	writeEepromIdConfig();
}
/*=============================================================================
LEITURA DE ACIONAMENTO
==============================================================================*/
void leituraAcionamentoService() {
	PacoteAcionamentoTypeDef pacote;
	uint16_t dado = 0;
	uint32_t idRecebido = 0;
	uint32_t idConfig = 0;
	signed int ponteiro = 0;

	if(!validaPacoteService()) {
		apagaSoqueteBuffer();
		return;
	}

	ponteiro = indexOf(bufferSoquete, "\"id\":");

	if(ponteiro < 0) {
		apagaSoqueteBuffer();
		return;
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			ponteiro = i;
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(bufferSoquete[i] == ',' || bufferSoquete[i] == '}') {
			i = TAMANHO_BUFFER_SOQUETE;
		}
		else {
			idRecebido *= 10;
			idRecebido += charToByte(bufferSoquete[i]);
		}
	}

	if(idRecebido == ultimoIdAcionamento) {
		apagaSoqueteBuffer();
		return;
	}

	ponteiro = indexOf(bufferSoquete, "\driveData\":");

	if(ponteiro < 0) {
		apagaSoqueteBuffer();
		return;
	}

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			ponteiro = i;
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	dado = charToByte(bufferSoquete[ponteiro]);
	if(dado == MODO_DESLOCAMENTO || dado == MODO_IRRIGACAO) {
		pacote.modoOperacao = dado;
	}
	ponteiro ++;

	dado = charToByte(bufferSoquete[ponteiro]);
	if(dado == REMOTO_DESLIGADO || dado == REMOTO_DIREITA || dado == REMOTO_ESQUERDA) {
		pacote.sentidoOperacao = dado;

		if(dado == REMOTO_DESLIGADO) {
			flagReportaDesligou = true;
		}
	}
	else {
		pacote.sentidoOperacao = REMOTO_DESLIGADO;
	}
	ponteiro ++;

	pacote.retornoHome = charToBool(bufferSoquete[ponteiro]);
	ponteiro += 2;

	dado = charToByte(bufferSoquete[ponteiro]);
	pacote.selecaoPivot = dado;
	ponteiro += 2;

	dado = 0;
	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			dado *= 10;
			dado += charToByte(bufferSoquete[i]);
		}
		else {
			pacote.valorLamina = dado;
			ponteiro = i + 1;
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	pacote.fertiIrrigacao = charToBool(bufferSoquete[ponteiro]);
	ponteiro += 2;

	pacote.overrideBombaBooster = charToBool(bufferSoquete[ponteiro]);
	ponteiro += 2;

	for(uint16_t i = ponteiro; i < TAMANHO_BUFFER_SOQUETE; i ++) {
		if(isNumber(bufferSoquete[i])) {
			idConfig *= 10;
			idConfig += charToByte(bufferSoquete[i]);
		}
		else {
			if(idConfig != ultimoIdConfig) {
				flagServiceLeituraConfig = true;
			}
			ponteiro = i + 1;
			i = TAMANHO_BUFFER_SOQUETE;
		}
	}

	dado = charToBool(bufferSoquete[ponteiro]);
	if(dado == 1) { //executa comando
		modoOperacaoRemoto = pacote.modoOperacao;
		sentidoRemoto = pacote.sentidoOperacao;
		flagRetornoHome = pacote.retornoHome;
		flagFertiIrrigacao = pacote.fertiIrrigacao;
		flagOverrideBombaBooster = pacote.overrideBombaBooster;

		if(pacote.valorLamina > 0 && pacote.valorLamina <= VALOR_MAXIMO_LAMINA_MM) {
			if(flagOperacao && pacote.valorLamina != laminaDagua) {
				flagTrocouLaminaDagua = true;
				trocaLaminaDagua = pacote.valorLamina;
			}
		}

		if(pacote.selecaoPivot == 1) {
			flagSelecaoPivot = false;
		}
		else if(pacote.selecaoPivot == 2) {
			flagSelecaoPivot = true;
		}

		strcat(reporteService, "Operacao: ");
		sprintfReporteService(modoOperacaoRemoto, 0);
		strcat(reporteService, "\r\n");

		strcat(reporteService, "Sentido: ");
		sprintfReporteService(sentidoRemoto, 0);
		strcat(reporteService, "\r\n");

		strcat(reporteService, "Home: ");
		sprintfReporteService(flagRetornoHome, 0);
		strcat(reporteService, "\r\n");

		strcat(reporteService, "Pivot: ");
		sprintfReporteService(flagSelecaoPivot, 0);
		strcat(reporteService, "\r\n");

		if(flagTrocouLaminaDagua) {
			strcat(reporteService, "Lamina Dagua: ");
			sprintfReporteService(trocaLaminaDagua, 0);
			strcat(reporteService, "\r\n");
		}

		strcat(reporteService, "Ferti: ");
		sprintfReporteService(flagFertiIrrigacao, 0);
		strcat(reporteService, "\r\n");

		strcat(reporteService, "Bomba Override: ");
		sprintfReporteService(flagOverrideBombaBooster, 0);
		strcat(reporteService, "\r\n");

		HAL_UART_Transmit(&huart1, &reporteService, strlen(reporteService), 500);
		apagaSoqueteBuffer();
		writeEepromFertiIrrigacao();
	}

	limpaReporteService();
	ultimoIdAcionamento = idRecebido;
	writeEepromIdAcionamento();
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
