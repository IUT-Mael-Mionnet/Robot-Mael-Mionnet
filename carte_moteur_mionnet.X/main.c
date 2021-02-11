#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"

int main (void){
InitOscillator();
InitIO();
InitTimer23();
InitTimer1();
InitPWM();
//PWMSetSpeed(-25, MOTEUR_DROIT);
//PWMSetSpeed(-25, MOTEUR_GAUCHE);
PWMSetSpeedConsigne(20,MOTEUR_GAUCHE);

LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;

unsigned int * resultat;

while(1){
    if (ADCIsConversionFinished() == 1)
    {
        ADCClearConversionFinishedFlag();
        resultat = ADCGetResult();
    }
} // fin main
}

