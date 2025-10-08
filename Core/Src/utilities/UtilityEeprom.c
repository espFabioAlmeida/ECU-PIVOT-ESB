///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityEeprom
//AUTOR:      Fábio Almeida
//CRIADO:     03/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
#include "codigosLogs.h"
/*==============================================================================
CONSTANTES
==============================================================================*/
#define	EEPROM_ADDR_0	0x50
#define EEPROM_ADDR_1   0x51
#define EEPROM_ADDR_2   0x52
#define EEPROM_ADDR_3   0x53
#define EEPROM_ADDR_4   0x54
#define EEPROM_ADDR_5   0x55
#define EEPROM_ADDR_6   0x56
#define EEPROM_ADDR_7   0x57

#define	EEPROM_INICIALIZADA		0x0A
#define TAMANHO_EEPROM 0x7D00 //24lc256 -> 256000/8 = 32kbytes
/*==============================================================================
WRITE EXTERNAL EEPROM
==============================================================================*/
void writeExternalEeprom(uint8_t device, uint16_t address, uint8_t data) {
	uint8_t pData[3];
	uint8_t deviceHex = 0x00;
	pData[0] = make8(address, 1);
	pData[1] = make8(address, 0);
	pData[2] = data;

	switch(device) {
		case 0:	deviceHex = EEPROM_ADDR_0; break;
		case 1:	deviceHex = EEPROM_ADDR_1; break;
		case 2:	deviceHex = EEPROM_ADDR_2; break;
		case 3:	deviceHex = EEPROM_ADDR_3; break;
		case 4:	deviceHex = EEPROM_ADDR_4; break;
		case 5:	deviceHex = EEPROM_ADDR_5; break;
		case 6:	deviceHex = EEPROM_ADDR_6; break;
		case 7:	deviceHex = EEPROM_ADDR_7; break;
		default: return;
	}

	deviceHex <<= 1;

	HAL_I2C_Master_Transmit(&hi2c1, deviceHex, &pData, 3, 100);
	HAL_Delay(10);
}
/*==============================================================================
READ EXTERNAL EEPROM
==============================================================================*/
uint8_t readExternalEeprom(uint8_t device, uint16_t address) {
	uint8_t data = 0;
	uint8_t pData[2];
	uint8_t deviceHex = 0x00;
	pData[0] = make8(address, 1);
	pData[1] = make8(address, 0);

	switch(device) {
		case 0:	deviceHex = EEPROM_ADDR_0; break;
		case 1:	deviceHex = EEPROM_ADDR_1; break;
		case 2:	deviceHex = EEPROM_ADDR_2; break;
		case 3:	deviceHex = EEPROM_ADDR_3; break;
		case 4:	deviceHex = EEPROM_ADDR_4; break;
		case 5:	deviceHex = EEPROM_ADDR_5; break;
		case 6:	deviceHex = EEPROM_ADDR_6; break;
		case 7:	deviceHex = EEPROM_ADDR_7; break;
		default: return 0x00;
	}
	deviceHex <<= 1;

	HAL_I2C_Master_Transmit(&hi2c1, deviceHex, &pData, 2, 100);
	HAL_Delay(10);
	deviceHex ++;
	HAL_I2C_Master_Receive(&hi2c1, deviceHex, &data, 1, 100);
	HAL_Delay(10);

	return data;
}
/*==============================================================================
SALVA LAMINA DAGUA
==============================================================================*/
void writeEepromLamina() {
	writeExternalEeprom(0, 1, make8(laminaDagua, 1));
	writeExternalEeprom(0, 2, make8(laminaDagua, 0));
	calculaLaminaDagua();
}
/*==============================================================================
SALVA PLACA SOQUETE
==============================================================================*/
void writeEepromPlacaSoquete() {
	writeExternalEeprom(0, 3, placaSoquete);
}
/*==============================================================================
WRITE EEPROM - GPRS APN
==============================================================================*/
void writeEepromGPRSAPN() {
	for(uint16_t i = 0; i < QUANTIDADE_CHAR_GPRS_APN; i ++) {
		if(gprsAPN[i] == ' ' || gprsAPN[i] == 0xFF) {
			gprsAPN[i] = 0x00;
		}
	}

	for(uint16_t i = 0; i < QUANTIDADE_CHAR_GPRS_APN; i ++) {
		writeExternalEeprom(0, 284 + i, gprsAPN[i]);
	}
}
/*==============================================================================
WRITE EEPROM - HORARIO DE PONTA
==============================================================================*/
void writeEepromHorarioPonta() {
	writeExternalEeprom(0, 5, horarioInicioPonta.Hours);
	writeExternalEeprom(0, 6, horarioInicioPonta.Minutes);
	writeExternalEeprom(0, 7, horarioFimPonta.Hours);
	writeExternalEeprom(0, 8, horarioFimPonta.Minutes);
}
/*==============================================================================
WRITE EEPROM - OPERACAO
==============================================================================*/
void writeEepromOperacao() {
	writeExternalEeprom(0, 4, agendaHorarioAcionado);
	writeExternalEeprom(0, 10, modoOperacao);
	writeExternalEeprom(0, 11, sentidoMotor);
	writeExternalEeprom(0, 251, flagRetornoHome);
}
/*==============================================================================
WRITE EEPROM - SENHA
==============================================================================*/
void writeEepromSenha() {
	writeExternalEeprom(0, 12, make8(senha, 1));
	writeExternalEeprom(0, 13, make8(senha, 0));
}
/*==============================================================================
SALVA CICLO IRRIGACAO
==============================================================================*/
void writeEepromCicloIrrigacao() {
	writeExternalEeprom(0, 14, cicloIrrigacao);
}
/*==============================================================================
SALVA FERTI IRRIGACAO
==============================================================================*/
void writeEepromFertiIrrigacao() {
	writeExternalEeprom(0, 15, flagFertiIrrigacao);
}
/*==============================================================================
SALVA ALARME PRESSAO
==============================================================================*/
void writeEepromAlarmePressao() {
	writeExternalEeprom(0, 16, alarmePressao);
	writeExternalEeprom(0, 17, histereseAlarmePressao);
}
/*==============================================================================
SALVA TEMPO PRESSURIZACAOa
==============================================================================*/
void writeEepromTempoPressurizacao() {
	writeExternalEeprom(0, 18, tempoPressurizacao);
	tempoPressurizacaoSegundos = tempoPressurizacao * 60;
	contadorBombaPrincipal = tempoPressurizacaoSegundos;
}
/*==============================================================================
WRITE EEPROM - NUMERO SERIAL
==============================================================================*/
void writeEepromNumeroSerial() {
	writeExternalEeprom(0, 51, make8(numeroSerial, 3));
	writeExternalEeprom(0, 52, make8(numeroSerial, 2));
	writeExternalEeprom(0, 53, make8(numeroSerial, 1));
	writeExternalEeprom(0, 54, make8(numeroSerial, 0));
}
/*==============================================================================
WRITE EEPROM POSICAO ATUAL GPS
==============================================================================*/
void writeEepromPosicaoAtualGPS() {
	writeExternalEeprom(0, 19, posicaoAtualGPS.hemisferio);
	writeExternalEeprom(0, 20, posicaoAtualGPS.indicadorWE);

	writeExternalEeprom(0, 21, make8(posicaoAtualGPS.latitudeGraus, 1));
	writeExternalEeprom(0, 22, make8(posicaoAtualGPS.latitudeGraus, 0));
	writeExternalEeprom(0, 23, make8(posicaoAtualGPS.longitudeGraus, 1));
	writeExternalEeprom(0, 24, make8(posicaoAtualGPS.longitudeGraus, 0));

	writeExternalEeprom(0, 25, make8(posicaoAtualGPS.latitudeMinutos, 1));
	writeExternalEeprom(0, 26, make8(posicaoAtualGPS.latitudeMinutos, 0));
	writeExternalEeprom(0, 27, make8(posicaoAtualGPS.longitudeMinutos, 1));
	writeExternalEeprom(0, 28, make8(posicaoAtualGPS.longitudeMinutos, 0));

	writeExternalEeprom(0, 29, make8(posicaoAtualGPS.latitudeSegundos, 3));
	writeExternalEeprom(0, 30, make8(posicaoAtualGPS.latitudeSegundos, 2));
	writeExternalEeprom(0, 31, make8(posicaoAtualGPS.latitudeSegundos, 1));
	writeExternalEeprom(0, 32, make8(posicaoAtualGPS.latitudeSegundos, 0));

	writeExternalEeprom(0, 33, make8(posicaoAtualGPS.longitudeSegundos, 3));
	writeExternalEeprom(0, 34, make8(posicaoAtualGPS.longitudeSegundos, 2));
	writeExternalEeprom(0, 35, make8(posicaoAtualGPS.longitudeSegundos, 1));
	writeExternalEeprom(0, 36, make8(posicaoAtualGPS.longitudeSegundos, 0));
}
/*==============================================================================
WRITE EEPROM - OBSTACULO
==============================================================================*/
void writeEepromObstaculo(uint8_t offset) {
	writeExternalEeprom(0, 55 + offset, posicoesObstaculos[offset].hemisferio);
	writeExternalEeprom(0, 63 + offset, posicoesObstaculos[offset].indicadorWE);
	writeExternalEeprom(0, 71 + offset, flagObstaculoAtivado[offset]);

	writeExternalEeprom(0, 79 + offset, make8(posicoesObstaculos[offset].latitudeGraus, 1));
	writeExternalEeprom(0, 87 + offset, make8(posicoesObstaculos[offset].latitudeGraus, 0));
	writeExternalEeprom(0, 95 + offset, make8(posicoesObstaculos[offset].longitudeGraus, 1));
	writeExternalEeprom(0, 103 + offset, make8(posicoesObstaculos[offset].longitudeGraus, 0));

	writeExternalEeprom(0, 111 + offset, make8(posicoesObstaculos[offset].latitudeMinutos, 1));
	writeExternalEeprom(0, 119 + offset, make8(posicoesObstaculos[offset].latitudeMinutos, 0));
	writeExternalEeprom(0, 127 + offset, make8(posicoesObstaculos[offset].longitudeMinutos, 1));
	writeExternalEeprom(0, 135 + offset, make8(posicoesObstaculos[offset].longitudeMinutos, 0));

	writeExternalEeprom(0, 143 + offset, make8(posicoesObstaculos[offset].latitudeSegundos, 3));
	writeExternalEeprom(0, 151 + offset, make8(posicoesObstaculos[offset].latitudeSegundos, 2));
	writeExternalEeprom(0, 159 + offset, make8(posicoesObstaculos[offset].latitudeSegundos, 1));
	writeExternalEeprom(0, 167 + offset, make8(posicoesObstaculos[offset].latitudeSegundos, 0));

	writeExternalEeprom(0, 175 + offset, make8(posicoesObstaculos[offset].longitudeSegundos, 3));
	writeExternalEeprom(0, 183 + offset, make8(posicoesObstaculos[offset].longitudeSegundos, 2));
	writeExternalEeprom(0, 191 + offset, make8(posicoesObstaculos[offset].longitudeSegundos, 1));
	writeExternalEeprom(0, 199 + offset, make8(posicoesObstaculos[offset].longitudeSegundos, 0));

	writeExternalEeprom(0, 211 + offset, make8(raioAtuacaoObstaculo[offset], 1));
	writeExternalEeprom(0, 219 + offset, make8(raioAtuacaoObstaculo[offset], 0));
}
/*==============================================================================
WRITE EEPROM - HORIMETRO
==============================================================================*/
void writeEepromHorimetro() {
	writeExternalEeprom(0, 9, horimetro.minutos);
	writeExternalEeprom(0, 207, make8(horimetro.horas, 3));
	writeExternalEeprom(0, 208, make8(horimetro.horas, 2));
	writeExternalEeprom(0, 209, make8(horimetro.horas, 1));
	writeExternalEeprom(0, 210, make8(horimetro.horas, 0));
}
/*==============================================================================
WRITE EEPROM - ENDERECO LoRa
==============================================================================*/
void writeEepromEnderecoLoRa() {
	writeExternalEeprom(0, 227, make8(enderecoLoRaPivot, 1));
	writeExternalEeprom(0, 228, make8(enderecoLoRaPivot, 0));
	writeExternalEeprom(0, 229, make8(enderecoLoRaGNSS, 1));
	writeExternalEeprom(0, 230, make8(enderecoLoRaGNSS, 0));
}
/*==============================================================================
WRITE EEPROM - CANAL LoRa
==============================================================================*/
void writeEepromCanalLoRa() {
	writeExternalEeprom(0, 231, canalLoRa);
}
/*==============================================================================
WRITE EEPROM - COMUNICAÇÃO GNSS
==============================================================================*/
void writeEepromComunicacaoGNSS() {
	writeExternalEeprom(0, 232, comunicacaoGNSS);
}
/*==============================================================================
WRITE EEPROM - POSIÇÃO HOME
==============================================================================*/
void writeEepromPosicaoHome() {
	writeExternalEeprom(0, 233, posicaoHome.hemisferio);
	writeExternalEeprom(0, 234, posicaoHome.indicadorWE);
	writeExternalEeprom(0, 235, make8(posicaoHome.latitudeGraus, 1));
	writeExternalEeprom(0, 236, make8(posicaoHome.latitudeGraus, 0));
	writeExternalEeprom(0, 237, make8(posicaoHome.longitudeGraus, 1));
	writeExternalEeprom(0, 238, make8(posicaoHome.longitudeGraus, 0));
	writeExternalEeprom(0, 239, make8(posicaoHome.latitudeMinutos, 1));
	writeExternalEeprom(0, 240, make8(posicaoHome.latitudeMinutos, 0));
	writeExternalEeprom(0, 241, make8(posicaoHome.longitudeMinutos, 1));
	writeExternalEeprom(0, 242, make8(posicaoHome.longitudeMinutos, 0));
	writeExternalEeprom(0, 243, make8(posicaoHome.latitudeSegundos, 3));
	writeExternalEeprom(0, 244, make8(posicaoHome.latitudeSegundos, 2));
	writeExternalEeprom(0, 245, make8(posicaoHome.latitudeSegundos, 1));
	writeExternalEeprom(0, 246, make8(posicaoHome.latitudeSegundos, 0));
	writeExternalEeprom(0, 247, make8(posicaoHome.longitudeSegundos, 3));
	writeExternalEeprom(0, 248, make8(posicaoHome.longitudeSegundos, 2));
	writeExternalEeprom(0, 249, make8(posicaoHome.longitudeSegundos, 1));
	writeExternalEeprom(0, 250, make8(posicaoHome.longitudeSegundos, 0));
}
/*==============================================================================
WRITE ID ACIONAMENTO
==============================================================================*/
void writeEepromIdAcionamento() {
	writeExternalEeprom(0, 252, make8(ultimoIdAcionamento, 3));
	writeExternalEeprom(0, 253, make8(ultimoIdAcionamento, 2));
	writeExternalEeprom(0, 254, make8(ultimoIdAcionamento, 1));
	writeExternalEeprom(0, 255, make8(ultimoIdAcionamento, 0));
}
/*==============================================================================
WRITE ID CONFIG
==============================================================================*/
void writeEepromIdConfig() {
	writeExternalEeprom(0, 256, make8(ultimoIdConfig, 3));
	writeExternalEeprom(0, 257, make8(ultimoIdConfig, 2));
	writeExternalEeprom(0, 258, make8(ultimoIdConfig, 1));
	writeExternalEeprom(0, 259, make8(ultimoIdConfig, 0));
}
/*==============================================================================
WRITE TEMPO BASE LAMINA DAGUA
==============================================================================*/
void writeEepromTempoBaseLaminaDagua() {
	writeExternalEeprom(0, 260, make8(tempoBaseLaminaDagua, 1));
	writeExternalEeprom(0, 261, make8(tempoBaseLaminaDagua, 0));
	calculaLaminaDagua();
}
/*==============================================================================
WRITE TABELA LAMINA DAGUA
==============================================================================*/
void writeEepromTabelaLaminaDagua() {
	writeExternalEeprom(0, 320, make8(tabelaLaminaDagua05, 1));
	writeExternalEeprom(0, 321, make8(tabelaLaminaDagua05, 0));
	writeExternalEeprom(0, 322, make8(tabelaLaminaDagua12, 1));
	writeExternalEeprom(0, 323, make8(tabelaLaminaDagua12, 0));

	writeExternalEeprom(0, 262, make8(tabelaLaminaDagua20, 1));
	writeExternalEeprom(0, 263, make8(tabelaLaminaDagua20, 0));

	writeExternalEeprom(0, 324, make8(tabelaLaminaDagua30, 1));
	writeExternalEeprom(0, 325, make8(tabelaLaminaDagua30, 0));
	writeExternalEeprom(0, 326, make8(tabelaLaminaDagua40, 1));
	writeExternalEeprom(0, 327, make8(tabelaLaminaDagua40, 0));

	writeExternalEeprom(0, 264, make8(tabelaLaminaDagua50, 1));
	writeExternalEeprom(0, 265, make8(tabelaLaminaDagua50, 0));
	writeExternalEeprom(0, 266, make8(tabelaLaminaDagua70, 1));
	writeExternalEeprom(0, 267, make8(tabelaLaminaDagua70, 0));

	calculaLaminaDagua();
}
/*==============================================================================
WRITE EEPROM - WIFI SSID
==============================================================================*/
void writeEepromWiFiSSID() {
	for(uint16_t i = 0; i < QUANTIDADE_CHAR_WIFI_SSID; i ++) {
		writeExternalEeprom(0, i + 268, wifiSSID[i]);
	}
}
/*==============================================================================
WRITE EEPROM - WIFI SENHA
==============================================================================*/
void writeEepromWiFiSenha() {
	for(uint16_t i = 0; i < QUANTIDADE_CHAR_WIFI_SENHA; i ++) {
		writeExternalEeprom(0, i + 300, wifiSenha[i]);
	}
}
/*==============================================================================
WRITE EEPROM - WIFI DHCP
==============================================================================*/
void writeEepromWiFiDhcp() {
	writeExternalEeprom(0, 49, flagWiFiDhcp);
}
/*==============================================================================
WRITE EEPROM - WIFI IP
==============================================================================*/
void writeEepromWiFiIp() {
	for(uint8_t i = 0; i < 4; i ++) {
		writeExternalEeprom(0, 37 + i, wifiIp[i]);
	}
}
/*==============================================================================
WRITE EEPROM - WIFI DNS
==============================================================================*/
void writeEepromWiFiDNS() {
	for(uint8_t i = 0; i < 4; i ++) {
		writeExternalEeprom(0, 41 + i, wifiDNS[i]);
	}
}
/*==============================================================================
WRITE EEPROM - WIFI GATEWAY
==============================================================================*/
void writeEepromWiFiGateway() {
	for(uint8_t i = 0; i < 4; i ++) {
		writeExternalEeprom(0, 45 + i, wifiGateway[i]);
	}
}
/*==============================================================================
WRITE EEPROM - WIFI MAK
==============================================================================*/
void writeEepromWiFiMask() {
	for(uint16_t i = 0; i < 4; i ++) {
		writeExternalEeprom(0, 316 + i, wifiMask[i]);
	}
}
/*==============================================================================
WRITE EEPROM - AGENDA ACIONAMENTO
==============================================================================*/
void writeEepromAgendaAcionamento(uint8_t offset) {
	if(offset >= QUANTIDADE_AGENDA_ACIONAMENTO) {
		return;
	}
	writeExternalEeprom(0, 328 + offset, horarioLigarAgenda[offset].Hours);
	writeExternalEeprom(0, 330 + offset, horarioLigarAgenda[offset].Minutes);
	writeExternalEeprom(0, 332 + offset, horarioDesligarAgenda[offset].Hours);
	writeExternalEeprom(0, 334 + offset, horarioDesligarAgenda[offset].Minutes);
	writeExternalEeprom(0, 336 + offset, diaDaSemanaAgenda[offset]);
	writeExternalEeprom(0, 338 + offset, acionamentoAgenda[offset]);
}
/*==============================================================================
WRITE EEPROM - POSIÇÃO INICIO OPERAÇÃO
==============================================================================*/
void writeEepromPosicaoInicioOperacao() {
	writeExternalEeprom(0, 340, posicaoInicioOperacao.hemisferio);
	writeExternalEeprom(0, 341, posicaoInicioOperacao.indicadorWE);
	writeExternalEeprom(0, 342, make8(posicaoInicioOperacao.latitudeGraus, 1));
	writeExternalEeprom(0, 343, make8(posicaoInicioOperacao.latitudeGraus, 0));
	writeExternalEeprom(0, 344, make8(posicaoInicioOperacao.longitudeGraus, 1));
	writeExternalEeprom(0, 345, make8(posicaoInicioOperacao.longitudeGraus, 0));
	writeExternalEeprom(0, 346, make8(posicaoInicioOperacao.latitudeMinutos, 1));
	writeExternalEeprom(0, 347, make8(posicaoInicioOperacao.latitudeMinutos, 0));
	writeExternalEeprom(0, 348, make8(posicaoInicioOperacao.longitudeMinutos, 1));
	writeExternalEeprom(0, 349, make8(posicaoInicioOperacao.longitudeMinutos, 0));
	writeExternalEeprom(0, 350, make8(posicaoInicioOperacao.latitudeSegundos, 3));
	writeExternalEeprom(0, 351, make8(posicaoInicioOperacao.latitudeSegundos, 2));
	writeExternalEeprom(0, 352, make8(posicaoInicioOperacao.latitudeSegundos, 1));
	writeExternalEeprom(0, 353, make8(posicaoInicioOperacao.latitudeSegundos, 0));
	writeExternalEeprom(0, 354, make8(posicaoInicioOperacao.longitudeSegundos, 3));
	writeExternalEeprom(0, 355, make8(posicaoInicioOperacao.longitudeSegundos, 2));
	writeExternalEeprom(0, 356, make8(posicaoInicioOperacao.longitudeSegundos, 1));
	writeExternalEeprom(0, 357, make8(posicaoInicioOperacao.longitudeSegundos, 0));

	writeExternalEeprom(0, 358, flagPosicaoInicioOperacaoConfiavel);
}
/*==============================================================================
WRITE EEPROM - ENDERECO LoRa/GATEWAY
==============================================================================*/
void writeEepromEnderecoLoRaGateway() {
	writeExternalEeprom(0, 359, make8(enderecoLoRaPivotGateway, 1));
	writeExternalEeprom(0, 360, make8(enderecoLoRaPivotGateway, 0));
	writeExternalEeprom(0, 361, make8(enderecoLoRaGateway, 1));
	writeExternalEeprom(0, 362, make8(enderecoLoRaGateway, 0));
}
/*==============================================================================
WRITE EEPROM - CANAL LoRa
==============================================================================*/
void writeEepromCanalLoRaGatway() {
	writeExternalEeprom(0, 363, canalLoRaGateway);
}
/*==============================================================================
WRITE EEPROM - DADOS HORARIO PONTA
==============================================================================*/
void writeEepromDadosHorarioPonta() {
	writeExternalEeprom(0, 364, modoOperacaoHorarioPonta);
	writeExternalEeprom(0, 365, sentidoHorarioPonta);
	writeExternalEeprom(0, 366, flagRetornoHomeHorarioPonta);
	writeExternalEeprom(0, 367, flagSaiuHorarioPonta);
}
/*==============================================================================
WRITE ALL EEPROM
==============================================================================*/
void writeAllEeprom() {
	writeEepromLamina();
	writeEepromPlacaSoquete();
	writeEepromHorarioPonta();
	writeEepromSenha();
	writeEepromPosicaoAtualGPS();
	writeEepromCicloIrrigacao();
	writeEepromFertiIrrigacao();
	writeEepromAlarmePressao();
	writeEepromTempoPressurizacao();
	writeEepromNumeroSerial();
	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		writeEepromObstaculo(i);
	}
	writeEepromHorimetro();
	writeEepromEnderecoLoRa();
	writeEepromCanalLoRa();
	writeEepromComunicacaoGNSS();
	writeEepromOperacao();
	writeEepromPosicaoHome();
	writeEepromTempoBaseLaminaDagua();
	writeEepromTabelaLaminaDagua();
	writeEepromGPRSAPN();

	for(uint8_t i = 0; i < QUANTIDADE_AGENDA_ACIONAMENTO; i ++) {
		writeEepromAgendaAcionamento(i);
	}

	writeEepromEnderecoLoRaGateway();
	writeEepromCanalLoRaGatway();
	writeEepromDadosHorarioPonta();
}
/*==============================================================================
READ EEPROM
==============================================================================*/
void readEeprom() {
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_0 + 1, 10, 100)) {
		laminaDagua = make16(readExternalEeprom(0, 1), readExternalEeprom(0, 2));

		placaSoquete = readExternalEeprom(0, 3);

		agendaHorarioAcionado = readExternalEeprom(0, 4);

		horarioInicioPonta.Hours = readExternalEeprom(0, 5);
		horarioInicioPonta.Minutes = readExternalEeprom(0, 6);
		horarioFimPonta.Hours = readExternalEeprom(0, 7);
		horarioFimPonta.Minutes = readExternalEeprom(0, 8);

		horimetro.minutos = readExternalEeprom(0, 9);

		modoOperacao = readExternalEeprom(0, 10);
		sentidoMotor = readExternalEeprom(0, 11);

		if(modoOperacao != MODO_DESLOCAMENTO && modoOperacao != MODO_IRRIGACAO) {
			modoOperacao = MODO_DESLOCAMENTO;
		}

		if(sentidoMotor != MOTOR_DESLIGADO && sentidoMotor != MOTOR_DIREITA && sentidoMotor != MOTOR_ESQUERDA) {
			sentidoMotor = MOTOR_DESLIGADO;
		}

		salvaLogEeprom();
		if(sentidoMotor == MOTOR_DIREITA) {
			flagManualDireita = true;
			flagAcionamentoPelaEeprom = true;
		}
		else if(sentidoMotor == MOTOR_ESQUERDA) {
			flagManualEsquerda = true;
			flagAcionamentoPelaEeprom = true;
		}
		sentidoMotor = MOTOR_DESLIGADO; //retorna para desligado e deixa os flags acionarem os motores

		senha = make16(readExternalEeprom(0, 12), readExternalEeprom(0, 13));
		cicloIrrigacao = readExternalEeprom(0, 14);
		flagFertiIrrigacao = readExternalEeprom(0, 15);
		alarmePressao = readExternalEeprom(0, 16);
		histereseAlarmePressao = readExternalEeprom(0, 17);
		tempoPressurizacao = readExternalEeprom(0, 18);
		tempoPressurizacaoSegundos = tempoPressurizacao * 60;
		contadorBombaPrincipal = tempoPressurizacaoSegundos;

		zeraPosicaoGNSS();
		posicaoAtualGPS.hemisferio = readExternalEeprom(0, 19);
		posicaoAtualGPS.indicadorWE = readExternalEeprom(0, 20);
		posicaoAtualGPS.latitudeGraus = make16(readExternalEeprom(0, 21), readExternalEeprom(0, 22));
		posicaoAtualGPS.longitudeGraus = make16(readExternalEeprom(0, 23), readExternalEeprom(0, 24));
		posicaoAtualGPS.latitudeMinutos = make16(readExternalEeprom(0, 25), readExternalEeprom(0, 26));
		posicaoAtualGPS.longitudeMinutos = make16(readExternalEeprom(0, 27), readExternalEeprom(0, 28));
		posicaoAtualGPS.latitudeSegundos = make32(readExternalEeprom(0, 29), readExternalEeprom(0, 30),
				readExternalEeprom(0, 31), readExternalEeprom(0, 32));
		posicaoAtualGPS.longitudeSegundos = make32(readExternalEeprom(0, 33), readExternalEeprom(0, 34),
							readExternalEeprom(0, 35), readExternalEeprom(0, 36));

		if(posicaoAtualGPS.hemisferio != 'N' && posicaoAtualGPS.hemisferio != 'S') {
			posicaoAtualGPS.hemisferio = 'N';
		}

		if(posicaoAtualGPS.indicadorWE != 'E' && posicaoAtualGPS.indicadorWE != 'W') {
			posicaoAtualGPS.indicadorWE = 'E';
		}

		for(uint8_t i = 0; i < 4; i ++) {
			wifiIp[i] = readExternalEeprom(0, 37 + i);
			wifiDNS[i] = readExternalEeprom(0, 41 + i);
			wifiGateway[i] = readExternalEeprom(0, 45 + i);
		}

		flagWiFiDhcp = readExternalEeprom(0, 49);

		//50 - Livre

		numeroSerial = make32(readExternalEeprom(0, 51), readExternalEeprom(0, 52),
				readExternalEeprom(0, 53), readExternalEeprom(0, 54));

		for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
			posicoesObstaculos[i].hemisferio = readExternalEeprom(0, 55 + i);
			posicoesObstaculos[i].indicadorWE = readExternalEeprom(0, 63 + i);
			flagObstaculoAtivado[i] = readExternalEeprom(0, 71 + i);
			posicoesObstaculos[i].latitudeGraus = make16(readExternalEeprom(0, 79 + i), readExternalEeprom(0, 87 + i));
			posicoesObstaculos[i].longitudeGraus = make16(readExternalEeprom(0, 95 + i), readExternalEeprom(0, 103 + i));
			posicoesObstaculos[i].latitudeMinutos = make16(readExternalEeprom(0, 111 + i), readExternalEeprom(0, 119 + i));
			posicoesObstaculos[i].longitudeMinutos = make16(readExternalEeprom(0, 127 + i), readExternalEeprom(0, 135 + i));
			posicoesObstaculos[i].latitudeSegundos = make32(readExternalEeprom(0, 143 + i), readExternalEeprom(0, 151 + i),
					readExternalEeprom(0, 159 + i), readExternalEeprom(0, 167 + i));
			posicoesObstaculos[i].longitudeSegundos = make32(readExternalEeprom(0, 175 + i), readExternalEeprom(0, 183 + i),
								readExternalEeprom(0, 191 + i), readExternalEeprom(0, 199 + i));

			raioAtuacaoObstaculo[i] = make16(readExternalEeprom(0, 211 + i), readExternalEeprom(0, 219 + i));
		}

		horimetro.horas = make32(readExternalEeprom(0, 207), readExternalEeprom(0, 208),
				readExternalEeprom(0, 209), readExternalEeprom(0, 210));

		enderecoLoRaPivot = make16(readExternalEeprom(0, 227), readExternalEeprom(0, 228));
		enderecoLoRaGNSS = make16(readExternalEeprom(0, 229), readExternalEeprom(0, 230));
		canalLoRa = readExternalEeprom(0, 231);
		comunicacaoGNSS = readExternalEeprom(0, 232);

		posicaoHome.hemisferio = readExternalEeprom(0, 233);
		posicaoHome.indicadorWE = readExternalEeprom(0, 234);
		posicaoHome.latitudeGraus = make16(readExternalEeprom(0, 235), readExternalEeprom(0, 236));
		posicaoHome.longitudeGraus = make16(readExternalEeprom(0, 237), readExternalEeprom(0, 238));
		posicaoHome.latitudeMinutos = make16(readExternalEeprom(0, 239), readExternalEeprom(0, 240));
		posicaoHome.longitudeMinutos = make16(readExternalEeprom(0, 241), readExternalEeprom(0, 242));
		posicaoHome.latitudeSegundos = make32(readExternalEeprom(0, 243), readExternalEeprom(0, 244),
				readExternalEeprom(0, 245), readExternalEeprom(0, 246));
		posicaoHome.longitudeSegundos = make32(readExternalEeprom(0, 247), readExternalEeprom(0, 248),
				readExternalEeprom(0, 249), readExternalEeprom(0, 250));

		flagRetornoHome = readExternalEeprom(0, 251);

		ultimoIdAcionamento = make32(readExternalEeprom(0, 252), readExternalEeprom(0, 253),
				readExternalEeprom(0, 254), readExternalEeprom(0, 255));
		ultimoIdConfig = make32(readExternalEeprom(0, 256), readExternalEeprom(0, 257),
				readExternalEeprom(0, 258), readExternalEeprom(0, 259));

		tempoBaseLaminaDagua = make16(readExternalEeprom(0, 260), readExternalEeprom(0, 261));
		tabelaLaminaDagua20 = make16(readExternalEeprom(0, 262), readExternalEeprom(0, 263));
		tabelaLaminaDagua50 = make16(readExternalEeprom(0, 264), readExternalEeprom(0, 265));
		tabelaLaminaDagua70 = make16(readExternalEeprom(0, 266), readExternalEeprom(0, 267));

		memset(wifiSSID, 0x00, QUANTIDADE_CHAR_WIFI_SSID + 1);
		for(uint16_t i = 0; i < QUANTIDADE_CHAR_WIFI_SSID; i ++) {
			wifiSSID[i] = readExternalEeprom(0, i + 268);
		}

		for(uint16_t i = 0; i < QUANTIDADE_CHAR_GPRS_APN; i ++) {
			gprsAPN[i] = readExternalEeprom(0, 284 + i);
		}

		memset(wifiSenha, 0x00, QUANTIDADE_CHAR_WIFI_SENHA + 1);
		for(uint16_t i = 0; i < QUANTIDADE_CHAR_WIFI_SENHA; i ++) {
			wifiSenha[i] = readExternalEeprom(0, i + 300);
		}

		for(uint16_t i = 0; i < 4; i ++) {
			wifiMask[i] = readExternalEeprom(0, i + 316);
		}

		tabelaLaminaDagua05 = make16(readExternalEeprom(0, 320), readExternalEeprom(0, 321));
		tabelaLaminaDagua12 = make16(readExternalEeprom(0, 322), readExternalEeprom(0, 323));
		tabelaLaminaDagua30 = make16(readExternalEeprom(0, 324), readExternalEeprom(0, 325));
		tabelaLaminaDagua40 = make16(readExternalEeprom(0, 326), readExternalEeprom(0, 327));

		for(uint16_t i = 0; i < QUANTIDADE_AGENDA_ACIONAMENTO; i ++) {
			horarioLigarAgenda[i].Hours = readExternalEeprom(0, 328 + i);
			horarioLigarAgenda[i].Minutes = readExternalEeprom(0, 330 + i);
			horarioDesligarAgenda[i].Hours = readExternalEeprom(0, 332 + i);
			horarioDesligarAgenda[i].Minutes = readExternalEeprom(0, 334 + i);
			diaDaSemanaAgenda[i] = readExternalEeprom(0, 336 + i);
			acionamentoAgenda[i] = readExternalEeprom(0, 338 + i);
		}

		posicaoInicioOperacao.hemisferio = readExternalEeprom(0, 340);
		posicaoInicioOperacao.indicadorWE = readExternalEeprom(0, 341);
		posicaoInicioOperacao.latitudeGraus = make16(readExternalEeprom(0, 342), readExternalEeprom(0, 343));
		posicaoInicioOperacao.longitudeGraus = make16(readExternalEeprom(0, 344), readExternalEeprom(0, 345));
		posicaoInicioOperacao.latitudeMinutos = make16(readExternalEeprom(0, 346), readExternalEeprom(0, 347));
		posicaoInicioOperacao.longitudeMinutos = make16(readExternalEeprom(0, 348), readExternalEeprom(0, 349));
		posicaoInicioOperacao.latitudeSegundos = make32(readExternalEeprom(0, 350), readExternalEeprom(0, 351),
				readExternalEeprom(0, 352), readExternalEeprom(0, 353));
		posicaoInicioOperacao.longitudeSegundos = make32(readExternalEeprom(0, 354), readExternalEeprom(0, 355),
							readExternalEeprom(0, 356), readExternalEeprom(0, 357));

		if(posicaoInicioOperacao.hemisferio != 'N' && posicaoInicioOperacao.hemisferio != 'S') {
			posicaoInicioOperacao.hemisferio = 'N';
		}

		if(posicaoInicioOperacao.indicadorWE != 'E' && posicaoInicioOperacao.indicadorWE != 'W') {
			posicaoInicioOperacao.indicadorWE = 'E';
		}

		flagPosicaoInicioOperacaoConfiavel = readExternalEeprom(0, 358);

		enderecoLoRaPivotGateway = make16(readExternalEeprom(0, 359), readExternalEeprom(0, 360));
		enderecoLoRaGateway = make16(readExternalEeprom(0, 361), readExternalEeprom(0, 362));
		canalLoRaGateway = readExternalEeprom(0, 363);

		modoOperacaoHorarioPonta = readExternalEeprom(0, 364);
		sentidoHorarioPonta = readExternalEeprom(0, 365);
		flagRetornoHomeHorarioPonta = readExternalEeprom(0, 366);
		flagSaiuHorarioPonta = readExternalEeprom(0, 367);
	}
}
/*==============================================================================
APAGA EEPROM
==============================================================================*/
void apagaEeprom() {
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_0 + 1, 10, 100)) {
		placaSoquete = SOQUETE_GPRS;
		laminaDagua = 1000;
		senha = 0;
		alarmePressao = 80;
		histereseAlarmePressao = 10;
		tempoPressurizacao = 1;
		cicloIrrigacao = IRRIGACAO_1_CICLO;
		modoOperacao = MODO_DESLOCAMENTO;
		sentidoMotor = MOTOR_DESLIGADO;
		agendaHorarioAcionado = 99;
		flagRetornoHome = false;
		flagFertiIrrigacao = false;
		numeroSerial = 0;
		tempoBaseLaminaDagua = 200;
		tabelaLaminaDagua05 = 7000;
		tabelaLaminaDagua12 = 5000;
		tabelaLaminaDagua20 = 4000;
		tabelaLaminaDagua30 = 3000;
		tabelaLaminaDagua40 = 2000;
		tabelaLaminaDagua50 = 1200;
		tabelaLaminaDagua70 = 500;

		ultimoIdAcionamento = 0;
		ultimoIdConfig = 0;

		memset(gprsAPN, 0x00, QUANTIDADE_CHAR_GPRS_APN);
		strcat(gprsAPN, "emnify");

		memset(wifiSSID, 0x00, QUANTIDADE_CHAR_WIFI_SSID + 1);
		memset(wifiSenha, 0x00, QUANTIDADE_CHAR_WIFI_SENHA + 1);

		enderecoLoRaPivot = 0x0010;
		enderecoLoRaGNSS = 0x0011;
		canalLoRa = 0x1F;
		comunicacaoGNSS = GNSS_485;

		enderecoLoRaPivotGateway = 0x0110;
		enderecoLoRaGateway = 0x0112;
		canalLoRaGateway = 0x10;

		zeraPosicaoGNSS();

		for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
			posicoesObstaculos[i].hemisferio = 'S';
			posicoesObstaculos[i].indicadorWE = 'W';
			flagObstaculoAtivado[i] = false;
			posicoesObstaculos[i].latitudeGraus = 0;
			posicoesObstaculos[i].longitudeGraus = 0;
			posicoesObstaculos[i].latitudeMinutos = 0;
			posicoesObstaculos[i].longitudeMinutos = 0;
			posicoesObstaculos[i].latitudeSegundos = 0;
			posicoesObstaculos[i].longitudeSegundos = 0;
			raioAtuacaoObstaculo[i] = 10;
		}

		posicaoHome.hemisferio = 'S';
		posicaoHome.indicadorWE = 'W';
		posicaoHome.latitudeGraus = 0;
		posicaoHome.latitudeMinutos = 0;
		posicaoHome.latitudeSegundos = 0;
		posicaoHome.longitudeGraus = 0;
		posicaoHome.longitudeMinutos = 0;
		posicaoHome.longitudeSegundos = 0;

		horarioInicioPonta.Hours = 0;
		horarioInicioPonta.Minutes = 0;
		horarioFimPonta.Hours = 0;
		horarioFimPonta.Minutes = 0;

		horimetro.horas = 0;
		horimetro.minutos = 0;

		flagWiFiDhcp = true;
		wifiIp[0] = 192;
		wifiIp[1] = 168;
		wifiIp[2] = 0;
		wifiIp[3] = 50;

		wifiDNS[0] = 8;
		wifiDNS[1] = 8;
		wifiDNS[2] = 8;
		wifiDNS[3] = 8;

		wifiGateway[0] = 192;
		wifiGateway[1] = 168;
		wifiGateway[2] = 0;
		wifiGateway[3] = 1;

		wifiMask[0] = 255;
		wifiMask[1] = 255;
		wifiMask[2] = 255;
		wifiMask[3] = 0;

		for(uint8_t i = 0; i < QUANTIDADE_AGENDA_ACIONAMENTO; i ++) {
			horarioLigarAgenda[i].Hours = 0;
			horarioLigarAgenda[i].Minutes = 0;
			horarioDesligarAgenda[i].Hours = 0;
			horarioDesligarAgenda[i].Minutes = 0;
			diaDaSemanaAgenda[i] = 0;
			acionamentoAgenda[i] = MOTOR_DESLIGADO;
		}

		writeExternalEeprom(0, 0, EEPROM_INICIALIZADA);
		writeAllEeprom();
	}
}
/*==============================================================================
WRITE LOG
==============================================================================*/
void writeLog(LogTypeDef log) {
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 3, log.hora);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 4, log.minuto);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 5, log.segundo);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 6, log.dia);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 7, log.mes);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 8, log.ano);
	writeExternalEeprom(1, posicaoMemoriaLog * 7 + 9, log.codigo);

	posicaoMemoriaLog ++;
	if(posicaoMemoriaLog >= QUANTIDADE_REGISTROS_LOG) {
		posicaoMemoriaLog = 0;
	}

	writeExternalEeprom(1, 1, make8(posicaoMemoriaLog, 1));
	writeExternalEeprom(1, 2, make8(posicaoMemoriaLog, 0));
}
/*==============================================================================
READ LOG
==============================================================================*/
void readLog(uint16_t posicao) {
	logLido.hora = readExternalEeprom(1, posicao * 7 + 3);
	logLido.minuto = readExternalEeprom(1, posicao * 7 + 4);
	logLido.segundo = readExternalEeprom(1, posicao * 7 + 5);
	logLido.dia = readExternalEeprom(1, posicao * 7 + 6);
	logLido.mes = readExternalEeprom(1, posicao * 7 + 7);
	logLido.ano = readExternalEeprom(1, posicao * 7 + 8);
	logLido.codigo = readExternalEeprom(1, posicao * 7 + 9);
}
/*==============================================================================
READ POSICAO MEMORIA LOG
==============================================================================*/
void readPosicaoMemoriaLog() {
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_1 + 1, 10, 100)) {
		posicaoMemoriaLog = make16(readExternalEeprom(1, 1), readExternalEeprom(1, 2));
	}
}
/*==============================================================================
INICIALIZA LOG
==============================================================================*/
void inicializaLog() {
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_1 + 1, 10, 100)) {
		posicaoMemoriaLog = 0;
		writeExternalEeprom(1, 0, EEPROM_INICIALIZADA);
		writeExternalEeprom(1, 1, 0x00);
		writeExternalEeprom(1, 2, 0x00);
	}
}
/*==============================================================================
VERIFICA EEPROM
==============================================================================*/
void verificaEeprom() {
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_0 + 1, 10, 100)) {
		if(readExternalEeprom(0, 0) != EEPROM_INICIALIZADA) {
			apagaEeprom();
		}
	}

	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR_1 + 1, 10, 100)) {
		if(readExternalEeprom(1, 0) != EEPROM_INICIALIZADA) {
			inicializaLog();
		}
	}
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
