///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasCoordenadas
//AUTOR:      Fábio Almeida
//CIRADO:     02/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define DISTANCIA_HOME	30
#define MILHA_NAUTICA 18520 //1852 x10
#define TEMPO_OBSTACULO_ENCONTRADO 5
#define TEMPO_HOME_ENCONTRADO 5
/*=============================================================================
CONVERTE POSICAO EM METROS
==============================================================================*/
uint32_t posicaoParaMetros(uint16_t graus, uint16_t minutos, uint32_t segundos) {
	uint32_t metros = graus;
	uint64_t calculo = segundos;

	metros *= 60; //1°=60NM
	metros += minutos; //1min = 1NM
	metros *= MILHA_NAUTICA;

	calculo *= MILHA_NAUTICA; //regra de 3 para transformar em metros (60s = 1NM)
	calculo /= 60000; //A escala dos segundos é 60.000 = 60s
	segundos = calculo;

	metros += segundos;
	return metros;
}
/*=============================================================================
CALCULA DISTANCIA ENTRE DOS PONTOS
==============================================================================*/
uint32_t calculaDistancia(CoordenadasTypeDef coordenada) {
	uint32_t distanciaLongitudinal = 0, distanciaLatitudinal = 0, distancia = 0;
	uint32_t valor1 = 0, valor2 = 0;
	CoordenadasTypeDef coordenadaComum;

	//Cria-se um ponto virtual, onde busca a latitude de um dos pontos e a longitude do outro
	coordenadaComum.latitudeGraus = posicaoAtualGPS.latitudeGraus;
	coordenadaComum.latitudeMinutos = posicaoAtualGPS.latitudeMinutos;
	coordenadaComum.latitudeSegundos = posicaoAtualGPS.latitudeSegundos;
	coordenadaComum.hemisferio = posicaoAtualGPS.hemisferio;

	coordenadaComum.longitudeGraus = coordenada.longitudeGraus;
	coordenadaComum.longitudeMinutos = coordenada.longitudeMinutos;
	coordenadaComum.longitudeSegundos = coordenada.longitudeSegundos;
	coordenadaComum.indicadorWE = coordenada.indicadorWE;

	//Calcula a distancia entre o ponto virtual e um dos pontos (possuem a mesma longitude)
	valor1 = posicaoParaMetros(coordenadaComum.latitudeGraus, coordenadaComum.latitudeMinutos, coordenadaComum.latitudeSegundos);
	valor2 = posicaoParaMetros(coordenada.latitudeGraus, coordenada.latitudeMinutos, coordenada.latitudeSegundos);

	if(coordenadaComum.hemisferio == coordenada.hemisferio) {
		if(valor1 >= valor2) {
			distanciaLatitudinal = valor1 - valor2;
		}
		else {
			distanciaLatitudinal = valor2 - valor1;
		}
	}
	else {
		distanciaLatitudinal = valor1 + valor2;
	}

	//Calcula a distancia entre o ponto virtual e o outro ponto (possuem a mesma latitude)
	valor1 = posicaoParaMetros(coordenadaComum.longitudeGraus, coordenadaComum.longitudeMinutos, coordenadaComum.longitudeSegundos);
	valor2 = posicaoParaMetros(posicaoAtualGPS.longitudeGraus, posicaoAtualGPS.longitudeMinutos, posicaoAtualGPS.longitudeSegundos);

	if(coordenadaComum.indicadorWE == posicaoAtualGPS.indicadorWE) {
		if(valor1 >= valor2) {
			distanciaLongitudinal = valor1 - valor2;
		}
		else {
			distanciaLongitudinal = valor2 - valor1;
		}
	}
	else {
		distanciaLongitudinal = valor1 + valor2;
	}

	//Utiliza pitágoras para saber a distância entre os dois pontos
	distanciaLatitudinal *= distanciaLatitudinal;
	distanciaLongitudinal *= distanciaLongitudinal;
	distancia = distanciaLatitudinal + distanciaLongitudinal;
	distancia = sqrt(distancia);
	return distancia;
}
/*=============================================================================
VERIFICA SE ESTÁ EM ÁREA DE OBSTÁCULO ATIVO
==============================================================================*/
void verificaObstaculos() {
	static uint8_t contadorObstaculos = 0;
	uint8_t encontrado = false;
	/*
	//Debug
	posicaoAtualGPS.latitudeGraus = 26;
	posicaoAtualGPS.latitudeMinutos = 52;
	posicaoAtualGPS.latitudeSegundos = 49429;
	posicaoAtualGPS.hemisferio = 'S';
	posicaoAtualGPS.longitudeGraus = 49;
	posicaoAtualGPS.longitudeMinutos = 4;
	posicaoAtualGPS.longitudeSegundos = 30907;
	posicaoAtualGPS.indicadorWE = 'W';

	posicoesObstaculos[0].latitudeGraus = 26;
	posicoesObstaculos[0].latitudeMinutos = 52;
	posicoesObstaculos[0].latitudeSegundos = 49530;
	posicoesObstaculos[0].hemisferio = 'S';
	posicoesObstaculos[0].longitudeGraus = 49;
	posicoesObstaculos[0].longitudeMinutos = 4;
	posicoesObstaculos[0].longitudeSegundos = 31145;
	posicoesObstaculos[0].indicadorWE = 'W';
	*/

	for(uint8_t i = 0; i < QUANTIDADE_OBSTACULOS; i ++) {
		if(flagObstaculoAtivado[i]) {
			if(raioAtuacaoObstaculo[i] >= calculaDistancia(posicoesObstaculos[i])) {
				encontrado = true;
				i = QUANTIDADE_OBSTACULOS;
			}
		}
	}

	if(flagObstaculoEncontrado == encontrado) {
		contadorObstaculos = 0;
		return;
	}

	contadorObstaculos ++;
	if(contadorObstaculos >= TEMPO_OBSTACULO_ENCONTRADO) {
		flagObstaculoEncontrado = encontrado;
	}
}
/*=============================================================================
VERIFICA SE ESTÁ EM ÁREA DE HOME
==============================================================================*/
void verificaPosicaoHome() {
	static uint8_t contadorHome = 0;
	uint8_t encontrado = false;

	if(!posicaoHome.latitudeGraus && !posicaoHome.latitudeMinutos && !posicaoHome.latitudeSegundos &&
			!posicaoHome.longitudeGraus && !posicaoHome.longitudeMinutos && !posicaoHome.longitudeSegundos) {
		flagHomeEncontrado = false;
		return;
	}

	if(DISTANCIA_HOME >= calculaDistancia(posicaoHome)){
		encontrado = true;
	}

	if(flagHomeEncontrado == encontrado) {
		contadorHome = 0;
		return;
	}

	contadorHome ++;
	if(contadorHome >= TEMPO_HOME_ENCONTRADO) {
		flagHomeEncontrado = encontrado;
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
