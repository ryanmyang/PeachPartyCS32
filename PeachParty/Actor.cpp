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
//    std::cerr << "Activator constructor" << std::endl;
//    std::cerr << "Peach after cons: " << m_peach << std::endl;
}

void Activator::initPlayers() {
    m_peach = getWorld()->getPeach();
    m_yoshi = getWorld()->getYoshi();
}

void Activator::doSomething() {
    if (!isAlive()) return;
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

void CoinSquare::affectPlayer(PlayerAvatar * p) {
//    std::cerr << "coinsquare affect" << std::endl;

    // Exit if not new, or if not stopped
    if( (p->getLastX() == getX() && p->getLastY() == getY()) || p->getTicks()!=0) {
//        std::cerr << "not new or not stopped" << p->getTicks() << std::endl;
        return;
    }
    
//    std::cerr << "coin affect player " << p->getPlayerNum() << std::endl;

    p->addCoins( m_grantsCoins?3:-3);
    getWorld()->playSound(m_grantsCoins?SOUND_GIVE_COIN:SOUND_TAKE_COIN);
}

// ---------------------- StarSquare ----------------------
StarSquare::StarSquare(StudentWorld* w, int initX, int initY):Activator(w, IID_STAR_SQUARE, initX, initY, right, 1),Actor(w, IID_STAR_SQUARE, initX, initY, right, 1){
    
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

void BankSquare::affectPlayer(PlayerAvatar * p) {
//    std::cout << "Player stepping on bank: " << p->getPlayerNum() << std::endl;
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

void DirectionSquare::affectPlayer(PlayerAvatar * p) {
   // Exit if not new
    p->setMoveDir(m_dir);
    if(m_dir == 180) {p->setDirection(180);}
   
}
// ---------------------- EventSquare ----------------------
EventSquare::EventSquare(StudentWorld* w, int initX, int initY):Activator(w, IID_EVENT_SQUARE, initX, initY, right, 1),Actor(w, IID_EVENT_SQUARE, initX, initY, right, 1){
    
}

void EventSquare::affectPlayer(PlayerAvatar * p) {
    // Exit if not new or not landed
    if( ( p->getLastX() == getX() && p->getLastY() == getY() ) || p->getTicks() != 0) {
        return;
    }
//    std::cerr << "invoking event square!" << std::endl;
    switch( randInt(0,2) ) {
        case 0:
            p->setTeleported(true);
            p->randomTP();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            break;
        case 1:
            p->swap();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            break;
        case 2:
            std::cerr << "Player " << std::to_string(p->getPlayerNum()) << " got a vortex!" << std::endl;
            p->giveVortex();
            break;
    }
}

// ---------------------- DroppingSquare ----------------------
DroppingSquare::DroppingSquare(StudentWorld* w, int initX, int initY):Activator(w, IID_DROPPING_SQUARE, initX, initY, right, 1),Actor(w, IID_DROPPING_SQUARE, initX, initY, right, 1){
    
}

void DroppingSquare::affectPlayer(PlayerAvatar * p) {
    // Exit if not new
    if( (p->getLastX() == getX() && p->getLastY() == getY()) || p->getTicks()!=0) {
        return;
    }
    switch(randInt(0, 1)) {
        case 1:
            if (p->getCoins()-10<0) {
                p->setCoins(0);
            } else {p->addCoins(-10);}
            break;
        case 2:
            if (p->getStars()>0) {
                p->setStars(p->getStars()-1);
            }
    }
    getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    
}

// ---------------------- MovingActor ----------------------
MovingActor::MovingActor(StudentWorld* w, int img, int initX, int initY, int moveDir):Actor(w, img, initX, initY) {
    m_ticks_to_move = 0;
    m_moveDir = moveDir;
}

// nextTileEmpty returns -1 if not on grid, 0 if not empty, 1 if empty
int MovingActor::nextTileEmpty(int moveDir) {
    if(moveDir%90!=0) {return -1;}
    if(getX()%SPRITE_WIDTH==0 && getY()%SPRITE_HEIGHT==0){
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

void MovingActor::adjustIfAtTurn() {
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
}

void MovingActor::randomTP() {
//    std::cerr << "randomtp" << std::endl;
    int x = 0; int y = 0;
    getWorld()->setRandomValidLoc(x, y);
    moveTo(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
}

// ---------------------- Baddie ----------------------
Baddie::Baddie(StudentWorld* w, int img, int initX, int initY, int maxWander, int dir):Activator(w, img, initX, initY), MovingActor(w, img, initX, initY), Actor(w, img, initX, initY, right, 0) {
    m_pauseCount = 180;
    m_isPaused = true;
    m_maxWander = maxWander;
    setLastX(initX * SPRITE_WIDTH);
    setLastY(initY * SPRITE_WIDTH);
}

void Baddie::impact() {
    randomTP();
    setMoveDir(0);
    setDirection(0);
    m_isPaused = true;
    m_pauseCount = 180;
}

// Mostly identical doSomething() function for both baddies
void Baddie::doSomething() {
    
    if(m_isPaused) {
        affectBothPlayers();
        
        m_pauseCount--;
        
        // If end of pause aka pause is 0
        if(m_pauseCount <= 0) {
            
            // Wander in random directin:
            int squaresToMove = randInt(1, m_maxWander);
            setTicks(squaresToMove * 8);
            
            // Choose random dir and change movedir
            int newDir = -1;
            while( nextTileEmpty(newDir)!=0) {
                newDir = randInt(0, 3) * 90;
            }
            setMoveDir(newDir);
            setDirection(newDir==left?180:0);
            m_isPaused = false;
        }
        setLastX(getX());
        setLastY(getY());
    }
    // else if walking
    else {
        setLastX(getX());
        setLastY(getY());
        // a If on square and at a fork
        int numPaths = 0;
        for (int d = 0; d < 360; d+=90) {
            if(nextTileEmpty(d) == 0) numPaths++;
        }
        // nextTileEmpty will return -1 if not on square
        if (numPaths > 2) {
            // choose random dir and change
            int newDir = -1;
            while( nextTileEmpty(newDir)!=0) {
                newDir = randInt(0, 3) * 90;
            }
            setMoveDir(newDir);
            setDirection(newDir==left?180:0);
        } else {
            // b else if on square and can't move forward, at turn
            adjustIfAtTurn();
        }
        
        // cde move, decrement, state/counter updates
        moveAtAngle(getMoveDir(), 2);
        setTicks(getTicks()-1);
        if(getTicks()==0) {
            m_isPaused = true;
            m_pauseCount = 180;
            stopFunc();
        }
    }
    
    
}

// ---------------------- Boo ----------------------
Boo::Boo(StudentWorld* w, int initX, int initY):Baddie(w, IID_BOO, initX, initY, 3),Actor(w, IID_BOO, initX, initY) {
    
}

void Boo::affectPlayer(PlayerAvatar* p) {
    // Do nothing if both boo and the player's last square was this one, OR if the player is passing thru
    if( ( p->getLastY()==getY() && p->getLastX()==getX() && getLastY()==getY() && getLastX()==getX() ) || !p->isWaiting()) {return;}
    std::cerr << "boo success" << std::endl;

    PlayerAvatar* other = p->getPlayerNum()==1?getWorld()->getYoshi():getWorld()->getPeach();
    switch(randInt(0, 1)) {
        case 0:{
            int temp = other->getCoins();
            other->setCoins(p->getCoins());
            p->setCoins(temp);
            std::cerr << "coin swap" << std::endl;
            break;}
        case 1:{
            int temp2 = other->getStars();
            other->setStars(p->getStars());
            p->setStars(temp2);
            std::cerr << "star swap" << std::endl;
            break;}
    }
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
    
}

// ---------------------- Bowser ----------------------
Bowser::Bowser(StudentWorld* w, int initX, int initY):Baddie(w, IID_BOWSER, initX, initY, 10),Actor(w, IID_BOWSER, initX, initY) {
    
}

void Bowser::affectPlayer(PlayerAvatar* p) {
    if( ( p->getLastY()==getY() && p->getLastX()==getX() && getLastY()==getY() && getLastX()==getX() ) || !p->isWaiting()) {return;}
    
    switch(randInt(0, 1)) {
        case 0:
            p->setCoins(0);
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            break;
    }
    
}

void Bowser::stopFunc() {
    if(randInt(0, 3)==0) {
        getWorld()->addDroppingSquare(getX(), getY());
        getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
    }
}

// ---------------------- VORTEX ----------------------
Vortex::Vortex(StudentWorld* w, int initX, int initY, int moveDir):Actor(w, IID_VORTEX, initX, initY, 0, 0) {
    m_moveDir = moveDir;
}

void Vortex::doSomething() {
    // Exit if dead
    if(!isAlive()) {return;}
    // Move
    moveAtAngle(m_moveDir, 2);
    // Kill if leaves board
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() >= VIEW_HEIGHT || getY() < 0) {
        kill();
    }
    
    std::cerr << getWorld()->getPeach()->getTicks() << " Y:"<< getWorld()->getYoshi()->getTicks() << std::endl;

    
    Actor* target = getWorld()->returnOneImpactable(getX(), getY());
    if(target) {
        std::cerr << "vortex IMPACT ------------------- Ticks: ";
        
        target->impact();

        kill();
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);

    }
    
}

// ---------------------- PlayerAvatar ----------------------
PlayerAvatar::PlayerAvatar(StudentWorld*w, bool isPeach, int initX, int initY):MovingActor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY),Actor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY) {
    m_coins = 0;
    m_stars = 0;
    m_hasVortex = false;
    m_waitingToRoll = true;
    m_playerNum = isPeach?1:2;
    setLastX(initX*SPRITE_WIDTH);
    setLastY(initY*SPRITE_HEIGHT);
    m_justTeleported = false;
}



void PlayerAvatar::swap() {
    std::cerr << "swap" << std::endl;
    PlayerAvatar* other = getPlayerNum()==1?getWorld()->getYoshi():getWorld()->getPeach();
    std::cerr << getPlayerNum() << other->getPlayerNum() << std::endl;
    
    
    int tempx = other->getX();
    int tempy = other->getY();
    other->moveTo(getX(), getY());
    
    moveTo(tempx,tempy);
    
    // Swap ticks
    int tempTicks = getTicks();
    setTicks(other->getTicks());
    other->setTicks(tempTicks);
    
    // Swap movedir
    int tempMove = getMoveDir();
    setMoveDir(other->getMoveDir());
    other->setMoveDir(tempMove);
    
    // Swap direction
    int tempDir = getDirection();
    setDirection(other->getDirection());
    other->setDirection(tempDir);

    //Swap waiting
    bool tempWait = m_waitingToRoll;
    m_waitingToRoll = other->m_waitingToRoll;
    other->m_waitingToRoll = tempWait;
    
    other->setLastX(other->getX());
    other->setLastY(other->getY());

    
}

void PlayerAvatar::doSomething(){
    setLastX(getX());
    setLastY(getY());
    //std::cerr << "playerdoing: " << m_playerNum << "empty?:" << nextTileEmpty(getMoveDir())<< "movedir: "<< getMoveDir() << std::endl;
//    std::cerr << "doSomething()" << std::endl;
    
// Waiting to roll state
    if(m_waitingToRoll) {
        // a) Validate diraction
        int ogMD = getMoveDir();
        
        // Adjust player if facing invalid/empty direction
        while( nextTileEmpty(getMoveDir())==1 && getMoveDir() !=ogMD) {
            setMoveDir( randInt(0,3)*90 );
            if(getMoveDir()==left) {
                setDirection(180);
            } else { setDirection(0);}
        }
        
        
        // b) Check for Action
        int action = getWorld()->getAction(m_playerNum);
        
        // c) Action roll
        if (action == ACTION_ROLL){
            int dieRoll = randInt(1, 10);
            setTicks(dieRoll * 8);
            m_waitingToRoll = false;
        }
//[]    d) Action fire
        else if(action == ACTION_FIRE && m_hasVortex) {
            Actor* v = new Vortex(getWorld(), getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, getMoveDir());
            m_hasVortex = false;
            getWorld()->addActor(v);
            getWorld()->playSound(SOUND_GIVE_VORTEX);
            return;
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
    Board::GridEntry currentSquare = getWorld()->getSquare(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT);
    if ((numPaths >2||wasJustTeleported()) && currentSquare != Board::down_dir_square && currentSquare != Board::left_dir_square && currentSquare != Board::right_dir_square && currentSquare != Board::up_dir_square ) {
        //std::cerr << "X: " <<getX() << ", Y: " << getY() << std::endl;
        
        //std::cerr << "NUMPATHS OVER 2" << std::endl;
        int action = getWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_UP:
                // Return if trying to go backwards and not just teleported, OR if its empty that way
                if ((getMoveDir() == down &&!wasJustTeleported() )|| nextTileEmpty(up)==1) return;
                setMoveDir(up);
                setDirection(0);
                setTeleported(false);
                break;
            case ACTION_RIGHT:
                // Return if trying to go backwards and not just teleported, OR if its empty that way
                if ((getMoveDir() == left &&!wasJustTeleported() )|| nextTileEmpty(right)==1) return;
                setMoveDir(right);
                setDirection(0);
                setTeleported(false);
                break;
            case ACTION_DOWN:
                // Return if trying to go backwards and not just teleported, OR if its empty that way
                if ((getMoveDir() == up &&!wasJustTeleported() )|| nextTileEmpty(down)==1) return;
                setMoveDir(down);
                setDirection(0);
                setTeleported(false);
                break;
            case ACTION_LEFT:
                // Return if trying to go backwards and not just teleported, OR if its empty that way
                if ((getMoveDir() == right &&!wasJustTeleported() )|| nextTileEmpty(left)==1) return;
                setMoveDir(left);
                setDirection(180);
                setTeleported(false);
                break;
            default:
                return;
        }
    }

//      c) If can't continue in main direction
    //std::cerr << "nexttile: " <<xCopy/BOARD_WIDTH<<","<<yCopy/BOARD_HEIGHT << "empty?:" << (nextTile==Board::empty) << std::endl;
    // If up or down, try left or right
    adjustIfAtTurn();

//      d-f Move and end movestate
    moveAtAngle(getMoveDir(), 2);
    setTicks(getTicks()-1);
    if(getTicks()==0)
        m_waitingToRoll = true;
    

}


