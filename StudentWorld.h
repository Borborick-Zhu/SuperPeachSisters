#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class actorBase;
class Peach;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool hasObject(int x, int y, bool ifBonk, bool damage, bool checkPeach);
    void addObject(actorBase *thing);
    Peach* getPeach();

private:
    Peach* m_peach;
    std::vector<actorBase*> actors;
};

#endif // STUDENTWORLD_H_
