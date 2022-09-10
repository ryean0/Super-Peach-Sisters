#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// @Constructor
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), isGameOver(false), isLevelOver(false)
{
    // Blocking Object grid
    for (int i=0; i<VIEW_WIDTH-1; i++) {
        for (int j=0; j<VIEW_HEIGHT-1; j++) {
            m_grid[i][j] = false;
        }
    }
}

// @Destructor
StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::addActor(Actor *x) {
    m_actors.push_back(x);
}

int StudentWorld::init()
{
    // Reset parameters of student world
    isLevelOver = false;
    isGameOver = false;
    for (int i=0; i<VIEW_WIDTH-1; i++) {
        for (int j=0; j<VIEW_HEIGHT-1; j++) {
            m_grid[i][j] = false;
        }
    }
    int currentLevel = getLevel();
    // load up first level data file using Level Class
    Level lev(assetPath());
    string level_file = "level0" + to_string(currentLevel) + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level01.txt data file" << endl;
    } else if (result == Level::load_fail_bad_format) {
        cerr << "level01.txt is improperly formatted" << endl;
    } else if (result == Level::load_success) {
        Level::GridEntry ge;
        for (int x=0; x<VIEW_WIDTH; x++) {
            for (int y=0; y<VIEW_HEIGHT; y++) {
                ge = lev.getContentsOf(x, y);
                switch (ge) {
                    case Level::empty:
                        // cout << "Location" << x << "," << y << " is empty" << endl;
                        break;
                    case Level::koopa: {
                        // cout << "Location" << x << "," << y << " starts with a koopa" << endl;
                        Koopa *myKoopa = new Koopa(IID_KOOPA, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 0, 1.0, true, this);
                        addActor(myKoopa);
                        break;
                    }
                    case Level::goomba: {
                        // cout << "Location" << x << "," << y << " starts with a goomba" << endl;
                        Goomba *myGoomba = new Goomba(IID_GOOMBA, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 0, 1.0, true, this);
                        addActor(myGoomba);
                        break;
                    }
                    case Level::peach: {
                        // cout << "Location" << x << "," << y << " starts with peach" << endl;
                        Peach *myPeach = new Peach(IID_PEACH, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 0, 1.0, true, this, 1); // implement constructor
                        m_peach = myPeach;
                        break;
                    }
                    case Level::flag: {
                        // cout << "Location" << x << "," << y << " starts with a mario" << endl;
                        Flag *myFlag = new Flag(IID_FLAG, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 1, 1.0, true, this);
                        addActor(myFlag);
                        break;
                    }
                    case Level::block: {
                        // cout << "Location" << x << "," << y << " starts with a block" << endl;
                        Block *myBlock = new Block(IID_BLOCK, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 2, 1.0, true, this, 0); // implement constructor
                        addActor(myBlock); // add block to actors map
                        
                        // set all the pixels for the block to be true in m_grid
                        if (x == 0) {
                            for (int i=(x*SPRITE_WIDTH); i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        } else {
                            for (int i=(x*SPRITE_WIDTH)-4; i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        }
                        break;
                    }
                    case Level::star_goodie_block: {
                        // cout << "Location" << x << "," << y << " starts with a star goodie block" << endl;
                        Block *myBlock = new Block(IID_BLOCK, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 2, 1.0, true, this, 1); // implement constructor
                        addActor(myBlock); // add block to actors map
                        
                        // set all the pixels for the block to be true in m_grid
                        if (x == 0) {
                            for (int i=(x*SPRITE_WIDTH); i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        } else {
                            for (int i=(x*SPRITE_WIDTH)-4; i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        }
                        break;
                    }
                    case Level::piranha: {
                        // cout << "Location" << x << "," << y << " starts with a mushroom goodie block" << endl;
                        Piranha *myPiranha = new Piranha(IID_PIRANHA, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 0, 1.0, true, this);
                        addActor(myPiranha);
                        break;
                    }
                    case Level::mushroom_goodie_block: {
                        // cout << "Location" << x << "," << y << " starts with a star goodie block" << endl;
                        Block *myBlock = new Block(IID_BLOCK, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 2, 1.0, true, this, 2); // implement constructor
                        addActor(myBlock); // add block to actors map
                        
                        // set all the pixels for the block to be true in m_grid
                        if (x == 0) {
                            for (int i=(x*SPRITE_WIDTH); i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        } else {
                            for (int i=(x*SPRITE_WIDTH)-4; i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        }
                        break;
                    }
                    case Level::flower_goodie_block: {
                        // cout << "Location" << x << "," << y << " starts with a flower goodie block" << endl;
                        Block *myBlock = new Block(IID_BLOCK, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 2, 1.0, true, this, 3); // implement constructor
                        addActor(myBlock); // add block to actors map
                        
                        // set all the pixels for the block to be true in m_grid
                        if (x == 0) {
                            for (int i=(x*SPRITE_WIDTH); i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        } else {
                            for (int i=(x*SPRITE_WIDTH)-4; i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        }
                        break;
                    }
                    case Level::pipe: {
                        // cout << "Location" << x << "," << y << " starts with a pipe" << endl;
                        Pipe *myPipe = new Pipe(IID_PIPE, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 2, 1.0, true, this);
                        addActor(myPipe); // add block to actors map
                        
                        // set all the pixels for the block to be true in m_grid
                        if (x == 0) {
                            for (int i=(x*SPRITE_WIDTH); i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        } else {
                            for (int i=(x*SPRITE_WIDTH)-4; i<(x*SPRITE_WIDTH)+SPRITE_WIDTH-1; i++) {
                                for (int j=(y*SPRITE_HEIGHT); j<(y*SPRITE_HEIGHT)+SPRITE_HEIGHT-1; j++) {
                                    m_grid[i][j] = true;
                                }
                            }
                        }
                        break;
                    }
                    case Level::mario: {
                        // cout << "Location" << x << "," << y << " starts with a mario" << endl;
                        Mario *myMario = new Mario(IID_MARIO, x*(SPRITE_WIDTH), y*(SPRITE_HEIGHT), 0, 1, 1.0, true, this);
                        addActor(myMario);
                        break;
                    }
   
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

Peach *StudentWorld::getPeach() {
    return m_peach;
}

bool StudentWorld::isBlockingObjectAt(int x, int y) {
    // check if there's a blocking object at coordinates
    return m_grid[x][y];
}

Actor* StudentWorld::getBlockingObjectAt(int x, int y) {
    for (int i=0; i<m_actors.size(); i++) {
        if (m_actors.at(i) == nullptr || !m_actors.at(i)->isAlive()) {continue;}
        if (m_actors.at(i)->getX() >= x && m_actors.at(i)->getX() <= x + SPRITE_WIDTH-1 && m_actors.at(i)->getY() >= y && m_actors.at(i)->getY() <= y+ SPRITE_HEIGHT-1) {
            return m_actors.at(i);
        }
    }
    return nullptr;
}


int StudentWorld::move()
{

    // ask all the actors to do something
    m_peach->doSomething();
    for (int i=0; i<m_actors.size(); i++) {
        if (m_actors.at(i) == nullptr) { continue; }
        m_actors.at(i)->doSomething();
        // end game if peach dies
        if (m_peach->isAlive() == false) {
            playSound(SOUND_PLAYER_DIE);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (isLevelOver) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (isGameOver) {
            playSound(SOUND_GAME_OVER);
            return GWSTATUS_PLAYER_WON;
        }
        
    }
    // delete all actors
    for (int i=0; i<m_actors.size(); i++) {
        if (m_actors.at(i)->isAlive() == false) {
            delete m_actors.at(i);
            m_actors.erase(m_actors.begin()+i);
        }
    }
    // setting game text
    string gameText = "Lives " + to_string(getLives()) + " Level: " + to_string(getLevel()) + " Points: " + to_string(getScore());
    if (m_peach->hasStarPower()) {
        gameText += " StarPower!";
    }
    if (m_peach->hasJumpPower()) {
        gameText += " JumpPower!";
    }
    if (m_peach->hasShootPower()) {
        gameText += " ShootPower!";
    }
    setGameStatText(gameText);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // delete all actors
    for (int i=0; i<m_actors.size(); i++) {
        delete m_actors.at(i);
    }
    m_actors.clear();
    // delete peach
    delete m_peach;
    
}

void StudentWorld::setLevelOver() {
    isLevelOver = true;
    return;
}

void StudentWorld::setGameOver() {
    isGameOver = true;
    return;
}
