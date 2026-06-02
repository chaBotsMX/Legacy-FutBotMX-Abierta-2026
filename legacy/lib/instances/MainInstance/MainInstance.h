#ifndef MAININSTANCE_H
#define MAININSTANCE_H

#include <BohleBots_BNO055.h>
#include <Wire.h>
#include "Omni.h"
#include "utils.h"
#include "dataTypes.h"
#include "VisionProccesing/VisionProccesing.h"
#include "trygonometry.h"
#include "definitions.h"


extern BNO bno;
extern Omni drive;
extern elapsedMillis haveBallTimer;





class MainInstance {
    public:
        MainInstance();
        void init();
        void update();
    private:
        void getReferenceBNO(Data &info);
        int getOrientationError(Data &info);
        int smoothGoalAngle(int newAngle, int currentHeading,const Data info);
        void turnTowardsGoal(GoalInfo &goal);
        void tryScoring(Data &info);
        void turnTowardsGoalSmooth(GoalInfo &goal);
        int decideStrategy();
        void executeStrategy(int strategy);
        int correctLineAngle(int angle);
        bool receiveInfoFromLineTeensy(HardwareSerial &port);
        bool receiveInfoFromCameraTeensy(HardwareSerial &port, Data &info);
        void filterData();
        void getIncomingData();
        void goToGoal(Data &info);
        VisionProccesing vision;
        Data data;
};

#endif // MAININSTANCE_H