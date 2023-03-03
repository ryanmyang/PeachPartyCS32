#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}

int StudentWorld::init()
{
    // Initialize Board
    m_board = new Board;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = m_board->loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
    cerr << "Could not find board01.txt data file\n";
    else if (result == Board::load_fail_bad_format)
    cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
    cerr << "Successfully loaded board\n";
    }
    
    for(int i = 0; i <=BOARD_WIDTH; i++) {
        for (int j = 0; j <=BOARD_HEIGHT; j++) {
            switch (m_board->getContentsOf(i, j)) {
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(this, false, i, j));
                    break;
                case Board::player:
                    m_peach = new PlayerAvatar(this,true,i,j);
                    m_yoshi = new PlayerAvatar(this,false,i,j);
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    
                    break;
                case Board::empty:
                    
                    break;
                case Board::up_dir_square:
                        
                    break;
                case Board::down_dir_square:
                        
                    break;
                case Board::left_dir_square:
                    
                    break;
                case Board::right_dir_square:
                    
                    break;
                case Board::event_square:
                    
                    break;
                case Board::bank_square:
                    
                    break;
                case Board::star_square:
                    
                    break;
                case Board::bowser:
                    
                    break;
                case Board::boo:
                    
                    break;
            }
        }
    }
    
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    // Loop doSomething
    
    for(int i = 0; i < m_actors.size(); i++) {
        if(m_actors[i]->isAlive()) {
            m_actors[i]->doSomething();
        }
    }
    
    // Erase Dead Actors
    for(int i = 0; i < m_actors.size(); i++) {
        if(!m_actors[i]->isAlive()) {
            delete m_actors[0];
            m_actors.erase(m_actors.begin()+i);
        }
    }
    m_peach->doSomething();
    m_yoshi->doSomething();
    
    
//
    setGameStatText("P1 Roll: " + to_string(getPeach()->getTicks()/8) + " Stars: X $$: "+ to_string(getPeach()->getCoins()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: 9 | P2 Roll: " + to_string(getYoshi()->getTicks()/8) + " Stars: 1 $$: " + to_string(getYoshi()->getCoins()) );
//
    // NEED TO CHECK IF YOSHI OR PEACH WON BY CHECKING THE STARS AND COINS
    if (timeRemaining() <= 0){
        return GWSTATUS_PEACH_WON;
    }
//    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{

    for(int i = 0; i < m_actors.size(); i++) {
        if(m_actors[i]!=nullptr) {
            //std::cerr<<"size: " << m_actors.size() << "i" << i<< std::endl;
            delete m_actors[i];
        }
    }
    m_actors.clear();
    
    delete m_yoshi;
    delete m_peach;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}



