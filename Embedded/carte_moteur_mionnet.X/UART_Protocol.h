#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#define	Waiting 0
#define	Function_MSB 1
#define	Function_LSB 2
#define	PayloadLength_MSB 3
#define	PayloadLength_LSB 4
#define	Payload 5
#define	CheckSum 6

#define	attente 0
#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052


void UartEncodeAndSendMessage();
unsigned char UartCalculateChecksum();
void UartDecodeMessage();
void UartProcessDecodedMessage();
void SetRobotState();
void SetRobotAutoControlState();


#endif	/* UART_PROTOCOL_H */

