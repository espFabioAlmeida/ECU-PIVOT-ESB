///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    Utility LCD
//AUTOR:      Fábio Almeida
//CIRADO:     05/02/2021
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
CONSTANTES DO ARQUIVO
==============================================================================*/
#define LCD_SEG_LIN 0x40
#define LCD_NUM_COL 16
/*=============================================================================
ENVIA NIBBLE PARA O LCD
==============================================================================*/
void enviaNibbleLcd(uint8_t dado) {
	setPin(LCD_D4_GPIO_Port, LCD_D4_Pin, bitRead(dado, 0));
	setPin(LCD_D5_GPIO_Port, LCD_D5_Pin, bitRead(dado, 1));
	setPin(LCD_D6_GPIO_Port, LCD_D6_Pin, bitRead(dado, 2));
	setPin(LCD_D7_GPIO_Port, LCD_D7_Pin, bitRead(dado, 3));

    on(LCD_ENABLE_GPIO_Port, LCD_ENABLE_Pin);
    delayMicro(1);
    off(LCD_ENABLE_GPIO_Port, LCD_ENABLE_Pin);
}
/*=============================================================================
ENVIA BYTE PARA O LCD
==============================================================================*/
void enviaByteLcd(uint8_t endereco, uint8_t dado) {
    //Esta rotina irá enviar um dado ou um comando para o LCD conforme abaixo:
    // ENDEREÇO = 0 -> a variável DADO será uma instrução
    // ENDEREÇO = 1 -> a variável DADO será um caractere

	setPin(LCD_RS_GPIO_Port, LCD_RS_Pin, endereco);
    delayMicro(100);
    off(LCD_ENABLE_GPIO_Port, LCD_ENABLE_Pin);
    enviaNibbleLcd(dado >> 4);
    enviaNibbleLcd(dado & 0x0F);

    delayMicro(40);
}
/*=============================================================================
ESCREVE DADO NO LCD
==============================================================================*/
void lcdEscreveChar(char c) {
	enviaByteLcd(1, c);
}
/*=============================================================================
ESCREVE DADO NO LCD
==============================================================================*/
void lcdEscreveString(char c[], uint8_t size) {
	for(uint8_t i = 0; i < size; i ++) {
		if(c[i] && c[i] < 0xFF) {
			lcdEscreveChar(c[i]);
		}
		else {
			lcdEscreveChar(' ');
		}
	}
}
/*=============================================================================
ESCREVE DADO NO LCD
==============================================================================*/
void lcdEscreveString2Linhas(char c[]) {
	lcdPosLc(1, 1);
	for(uint8_t i = 0; i < 16; i ++) {
		if(c[i] && c[i] < 0xFF) {
			lcdEscreveChar(c[i]);
		}
		else {
			lcdEscreveChar(' ');
		}
	}
	lcdPosLc(2, 1);
	for(uint8_t i = 16; i < 32; i ++) {
		if(c[i] && c[i] < 0xFF) {
			lcdEscreveChar(c[i]);
		}
		else {
			lcdEscreveChar(' ');
		}
	}
}
/*=============================================================================
ESCREVE DADO NO LCD
==============================================================================*/
void lcdEscreve(char c[]) {
	for(uint8_t i = 0; i < strlen(c); i ++) {
		lcdEscreveChar(c[i]);
	}
}
/*=============================================================================
HABILITA/DESABILITA O CURSOR NO LCD
==============================================================================*/
void lcdCursor(uint8_t cmd) {
    cmd ? enviaByteLcd(0, 0x0D) : enviaByteLcd(0, 0x0C);
    HAL_Delay(2);
}
/*=============================================================================
LIMPA O LCD (APAGA TUDO)
==============================================================================*/
void limpaLcd() {
    enviaByteLcd(0, 0x01);
    HAL_Delay(10);
}
/*=============================================================================
INICIALIZA O LCD
==============================================================================*/
void inicializaLcd() {
    off(LCD_D4_GPIO_Port, LCD_D4_Pin);
    off(LCD_D5_GPIO_Port, LCD_D5_Pin);
    off(LCD_D6_GPIO_Port, LCD_D6_Pin);
    off(LCD_D7_GPIO_Port, LCD_D7_Pin);
    off(LCD_RS_GPIO_Port, LCD_RS_Pin);
    off(LCD_ENABLE_GPIO_Port, LCD_ENABLE_Pin);
    HAL_Delay(15);

    enviaNibbleLcd(0x03); //3x Inicia LCD
    HAL_Delay(5);
    enviaNibbleLcd(0x03);
    HAL_Delay(5);
    enviaNibbleLcd(0x03);
    HAL_Delay(5);
    enviaNibbleLcd(0x02); //Cursor Home
    HAL_Delay(1);

    enviaByteLcd(0, 0x28); // FUNCTION SET - Configura o LCD para 4 bits,
    // 2 linhas, fonte 5X7.
    enviaByteLcd(0, 0x0C); // DISPLAY CONTROL - Display ligado, sem cursor
    limpaLcd();            // Limpa o LCD
    enviaByteLcd(0, 0x06); // ENTRY MODE SET - Desloca o cursor para a direita
}
/*=============================================================================
POSICIONA O CURSOR NO LCD (LINHA x COLUNA)
==============================================================================*/
void lcdPosLc(uint8_t y, uint8_t x) {
    //Posiciona o curso na posição y,x (Linha, Coluna)
	uint8_t endereco;

    if(y != 1) {
        endereco = LCD_SEG_LIN;
    }
    else {
        endereco = 0;
    }

    endereco += x - 1;
    enviaByteLcd(0, 0x80 | endereco);
}
/*===========================================================================
ESCREVE VARIÁVEL NO DISPLAY
===========================================================================*/
void escreveDado(uint32_t valor, uint8_t l, uint8_t c, uint8_t qtdDig) {
	uint32_t stringDado[10];

    stringDado[9] = valor / 1000000000;
    stringDado[8] = valor / 100000000;
    stringDado[7] = valor / 10000000;
    stringDado[6] = valor / 1000000;
    stringDado[5] = valor / 100000;
    stringDado[4] = valor / 10000;
    stringDado[3] = valor / 1000;
    stringDado[2] = valor / 100;
    stringDado[1] = valor / 10;
    stringDado[0] = valor % 10;

    for(uint8_t i = 1; i < 10; i++) {
        if(stringDado[i] >= 10) {
            stringDado[i] %= 10;
        }
    }

    if(l > 0 && c > 0) { // L ou C = 0 não posiciona
        lcdPosLc(l, c);
    }

    switch (qtdDig) {
        case 10:
            enviaByteLcd(1, serializer(stringDado[9]));
        case 9:
            enviaByteLcd(1, serializer(stringDado[8]));
        case 8:
            enviaByteLcd(1, serializer(stringDado[7]));
        case 7:
            enviaByteLcd(1, serializer(stringDado[6]));
        case 6:
            enviaByteLcd(1, serializer(stringDado[5]));
        case 5:
            enviaByteLcd(1, serializer(stringDado[4]));
        case 4:
            enviaByteLcd(1, serializer(stringDado[3]));
        case 3:
            enviaByteLcd(1, serializer(stringDado[2]));
        case 2:
            enviaByteLcd(1, serializer(stringDado[1]));
        case 1:
            enviaByteLcd(1, serializer(stringDado[0]));
    }
}
/*===========================================================================
ESCREVE VARIÁVEL NO DISPLAY
===========================================================================*/
void escreveDadoHexadecimal(uint32_t valor, uint8_t l, uint8_t c, uint8_t qtdBytes) {
	uint8_t hihi = make8(valor, 3);
	uint8_t hilo = make8(valor, 2);
	uint8_t lohi = make8(valor, 1);
	uint8_t lolo = make8(valor, 0);
	uint8_t nibble = 0;

	if(l > 0 && c > 0) { // L ou C = 0 não posiciona
		lcdPosLc(l, c);
	}

	switch (qtdBytes) {
		case 4:
			nibble = hihi & 0xF0;
			nibble >>= 4;
			enviaByteLcd(1, serializer(nibble));
			enviaByteLcd(1, serializer(hihi & 0x0F));

		case 3:
			nibble = hilo & 0xF0;
			nibble >>= 4;
			enviaByteLcd(1, serializer(nibble));
			enviaByteLcd(1, serializer(hilo & 0x0F));

		case 2:
			nibble = lohi & 0xF0;
			nibble >>= 4;
			enviaByteLcd(1, serializer(nibble));
			enviaByteLcd(1, serializer(lohi & 0x0F));

		case 1:
			nibble = lolo & 0xF0;
			nibble >>= 4;
			enviaByteLcd(1, serializer(nibble));
			enviaByteLcd(1, serializer(lolo & 0x0F));
	}
}
/*===========================================================================
ESCREVE SÍMBOLO NO DISPLAY
===========================================================================*/
void escreveSimbolo(uint8_t simbol, uint8_t l, uint8_t c) {

    if(l > 0 && c > 0) { // L ou C = 0 não posiciona
        lcdPosLc(l, c);
    }

    switch(simbol) {
        case LCD_DOT_TODO_ACESO:
            enviaByteLcd(1, 255);
            break;
        case LCD_SETA_DIREITA:
            enviaByteLcd(1, 126);
            break;
        case LCD_SETA_ESQUERDA:
            enviaByteLcd(1, 127);
            break;
        case LCD_GRAUS:
            enviaByteLcd(1, 223);
            break;
        case LCD_MICRO:
            enviaByteLcd(1, 228);
            break;
        case LCD_RAIZ_QUADRADA:
            enviaByteLcd(1, 232);
            break;
        case LCD_OHM:
            enviaByteLcd(1, 244);
            break;
        case LCD_SIMBOLO_PI:
            enviaByteLcd(1, 247);
            break;
        case LCD_DIVISAO:
            enviaByteLcd(1, 253);
            break;
    }
}
/*===========================================================================
LIMPA LINHA
===========================================================================*/
void limpaLinha(uint8_t linha) {
    lcdPosLc(linha, 1);
    lcdEscreve("                ");
    lcdPosLc(linha, 1);
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
