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
    Board* getBoard() {return m_board;}

    private:
        std::vector<Actor*> m_actors;
        Board* m_board;
};

#endif // STUDENTWORLD_H_
