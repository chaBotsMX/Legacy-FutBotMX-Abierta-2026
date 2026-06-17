#ifndef MAININSTANCE_H
#define MAININSTANCE_H

#include <BohleBots_BNO055.h>
#include <Wire.h>

#include "../../utils/globalObjects.h"
#include "../../utils/utils.h"
#include "../../utils/dataTypes.h"
#include "../../utils/trygonometry.h"
#include "../../utils/definitions.h"
#include "MainUart/MainUart.h"
#include "PlayLogic/PlayLogic.h"

extern BNO bno;
extern elapsedMillis haveBallTimer;
extern elapsedMillis kickTestTimer;
class MainInstance {
    public:
        MainInstance();
        void init();
        void update();
    private:
        void getReferenceBNO(Data &info);
        int getOrientationError(Data &info);
        int correctLineAngle(int angle);
        void filterData();

        Data data;
        MainUart COMM;
        PlayLogic Play;
};

#endif // MAININSTANCE_H