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

#define text 0x0080
#define led 0x0020
#define dist 0x0030
#define vit 0x0040
#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052

void UartEncodeAndSendMessage();
unsigned char UartCalculateChecksum();
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength, unsigned char * payload);
void SetRobotState();
void SetRobotAutoControlState();

void manuelle (int function, int payloadLength, unsigned char * payload);
void automatique (void);

void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode();
int main (void);

typedef enum {
    FALSE = 0,
    TRUE = 1,
} boolean;

#endif	/* UART_PROTOCOL_H */

