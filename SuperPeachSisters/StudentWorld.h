#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath); // constructor
    ~StudentWorld(); // destructor
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isBlockingObjectAt(int x, int y);
    Actor *getBlockingObjectAt(int x, int y);
    void addActor(Actor*x);
    void setLevelOver();
    void setGameOver();
    Peach *getPeach();
    
    

private:
    Peach* m_peach; // pointer to Peach object
    vector<Actor*> m_actors; // keeps track of all other actors
    bool m_grid[VIEW_WIDTH-1][VIEW_HEIGHT-1]; // keeps track of blocking objects
    bool isLevelOver;
    bool isGameOver;
};

#endif // STUDENTWORLD_H_
