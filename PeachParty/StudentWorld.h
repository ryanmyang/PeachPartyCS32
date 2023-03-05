#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        virtual ~StudentWorld();
    PlayerAvatar* getPeach() {return m_peach;}
    PlayerAvatar* getYoshi() {return m_yoshi;}
    Board* getBoard() {return m_board;}
    int getBank() {return m_bankBal;}
    void addBank(int c) {m_bankBal+=c;}
    void resetBank() {m_bankBal = 0;}
    Board::GridEntry getSquare(int x, int y) {return m_board->getContentsOf(x,y);}
    void setRandomValidLoc(int &x, int&y);
    void addDroppingSquare(int x, int y);
    Actor* returnOneImpactable(int x, int y);
    void addActor(Actor* a) {m_actors.push_back(a);}

private:
    std::vector<Actor*> m_actors;
    PlayerAvatar* m_yoshi;
    PlayerAvatar* m_peach;
    Board* m_board;
    int m_bankBal;
        
};

#endif // STUDENTWORLD_H_
