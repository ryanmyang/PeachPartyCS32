#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ---------------------- Actor ----------------------
Actor::Actor(StudentWorld* w, int img, int initX, int initY, int dir, int depth, double size):GraphObject(img, initX*SPRITE_WIDTH, initY*SPRITE_HEIGHT, dir, depth, size) {
    m_alive = true;
    m_world = w;
}

bool Actor::isAlive() {return m_alive;}
void Actor::kill() {m_alive=false;}
StudentWorld* Actor::getWorld() {return m_world;}

// ---------------------- CoinSquare ----------------------
CoinSquare::CoinSquare(StudentWorld* w, bool grants, int initX, int initY):Actor(w, IID_BLUE_COIN_SQUARE, initX, initY){
    m_grantsCoins = grants;
}

void CoinSquare::doSomething() {
    if (!isAlive()) return;
    
}

// ---------------------- MovingActor ----------------------
MovingActor::MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir):Actor(w, img, initX, initY) {
    m_ticks_to_move = 0;
    m_moveDir = moveDir;
}


// ---------------------- PlayerAvatar ----------------------
PlayerAvatar::PlayerAvatar(StudentWorld*w, bool isPeach, int initX, int initY):MovingActor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY) {
    m_coins = 0;
    m_stars = 0;
    m_vortex = nullptr;
    m_waitingToRoll = true;
    m_playerNum = isPeach?1:2;
}



void PlayerAvatar::doSomething(){
//    std::cerr << "doSomething()" << std::endl;
    if(m_waitingToRoll) {
        int action =getWorld()->getAction(m_playerNum);
        if (action == ACTION_ROLL){
            int dieRoll = 1+(rand() % 10);
            setTicks(dieRoll * 8);
            m_waitingToRoll = false;
        }
        // Need action roll
        else {
            return;
        }
    }
    
    // Walking state
    if(!m_waitingToRoll) {
        int xCopy = getX();
        int yCopy = getY();
        std::cerr << std::endl << "dir: " << getMoveDir() << "pos: " << getX() << "," << getY() << std::endl;
        getPositionInThisDirection(getMoveDir(), SPRITE_WIDTH, xCopy, yCopy);
        
        // If down or left, change the next position to be just one pixel left (and then truncated)
        if(getMoveDir()==270){yCopy+=SPRITE_HEIGHT-1;}
        else if(getMoveDir()==180){xCopy+=SPRITE_WIDTH-1;}
        yCopy=yCopy<0?-SPRITE_HEIGHT:yCopy;
        xCopy=xCopy<0?-SPRITE_WIDTH:xCopy;
        
        Board::GridEntry nextTile = getWorld()->getBoard()->getContentsOf(xCopy/BOARD_WIDTH, yCopy/BOARD_HEIGHT);
        std::cerr << "nexttile: " <<xCopy/BOARD_WIDTH<<","<<yCopy/BOARD_HEIGHT << "empty?:" << (nextTile==Board::empty) << std::endl;
        // If up or down, try left or right
        if(nextTile==Board::empty && getMoveDir()%180==90) {
            // Check Right
            if(getWorld()->getBoard()->getContentsOf((getX()+SPRITE_WIDTH)/BOARD_WIDTH,getY()/BOARD_HEIGHT) != Board::empty) {
                setDirection(0);
                setMoveDir(0);
            }
            // Check Left
            else if(getWorld()->getBoard()->getContentsOf((getX()-1)/BOARD_WIDTH,getY()/BOARD_HEIGHT) != Board::empty) {
                setDirection(180);
                setMoveDir(180);
            }
        }
        
        // If left or right, check up or down
        else if(nextTile==Board::empty && getMoveDir()%180==0) {
            // Check up
            if(getWorld()->getBoard()->getContentsOf(getX()/BOARD_WIDTH,(getY()+SPRITE_HEIGHT)/BOARD_HEIGHT) != Board::empty) {
                setMoveDir(90);
                setDirection(0);
            }
            // Check Down
            else if(getWorld()->getBoard()->getContentsOf(getX()/BOARD_WIDTH,(getY()-1)/BOARD_HEIGHT) != Board::empty) {
                setDirection(0);
                setMoveDir(270);
            }
        }
        
        moveAtAngle(getMoveDir(), 2);
        setTicks(getTicks()-1);
        if(getTicks()==0)
            m_waitingToRoll = true;
        
    }
}
