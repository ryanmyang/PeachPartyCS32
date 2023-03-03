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

// ---------------------- Activator ----------------------
Activator::Activator(StudentWorld* w, int img, int initX, int initY, int dir, int depth):Actor(w, img, initX, initY, dir, depth) {
    m_peach = w->getPeach();
    m_yoshi = w->getYoshi();
}

void Activator::doSomething() {
    
}

void Activator::affectBothPlayers() {
    if(m_peach->getX() == getX() && m_peach->getY() == getY()) {
        affectPlayer(m_peach);
    }
    if(m_yoshi->getX() == getX() && m_yoshi->getY() == getY()) {
        affectPlayer(m_yoshi);
    }
}
// ---------------------- CoinSquare ----------------------
CoinSquare::CoinSquare(StudentWorld* w, bool grants, int initX, int initY):Activator(w, grants?IID_BLUE_COIN_SQUARE:IID_RED_COIN_SQUARE, initX, initY, right, 1),Actor(w, grants?IID_BLUE_COIN_SQUARE:IID_RED_COIN_SQUARE, initX, initY, right, 1){
    m_grantsCoins = grants;
}

void CoinSquare::doSomething() {
    if (!isAlive()) return;
    affectBothPlayers();
    
}

void CoinSquare::affectPlayer(PlayerAvatar * p) {
    // Exit if not new, or if not stopped
    if( (p->getLastX() == getX() && p->getLastY() == getY()) || p->getTicks()!=0) {
        return;
    }
    p->addCoins( m_grantsCoins?3:-3);
    getWorld()->playSound(m_grantsCoins?SOUND_GIVE_COIN:SOUND_TAKE_COIN);
}

// ---------------------- MovingActor ----------------------
MovingActor::MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir):Actor(w, img, initX, initY) {
    m_ticks_to_move = 0;
    m_moveDir = moveDir;
}

// True if needs to change direction because the board is empty
// If not firmly on a square, or if the next one isn't empty, return true
bool MovingActor::nextTileEmpty(int moveDir) {
    if(getX()%16==0 && getY()%16==0){
        int xCopy = getX();
        int yCopy = getY();
        //std::cerr << std::endl << "dir: " << getMoveDir() << "pos: " << getX() << "," << getY() << std::endl;
        getPositionInThisDirection(moveDir, SPRITE_WIDTH, xCopy, yCopy);
        
        // If down or left, change the next position to be just one pixel left (and then truncated)
        
        Board::GridEntry nextTile = getWorld()->getBoard()->getContentsOf(xCopy/BOARD_WIDTH, yCopy/BOARD_HEIGHT);
        return nextTile==Board::empty;
    }
    return false;
}

// ---------------------- PlayerAvatar ----------------------
PlayerAvatar::PlayerAvatar(StudentWorld*w, bool isPeach, int initX, int initY):MovingActor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY),Actor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY) {
    m_coins = 0;
    m_stars = 0;
    m_vortex = nullptr;
    m_waitingToRoll = true;
    m_playerNum = isPeach?1:2;
}



void PlayerAvatar::doSomething(){
    m_lastX = getX();
    m_lastY = getY();
    std::cerr << "playerdoing: " << m_playerNum << "empty?:" << nextTileEmpty(getMoveDir())<< "movedir: "<< getMoveDir() << std::endl;
//    std::cerr << "doSomething()" << std::endl;
    
// Waiting to roll state
    if(m_waitingToRoll) {
        // a) Validate diraction
        int ogMD = getMoveDir();
        while(nextTileEmpty(getMoveDir())&& getMoveDir() !=ogMD) {
            setMoveDir( (rand() % 4)*90 );
            if(getMoveDir()==left) {
                setDirection(180);
            } else { setDirection(0);}
        }
        
        
        // b) Check for Action
        int action = getWorld()->getAction(m_playerNum);
        
        // c) Action roll
        if (action == ACTION_ROLL){
            int dieRoll = 1+(rand() % 10);
            setTicks(dieRoll * 8);
            m_waitingToRoll = false;
        }
//[]    d) Action fire
        else if(action == ACTION_FIRE) {
            
        }
        else { return; }
    }
    
    // Walking state
    if(!m_waitingToRoll) {
//[]    a) If on directional square
        
        
//[]    b) If at fork
        
//      c) If can't continue in main direction
        //std::cerr << "nexttile: " <<xCopy/BOARD_WIDTH<<","<<yCopy/BOARD_HEIGHT << "empty?:" << (nextTile==Board::empty) << std::endl;
        // If up or down, try left or right
        if(nextTileEmpty(getMoveDir()) && getMoveDir()%180==90) {
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
        else if(nextTileEmpty(getMoveDir()) && getMoveDir()%180==0) {
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

//      d-f Move and end movestate
        moveAtAngle(getMoveDir(), 2);
        setTicks(getTicks()-1);
        if(getTicks()==0)
            m_waitingToRoll = true;
        
    }
}
