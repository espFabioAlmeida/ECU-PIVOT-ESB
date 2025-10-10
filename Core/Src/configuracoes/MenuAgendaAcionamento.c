///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    MenuAgendaAcionamento
//AUTOR:      Fábio Almeida
//CIRADO:     05/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*=============================================================================
AJUSTE HORARIO
==============================================================================*/
void submenuAjustaHorarioAgendaAcionamento(uint8_t horarioFim, uint8_t offset) {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t dezeHora, unidHora, dezeMinuto, unidMinuto;

	if(horarioFim) {
		dezeHora = horarioDesligarAgenda[offset].Hours / 10;
		unidHora = horarioDesligarAgenda[offset].Hours % 10;
		dezeMinuto = horarioDesligarAgenda[offset].Minutes / 10;
		unidMinuto = horarioDesligarAgenda[offset].Minutes % 10;
	}
	else {
		dezeHora = horarioLigarAgenda[offset].Hours / 10;
		unidHora = horarioLigarAgenda[offset].Hours % 10;
		dezeMinuto = horarioLigarAgenda[offset].Minutes / 10;
		unidMinuto = horarioLigarAgenda[offset].Minutes % 10;
	}

	telAjusteHorarioAgenda(horarioFim);
	escreveDado(dezeHora, 2, 1, 1);
	escreveDado(unidHora, 0, 0, 1);
	escreveDado(dezeMinuto, 2, 4, 1);
	escreveDado(unidMinuto, 0, 0, 1);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp()) {
			switch(display) {
				case 1:
					dezeHora = incrementaDecrementa(dezeHora, 0, 2, true);
					if(dezeHora == 2 && unidHora > 3) {
						unidHora = 3;
					}
					break;

				case 2:
					unidHora = incrementaDecrementa(unidHora, 0, 9, true);
					if(dezeHora == 2 && unidHora > 3) {
						unidHora = 0;
					}
					break;

				case 4: dezeMinuto = incrementaDecrementa(dezeMinuto, 0, 5, true); break;
				case 5: unidMinuto = incrementaDecrementa(unidMinuto, 0, 9, true); break;
			}

			escreveDado(dezeHora, 2, 1, 1);
			escreveDado(unidHora, 0, 0, 1);
			escreveDado(dezeMinuto, 2, 4, 1);
			escreveDado(unidMinuto, 0, 0, 1);
			lcdPosLc(2, display);

			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			switch(display) {
				case 1:
					dezeHora = incrementaDecrementa(dezeHora, 0, 2, false);
					if(dezeHora == 2 && unidHora > 3) {
						unidHora = 3;
					}
					break;

				case 2:
					unidHora = incrementaDecrementa(unidHora, 0, 9, false);
					if(dezeHora == 2 && unidHora > 3) {
						unidHora = 3;
					}
					break;

				case 4: dezeMinuto = incrementaDecrementa(dezeMinuto, 0, 5, false); break;
				case 5: unidMinuto = incrementaDecrementa(unidMinuto, 0, 9, false); break;
			}

			escreveDado(dezeHora, 2, 1, 1);
			escreveDado(unidHora, 0, 0, 1);
			escreveDado(dezeMinuto, 2, 4, 1);
			escreveDado(unidMinuto, 0, 0, 1);
			lcdPosLc(2, display);

			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 5) {
				display ++;
				if(display == 3) {
					display = 4;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(display > 1) {
				display --;
				if(display == 3) {
					display = 2;
				}
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());

			if(horarioFim) {
				horarioDesligarAgenda[offset].Hours = dezeHora * 10 + unidHora;
				horarioDesligarAgenda[offset].Minutes = dezeMinuto * 10 + unidMinuto;
			}
			else {
				horarioLigarAgenda[offset].Hours = dezeHora * 10 + unidHora;
				horarioLigarAgenda[offset].Minutes = dezeMinuto * 10 + unidMinuto;
			}
			flagSubmenu = false;
		}
	}

	lcdCursor(false);
}
/*=============================================================================
DIA DA SEMANA
==============================================================================*/
void submenuDiaSemanaAgendaAcionamento(uint8_t offset) {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t diaSemana = diaDaSemanaAgenda[offset]; //bit0=seg --> bit6=dom

	diaSemana = bitReset(diaSemana, 7); //garante 0 no bit7
	telaSelecioneDiaSemana();
	telaSelecaoDiaSemanaAgenda(diaSemana);
	lcdCursor(true);
	lcdPosLc(2, display);

	while(flagSubmenu) {
		if(botaoUp() || botaoDown()) {
			uint8_t ponteiro = display - 1;
			ponteiro /= 2;
			if(bitRead(diaSemana, ponteiro)) {
				diaSemana = bitReset(diaSemana, ponteiro);
			}
			else {
				diaSemana = bitSet(diaSemana, ponteiro);
			}

			telaSelecaoDiaSemanaAgenda(diaSemana);
			lcdCursor(true);
			lcdPosLc(2, display);

			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoRight()) {
			if(display < 13) {
				display += 2;
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoLeft()) {
			if(display > 1) {
				display -= 2;
				lcdPosLc(2, display);
			}
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			diaDaSemanaAgenda[offset] = diaSemana;
			flagSubmenu = false;
		}
	}

	lcdCursor(false);
}
/*=============================================================================
DIA DA SEMANA
==============================================================================*/
void submenuOperacaoAgendaAcionamento(uint8_t offset) {
	uint8_t flagSubmenu = true, display = 1;
	uint8_t operacao = acionamentoAgenda[offset]; //bit0=seg --> bit6=dom

	telaOperacaoAgenda(operacao);

	while(flagSubmenu) {
		if(botaoUp()) {
			operacao ++;
			if(operacao > 2) {
				operacao = MOTOR_DESLIGADO;
			}
			telaOperacaoAgenda(operacao);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			operacao --;
			if(operacao > 2) {
				operacao = MOTOR_ESQUERDA;
			}
			telaOperacaoAgenda(operacao);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			acionamentoAgenda[offset] = operacao;
			flagSubmenu = false;
		}
	}

	lcdCursor(false);
}
/*=============================================================================
SELECAO AGENDA
==============================================================================*/
void submenuAgendaAcionamento() {
	uint8_t flagSubmenu = true, horario = 1;
	telaSelecaoAgendaAcionamento();

	while(flagSubmenu) {
		if(botaoUp()) {
			horario ++;
			if(horario > QUANTIDADE_AGENDA_ACIONAMENTO) {
				horario = 1;
			}
			escreveDado(horario, 0, 0, 1);
			lcdPosLc(2, 1);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoDown()) {
			horario --;
			if(!horario) {
				horario = QUANTIDADE_AGENDA_ACIONAMENTO;
			}
			escreveDado(horario, 0, 0, 1);
			lcdPosLc(2, 1);
			HAL_Delay(TEMPO_DUPLO_CLIQUE);
		}

		if(botaoOk()) {
			while(botaoOk());
			submenuAjustaHorarioAgendaAcionamento(false, horario - 1);
			submenuAjustaHorarioAgendaAcionamento(true, horario - 1);
			submenuDiaSemanaAgendaAcionamento(horario - 1);
			submenuOperacaoAgendaAcionamento(horario - 1);
			writeEepromAgendaAcionamento(horario - 1);
			flagSubmenu = false;
		}
	}

	lcdCursor(false);
}
/*=============================================================================
FIM DO ARQUIVO
==============================================================================*/
