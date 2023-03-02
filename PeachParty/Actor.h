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

class PlayerAvatar;
class Activator : virtual public Actor {
public:
    Activator(StudentWorld* w, int img, int initX, int initY, int dir=right, int depth=0);
    PlayerAvatar* playersHere();
    void doPlayer(PlayerAvatar*);
};

class CoinSquare : public Activator {
    public:
        virtual void doSomething();
        CoinSquare(StudentWorld* w, bool grants, int initX, int initY);
    private:
        bool m_grantsCoins;
        
};

class MovingActor : virtual public Actor {
public:
    MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir = 0);
    int getTicks(){return m_ticks_to_move;}
    void setTicks(int t){m_ticks_to_move = t;}
    void setMoveDir(int d) {m_moveDir = d;}
    int getMoveDir() {return m_moveDir;}
    bool nextTileEmpty(int moveDir);
    
private:
    int m_ticks_to_move;
    int m_moveDir = 0;
};

class PlayerAvatar : public MovingActor {
public:
    PlayerAvatar(StudentWorld* w, bool isPeach, int initX, int initY);
    virtual void doSomething();
    int getCoins() {return m_coins;}
    void addCoins(int c) {m_coins+=c;}
    int getStars() {return m_stars;}
    void addStars(int s) {m_stars+=s;}
    void addVortex(Actor* v) {m_vortex = v;}
    int getPlayerNum() {return m_playerNum;}
    Actor* getVortex() {return m_vortex;}
    
private:
    int m_coins;
    int m_stars;
    Actor* m_vortex;
    bool m_waitingToRoll;
    int m_playerNum;
};

#endif // ACTOR_H_

