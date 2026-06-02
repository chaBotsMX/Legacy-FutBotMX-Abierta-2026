#ifndef VISIONPROCCESING_H
#define VISIONPROCCESING_H

#include "dataTypes.h"
#include <elapsedMillis.h>
#include "utils.h"
#include "definitions.h"
class VisionProccesing {
    public:
        VisionProccesing();
        float getProportionY(GoalInfo &goal);
        float getProportionX(GoalInfo &goal);
        bool isInLine(Data &verify);
        bool isSeeingBall(Data &verify);
        bool isHaveBall(Data &verify);
        bool isSeeingBothGoals(Data &verify);
        bool isSeeingBackGoal(Data &verify);
        bool isSeeingFrontGoal(Data &verify);
        int getGoalAngle(int inputX, int inputY);            
        int getBallCatchTrajectory(int ballAngle, int ballDistance);
        int getBallCatchSpeed(int ballAngle, int ballDistance);
        bool filterHaveBall(bool haveBall, elapsedMillis &timer,Data &verify);    
        int getErrorTowardsGoal(GoalInfo &goal, int currentHeading);
    private:

};


#endif // VISIONPROCCESING_H