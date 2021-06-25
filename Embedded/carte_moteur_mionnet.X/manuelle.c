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
#include "automatique.h"

void manuelle(int function, int payloadLength, unsigned char * payload) {
    PWMSetSpeedConsigne(0, MOTEUR_DROIT);
    PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
    commande_manuelle(function, payloadLength, payload);


}

void commande_manuelle(int function, int payloadLength, unsigned char * payload) {


    switch (function) {
        case attente:
            function = payloadLength;
            break;



            break;

        case led:
            if (payload[0] == 0x49 && payload[1] == 0x31)
                LED_ORANGE = 1;
            if (payload[0] == 0x4F && payload[1] == 0x31)
                LED_ORANGE = 0;
            if (payload[0] == 0x49 && payload[1] == 0x32)
                LED_BLANCHE = 1;
            if (payload[0] == 0x4F && payload[1] == 0x32)
                LED_BLANCHE = 0;
            if (payload[0] == 0x49 && payload[1] == 0x33)
                LED_BLEUE = 1;
            if (payload[0] == 0x4F && payload[1] == 0x33)
                LED_BLEUE = 0;
            function = attente;
            break;

        default:
            function = attente;
            break;

    }
}