/*
 * MEGA32A_UART_LIBRARY.cpp
 *
 * Created: 4/30/2015 11:44:27 AM
 *  Author: Jimmy
 */ 

#ifndef USART_BAUD
 #error "USART Baud rate must be defined"
#endif

#define BAUD USART_BAUD

#include <avr/io.h>

#if defined(__AVR_ATmega328P__)
  #define UCSRA UCSR0A
  #define MPCM MPCM0
  #define U2X U2X0
  #define UPE UPE0
  #define DOR DOR0
  #define FE FE0
  #define UDRE UDRE0
  #define TXC TXC0
  #define RXC RXC0

  #define UCSRB UCSR0B
  #define TXB8 TXB80
  #define RXB8 RXB80
  #define UCSZ2 UCSZ02
  #define TXEN TXEN0
  #define RXEN RXEN0
  #define UDRIE UDRIE0
  #define TXCIE TXCIE0
  #define RXCIE RXCIE0

  #define UDR UDR0

  #define UBRRL UBRR0L
  #define UBRRH UBRR0H
#endif

// Baudrate tolerance setting
// not sure if this will break things in the future
#define BAUD_TOL 9
#include <util/setbaud.h>

void initUart(){
	
	/* Asynchronous Double Speed Mode
	    BAUD = (fosc/(8(UBRR+1))) */
#if USE_2X == 1
	UCSRA |= (1 << U2X);
#else
	UCSRA &= ~(1 << U2X);
#endif

	/* Turn on the transmission and reception circuitry. */
	/*
	RXCIE: RX Complete Interrupt Enable
	TXCIE: TX Complete Interrupt Enable
	UDRIE: USART Data Register Empty Interrupt Enable
	RXEN: Receiver Enable
	TXEN: Transmitter Enable
	UCSZ2: Character Size
	RXB8: Receive Data Bit 8
	TXB8: Transmit Data Bit 8
	*/
	UCSRB |= (1 << RXEN) | (1 << TXEN);

	/* BAUD prescale 
	 * set by <util/setbaud.h> */

	UBRRL = UBRRL_VALUE;
	UBRRH = UBRRH_VALUE;
}

void enableRXInterrupts(){
	UCSRB |= (1 << RXCIE);
}

void enableTXInterrupts(){
	UCSRB |= _BV(UDRIE);
}

void disableTXInterrupts(){
	UCSRB &= ~_BV(UDRIE);
}

void setBaudPrescaler(int prescaler){
	/* BAUD prescale */
	//For use in calculating the baud rate, this is not the actual BAUD rate.
	UBRRL = prescaler;	
}

char readFromUartRXReg(){
	char retChar = UDR;
	return retChar;
}

void USART_SendByte(uint8_t u8Data) {

	// Wait until last byte has been transmitted
	while((UCSRA &(1<<UDRE)) == 0);

	// Transmit data
	UDR = u8Data;
}

// not being used but here for completeness
// Wait until a byte has been received and return received data
uint8_t USART_ReceiveByte() {
	while((UCSRA &(1<<RXC)) == 0);
	return UDR;
}

//Turns a hexidecimal character into the actual number it represents
uint8_t hexLetterToNumber(uint8_t c) {
	if(c >= 0x30 && c <= 0x39){
		c-= 0x30;
	}
	else{
		c = (c - 0x41) + 10;
	}
	return c;
}

uint8_t USART_ReceiveHexi() {
	uint8_t upper = hexLetterToNumber(USART_ReceiveByte());
	uint8_t lower = hexLetterToNumber(USART_ReceiveByte());
	upper = upper << 4;
	return upper | lower;
}

void USART_SendHexByte(uint8_t byte) {
	char upper = (byte & 0b11110000 );
	char lower = (byte & 0b00001111 );
	
	upper = ((upper >> 4) & 0b00001111);
	
	if(upper <10) {
		USART_SendByte(upper+'0');
	}
	else {
		USART_SendByte((upper-10) + 'A');
	}
	if(lower <10) {
		USART_SendByte(lower+'0');
	}
	else {
		USART_SendByte((lower-10) + 'A');
	}
}

void USART_sendHexArray(uint8_t *bytes, int length) {
	int i = 0;
	for(i=0;i<length;i++) {
		USART_SendHexByte(bytes[i]);
	}
}

void USART_sendString(const char *str) {
	int i=0;
	while(str[i] != 0x00) {
		USART_SendByte(str[i]);
		i++;
	}
}
