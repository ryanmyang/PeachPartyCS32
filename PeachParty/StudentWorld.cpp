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
    m_bankBal = 0;
    m_yoshi = nullptr;
    m_peach = nullptr;
}

int StudentWorld::init()
{
    // Initialize Board
    m_board = new Board;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = m_board->loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
        
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
    }
    
    for(int i = 0; i <=BOARD_WIDTH; i++) {
        for (int j = 0; j <=BOARD_HEIGHT; j++) {
            switch (m_board->getContentsOf(i, j)) {
                case Board::player:
                    m_peach = new PlayerAvatar(this,true,i,j);
                    m_yoshi = new PlayerAvatar(this,false,i,j);
                    std::cerr << "Players init" << std::endl;
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    break;
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(this, false, i, j));
                    break;
                case Board::empty:
                    break;
                case Board::up_dir_square:
                    m_actors.push_back(new DirectionSquare(this, 90, i, j));
                    break;
                case Board::down_dir_square:
                    m_actors.push_back(new DirectionSquare(this, 270, i, j));
                    break;
                case Board::left_dir_square:
                    m_actors.push_back(new DirectionSquare(this, 180, i, j));
                    break;
                case Board::right_dir_square:
                    m_actors.push_back(new DirectionSquare(this, 0, i, j));
                    break;
                case Board::event_square:
                    m_actors.push_back(new EventSquare(this, i, j));
                    break;
                case Board::bank_square:
                    m_actors.push_back(new BankSquare(this, i, j));
                    break;
                case Board::star_square:
                    m_actors.push_back(new StarSquare(this, i, j));
                    break;
                case Board::bowser:
                    m_actors.push_back(new Bowser(this, i,j));
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    break;
                case Board::boo:
                    m_actors.push_back(new Boo(this, i,j));
                    m_actors.push_back(new CoinSquare(this, true, i, j));
                    break;
            }
        }
    }
    for(int i = 0; i < m_actors.size(); i++) {
        if(m_actors[i]->activates()) {
            dynamic_cast<Activator*>(m_actors[i])->initPlayers();
        }
    }
    
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addDroppingSquare(int x, int y) {
    for(int i = 0; i < m_actors.size(); i++) {
        Actor* a = m_actors[i];
        // if actor xy matches bowser
        if (a->getX() == x && a->getY() == y && a->dropReplace()) {
            //replace this with dropping square
            std::cerr << "deleting something" << std::endl;
            delete a;
            m_actors[i] = (new DroppingSquare(this, x/16, y/16));
            dynamic_cast<DroppingSquare*>(m_actors[i])->initPlayers();
        }
    }
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
    
    
// Status Text
    setGameStatText("P1 Roll: " + to_string(getPeach()->getTicks()/8) + " Stars: " + to_string(getPeach()->getStars()) + " $$: "+ to_string(getPeach()->getCoins()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getBank()) + " | P2 Roll: " + to_string(getYoshi()->getTicks()/8) + " Stars: " + to_string(getYoshi()->getStars()) + " $$: " + to_string(getYoshi()->getCoins()) );
//
    // NEED TO CHECK IF YOSHI OR PEACH WON BY CHECKING THE STARS AND COINS
    if (timeRemaining() <= 0){
        PlayerAvatar* winner;
        if ( m_peach->getStars() == m_yoshi->getStars() && m_peach->getCoins() == m_yoshi->getCoins() ) {
            winner = 1+(rand()%2) == 1?m_yoshi:m_peach;
        }
        else if(m_peach->getStars()>m_yoshi->getStars()) {
            winner = m_peach;
        } else if (m_peach->getCoins()>m_yoshi->getCoins()) {
            winner = m_peach;
        } else {
            winner = m_yoshi;
        }
        setFinalScore(winner->getStars(), winner->getCoins());
        return winner == m_peach?GWSTATUS_PEACH_WON:GWSTATUS_YOSHI_WON;
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
    if (m_yoshi!=nullptr) {
        delete m_yoshi;
        m_yoshi = nullptr;
    }
    if(m_peach!=nullptr) {
        delete m_peach;
        m_peach = nullptr;
    }
}

void StudentWorld::setRandomValidLoc(int &x, int&y) {
    x=-1;y=-1;
    while(m_board->getContentsOf(x, y)==Board::empty) {
        x = 1+(rand()%16);
        y = 1+(rand()%16);
    }
}

StudentWorld::~StudentWorld(){
    cleanUp();
}



