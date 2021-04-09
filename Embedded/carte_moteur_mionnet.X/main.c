#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"
#include "UART.h"

int main (void){
InitOscillator();
InitIO();
InitTimer23();
InitTimer1();
InitPWM();
InitADC1();
InitTimer4();
InitUART();


while(1){
    unsigned int * result = ADCGetResult();
    if (ADCIsConversionFinished() == 1)     //Conversion des données en distance (cm)
    {
        ADCClearConversionFinishedFlag();
        float volts = ((float) result[1]) * 3.3 / 4096 * 3.2 ;
        robotState.distanceTelemetreDroit = 34 / volts - 5 ;
        
        volts = ((float) result[2]) * 3.3 / 4096 * 3.2 ;
        robotState.distanceTelemetreCentre = 34 / volts - 5 ;
        
        volts = ((float) result[4] ) * 3.3 / 4096 * 3.2 ;
        robotState.distanceTelemetreGauche = 34 / volts - 5 ;
        
        volts = ((float) result[3] ) * 3.3 / 4096 * 3.2 ;
        robotState.distanceTelemetreExtremeGauche = 34 / volts - 5 ;
        
        volts = ((float) result[0] ) * 3.3 / 4096 * 3.2 ;
        robotState.distanceTelemetreExtremeDroit = 34 / volts - 5 ;
    }
    
    if (robotState.distanceTelemetreExtremeDroit < 20) // Si obstacle < 30 cm alors LED orange allumé
        LED_ORANGE = 1;

    else 
        LED_ORANGE = 0;     //Sinon LED éteinte

    if (robotState.distanceTelemetreCentre < 20)    // Si obstacle < 30 cm alors LED bleue allumé
        LED_BLEUE = 1;
    
    else 
        LED_BLEUE = 0;      //Sinon LED éteinte

    if (robotState.distanceTelemetreExtremeGauche < 20)    // Si obstacle < 30 cm alors LED blanche allumé
        LED_BLANCHE = 1;
    
    else 
        LED_BLANCHE = 0;        //Sinon LED éteinte
    
    //SendMessageDirect((unsigned char *) "Bonjour", 7);
    //__delay32(40000000);
} // fin main
}

unsigned char stateRobot;
//Attention moteur doit inverser
void OperatingSystemLoop(void)
{
    switch (stateRobot)
    {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
        break;

        case STATE_AVANCE:      //Le robot avance
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);        //25
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
        break;
        
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;

        case STATE_TOURNE_GAUCHE:       //Le robot tourne à gauche
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);     //20
            PWMSetSpeedConsigne(-5, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
        break;
        
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;

        case STATE_TOURNE_DROITE:       //Le robot tourne à droite
            PWMSetSpeedConsigne(5, MOTEUR_DROIT);       //-5
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
        break;
        
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:         //demi-tour gauche
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);     //15
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
        break;
        
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;

        case STATE_TOURNE_SUR_PLACE_DROITE:         //demi-tour droite
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);      //-15
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
        break;
        
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_RALENTI:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);     //15
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_RALENTI_EN_COURS;
        break;
        
        case STATE_RALENTI_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);     //10
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS;
        break;
        
        case STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);     //20
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_DROITE_EN_COURS;
        break;
        
        case STATE_TOURNE_UN_PEU_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_RECULE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);     //-10
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_EN_COURS;
        break;
        
        case STATE_RECULE_EN_COURS:
            if (timestamp > 1000)
                SetNextRobotStateInAutomaticMode();
        break;
            
        default :
            stateRobot = STATE_ATTENTE;
        break;
    }
}

unsigned char nextStateRobot=0;

void SetNextRobotStateInAutomaticMode()
{
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < 20 && robotState.distanceTelemetreCentre < 20 && robotState.distanceTelemetreGauche < 20 && robotState.distanceTelemetreExtremeDroit < 20 && robotState.distanceTelemetreExtremeGauche < 20)
        positionObstacle = OBSTACLE_PARTOUT;
    
//    else if (robotState.distanceTelemetreDroit < 20 && robotState.distanceTelemetreCentre < 20 && robotState.distanceTelemetreGauche < 20 && robotState.distanceTelemetreExtremeDroit > 20 && robotState.distanceTelemetreExtremeGauche < 20)
//        positionObstacle = OBSTACLE_PARTOUT_GAUCHE;
//    
//    else if (robotState.distanceTelemetreDroit < 20 && robotState.distanceTelemetreCentre < 20 && robotState.distanceTelemetreGauche < 20 && robotState.distanceTelemetreExtremeDroit < 20 && robotState.distanceTelemetreExtremeGauche > 20)
//        positionObstacle = OBSTACLE_PARTOUT_DROIT;
    
    else if (robotState.distanceTelemetreDroit < 20 && robotState.distanceTelemetreCentre < 20 && robotState.distanceTelemetreGauche < 20)
        positionObstacle = OBSTACLE_AU_MILIEU;
    
    else if (robotState.distanceTelemetreDroit > 20 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche > 20 && robotState.distanceTelemetreExtremeDroit < 10 && robotState.distanceTelemetreExtremeGauche < 10)
        positionObstacle = OBSTACLE_EXT_GAUCHE_ET_EXT_DROITE;
    
    else if (robotState.distanceTelemetreDroit < 10 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche < 10)
        positionObstacle = OBSTACLE_GAUCHE_ET_DROITE;
    
//    else if (robotState.distanceTelemetreDroit < 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche < 30)
//        positionObstacle = OBSTACLE_GAUCHE_ET_DROITE_LOIN;
    
//    else if (robotState.distanceTelemetreDroit < 25 && robotState.distanceTelemetreCentre < 25 && robotState.distanceTelemetreGauche > 30 && robotState.distanceTelemetreDroit > 20 && robotState.distanceTelemetreCentre > 20 && robotState.distanceTelemetreExtremeDroit < 25 && robotState.distanceTelemetreExtremeGauche < 25 && robotState.distanceTelemetreExtremeDroit > 15 && robotState.distanceTelemetreExtremeGauche > 15)
//        positionObstacle = OBSTACLE_A_DROITE_ET_DEVANT;
//    
//    else if (robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre < 30 && robotState.distanceTelemetreGauche < 30 && robotState.distanceTelemetreCentre > 20 && robotState.distanceTelemetreGauche > 20)
//        positionObstacle = OBSTACLE_A_GAUCHE_ET_DEVANT;
    
    else if ( robotState.distanceTelemetreDroit < 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche > 30) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    
    else if(robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    
    else if(robotState.distanceTelemetreCentre < 20) //Obstacle en face donc demi-tour
        positionObstacle = OBSTACLE_EN_FACE;
    
    else if(robotState.distanceTelemetreCentre < 25 && robotState.distanceTelemetreCentre > 20) //Obstacle en face loin donc ralenti
        positionObstacle = OBSTACLE_EN_FACE_LOIN;
    
    else if (robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche > 30 && robotState.distanceTelemetreExtremeDroit < 20 && robotState.distanceTelemetreExtremeGauche > 30)
        positionObstacle = OBSTACLE_EXT_DROIT;
    
    else if (robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche > 30 && robotState.distanceTelemetreExtremeDroit > 30 && robotState.distanceTelemetreExtremeGauche < 20)
        positionObstacle = OBSTACLE_EXT_GAUCHE;
    
    else if(robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre > 30 && robotState.distanceTelemetreGauche > 30) //pas d'obstacle
        positionObstacle = PAS_D_OBSTACLE;

    
    
    
    if (positionObstacle == OBSTACLE_PARTOUT)
        nextStateRobot = STATE_RECULE;
    
    else if (positionObstacle == OBSTACLE_PARTOUT_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    else if (positionObstacle == OBSTACLE_PARTOUT_DROIT)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    else if (positionObstacle == OBSTACLE_AU_MILIEU)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    else if (positionObstacle == OBSTACLE_EXT_GAUCHE_ET_EXT_DROITE)
        nextStateRobot = STATE_RALENTI;
    
    else if (positionObstacle == OBSTACLE_GAUCHE_ET_DROITE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    else if (positionObstacle == OBSTACLE_GAUCHE_ET_DROITE_LOIN)
        nextStateRobot = STATE_RALENTI;
    
//    else if (positionObstacle == OBSTACLE_A_DROITE_ET_DEVANT)
//        nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
//    
//    else if (positionObstacle == OBSTACLE_A_GAUCHE_ET_DEVANT)
//        nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
    
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
   
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
      
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        
    else if (positionObstacle == OBSTACLE_EN_FACE_LOIN)
        nextStateRobot = STATE_RALENTI;
    
    else if (positionObstacle == OBSTACLE_EXT_DROIT)
        nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
    
    else if (positionObstacle == OBSTACLE_EXT_GAUCHE)
        nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
    
    else if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
    
    }