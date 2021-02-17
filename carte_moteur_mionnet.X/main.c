#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"

int main (void){
InitOscillator();
InitIO();
InitTimer23();
InitTimer1();
InitPWM();
InitADC1();
//PWMSetSpeed(-25, MOTEUR_DROIT);
//PWMSetSpeed(-25, MOTEUR_GAUCHE);
PWMSetSpeedConsigne(20,MOTEUR_GAUCHE);

//LED_BLANCHE = 1;
//LED_BLEUE = 1;
//LED_ORANGE = 1;

while(1){
    unsigned int * result = ADCGetResult();
    if (ADCIsConversionFinished() == 1)
    {
        ADCClearConversionFinishedFlag();
        float volts = ((float) result[2]) * 3.3 / 4096 * 3.2 ;
        robotState.DistanceTelemetreDroit = 34 / volts - 5 ;
        volts = ((float) result[1]) * 3.3 / 4096 * 3.2 ;
        robotState.DistanceTelemetreCentre = 34 / volts - 5 ;
        volts = ((float) result[0] ) * 3.3 / 4096 * 3.2 ;
        robotState.DistanceTelemetreGauche = 34 / volts - 5 ;
    }

    if (result[0] > 349)
    {
        LED_ORANGE = 1;
    }
    else 
    {
        LED_ORANGE = 0;
    }
    if (result[1] > 349)
    {
        LED_BLEUE = 1;
    }
    else 
    {
        LED_BLEUE = 0;
    }
    if (result[2] > 349)
    {
        LED_BLANCHE = 1;
    }
    else 
    {
        LED_BLANCHE = 0;
    }
} // fin main
}

