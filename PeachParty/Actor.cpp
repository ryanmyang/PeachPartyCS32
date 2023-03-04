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
    std::cerr << "Activator constructor" << std::endl;
    std::cerr << "Peach after cons: " << m_peach << std::endl;
}

void Activator::initPlayers() {
    m_peach = getWorld()->getPeach();
    m_yoshi = getWorld()->getYoshi();
}

void Activator::doSomething() {
    affectBothPlayers();
}

void Activator::affectBothPlayers() {
//    std::cerr << "Peach: " << m_peach << std::endl;
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

// ---------------------- StarSquare ----------------------
StarSquare::StarSquare(StudentWorld* w, int initX, int initY):Activator(w, IID_STAR_SQUARE, initX, initY, right, 1),Actor(w, IID_STAR_SQUARE, initX, initY, right, 1){
    
}

void StarSquare::doSomething() {
    affectBothPlayers();
}

void StarSquare::affectPlayer(PlayerAvatar * p) {
    // Exit if not new
    if( (p->getLastX() == getX() && p->getLastY() == getY()) ) {
        return;
    }
    if (p->getCoins() < 20) {return;}
    p->addCoins(-20);
    p->addStars(1);
    getWorld()->playSound(SOUND_GIVE_STAR);
}

 //---------------------- BankSquare ----------------------
BankSquare::BankSquare(StudentWorld* w, int initX, int initY):Activator(w, IID_BANK_SQUARE, initX, initY, right, 1),Actor(w, IID_BANK_SQUARE, initX, initY, right, 1){
    
}

void BankSquare::doSomething() {
    affectBothPlayers();
}

void BankSquare::affectPlayer(PlayerAvatar * p) {
    std::cout << "Player stepping on bank: " << p->getPlayerNum() << std::endl;
    // Exit if not new
    if( (p->getLastX() == getX() && p->getLastY() == getY()) ) {return;}
    
    // If landed upon, give coins
    if (p->getTicks() == 0) {
        p->addCoins(getWorld()->getBank());
        getWorld()->resetBank();
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    } else {
        for(int i = 0; p->getCoins()>0 && i < 5; i++) {
            p->addCoins(-1);
            getWorld()->addBank(1);
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
}

//---------------------- DirectionSquare ----------------------
DirectionSquare::DirectionSquare(StudentWorld* w, int dir, int initX, int initY):Activator(w, IID_DIR_SQUARE, initX, initY, right, 1),Actor(w, IID_DIR_SQUARE, initX, initY, dir, 1){
    m_dir = dir;
}

void DirectionSquare::doSomething() {
   affectBothPlayers();
}

void DirectionSquare::affectPlayer(PlayerAvatar * p) {
   //std::cout << "Player stepping on bank: " << p->getPlayerNum() << std::endl;
   // Exit if not new
    p->setMoveDir(m_dir);
   
}


// ---------------------- MovingActor ----------------------
MovingActor::MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir):Actor(w, img, initX, initY) {
    m_ticks_to_move = 0;
    m_moveDir = moveDir;
}

// nextTileEmpty returns -1 if not on grid, 0 if not empty, 1 if empty
int MovingActor::nextTileEmpty(int moveDir) {
    if(getX()%16==0 && getY()%16==0){
        int xCopy = getX();
        int yCopy = getY();
        //std::cerr << std::endl << "dir: " << getMoveDir() << "pos: " << getX() << "," << getY() << std::endl;
        getPositionInThisDirection(moveDir, SPRITE_WIDTH, xCopy, yCopy);
        
        // If down or left, change the next position to be just one pixel left (and then truncated)
        
        Board::GridEntry nextTile = getWorld()->getBoard()->getContentsOf(xCopy/BOARD_WIDTH, yCopy/BOARD_HEIGHT);
        return nextTile==Board::empty?1:0;
    }
    return -1;
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
    //std::cerr << "playerdoing: " << m_playerNum << "empty?:" << nextTileEmpty(getMoveDir())<< "movedir: "<< getMoveDir() << std::endl;
//    std::cerr << "doSomething()" << std::endl;
    
// Waiting to roll state
    if(m_waitingToRoll) {
        // a) Validate diraction
        int ogMD = getMoveDir();
        // Adjust player if facing invalid/empty direction
        while( nextTileEmpty(getMoveDir())==1 && getMoveDir() !=ogMD) {
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
    
    // Walking state - Don't need to check because the waiting state would have returned if it's not in the walking state

//[]    a) If on directional square
    
    
//[]    b) If at fork
    
    // Count paths
    int numPaths = 0;
    for (int d = 0; d < 360; d+=90) {
        if(nextTileEmpty(d) == 0) numPaths++;
        //std::cout << "path angle testing: " << d << std::endl;
    }
    //std::cout << "numpaths: " << numPaths << std::endl;
    // IF AT FORK, attempt to change direction based on input
    Board::GridEntry currentSquare = getWorld()->getSquare(getX()/16, getY()/16);
    if (numPaths >2 && currentSquare != Board::down_dir_square && currentSquare != Board::left_dir_square && currentSquare != Board::right_dir_square && currentSquare != Board::up_dir_square ) {
        //std::cerr << "X: " <<getX() << ", Y: " << getY() << std::endl;
        
        //std::cerr << "NUMPATHS OVER 2" << std::endl;
        int action = getWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_UP:
                if (getMoveDir() == down || nextTileEmpty(up)==1) return;
                setMoveDir(up);
                setDirection(0);
                break;
            case ACTION_RIGHT:
                if (getMoveDir() == left || nextTileEmpty(right)==1) return;
                setMoveDir(right);
                setDirection(0);
                break;
            case ACTION_DOWN:
                if (getMoveDir() == up || nextTileEmpty(down)==1) return;
                setMoveDir(down);
                setDirection(0);
                break;
            case ACTION_LEFT:
                if (getMoveDir() == right || nextTileEmpty(left)==1) return;
                setMoveDir(left);
                setDirection(180);
                break;
            default:
                return;
        }
    }

//      c) If can't continue in main direction
    //std::cerr << "nexttile: " <<xCopy/BOARD_WIDTH<<","<<yCopy/BOARD_HEIGHT << "empty?:" << (nextTile==Board::empty) << std::endl;
    // If up or down, try left or right
    if(nextTileEmpty(getMoveDir())==1 && getMoveDir()%180==90) {
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
    else if(nextTileEmpty(getMoveDir())==1 && getMoveDir()%180==0) {
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
