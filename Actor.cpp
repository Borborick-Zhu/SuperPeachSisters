#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
// might be illegal.
int random(int a, int b) {
    int r = rand() % 2;
    if(r == 0)
        return a;
    else
        return b;
}

//ACTORBASE//////////////////////////////////////////////
actorBase::actorBase(int imageID, int startX, int startY, int direction, int depth, int size, StudentWorld* world)
:GraphObject(imageID, startX, startY, direction, depth, size)
{
    currentWorld = world;
    m_alive = true;
}


bool actorBase::isAlive() {
    if (m_alive)
        return true;
    return false;
}

StudentWorld* actorBase::getWorld() const {
    return currentWorld;
}

void actorBase::toggleAlive() {
    if (m_alive)
        m_alive = false;
}

//PEACH//////////////////////////////////////////////////
Peach::Peach(int startX, int startY, StudentWorld* world)
:actorBase(IID_PEACH, startX, startY, 0, 0, 1, world), m_lives(1), m_invincible(0), m_jumpPower(false), m_shootPower(false), remaining_jump_distance(0), reachedFlag(false), overLapMario(false), tempInvincible(false), invincible(false), m_tempInvincible(0), recharge(0)
{}


void Peach::doSomething()
{
    if (!isAlive())
        return;
    
    if (isInvincible()) {
        m_invincible--;
        if (m_invincible == 0) {
            invincible = false;
        }
    }
    
    if (m_tempInvincible > 0 && tempInvincible) {
        m_tempInvincible--;
        if (m_tempInvincible == 0) {
            tempInvincible = false;
        }
    }
 
    if (recharge > 0) {
        recharge--;
    }

    if (remaining_jump_distance > 0) {
        if (!getWorld()->hasObject(getX(),getY() + 4, true, false, false)) {
            moveTo(getX(),getY() + 4);
            remaining_jump_distance--;
        } else {
            remaining_jump_distance = 0;
        }
    }

    if (remaining_jump_distance == 0) {
        if (!getWorld()->hasObject(getX(), getY(), false, false, false) && !getWorld()->hasObject(getX(), getY() - 1, false, false, false) && !getWorld()->hasObject(getX(), getY() - 2, false, false, false) && !getWorld()->hasObject(getX(), getY() - 3, false, false, false)) {
            moveTo(getX(), getY() - 4);
        }
    }
    

    if (isInvincible() && !tempInvincible) {
        getWorld()->hasObject(getX(), getY(), true, false, false);
    }
    
    
    int moveDirection;
    if (getWorld()->getKey(moveDirection)) {
        switch(moveDirection) {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (!getWorld()->hasObject(getX() - 4, getY(), false, false, false))
                    moveTo(getX() - 4, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (!getWorld()->hasObject(getX() + 4, getY(), false, false, false))
                    moveTo(getX() + 4, getY());
                break;
            case KEY_PRESS_UP:
                if (getWorld()->hasObject(getX(), getY() - 1, false, false, false)) {
                    if (!m_jumpPower) {
                        remaining_jump_distance = 8;
                    } else {
                        remaining_jump_distance = 12;
                    }
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            case KEY_PRESS_SPACE:
                if (!m_shootPower) {
                    break;
                }
                if (recharge > 0) {
                    break;
                }
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                if (getDirection() == right) {
                    PeachFireBall* temp = new PeachFireBall(IID_PEACH_FIRE, getX() + 4, getY(),getDirection(), getWorld());
                    getWorld()->addObject(temp);
                } else if (getDirection() == left) {
                    PeachFireBall* temp = new PeachFireBall(IID_PEACH_FIRE, getX() - 4, getY(),getDirection(), getWorld());
                    getWorld()->addObject(temp);
                }
                recharge = 8;
                break;
        }
    }
}

void Peach::bonk() {
    
    if (isInvincible())
        return;
    
    if (tempInvincible == true && m_tempInvincible > 0) {
        return;
    }
    
    m_lives--;
    tempInvincible = true;
    m_tempInvincible = 10;
    
    if (m_shootPower) {
        m_shootPower = false;
    }
    
    if (m_jumpPower) {
        m_jumpPower = false;
    }
    
    if (m_lives >= 1) {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    
    if (m_lives == 0) {
        toggleAlive();
    }
}


bool Peach::isShootPower() {
    if (m_shootPower) {
        return true;
    } else {
        return false;
    }
}

bool Peach::isJumpPower() {
    if(m_jumpPower) {
        return true;
    } else {
        return false;
    }
}

bool Peach::isInvincible() {
    if (m_invincible > 0) {
        return true;
    }
    return false;
}

bool Peach::ifReachedFlag() {
    if (reachedFlag) {
        return true;
    } else {
        return false;
    }
}

void Peach::toggleOverlapMario() {
    if (!overLapMario) {
        overLapMario = true;
    }
}

bool Peach::ifReachedMario() {
    if (overLapMario) {
        return true;
    } else {
        return false;
    }
}

void Peach::toggleReachedFlag() {
    reachedFlag = true;
}

void Peach::setInvincible() {
    m_invincible = 150;
}

void Peach::toggleShootPower() {
    if (!m_shootPower)
        m_shootPower = true;
}

bool Peach::isSolid() {
    return false;
}

void Peach::addHP() {
    m_lives = 2;
}

void Peach::toggleJumpPower() {
    if (!m_jumpPower)
        m_jumpPower = true;
}

bool Peach::isMonster() {
    return false;
}

void Peach::damageObject() {
    bonk();
}

//BLOCKS AND PIPES///////////////////////////////////////
Block::Block(int startX, int startY, StudentWorld *world, string goodie)
:actorBase(IID_BLOCK, startX, startY, 0, 2, 1, world), m_bonked(false)
{
    m_goodie = goodie;
    
}

void Block::doSomething() {}
void Block::bonk() {
    if (m_goodie == "" || m_bonked == true) {
        m_bonked = true;
        getWorld()->playSound(SOUND_PLAYER_BONK);
    } else if (m_goodie == "star" && !m_bonked) {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        Star *temp = new Star(IID_STAR,getX() * SPRITE_WIDTH, getY() * SPRITE_HEIGHT, getWorld());
        getWorld()->addObject(temp);
        temp->moveTo(getX(), getY() + 8);
    } else if (m_goodie == "flower" && !m_bonked) {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        Flower *temp = new Flower(IID_FLOWER,getX() * SPRITE_WIDTH, getY() * SPRITE_HEIGHT, getWorld());
        getWorld()->addObject(temp);
        temp->moveTo(getX(), getY() + 8);
    } else if (m_goodie == "mushroom" && !m_bonked) {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        Mushroom *temp = new Mushroom(IID_MUSHROOM,getX() * SPRITE_WIDTH, getY() * SPRITE_HEIGHT, getWorld());
        getWorld()->addObject(temp);
        temp->moveTo(getX(), getY() + 8);
    }
        m_bonked = true;
}

bool Block::isBonked() {
    if (m_bonked) {
        return true;
    }
    return false;
}

bool Block::isSolid() {
    return true;
}

bool Block::isMonster() {
    return false;
}

void Block::damageObject() {}

Pipe::Pipe(int startX, int startY, StudentWorld* world)
:actorBase(IID_PIPE, startX, startY, 0, 2, 1, world)
{}
void Pipe::bonk() {}
void Pipe::doSomething() {}
bool Pipe::isSolid() {
    return true;
}
bool Pipe::isMonster() {
    return false;
}
void Pipe::damageObject() {}

//LEVEL COMPLETION (FLAG AND MARIO)//////////////////////
Flag::Flag(int startX, int startY, StudentWorld *world)
:actorBase(IID_FLAG, startX, startY, 0, 1, 1, world)
{}
void Flag::bonk() {}
void Flag::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->hasObject(getX(), getY(), false, false, true)) {
        getWorld()->increaseScore(1000);
        getWorld()->getPeach()->toggleReachedFlag();
        toggleAlive();
        
    }
}
bool Flag::isSolid() {
    return false;
}
bool Flag::isMonster() {
    return false;
}
void Flag::damageObject() {}

Mario::Mario(int startX, int startY, StudentWorld *world)
:actorBase(IID_MARIO, startX, startY, 0, 1, 1, world)
{}
void Mario::bonk() {}
void Mario::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->hasObject(getX(), getY(), false, false, true)) {
        getWorld()->increaseScore(1000);
        getWorld()->getPeach()->toggleOverlapMario();
        toggleAlive();
    }
}
bool Mario::isSolid() {
    return false;
}
bool Mario::isMonster() {
    return false;
}
void Mario::damageObject() {}

//POWERUPS//////////////////////////////////////////////
Powerup::Powerup(int ID, int startX, int startY, StudentWorld *world)
:actorBase(ID, startX, startY, 0, 1, 1, world)
{}

void Powerup::doSomething() {
    
    if (getWorld()->hasObject(getX(), getY(), false, false, true)) {
        differentActions();
        toggleAlive();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    
    if (!getWorld()->hasObject(getX(), getY() - 2, false, false, false)) {
        moveTo(getX(), getY() - 2);
    }
    
    //move in direction if there are no blocks.
    if (getDirection() == left) {
        if (getWorld()->hasObject(getX() - 2, getY(), false, false, false)) {
            if (getDirection() == right)
                setDirection(left);
            else
                setDirection(right);
            return;
        }
        moveTo(getX() - 2, getY());
    } else if (getDirection() == right) {
        if (getWorld()->hasObject(getX() + 2, getY(), false, false, false)) {
            if (getDirection() == right)
                setDirection(left);
            else
                setDirection(right);
            return;
        }
        moveTo(getX() + 2, getY());
    }
}

void Powerup::bonk() {}
void Powerup::damageObject() {}
bool Powerup::isSolid() {return false;}
bool Powerup::isMonster() {return false;}

Star::Star(int ID, int startX, int startY, StudentWorld *world)
:Powerup(ID, startX, startY, world) {
    
}

void Star::differentActions() {
    getWorld()->increaseScore(100);
    getWorld()->getPeach()->setInvincible();
}

Mushroom::Mushroom(int ID, int startX, int startY, StudentWorld *world)
:Powerup(ID, startX, startY, world) {
    
}

void Mushroom::differentActions() {
    getWorld()->increaseScore(75);
    getWorld()->getPeach()->toggleJumpPower();
    getWorld()->getPeach()->addHP();
}

Flower::Flower(int ID, int startX, int startY, StudentWorld *world)
:Powerup(ID, startX, startY, world) {
}


void Flower::differentActions() {
    getWorld()->increaseScore(50);
    getWorld()->getPeach()->toggleShootPower();
    getWorld()->getPeach()->addHP();
}

//FIREBALLS///////////////////////////////////////////////
FireBalls::FireBalls(int ID, int startX, int startY, int direction, StudentWorld *world)
:actorBase(ID, startX, startY, direction,1,1, world)
{
    m_samePosition = true;
}

void FireBalls::doSomething() {
    
    differentFireballActions();
    
    if (!getWorld()->hasObject(getX(), getY() - 2, false, false, false)) {
        moveTo(getX(), getY() - 2);
        m_samePosition  = false;
    }
    
    if (getDirection() == right) {
        if (getWorld()->hasObject(getX() + 2, getY(), false, false, false)) {
            toggleAlive();
            return;
        }
        moveTo(getX() + 2, getY());
        m_samePosition  = false;
    } else if (getDirection() == left) {
        if (getWorld()->hasObject(getX() - 2, getY(), false, false, false)) {
            toggleAlive();
            return;
        }
        moveTo(getX() - 2, getY());
        m_samePosition  = false;
    }
}

void FireBalls::bonk() {}
bool FireBalls::isSolid() {
    return false;
}
bool FireBalls::isMonster() {
    return false;
}

bool FireBalls::isSamePosition() {
    if (m_samePosition) {
        return true;
    } else {
        return false;
    }
}

void FireBalls::damageObject() {}

PeachFireBall::PeachFireBall(int ID, int startX, int startY, int direction, StudentWorld *world)
:FireBalls(ID, startX, startY, direction, world)
{}

void PeachFireBall::differentFireballActions() {
    if (getWorld()->hasObject(getX(), getY(), false, true, false)) {
        toggleAlive();
        return;
    }
}

void PiranhaFireball::differentFireballActions() {
    if(!isSamePosition() && getWorld()->hasObject(getX(), getY(), false, false, true)) {
        getWorld()->getPeach()->damageObject();
        toggleAlive();
        return;
    }
}

PiranhaFireball::PiranhaFireball(int ID, int startX, int startY, int direction, StudentWorld *world)
:FireBalls(ID, startX, startY, direction, world)
{}
//MONSTERS//////////////////////////////////////////////////////////////////////
Enemy::Enemy(int ID, int startX, int startY, bool hasShell, StudentWorld *world)
:actorBase(ID, startX, startY, randInt(0,1) * 180, 1, 0, world)
{
    m_hasShell = hasShell;
}
void Enemy::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->hasObject(getX(), getY(), false, false, true)) {
        getWorld()->getPeach()->damageObject();
        return;
    }
    
    if (getDirection() == right)  {
        if (!getWorld()->hasObject(getX() + SPRITE_WIDTH, getY() - SPRITE_WIDTH, false, false, false)) {
            setDirection(left);
        }
        
        if (getWorld()->hasObject(getX() + 1, getY(), false, false, false)) {
            setDirection(left);
        }
        else {
            moveTo(getX() + 1, getY());
        }
    }
    
    if (getDirection() == left) {
        
        if (!getWorld()->hasObject(getX() - SPRITE_WIDTH, getY() - SPRITE_WIDTH, false, false, false)) {
            setDirection(right);
        }
        
        if (getWorld()->hasObject(getX() - 1, getY(), false, false, false)) {
            setDirection(right);
        }
        else {
            moveTo(getX() - 1, getY());
        }
    }
}
void Enemy::bonk() {
    if (getWorld()->getPeach()->isInvincible()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        toggleAlive();
        if (m_hasShell) {
            Shell *temp = new Shell(getX(), getY(), getDirection(),getWorld());
            getWorld()->addObject(temp);
        }
        
    }
}
bool Enemy::isSolid() {return false;}
bool Enemy::isMonster() {return true;}
void Enemy::damageObject() {
    getWorld()->increaseScore(100);
    toggleAlive();
    if (m_hasShell) {
        Shell *temp = new Shell(getX(), getY(), getDirection(),getWorld());
        getWorld()->addObject(temp);
    }
}

Piranha::Piranha(int startX, int startY, StudentWorld *world)
:actorBase(IID_PIRANHA, startX, startY, randInt(0,1) * 180, 1, 0, world)
{
    m_firingDelay = 0;
}



void Piranha::doSomething() {
    if (!isAlive())
        return;
    increaseAnimationNumber();
    if (getWorld()->hasObject(getX(), getY(), false, false, true)) {
        getWorld()->getPeach()->bonk();
        return;
    }
    
    if (!(getWorld()->getPeach()->getY() <= getY() + 1.5*SPRITE_HEIGHT) || !(getWorld()->getPeach()->getY() >= getY() - 1.5*SPRITE_HEIGHT)) {
        return;
    }
    
    if (getWorld()->getPeach()->getX() > getX()) {
        //peach is to the right.
        setDirection(right);
    } else if (getWorld()->getPeach()->getX() < getX()) {
        setDirection(left);
    }
    
    if (m_firingDelay > 0) {
        m_firingDelay--;
        return;
    } else {
        if ((getWorld()->getPeach()->getX() - getX() < 0 && -1 * (getWorld()->getPeach()->getX() - getX()) < 8 * SPRITE_WIDTH) || (getWorld()->getPeach()->getX() - getX() >= 0 && getWorld()->getPeach()->getX() - getX() < 8 * SPRITE_WIDTH)) {
            
            PiranhaFireball* temp = new PiranhaFireball(IID_PIRANHA_FIRE, getX(), getY(), getDirection(), getWorld());
            getWorld()->addObject(temp);
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            m_firingDelay = 40;
        }
    }
}

void Piranha::bonk() {
    if(getWorld()->getPeach()->isInvincible()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        toggleAlive();
    }
}

bool Piranha::isSolid() {return false;}
bool Piranha::isMonster() {return true;}
void Piranha::damageObject() {
    getWorld()->increaseScore(100);
    toggleAlive();
}

Shell::Shell(int startX, int startY, int direction, StudentWorld *world)
:actorBase(IID_SHELL, startX, startY,direction, 1, 1, world)
{}

void Shell::doSomething() {
    if (getWorld()->hasObject(getX(), getY(), false, true, false)) {
        toggleAlive();
        return;
    }
    
    if (!getWorld()->hasObject(getX(), getY() - 2, false, false, false)) {
        moveTo(getX(), getY() - 2);
    }
    
    if (getDirection() == right) {
        if (getWorld()->hasObject(getX() + 2, getY(), false, false, false)) {
            toggleAlive();
            return;
        }
        moveTo(getX() + 2, getY());
    }
    
    if (getDirection() == left) {
        if (getWorld()->hasObject(getX() - 2, getY(), false, false, false)) {
            toggleAlive();
            return;
        }
        moveTo(getX() - 2, getY());
    }
}

void Shell::bonk() {}
bool Shell::isSolid() {return false;}
bool Shell::isMonster() {return false;}
void Shell::damageObject() {}





