///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasHorarioPonta
//AUTOR:      Fábio Almeida
//CIRADO:     02/08/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
VERIFICA HORARIO DE PONTA
==============================================================================*/
void verificaHorarioPonta() {
	uint16_t horarioInicioMinutos = 0, horarioFimMinutos = 0, horarioAtualMinutos = 0;
	if(horarioInicioPonta.Hours == horarioFimPonta.Hours) {
		if(horarioInicioPonta.Minutes == horarioFimPonta.Minutes) {
			flagHorarioPonta = false;
			return;
		}
	}

	if(contadorIniciaVerificacaoHorarioPonta < TEMPO_VERIFICA_HORARIO_PONTA) {
		flagHorarioPonta = false;
		return;
	}

	horarioInicioMinutos = horarioInicioPonta.Hours;
	horarioInicioMinutos *= 60;
	horarioInicioMinutos += horarioInicioPonta.Minutes;
	horarioFimMinutos = horarioFimPonta.Hours;
	horarioFimMinutos *= 60;
	horarioFimMinutos += horarioFimPonta.Minutes;
	horarioAtualMinutos = sTime.Hours;
	horarioAtualMinutos *= 60;
	horarioAtualMinutos += sTime.Minutes;


	if(horarioInicioMinutos < horarioFimMinutos) { //Inicia e termina no mesmo dia
		if(horarioAtualMinutos >= horarioInicioMinutos && horarioAtualMinutos < horarioFimMinutos) {
			flagHorarioPonta = true;
		}
		else {
			flagHorarioPonta = false;
		}
		return;
	}

	//Inicia em um dia e termina no próximo
	if(horarioAtualMinutos >= horarioInicioMinutos || horarioAtualMinutos < horarioFimMinutos) {
		flagHorarioPonta = true;
	}
	else {
		flagHorarioPonta = false;
	}
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
