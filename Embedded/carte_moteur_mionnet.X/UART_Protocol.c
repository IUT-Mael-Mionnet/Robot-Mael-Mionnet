#include <xc.h>
#include ?UART_Protocol.h?
#include "UART.h"

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
//Fonction prenant entrée la trame et sa longueur pour calculer le checksum
    char checksum;
    checksum = 0xFE;
    checksum ^= (char) (msgFunction >>8);
    checksum ^= (char) (msgFunction >>0);
    checksum ^= (char) (msgPayloadLength >>8);
    checksum ^= (char) (msgPayloadLength >>0);
    
    for ( int i = 0 ; i < msgPayloadLength ; i++)
    {
        checksum ^= msgPayload;
    }
    
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    //Fonction d?encodage et d?envoi d?un message
    char cheksum = UartCalculateChecksum (int msgFunction,int msgPayloadLength , unsigned char* msgPayload);
    unsigned char msg = new char [ 6 + msgPayloadLength ];
    int pos = 0;
    msg[pos++] = 0xFE;
    msg[pos++] = (char)(msgFunction >> 8);
    msg[pos++] = (char)(msgFunction >> 0);
    msg[pos++] = (char)(msgPayloadLength >> 8);
    msg[pos++] = (char)(msgPayloadLength >> 0);
    
    for(int i = 0; i < msgPayloadLength; i++)
    {
        msg[pos++] = msgPayload[i];
    }
    for (i = 0; i < 6 + msgPayloadLength ; i++)
    {
        while (U1STAbits.UTXBF); // wait while Tx buffer full
        U1TXREG = *(message) ++; //Transmit one character
    }
    
}

//int msgDecodedFunction = 0;
//int msgDecodedPayloadLength = 0;
//unsigned char msgDecodedPayload[128];
//int msgDecodedPayloadIndex = 0;
//
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