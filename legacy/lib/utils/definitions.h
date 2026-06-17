#ifndef DEFINITIONS_H
#define DEFINITIONS_H


// Deficiones constantes que no se deben cambiar
#define START_BYTE 0xAA
#define END_BYTE   0x55
#define BALL_DEBOUNCE_MS 100
#define GOAL_STEP_MS 25
#define OUT_OF_RANGE 500
#define COMM_TIMEOUT 500
#define STAY_STILL 500

// Definiciones que afectan al juego del robot
#define MAX_ROTATION_TOWARDS_GOAL 20
#define ROTATE_MAGNITUDE_TRESHOLD 50
#define ROTATE_KICK_ERROR 20


#define BASE_CHASE_PWM 175
#define BASE_BALL_IN_FRONT_PWM 160
#define CATCH_BALL_SPEED_MULTIPLICATION_CONSTANT 0.2

#define GO_TO_GOAL_PWM 190
#define GO_TO_GOAL_KICK_ERROR 50
#endif