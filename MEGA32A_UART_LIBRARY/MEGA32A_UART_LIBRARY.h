/*
 * MEGA32A_UART_LIBRARY.h
 *
 * Created: 4/30/2015 11:52:53 AM
 *  Author: Jimmy
 */ 


#ifndef MEGA32A_UART_LIBRARY_H_
#define MEGA32A_UART_LIBRARY_H_



void initUart();

void enableRXInterrupts();

void setBaudPrescaler(int prescaler);

char readFromUartRXReg();

void USART_SendByte(uint8_t u8Data);

uint8_t USART_ReceiveByte();

//Turns a hexidecimal character into the actual number it represents
uint8_t hexLetterToNumber(uint8_t c);

//This is a blocking call.
//Waits for two bytes in hexadecimal fomrat to come in through the UART and
//Returns the byte represented by the hexadecimal received.
//Example:
//"EA" received through UART,
//The function returns 0x11101010
uint8_t USART_ReceiveHexi();

void SendHexiByte(uint8_t byte);

void SendHexiArary(uint8_t *bytes, int length);

void serialSendString(const char *str);

#endif /* MEGA32A_UART_LIBRARY_H_ */