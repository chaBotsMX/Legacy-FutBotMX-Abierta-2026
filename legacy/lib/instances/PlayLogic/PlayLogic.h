#ifndef PLAYLOGIC_H
#define PLAYLOGIC_H


#include <elapsedMillis.h>
#include "../../utils/definitions.h"
#include "../../utils/dataTypes.h"
#include "../../utils/utils.h"
#include "../../utils/trygonometry.h"
#include "../../utils/globalObjects.h"

enum class ScoringState {
    Idle,
    ChooseMethod,
    TakeOutOfCorner,
    CenterBeforeKick,
    GoToGoal,
    AimAndKick
};

enum class Strategy {
    StandBy,
    EscapeLine,
    Score,
    ChaseBall,
    RepositionWithBothGoals,
    RepositionWithBackGoal,
    RepositionWithFrontGoal
};

class PlayLogic {
    public:
        Strategy decideStrategy(Data &newData);
        void executeStrategy(Strategy strategy);
        const char* getStrategyName(Strategy strategy) const;
        const char* getScoringStateName() const;
    private:
        Data local; 
        ScoringState scoringState = ScoringState::Idle;
        elapsedMillis centerBeforeKickTimer = 0;
        int smoothGoalAngle(int goalAngle, int heading);
        void resetScoring();
        bool shouldTakeOutOfCorner() const;
        void executeEscapeLine();
        void executeChaseBall();
        void executeReposition();
        void executeStandBy();
        void takeOutOfCorner();
        void centerBeforeKick();
        void updateScoring();
        void goToGoal(); 
        void goToNeutralFront();
        void rotateAndKick();   
        void goToNeutralCenter();
        void goToNeutralBack();
        Vector neutralCenter = {0, 0};
        Vector neutralSoloFront = {180,40};
        Vector neutralSoloBack = {0,40};
};

#endif
