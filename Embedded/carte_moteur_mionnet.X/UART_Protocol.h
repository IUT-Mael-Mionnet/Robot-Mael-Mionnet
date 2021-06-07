#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#define	Waiting 0
#define	Function_MSB 1
#define	Function_LSB 2
#define	PayloadLength_MSB 3
#define	PayloadLength_LSB 4
#define	Payload 5
#define	CheckSum 6


void UartEncodeAndSendMessage();
unsigned char UartCalculateChecksum();
void UartDecodeMessage();
void UartProcessDecodedMessage();


#endif	/* UART_PROTOCOL_H */

