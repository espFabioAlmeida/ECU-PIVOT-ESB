///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    SubrotinasAgendaAcionamento
//AUTOR:      Fábio Almeida
//CIRADO:     02/01/2025
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
VERIFICA DIA DA SEMANA
==============================================================================*/
uint8_t verificaDiaSemanaAgendaAcionamento(uint8_t offset) {
	uint8_t diaSemana = diaDaSemanaAgenda[offset];
	switch(sDate.WeekDay) {
		case RTC_WEEKDAY_MONDAY: return bitRead(diaSemana, 0); break;
		case RTC_WEEKDAY_TUESDAY: return bitRead(diaSemana, 1); break;
		case RTC_WEEKDAY_WEDNESDAY: return bitRead(diaSemana, 2); break;
		case RTC_WEEKDAY_THURSDAY: return bitRead(diaSemana, 3); break;
		case RTC_WEEKDAY_FRIDAY: return bitRead(diaSemana, 4); break;
		case RTC_WEEKDAY_SATURDAY: return bitRead(diaSemana, 5); break;
		case RTC_WEEKDAY_SUNDAY: return bitRead(diaSemana, 6); break;
		default: return false; break;
	}
}
/*=============================================================================
VERIFICA AGENDA ACIONAMENTO
==============================================================================*/
void verificaAgendaAcionamento() {
	static uint8_t reentradaHorarioLigar[QUANTIDADE_AGENDA_ACIONAMENTO];
	static uint8_t reentradaHorarioDesligar[QUANTIDADE_AGENDA_ACIONAMENTO];

	for(uint8_t i = 0; i < QUANTIDADE_AGENDA_ACIONAMENTO; i ++) {
		if(acionamentoAgenda[i] != MOTOR_DESLIGADO) {
			if(horarioLigarAgenda[i].Hours != horarioDesligarAgenda[i].Hours ||
					horarioLigarAgenda[i].Minutes != horarioDesligarAgenda[i].Minutes) {

				//Verificar ligar
				if(verificaDiaSemanaAgendaAcionamento(i)) {
					if(sTime.Hours == horarioLigarAgenda[i].Hours &&
							sTime.Minutes == horarioLigarAgenda[i].Minutes) {
						if(!reentradaHorarioLigar[i]) {
							reentradaHorarioLigar[i] = true;
							agendaHorarioAcionado = i + 1;
						}
					}
					else {
						reentradaHorarioLigar[i] = false;
					}
				}
				else {
					reentradaHorarioLigar[i] = false;
				}

				//Verifica Desligar (se foi acionada por essa agenda)
				if(agendaHorarioAcionado == i + 1) {
					if(sTime.Hours == horarioDesligarAgenda[i].Hours &&
							sTime.Minutes == horarioDesligarAgenda[i].Minutes) {
						if(!reentradaHorarioDesligar[i]) {
							reentradaHorarioDesligar[i] = true;
							agendaHorarioAcionado = 0;
						}
					}
					else {
						reentradaHorarioDesligar[i] = false;
					}
				}
				else {
					reentradaHorarioDesligar[i] = false;
				}
			}
			else {
				reentradaHorarioLigar[i] = false;
				reentradaHorarioDesligar[i] = false;
			}
		}
		else {
			reentradaHorarioLigar[i] = false;
			reentradaHorarioDesligar[i] = false;
		}
	}

}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
