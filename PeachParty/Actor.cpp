#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ---------------------- Actor ----------------------
Actor::Actor(StudentWorld* w, int img, int initX, int initY, int dir, int depth, double size):GraphObject(img, initX, initY, dir, depth, size) {
    m_alive = true;
    m_world = w;
}

bool Actor::isAlive() {return m_alive;}
void Actor::kill() {m_alive=false;}



// ---------------------- CoinSquare ----------------------
CoinSquare::CoinSquare(StudentWorld* w, bool grants, int initX, int initY):Actor(w, IID_BLUE_COIN_SQUARE, initX, initY){
    m_grantsCoins = grants;
}

void CoinSquare::doSomething() {
    if (!isAlive()) return;
    
}

// ---------------------- PlayerAvatar ----------------------
PlayerAvatar::PlayerAvatar(StudentWorld*w, bool isPeach, int initX, int initY):Actor(w, isPeach?IID_PEACH:IID_YOSHI, initX, initY) {
    m_coins = 0;
    m_stars = 0;
    m_vortex = nullptr;
    m_ticks_to_move = 0;
    m_waitingToRoll = true;
}

void PlayerAvatar::doSomething(){
    if(m_waitingToRoll) {
        int action =getWorld()->getAction(m_playerNum);
        if(action != ACTION_NONE) {
            if (action == ACTION_ROLL){
                int dieRoll = 1+(rand() % 10);
                m_ticks_to_move = dieRoll * 8;
                m_waitingToRoll = false;
                
            }
            // Need action roll
            else {
                return;
            }
            
        }
    }
    
    if(!m_waitingToRoll) {
        
    }
}
