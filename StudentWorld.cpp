#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = nullptr;
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    int lvlNumber = getLevel();
    string lvlStr = to_string(lvlNumber);
    string levelFile = "level0" + lvlStr + ".txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level0" + lvlStr + ".txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level0" + lvlStr + ".txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                ge = lev.getContentsOf(i, j);
                if (ge == Level::peach) {
                    m_peach = new Peach(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this);
                }
                if (ge == Level::block) {
                    Block *temp = new Block(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this, "");
                    actors.push_back(temp);
                }
                if (ge == Level::pipe) {
                    Pipe *temp = new Pipe (i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this);
                    actors.push_back(temp);
                }
                if (ge == Level::flag) {
                    Flag *temp = new Flag(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this);
                    actors.push_back(temp);
                }
                if (ge == Level::mario) {
                    Mario *temp = new Mario(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
                    actors.push_back(temp);
                }
                
                if (ge == Level::star_goodie_block) {
                    Block *temp = new Block(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this, "star");
                    actors.push_back(temp);
                }
                
                if (ge == Level::mushroom_goodie_block) {
                    Block *temp = new Block(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this, "mushroom");
                    actors.push_back(temp);
                }
                
                if (ge == Level::flower_goodie_block) {
                    Block *temp = new Block(i * SPRITE_WIDTH, j *  SPRITE_HEIGHT, this, "flower");
                    actors.push_back(temp);
                }
                
                if (ge == Level::piranha) {
                    Piranha *temp = new Piranha(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
                    actors.push_back(temp);
                }
                
                if (ge == Level::goomba) {
                    Enemy *temp = new Enemy(IID_GOOMBA, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, false, this);
                    actors.push_back(temp);
                }
                
                if (ge == Level::koopa) {
                    Enemy *temp = new Enemy(IID_KOOPA, i * SPRITE_WIDTH, j * SPRITE_HEIGHT,true, this);
                    actors.push_back(temp);
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

// This code is here merely to allow the game to build, run, and terminate after you hit enter.
// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

int StudentWorld::move()
{
    if (m_peach->isAlive()) {
        m_peach->doSomething();
    }
    
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->isAlive()) {
            actors[i]->doSomething();
            if (!m_peach->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    if (m_peach->ifReachedFlag()) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (m_peach->ifReachedMario()) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    //check for the dead actors.
    for(vector<actorBase*>::iterator it = actors.begin(); it != actors.end();) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = actors.erase(it);
            it = actors.begin();
        } else {
            it++;
        }
    }
    string life = to_string(getLives());
    string levelNumber = to_string(getLevel());
    string points = to_string(getScore());
    string jump = "";
    string invincible = "";
    string shoot = "";
    if (m_peach->isJumpPower()) {
        jump = "JumpPower!";
    }
    if (m_peach->isInvincible()) {
        invincible = "StarPower!";
    }
    if (m_peach->isShootPower()) {
        shoot = "ShootPower!";
    }
    
    string input = "";
    if (getScore() == 0) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: 00000" + points + " " + invincible + " " + shoot + " " + jump;
    } else if (getScore() < 100) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: 0000" + points + " " + invincible + " " + shoot + " " + jump;
    } else if (getScore() < 1000) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: 000" + points + " " + invincible + " " + shoot + " " + jump;
    } else if (getScore() < 10000) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: 00" + points + " " + invincible + " " + shoot + " " + jump;
    } else if (getScore() < 100000) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: 0" + points + " " + invincible + " " + shoot + " " + jump;
    } else if (getScore() < 1000000) {
        input = "Lives: " + life + " Level: " + levelNumber + " Points: " + points + " " + invincible + " " + shoot + " " + jump;
    }
    
    setGameStatText(input);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete peach.
    delete m_peach;
    
    // delete all the actors. 
    for (vector<actorBase*>::iterator it = actors.begin(); it != actors.end();) {
        delete *it;
        it = actors.erase(it);
        it = actors.begin();
    }
    
}

bool StudentWorld::hasObject(int x, int y, bool ifBonk, bool damage, bool checkPeach)
{
    if (checkPeach) {
        if (m_peach->getX() <= x + SPRITE_WIDTH - 1 && x <= m_peach->getX() + SPRITE_WIDTH - 1 && m_peach->getY() <= y + SPRITE_HEIGHT - 1 && y <= m_peach->getY() + SPRITE_HEIGHT - 1) {
            return true;
        }
        return false;
    } else {
        bool statement = false;
        for (int i = 0; i < actors.size(); i++) {
            actorBase *a = actors[i];

            if (a->getX() <= x + SPRITE_WIDTH - 1 && x <= a->getX() + SPRITE_WIDTH - 1) {
                if (a->getY() <= y + SPRITE_HEIGHT - 1 && y <= a->getY() + SPRITE_HEIGHT - 1) {
                    if (ifBonk) {
                        a->bonk();
                    }

                    if (a->isSolid()) {
                        statement = true;
                    }
                    else if (damage && a->isMonster() && a->isAlive()) {
                        a->damageObject();
                        statement = true;
                    }
                    else {
                        continue;
                    }
                        
                }
            }

        }
        return statement;
    }
    
}


Peach* StudentWorld::getPeach() {
    return m_peach;
}

void StudentWorld::addObject(actorBase *thing) {
    actors.push_back(thing);
}

