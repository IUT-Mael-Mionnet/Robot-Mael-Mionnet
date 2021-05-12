#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

void UartEncodeAndSendMessage();
unsigned char UartCalculateChecksum();
void UartDecodeMessage();
void UartProcessDecodedMessage();
#endif	/* UART_PROTOCOL_H */

