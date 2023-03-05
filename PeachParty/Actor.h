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
    virtual bool activates() {return false;}
        StudentWorld* getWorld();
    private:
        bool m_alive;
        StudentWorld* m_world;
    
};

class PlayerAvatar;
class Activator : virtual public Actor {
public:
    virtual bool activates() {return true;}
    virtual void doSomething();
    Activator(StudentWorld* w, int img, int initX, int initY, int dir=right, int depth=1);
    virtual void affectPlayer(PlayerAvatar* p) = 0;
    void affectBothPlayers();
    void initPlayers();
private:
    PlayerAvatar* m_peach;
    PlayerAvatar* m_yoshi;
    

};

class CoinSquare : public Activator {
public:
    virtual void doSomething();
    virtual void affectPlayer(PlayerAvatar* p);
    CoinSquare(StudentWorld* w, bool grants, int initX, int initY);
    private:
        bool m_grantsCoins;
        
};

class StarSquare : public Activator {
public:
    virtual void doSomething();
    virtual void affectPlayer(PlayerAvatar* p);
    StarSquare(StudentWorld* w, int initX, int initY);
    private:
};

class BankSquare : public Activator {
public:
    virtual void doSomething();
    virtual void affectPlayer(PlayerAvatar* p);
    BankSquare(StudentWorld* w, int initX, int initY);
    private:
};

class DirectionSquare : public Activator {
public:
    virtual void doSomething();
    virtual void affectPlayer(PlayerAvatar* p);
    DirectionSquare(StudentWorld* w, int dir, int initX, int initY);
private:
    int m_dir;
};

class EventSquare : public Activator {
public:
    virtual void doSomething();
    virtual void affectPlayer(PlayerAvatar* p);
    EventSquare(StudentWorld* w, int initX, int initY);
    private:
};

class MovingActor : virtual public Actor {
public:
    MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir = 0);
    int getTicks(){return m_ticks_to_move;}
    void setTicks(int t){m_ticks_to_move = t;}
    void setMoveDir(int d) {m_moveDir = d;}
    int getMoveDir() {return m_moveDir;}
    int nextTileEmpty(int moveDir);
    void adjustIfAtTurn();
    void randomTP();

    
private:
    int m_ticks_to_move;
    int m_moveDir = 0;
};

// BADDIE
class Baddie : public MovingActor, public Activator {
public:
    Baddie(StudentWorld* w, int img, int initX, int initY, int maxWander, int dir=right);
    virtual void doSomething();
    virtual void stopFunc() = 0;

    
private:
    int m_pauseCount;
    bool m_isPaused;
    int m_maxWander;
};

// BOO
class Boo: public Baddie {
public:
    Boo(StudentWorld* w, int initX, int initY);
    virtual void stopFunc(){}
    virtual void affectPlayer(PlayerAvatar* p);
private:
    
};

// BOWSWER
class Bowser: public Baddie {
public:
    Bowser(StudentWorld* w, int initX, int initY);
    virtual void stopFunc();
    virtual void affectPlayer(PlayerAvatar* p);
private:
    
};

class PlayerAvatar : public MovingActor {
public:
    PlayerAvatar(StudentWorld* w, bool isPeach, int initX, int initY);
    virtual void doSomething();
    int getCoins() {return m_coins;}
    void addCoins(int c) {m_coins+=c;}
    void setCoins(int c) {m_coins=c;}
    int getStars() {return m_stars;}
    void addStars(int s) {m_stars+=s;}
    void setStars(int s) {m_stars=s;}
    void addVortex(Actor* v) {m_vortex = v;}
    int getPlayerNum() {return m_playerNum;}
    Actor* getVortex() {return m_vortex;}
    int getLastX() {return m_lastX;}
    int getLastY() {return m_lastY;}
    bool forcedDir() {return m_forcedDir;}
    void forceDir() {m_forcedDir = true;}
    void swap();
    bool isWaiting() {return m_waitingToRoll;}
    
private:
    int m_coins;
    int m_stars;
    Actor* m_vortex;
    bool m_waitingToRoll;
    int m_playerNum;
    int m_lastX;
    int m_lastY;
    bool m_forcedDir;

};

#endif // ACTOR_H_

