#ifndef MAININSTANCE_H
#define MAININSTANCE_H

#include <BohleBots_BNO055.h>
#include <Wire.h>
#include "Omni.h"
#include "utils.h"


#define START_BYTE 0xAA
#define END_BYTE   0x55
#define BALL_DEBOUNCE_MS 500
#define GOAL_STEP_MS 25
extern BNO bno;
extern Omni drive;
extern elapsedMillis haveBallTimer;

struct Vector{
    int angle;
    int magnitude;
};


struct GoalInfo {
    Vector coordinates = {500, 500};
    int x;
    int y;
    int proportionY;
};


struct Data{
    int ballAng = 500;
    int ballDistance = 500;
    GoalInfo frontGoal = {500, 500,500,500};
    GoalInfo backGoal = {500, 500,500,500};
    Vector Cooordinates = {500, 500};
    unsigned long lastGoalStep = 0;
    int smoothGoalAng = 500;
    int lineAng = 500;
    bool haveBall = false;
    int orientationError = 500;
};

extern Data data;

class MainInstance {
    public:
        MainInstance();
        void init();
        void update();
    private:
        bool isInLine();
        bool isSeeingBall();
        bool isHaveBall();
        bool isSeeingBothGoals();
        bool isSeeingBackGoal();
        bool isSeeingFrontGoal();
        int getBallCatchTrajectory(int ballAngle, int ballDistance);
        int getBallCatchSpeed(int ballAngle, int ballDistance);
        int decideStrategy();
        void executeStrategy(int strategy);
        int correctLineAngle(int angle);
        bool filterHaveBall(bool haveBall, elapsedMillis &timer);    
        bool receiveInfoFromLineTeensy(HardwareSerial &port);
        bool receiveInfoFromCameraTeensy(HardwareSerial &port, Data &info);
        int getGoalAngle(int inputX, int inputY);
        int getEuclideanDistance(int x, int y);
        Vector sumVectors(Vector &v1, Vector &v2);
        void filterData();
        void getIncomingData();
        void goToGoal(Data &info);


};

#endif // MAININSTANCE_H