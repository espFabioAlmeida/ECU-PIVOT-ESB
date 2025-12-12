///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityGPIO
//AUTOR:      Fábio Almeida
//CRIADO:     04/08/2023
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
DEBOUNCE
==============================================================================*/
uint8_t debounce(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	#ifndef DEBOUNCE
		#define DEBOUNCE 120
	#endif

	if(!HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) {
		HAL_Delay(DEBOUNCE);
		if(!HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) {
			return true;
		}
	}

	return false;
}
/*==============================================================================
DEBOUNCE INVERSO
==============================================================================*/
uint8_t debounceInverso(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	#ifndef DEBOUNCE
		#define DEBOUNCE 120
	#endif

	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) {
		HAL_Delay(DEBOUNCE);
		if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) {
			return true;
		}
	}

	return false;
}
/*==============================================================================
BOTAO UP
==============================================================================*/
uint8_t botaoUp() {
	static uint8_t whileBotao = false;
	uint8_t botao = debounce(SW_UP_GPIO_Port, SW_UP_Pin);
	if(botao && !whileBotao) {
		flagBuzzer = true;
	}
	whileBotao = botao;
	return botao;
}
/*==============================================================================
BOTAO DOWN
==============================================================================*/
uint8_t botaoDown() {
	static uint8_t whileBotao = false;
	uint8_t botao = debounce(SW_DOWN_GPIO_Port, SW_DOWN_Pin);
	if(botao && !whileBotao) {
		flagBuzzer = true;
	}
	whileBotao = botao;
	return botao;
}
/*==============================================================================
BOTAO OK
==============================================================================*/
uint8_t botaoOk() {
	static uint8_t whileBotao = false;
	uint8_t botao = debounce(SW_OK_GPIO_Port, SW_OK_Pin);
	if(botao && !whileBotao) {
		flagBuzzer = true;
	}
	whileBotao = botao;
	return botao;
}
/*==============================================================================
BOTAO LEFT
==============================================================================*/
uint8_t botaoLeft() {
	static uint8_t whileBotao = false;
	uint8_t botao = debounce(SW_LEFT_GPIO_Port, SW_LEFT_Pin);
	if(botao && !whileBotao) {
		flagBuzzer = true;
	}
	whileBotao = botao;
	return botao;
}
/*==============================================================================
BOTAO LEFT
==============================================================================*/
uint8_t botaoRight() {
	static uint8_t whileBotao = false;
	uint8_t botao = debounce(SW_RIGHT_GPIO_Port, SW_RIGHT_Pin);
	if(botao && !whileBotao) {
		flagBuzzer = true;
	}
	whileBotao = botao;
	return botao;
}
/*==============================================================================
LEITURA ENTRADA
==============================================================================*/
uint8_t input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) {
		return true;
	}
	return false;
}
/*==============================================================================
ACIONA SAIDA
==============================================================================*/
void on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
}
/*==============================================================================
DESACIONA SAIDA
==============================================================================*/
void off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
}
/*==============================================================================
INVERTE SAIDA
==============================================================================*/
void toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}
/*==============================================================================
SETA SAIDA
==============================================================================*/
void setPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
