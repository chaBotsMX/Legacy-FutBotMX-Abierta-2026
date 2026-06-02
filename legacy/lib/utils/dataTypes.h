#ifndef DATA_TYPES_H
#define DATA_TYPES_H

struct Vector{
    int angle;
    int magnitude;
};


struct GoalInfo {
    Vector coordinates = {500, 500};
    int x;
    int y;
    float proportionY;
    float proportionX;
};


struct Data{
    int ballAng = 500;
    int ballDistance = 500;
    GoalInfo frontGoal;
    GoalInfo backGoal;
    Vector Cooordinates = {500, 500};
    unsigned long lastGoalStep = 0;
    int smoothGoalAng = 500;
    int lineAng = 500;
    bool haveBall = false;
    int setPoint = 500;
    int heading = 500;
    int orientationError = 500;
};
#endif