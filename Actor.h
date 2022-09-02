#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class actorBase: public GraphObject {
public:
    actorBase(int imageID, int startX, int startY, int direction, int depth, int size, StudentWorld* world);
    virtual void doSomething() = 0;
    virtual void bonk() = 0;
    virtual bool isSolid() = 0;
    virtual bool isMonster() = 0;
    virtual void damageObject() = 0;
    StudentWorld* getWorld() const;
    bool isAlive();
    void toggleAlive();
private:
    StudentWorld* currentWorld;
    bool m_alive; 
};

class Block: public actorBase {
public:
    Block(int startX, int startY, StudentWorld *world, std::string goodie);
    void doSomething();
    bool isBonked();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
    bool m_bonked;
    std::string m_goodie;
};

class Peach: public actorBase {
public:
    // general pure virtual functions.
    Peach(int startX, int startY, StudentWorld* world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
    
    // end of level functions.
    void toggleReachedFlag();
    bool ifReachedFlag();
    void toggleOverlapMario();
    bool ifReachedMario();
    
    //superpowers
    void setInvincible();
    void toggleShootPower();
    void addHP();
    void toggleJumpPower();
    
    // check for superpowers.
    bool isInvincible();
    bool isShootPower();
    bool isJumpPower();
private:
    int m_lives;
    bool m_jumpPower;
    bool m_shootPower;
    int remaining_jump_distance;
    int recharge;
    
    //invincibility
    int m_invincible;
    bool invincible;
    int m_tempInvincible;
    bool tempInvincible;
    
    // finish level/ game.
    bool reachedFlag;
    bool overLapMario;
    
};

class Pipe: public actorBase {
public:
    Pipe(int startX, int startY, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
};

class Flag: public actorBase {
public:
    Flag(int startX, int startY, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
};

class Mario: public actorBase {
public:
    Mario(int startX, int startY, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
};

class Powerup: public actorBase {
public:
    Powerup(int ID, int startX, int startY, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
    virtual void differentActions() = 0;
};

class Star: public Powerup {
public:
    Star(int ID, int startX, int startY, StudentWorld *world);
    
private:
    void differentActions();
};

class Mushroom: public Powerup {
public:
    Mushroom(int ID, int startX, int startY, StudentWorld *world);
private:
    void differentActions();
};

class Flower: public Powerup {
public:
    Flower(int ID, int startX, int startY, StudentWorld *world);
private:
    void differentActions();
};

class FireBalls: public actorBase {
public:
    FireBalls(int ID,int startX, int startY,int direction, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
    bool isSamePosition();
private:
    virtual void differentFireballActions() = 0;
    bool m_samePosition;
};

class PeachFireBall: public FireBalls {
public:
    PeachFireBall(int ID, int startX, int startY, int direction, StudentWorld *world);
private:
    void differentFireballActions();
};

class PiranhaFireball: public FireBalls {
public:
    PiranhaFireball(int ID, int startX, int startY, int direction, StudentWorld *world);
private:
    void differentFireballActions();
};

// MONSTERS///////////////////////////////////////////////
class Piranha: public actorBase {
public:
    Piranha(int startX, int startY, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
    int m_firingDelay;
};

class Enemy: public actorBase {
public:
    Enemy(int ID, int startX, int startY, bool hasShell, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
    bool m_hasShell;
};

class Shell: public actorBase {
public:
    Shell(int startX, int startY, int direction, StudentWorld *world);
    void doSomething();
    void bonk();
    bool isSolid();
    bool isMonster();
    void damageObject();
private:
};
#endif // ACTOR_H_
