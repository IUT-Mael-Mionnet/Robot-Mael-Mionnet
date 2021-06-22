#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "UART_Protocol.h"

int subCounter = 0;

void automatique() {
    LED_ORANGE=0;
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

        SendMessage((unsigned char *) "salut", 5);
        int i;
        for (i = 0; i < CB_RX1_GetDataSize(); i++) {
            unsigned char c = CB_RX1_Get();
            SendMessage(&c, 1);
        }
        if (subCounter % 10 == 0) {
            unsigned char payload [3];
            payload [0] = (char) (robotState.distanceTelemetreGauche);
            payload [1] = (char) (robotState.distanceTelemetreCentre);
            payload [2] = (char) (robotState.distanceTelemetreDroit);
            int size = sizeof (payload);
            UartEncodeAndSendMessage(0x0030, size, payload);
        }
        subCounter++;
        __delay32(40000);
        if (CB_RX1_IsDataAvailable()) {
            int i;
            for (i = 0; i < CB_RX1_GetDataSize(); i++) {

                UartDecodedMessage(CB_RX1_Get());
            }
        }
    }
}