#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;

class StudentWorld;

// *************** Actor Declaration ***********************
class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void doSomething() = 0;
    virtual bool holdsGoodie() = 0;
    virtual bool isDamageable() = 0;
    virtual void bonk() = 0;
    virtual void getDamaged() = 0;
    virtual bool hasStarPower(); // check for Peach's star power
    void setAlive(bool status);
    StudentWorld* getWorld();
    bool isAlive();
    bool isOverlap(Actor *a, Actor*b);
    bool checkSupport(Actor*a);
private:
    bool m_alive;
    StudentWorld *m_world;
};

// *************** Peach Declaration ***********************

class Peach : public Actor {
public:
    Peach(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world, int hitpoints);
    virtual void doSomething();
    void setJumpDistance(int value);
    void addPower(int type, bool status); // 0 for star, 1 for jump, 2 for shoot
    void setHitpoints(int val);
    virtual void bonk();
    virtual bool isDamageable();
    virtual bool holdsGoodie();
    void setRecharge(int val);
    virtual void getDamaged();
    virtual bool hasStarPower();
    bool hasJumpPower();
    bool hasShootPower();
private:
    int m_hitpoints;
    int m_invincible; // controls ticks for full invincibility
    bool m_starPower; // true when she has full invincibility
    int m_tempInvincible; // controls ticks for temp invincibility
    bool m_jumpPower;
    int m_jump_distance;
    bool m_shootPower;
    int m_recharge;
};

// *************** Enemy Declaration ***********************
class Enemy: public Actor {
public:
    Enemy(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void doSomething();
    virtual bool isDamageable();
    virtual void bonk(); // not used in enemy class
    virtual void bonk(Actor *a) = 0; // special overloaded bonk() function for Enemy objects
    virtual bool holdsGoodie();
};



// *************** Goomba Declaration ***********************
class Goomba: public Enemy {
public:
    Goomba(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void bonk(Actor *a);
    virtual void getDamaged();
};


// *************** Koopa Declaration ***********************
class Koopa: public Enemy {
public:
    Koopa(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void bonk(Actor *a);
    virtual void getDamaged();
};



// *************** Piranha Declaration ***********************
class Piranha: public Enemy {
public:
    Piranha(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void doSomething();
    virtual void bonk(Actor *a);
    virtual void getDamaged();
private:
    int m_firingDelay;
};


// *************** Goodie Declaration ***********************
class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void bonk();
    virtual void doSomething();
    virtual bool isDamageable();
    virtual void getDamaged();
    virtual bool holdsGoodie();
private:
    virtual void differentiation() = 0;
};

// *************** Star Declaration ***********************
class Star : public Goodie {
public:
    Star(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual void differentiation();
};

// *************** Mushroom Declaration ***********************
class Mushroom : public Goodie {
public:
    Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual void differentiation();
};

// *************** Flower Declaration ***********************
class Flower : public Goodie {
public:
    Flower(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual void differentiation();
};


// *************** Landscape Declaration ***********************
class Landscape : public Actor {
public:
    Landscape(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual bool isDamageable();
    virtual void getDamaged();
    virtual void doSomething();
    virtual void bonk();
    virtual bool holdsGoodie();
};


// *************** Pipe Declaration ***********************
class Pipe : public Landscape {
public:
    Pipe(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
};


// *************** Block Declaration ***********************

class Block : public Landscape {
public:
    Block(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world, int goodie);
    virtual void bonk();

private:
    int m_goodie; // 0 is regular block, 1 is star goodie, 2 is mushroom, 3 is flower
};

// *************** Endgame Declaration ***********************
class Endgame : public Actor {
public:
    Endgame(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void bonk();
    virtual void doSomething();
    virtual bool isDamageable();
    virtual void getDamaged();
    virtual bool holdsGoodie();
private:
    virtual void end() = 0;
};

// *************** Mario Declaration ***********************

class Mario : public Endgame {
public:
    Mario(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual void end();
};

// *************** Flag Declaration ***********************

class Flag : public Endgame {
public:
    Flag(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual void end();
};

// *************** Projectile Declaration ***********************
class Projectile : public Actor {
public:
    Projectile(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void doSomething();
    virtual void bonk();
    virtual bool isProjectile();
    virtual bool isDamageable();
    virtual bool holdsGoodie();
    virtual void getDamaged();
private:
    virtual bool differentiation() = 0;
};

// *************** Piranha Fireball Declaration ***********************
class PiranhaFireball : public Projectile {
public:
    PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual bool differentiation();
};

// *************** Peach Fireball Declaration ***********************
class PeachFireball : public Projectile {
public:
    PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
private:
    virtual bool differentiation();
};

// *************** Koopa Shell Declaration ***********************
class Shell : public Projectile {
public:
    Shell(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world);
    virtual void doSomething();
private:
    virtual bool differentiation();
};



#endif // ACTOR_H_
