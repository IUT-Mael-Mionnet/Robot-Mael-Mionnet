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
    unsigned char msg[6 + msgPayloadLength];
    int pos = 0;
    msg[pos++] = 0xFE;
    msg[pos++] = (msgFunction >> 8);
    msg[pos++] = (msgFunction >> 0);
    msg[pos++] = (msgPayloadLength >> 8);
    msg[pos++] = (msgPayloadLength >> 0);
    int i = 0;
    for(i = 0; i < msgPayloadLength; i++)
    {
        msg[pos++] = msgPayload[i];
    }
    checksum = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    msg[pos++] = checksum;
    SendMessage(msg, pos); 
   
}

/*
void UartDecodedMessage(unsigned char c)
{
    int static msgDecodedFunction;
    int static msgDecodedPayloadLength;
    unsigned char msgDecodedPayload[msgDecodedPayloadLength];
    //int static msgDecodedPayloadIndex = 0;
    int CalculCheckSum;
    int RecuCheckSum;
    int i;

 
    int static rcvState;
    
    rcvState = Waiting;
    
        switch (rcvState){
            
        case Waiting:
            if (c == 0xFE) {
                rcvState = Function_MSB;
                
            }
            break;

        case Function_MSB:
            msgDecodedFunction = c;
            msgDecodedFunction = msgDecodedFunction << 8;
            rcvState = Function_LSB;
            break;

        case Function_LSB:
            msgDecodedFunction += c;
            rcvState = PayloadLength_MSB;
            break;

        case PayloadLength_MSB:
            msgDecodedPayloadLength = c;
            msgDecodedPayloadLength = msgDecodedPayloadLength << 8;
            rcvState = PayloadLength_LSB;
            break;

        case PayloadLength_LSB:
            msgDecodedPayloadLength += c;
            rcvState = Payload;
            break;

        case Payload:
            
            for (i=0;i<msgDecodedPayloadLength;i++){
                int decalage;
                decalage = msgDecodedPayloadLength-i;
                msgDecodedPayload[i]= c << decalage;
            }
            rcvState = CheckSum;
            break;

        case CheckSum:
            RecuCheckSum = c ;
            CalculCheckSum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            if (CalculCheckSum == RecuCheckSum)
            {
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                rcvState = Waiting;
            }
            else
            {
                rcvState = Waiting;
            }
            rcvState = Waiting;
            break;

        default:
            rcvState = Waiting;
            break;
        }
        
//Fonction appelée après le décodage pour exécuter l?action
//correspondant au message reçu
//?
}


*/

//void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload)
//{
////Fonction prenant en entrée un octet et servant à reconstituer les trames
////?
//}
//
// 
//
////*************************************************************************/
////Fonctions correspondant aux messages
////*************************************************************************/
//
//
