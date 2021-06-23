#ifndef ROBOT_H
#define ROBOT_H

typedef enum {
    FALSE = 0,
    TRUE = 1,
} boolean;
extern volatile boolean mybool;

typedef struct robotStateBITS {
    union {

        struct 
        {
            unsigned char taskEnCours;
          
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTelemetreExtremeDroit;
            float distanceTelemetreDroit;
            float distanceTelemetreCentre;
            float distanceTelemetreGauche;
            float distanceTelemetreExtremeGauche;
            
            boolean isAutoControl;
            float isfunction;
            float ispayloadLength;
            unsigned char* ispayload;
            
        }
    ;}
;} 
ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;

#endif