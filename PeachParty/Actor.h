#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
    public:
        virtual void doSomething() = 0;
        Actor(StudentWorld* w, int img, int initX, int initY, int dir = right, int depth = 0, double size = 1.0);
        bool isAlive();
        void kill();
        StudentWorld* getWorld();
    private:
        bool m_alive;
        StudentWorld* m_world;
    
};

class CoinSquare : public Actor {
    public:
        virtual void doSomething();
        CoinSquare(StudentWorld* w, bool grants, int initX, int initY);
    private:
        bool m_grantsCoins;
        
};

class PlayerAvatar : public Actor {
public:
    PlayerAvatar(StudentWorld* w, bool isPeach, int initX, int initY);
    virtual void doSomething();
private:
    int m_coins;
    int m_stars;
    Actor* m_vortex;
    int m_ticks_to_move;
    bool m_waitingToRoll;
    int m_playerNum;
};

#endif // ACTOR_H_

