#include <xc.h>
#include "UART_Protocol.h"
#include "UART.h"
#include "CB_TX1.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char * msgPayload)
{
//Fonction prenant entrée la trame et sa longueur pour calculer le checksum
    unsigned char checksum;
    checksum = 0xFE;
    checksum ^= (char) (msgFunction >>8);
    checksum ^= (char) (msgFunction >>0);
    checksum ^= (char) (msgPayloadLength >>8);
    checksum ^= (char) (msgPayloadLength >>0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++)
    {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char checksum;
    checksum = UartCalculateChecksum (msgFunction, msgPayloadLength, msgPayload);
    unsigned char * msg;
    int pos = 0;
    msg[pos++] = 0xFE;
    msg[pos++] = (char)(msgFunction >> 8);
    msg[pos++] = (char)(msgFunction >> 0);
    msg[pos++] = (char)(msgPayloadLength >> 8);
    msg[pos++] = (char)(msgPayloadLength >> 0);
    int i = 0;
    for( i = 0; i < msgPayloadLength; i++)
    {
        msg[pos++] = msgPayload[i];
    }
    msg[pos++] = checksum;
    SendMessage(msg, pos);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

//void UartDecodeMessage(unsigned char c)
//{
////Fonction prenant en entrée un octet et servant à reconstituer les trames
////?
//}
//
//void UartProcessDecodedMessage(unsigned char function,unsigned char payloadLength, unsigned char* payload)
//{
////Fonction appelée après le décodage pour exécuter l?action
////correspondant au message reçu
////?
//}

 

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/