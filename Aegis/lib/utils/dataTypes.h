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

struct camsData{
    Vector camLeft;
    Vector camRight; 
};

struct Data{
    Vector ball;
    int orientationError;
    int posX;
    int posY;
};
#endif
