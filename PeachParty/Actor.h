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
    virtual bool dropReplace() {return false;}
    virtual bool activates() {return false;}
    StudentWorld* getWorld();
    virtual bool impactable() {return false;}
    virtual void impact() {}
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

    virtual void affectPlayer(PlayerAvatar* p);
    CoinSquare(StudentWorld* w, bool grants, int initX, int initY);
    virtual bool dropReplace() {return true;}
    private:
        bool m_grantsCoins;
        
};

class StarSquare : public Activator {
public:

    virtual void affectPlayer(PlayerAvatar* p);
    StarSquare(StudentWorld* w, int initX, int initY);
    virtual bool dropReplace() {return true;}
};

class BankSquare : public Activator {
public:
    virtual void affectPlayer(PlayerAvatar* p);
    BankSquare(StudentWorld* w, int initX, int initY);
    virtual bool dropReplace() {return true;}
};

class DirectionSquare : public Activator {
public:

    virtual void affectPlayer(PlayerAvatar* p);
    DirectionSquare(StudentWorld* w, int dir, int initX, int initY);
    virtual bool dropReplace() {return true;}
private:
    int m_dir;
};

class EventSquare : public Activator {
public:
    virtual void affectPlayer(PlayerAvatar* p);
    EventSquare(StudentWorld* w, int initX, int initY);
    virtual bool dropReplace() {return true;}
};

class DroppingSquare : public Activator {
public:
    virtual void affectPlayer(PlayerAvatar* p);
    DroppingSquare(StudentWorld* w, int initX, int initY);
        
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
    virtual bool impactable() {return true;}
    virtual void impact();

    
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

// BOWSER
class Bowser: public Baddie {
public:
    Bowser(StudentWorld* w, int initX, int initY);
    virtual void stopFunc();
    virtual void affectPlayer(PlayerAvatar* p);

private:
    
};

// VORTEX

class Vortex : public Actor {
public:
    Vortex(StudentWorld* w, int initX, int initY, int moveDir);
    virtual void doSomething();

private:
    int m_moveDir;
    
};

class PlayerAvatar : public MovingActor {
public:
    PlayerAvatar(StudentWorld* w, bool isPeach, int initX, int initY);
    virtual void doSomething();
    int getCoins() {return m_coins;}
    void addCoins(int c) {m_coins+=c;}
    void setCoins(int c) {m_coins=c;}
    int getStars() {return m_stars;}
    void giveVortex() {m_hasVortex = true;}
    bool hasVortex() {return m_hasVortex;}
    void addStars(int s) {m_stars+=s;}
    void setStars(int s) {m_stars=s;}
    int getPlayerNum() {return m_playerNum;}
    int getLastX() {return m_lastX;}
    int getLastY() {return m_lastY;}
    bool forcedDir() {return m_forcedDir;}
    void forceDir() {m_forcedDir = true;}
    void swap();
    bool isWaiting() {return m_waitingToRoll;}
    void setTeleported(bool b) {m_justTeleported = b;}
    bool wasJustTeleported() {return m_justTeleported;}
    
private:
    int m_coins;
    int m_stars;
    bool m_waitingToRoll;
    int m_playerNum;
    int m_lastX;
    int m_lastY;
    bool m_forcedDir;
    bool m_hasVortex;
    bool m_justTeleported;

};

#endif // ACTOR_H_

