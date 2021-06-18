#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include "timer.h"
#include <xc.h>
#include "PWM.h"
#include "Robot.h"
#include "ADC.h"
#include "UART_Protocol.h"
#include "UART.h"
#include "CB_TX1.h"
#include "IO.h"
#include "main.h"
#include "CB_RX1.h"

boolean isAutomatique = TRUE;

int subCounter = 0;

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

int msgDecodedFunction;
int msgDecodedPayloadLength;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex;
int calculatedCheckSum;
int receivedCheckSum;

    int static rcvState;
void UartDecodedMessage(unsigned char c)
{   
    
    switch (rcvState){

    case Waiting:
        if (c == 0xFE) {
            rcvState = Function_MSB;
            msgDecodedFunction = 0;
            msgDecodedPayloadLength = 0;
            msgDecodedPayloadIndex = 0;
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
        if (msgDecodedPayloadLength == 0) {
            rcvState = Waiting;
        }
        else {
            rcvState = Payload;
        }
        break;

    case Payload:
        msgDecodedPayload [msgDecodedPayloadIndex] = (char) c;
        msgDecodedPayloadIndex++;
        if (msgDecodedPayloadIndex == msgDecodedPayloadLength)
            {
                rcvState = CheckSum;
            }
        break;

    case CheckSum:
        receivedCheckSum = c ;
        calculatedCheckSum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
        if (calculatedCheckSum == receivedCheckSum)
        {
            UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
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



//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/

void SetRobotState(unsigned char * payload)
{ 

}

void SetRobotAutoControlState(unsigned char * payload)
{

}

void manuelle ()
{
                    int i;
                for (i = 0 ; i < CB_RX1_GetDataSize(); i++ ){
                    
                    UartDecodedMessage(CB_RX1_Get());
                }
            }


void automatique ()
{
 if (ADCIsConversionFinished() == 1) //Conversion des données en distance (cm)
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result[1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;

            volts = ((float) result[2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;

            volts = ((float) result[4]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

            volts = ((float) result[3]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;

            volts = ((float) result[0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;

            if (robotState.distanceTelemetreDroit < 20) // Si obstacle < 30 cm alors LED orange allumé
                LED_ORANGE = 1;

            else
                LED_ORANGE = 0; //Sinon LED éteinte

            if (robotState.distanceTelemetreCentre < 20) // Si obstacle < 30 cm alors LED bleue allumé
                LED_BLEUE = 1;

            else
                LED_BLEUE = 0; //Sinon LED éteinte

            if (robotState.distanceTelemetreGauche < 20) // Si obstacle < 30 cm alors LED blanche allumé
                LED_BLANCHE = 1;

            else
                LED_BLANCHE = 0; //Sinon LED éteinte

            SendMessage((unsigned char *)"salut", 5);
                int i;
                for (i = 0; i< CB_RX1_GetDataSize(); i++)
                {
                    unsigned char c = CB_RX1_Get();
                    SendMessage(&c, 1);
                }
            if (subCounter % 10 == 0){
                unsigned char payload [3];
                payload [0]= (char) (robotState.distanceTelemetreGauche);
                payload [1]= (char) (robotState.distanceTelemetreCentre);
                payload [2]= (char) (robotState.distanceTelemetreDroit);
                int size = sizeof (payload);
                UartEncodeAndSendMessage(0x0030, size, payload);
            }
            subCounter++;
            __delay32(40000);
            if (CB_RX1_IsDataAvailable()){
                int i;
                for (i = 0 ; i < CB_RX1_GetDataSize(); i++ ){
                    
                    UartDecodedMessage(CB_RX1_Get());
                }
            }
        }
}



void UartProcessDecodedMessage(int function, int payloadLength, unsigned char * payload) {
    //Fonction prenant en entrée un octet et servant à reconstituer les trames

    switch (function) {
        case attente:
            function = payloadLength;
            break;

        case SET_ROBOT_STATE:


            break;

        case SET_ROBOT_MANUAL_CONTROL:
            if (payload == 0) {
                void automatique();
            } else {
                void manuelle();

            }

            function = attente;
            break;

            //            case led:
            //                if (payload[0] == 0x49 && payload[1] == 0x31)
            //                    LED_ORANGE = 1;
            //                if (payload[0] == 0x4F && payload[1] == 0x31)
            //                    LED_ORANGE = 0;
            //                if (payload[0] == 0x49 && payload[1] == 0x32)
            //                    LED_BLANCHE = 1;
            //                if (payload[0] == 0x4F && payload[1] == 0x32)
            //                    LED_BLANCHE = 0;
            //                if (payload[0] == 0x49 && payload[1] == 0x33)
            //                    LED_BLEUE = 1;
            //                if (payload[0] == 0x4F && payload[1] == 0x33)
            //                    LED_BLEUE = 0;
            //                function = attente;
            //                break ;

        default:
            function = attente;
            break;
    }
}