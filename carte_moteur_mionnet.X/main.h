#ifndef MAIN_H
#define	MAIN_H

//Configuration des paramtres du chip
#define FCY 40000000

#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_DROITE 6
#define STATE_TOURNE_DROITE_EN_COURS 7
#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
#define STATE_TOURNE_SUR_PLACE_DROITE 10
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
#define STATE_ARRET 12
#define STATE_ARRET_EN_COURS 13
#define STATE_RECULE 14
#define STATE_RECULE_EN_COURS 15
#define STATE_RALENTI 16
#define STATE_RALENTI_EN_COURS 17
#define STATE_TOURNE_UN_PEU_GAUCHE 18
#define STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS 19
#define STATE_TOURNE_UN_PEU_DROITE 20
#define STATE_TOURNE_UN_PEU_DROITE_EN_COURS 21

#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_GAUCHE_ET_DROITE_LOIN 3
#define OBSTACLE_GAUCHE_ET_DROITE 4
#define OBSTACLE_A_DROITE_ET_DEVANT 5
#define OBSTACLE_EN_FACE 6
#define OBSTACLE_EN_FACE_LOIN 7
#define OBSTACLE_PARTOUT 8
#define OBSTACLE_A_GAUCHE_ET_DEVANT 9

void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode();
int main (void);

#endif