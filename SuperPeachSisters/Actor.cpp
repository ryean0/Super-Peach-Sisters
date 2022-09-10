#include "Actor.h"
#include "StudentWorld.h"


// ****************** ACTOR Implementation ********************

Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: GraphObject(imageID, startX, startY, dir, depth, size), m_alive(alive), m_world(world) {
    ;
}
StudentWorld* Actor::getWorld() {
    return m_world;
}

void Actor::setAlive(bool status) {
    m_alive = status;
    return;
}

bool Actor::isAlive() {
    return m_alive;
}

bool Actor::isOverlap(Actor *a, Actor *b) {
    if (((a->getX()+SPRITE_WIDTH-1 >= b->getX() && a->getX()+SPRITE_WIDTH-1 <= b->getX() + SPRITE_WIDTH -1) && (a->getY()+SPRITE_HEIGHT-1 >= b->getY() && a->getY()+SPRITE_HEIGHT-1 <= b->getY() + SPRITE_HEIGHT -1)) || ((b->getX()+SPRITE_WIDTH-1 >= a->getX() && b->getX()+SPRITE_WIDTH-1 <= a->getX() + SPRITE_WIDTH -1) && (b->getY()+SPRITE_HEIGHT-1 >= a->getY() && b->getY()+SPRITE_HEIGHT-1 <= a->getY() + SPRITE_HEIGHT -1))) {
        return true;
    } else {
        return false;
    }
}

bool Actor::checkSupport(Actor *a) {
    // check getX()+-4 getY()-4 based on direction
    if (a->getDirection() == 0) {
        // check getX()+
        return getWorld()->isBlockingObjectAt(getX()+8, getY()-8);
    }
    if (a->getDirection() == 180) {
        // check getX()-
        return getWorld()->isBlockingObjectAt(getX()-8, getY()-8);
    }
    return true;
}

bool Actor::hasStarPower() {
    return false;
}

// ****************** PEACH Implementation ********************

Peach::Peach(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world, int hitpoints)
: Actor(imageID, startX, startY, dir, depth, size, alive, world), m_hitpoints(hitpoints), m_jump_distance(0) {
    m_invincible = 0;
    m_starPower = false;
    m_jumpPower = false;
    m_shootPower = false;
    m_recharge = 0;
    m_tempInvincible = 0;
}

bool Peach::holdsGoodie() {
    return false;
}

void Peach::setJumpDistance(int value) {
    m_jump_distance = value;
}

void Peach::setHitpoints(int val) {
    m_hitpoints = val;
}

void Peach::bonk() {
    // can be bonked by enemies that overlaps with her
    if (m_starPower || m_tempInvincible > 0) {
        return;
    }
    setHitpoints(m_hitpoints-1);
    // set temporary invincibility
    m_tempInvincible = 10;
    // turn off power
    addPower(1, false);
    addPower(2, false);
    if (m_hitpoints >= 1) {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    } else {
        setAlive(false);
    }
    return;
}

void Peach::getDamaged() {
    Peach::bonk();
}

bool Peach::isDamageable() {
    return true;
}

bool Peach::hasStarPower() {
    return m_starPower;
}

bool Peach::hasJumpPower() {
    return m_jumpPower;
}

bool Peach::hasShootPower() {
    return m_shootPower;
}

void Peach::doSomething() {

    // check if Peach is currently alive
    if (!isAlive()) {
        return;
    }
    // check if Peach is currently invincible
    if (m_starPower) {
        m_invincible -= 1;
        if (m_invincible == 0) {
            addPower(0, false);
        }
    }
    // check if Peach is temporarily invincible
    if (m_tempInvincible > 0) {
        m_tempInvincible -= 1;
    }
    // Check if Peach is in recharge mode
    if (m_recharge > 0) {
        setRecharge(m_recharge-1);
    }
    // Check for overlap with other actors and call bonk() method
    Actor *a = getWorld()->getBlockingObjectAt(getX(), getY());
    if (a != nullptr) {
        if (!a->holdsGoodie()) {
            a->bonk();
        }
        if (a->isDamageable()) {
            // is an enemy -> we are sure that only peach calls this
            ;
        }
    }
    // Jump
    if (m_jump_distance > 0) {
        int new_y = getY() + 4;
        if (getWorld()->isBlockingObjectAt(getX(), new_y+4)) {
            // bonk blocking object
            Actor *blocking = getWorld()->getBlockingObjectAt(getX(), new_y);
            if (blocking != nullptr) { blocking->bonk();}
            setJumpDistance(0);
        } else {
            moveTo(getX(), new_y);
            setJumpDistance(m_jump_distance-1);
        }
    } else if (m_jump_distance == 0) {
        // Peach is falling
        if (!getWorld()->isBlockingObjectAt(getX(), getY()-4)) {
            moveTo(getX(), getY()-4);
        }
    }

    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT: {
                // move peach left
                setDirection(180);
                int new_x = getX() - 4;
                if (getWorld()->isBlockingObjectAt(new_x, getY())) {
                    break;
                } else {
                    moveTo(new_x, getY());
                }
                break;
            }
                
            case KEY_PRESS_RIGHT: {
                // move peach right
                setDirection(0);
                int new_x = getX() + 4;
                if (getWorld()->isBlockingObjectAt(new_x, getY())) {
                    break;
                } else {
                    moveTo(new_x, getY());
                }
                break;
            }
                
            case KEY_PRESS_UP: {
                if (getWorld()->isBlockingObjectAt(getX(), getY()-2)) {
                    if (m_jumpPower) {
                        setJumpDistance(12);
                    } else {
                        setJumpDistance(8);
                    }
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            }
                
            case KEY_PRESS_DOWN: {
                // move peach right
                setDirection(0);
                int new_x = getX() + 4;
                if (getWorld()->isBlockingObjectAt(new_x + 4, getY())) {
                    break;
                } else {
                    moveTo(new_x, getY());
                }
                break;
            }
                
            case KEY_PRESS_SPACE: {
                // check for shoot power
                if (m_shootPower) {
                    if (m_recharge == 0) {
                        getWorld()->playSound(SOUND_PLAYER_FIRE);
                        setRecharge(8);
                        // create new fireball
                        if (getDirection() == 0) {
                            PeachFireball *fireball = new PeachFireball(IID_PEACH_FIRE, getX()+4, getY(), 0, 1, 1.0, true, getWorld());
                            getWorld()->addActor(fireball);
                        } else if (getDirection() == 180) {
                            PeachFireball *fireball = new PeachFireball(IID_PEACH_FIRE, getX()-4, getY(), 180, 1, 1.0, true, getWorld());
                            getWorld()->addActor(fireball);
                        }
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
                
            default:
                break;
        }
    }
    return;
}

void Peach::setRecharge(int val) {
    m_recharge = val;
}

void Peach::addPower(int type, bool status) {
    switch (type) {
        case 0: {
            m_starPower = status;
            m_invincible = 150;
        }
        case 1: {
            m_jumpPower = status;
        }
        case 2: {
            m_shootPower = status;
        }
    }
}

// ****************** ENEMY Implementation ********************
Enemy::Enemy(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

bool Enemy::isDamageable() {
    if (getWorld()->getPeach()->hasStarPower()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getDamaged();
    }
    return true;
}

bool Enemy::holdsGoodie() {
    return false;
}

void Enemy::doSomething() {
    if (!isAlive()) { return; }
    if (isOverlap(getWorld()->getPeach(), this)) {
        getWorld()->getPeach()->bonk(); // bonk peach
        return;
    }
    if (getDirection() == 0) {
        if (getWorld()->isBlockingObjectAt(getX()+2, getY()) || !checkSupport(this)) {
            setDirection(180);
        } else {
            moveTo(getX()+1, getY());
        }
    } else if (getDirection() == 180) {
        if (getWorld()->isBlockingObjectAt(getX()-2, getY()) || !checkSupport(this)) {
            setDirection(0);
        } else {
            moveTo(getX()-1, getY());
        }
    }
}

void Enemy::bonk() {
    return;
}

// ****************** GOOMBA Implementation ********************
Goomba::Goomba(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Enemy(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Goomba::bonk(Actor *a) {
    if (a != getWorld()->getPeach()) {
        return;
    } else {
        // a is peach
        if (a->hasStarPower()) {
            getWorld()->playSound(SOUND_PLAYER_KICK);
            getDamaged();
        }
    }
}

void Goomba::getDamaged() {
    // increase score
    getWorld()->increaseScore(100);
    setAlive(false);
}



// ****************** KOOPA Implementation ********************
Koopa::Koopa(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Enemy(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Koopa::bonk(Actor *a) {
    if (a != getWorld()->getPeach()) {
        return;
    } else {
        // a is peach
        if (a->hasStarPower()) {
            getWorld()->playSound(SOUND_PLAYER_KICK);
            getWorld()->increaseScore(100);
            setAlive(false);
            Shell *myShell = new Shell(IID_SHELL, getX(), getY(), getDirection(), 1, 1.0, true, getWorld());
            getWorld()->addActor(myShell);
        }
    }
}

void Koopa::getDamaged() {
    getWorld()->increaseScore(100);
    setAlive(false);
    Shell *myShell = new Shell(IID_SHELL, getX(), getY(), getDirection(), 1, 1.0, true, getWorld());
    getWorld()->addActor(myShell);
}



// ****************** PIRANHA Implementation ********************
Piranha::Piranha(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Enemy(imageID, startX, startY, dir, depth, size, alive, world), m_firingDelay(0) {
    ;
}

void Piranha::getDamaged() {
    getWorld()->increaseScore(100);
    setAlive(false);
}

void Piranha::bonk(Actor *a) {
    return;
}

void Piranha::doSomething() {
    if (!isAlive()) {
        return;
    }
    increaseAnimationNumber();
    if (isOverlap(getWorld()->getPeach(), this)) {
        getWorld()->getPeach()->bonk(); // bonk peach
        return;
    }
    if (abs(getY() - getWorld()->getPeach()->getY()) <= 1.5*SPRITE_HEIGHT) {
        // check if peach is on left or right
        if (getX() - getWorld()->getPeach()->getX() < 0) {
            // peach is on right
            setDirection(0);
        } else {
            setDirection(180);
        }
        if (m_firingDelay == 0) {
            // no firing delay, check distance and shoot
            if (abs(getX() - getWorld()->getPeach()->getX()) <= 8*SPRITE_WIDTH) {
                // add new Piranha fireball
                PiranhaFireball* pFireball = new PiranhaFireball(IID_PIRANHA_FIRE, getX(), getY(), getDirection(), 1, 1.0, true, getWorld());
                getWorld()->addActor(pFireball);
                getWorld()->playSound(SOUND_PIRANHA_FIRE);
                m_firingDelay = 40;
            }
        } else {
            m_firingDelay -= 1;
            return;
        }
    } else {
        return;
    }
}

// ****************** GOODIE Implementation ********************
Goodie::Goodie(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}
bool Goodie::holdsGoodie() {
    return false;
}

void Goodie::bonk() {
    return;
}

void Goodie::getDamaged() {
    return;
}


void Goodie::doSomething() {
    Actor*peach = getWorld()->getPeach();
    if (isOverlap(peach, this)) {
        differentiation();
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    // fall if not blocked
    if (!getWorld()->isBlockingObjectAt(getX(), getY()-2)) {
        moveTo(getX(), getY()-2);
    }
    // move
    if (getDirection() == 0) {
        if (getWorld()->isBlockingObjectAt(getX()+1, getY())) {
            setDirection(180);
        } else {
            moveTo(getX()+2, getY());
        }
    } else if (getDirection() == 180) {
        if (getWorld()->isBlockingObjectAt(getX()-1, getY())) {
            setDirection(0);
        } else {
            moveTo(getX()-2, getY());
        }
    }
    return;
    }

bool Goodie::isDamageable() {
    return false;
}

// ****************** STAR Implementation ********************
Star::Star(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Goodie(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}
void Star::differentiation() {
    getWorld()->increaseScore(100); // increase score
    getWorld()->getPeach()->addPower(0, true);
    return;
}

// ****************** MUSHROOM Implementation ********************
Mushroom::Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Goodie(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Mushroom::differentiation() {
    getWorld()->increaseScore(75); // increase score
    getWorld()->getPeach()->addPower(1, true);
    getWorld()->getPeach()->setHitpoints(2);
    return;
}


// ****************** FLOWER Implementation ********************
Flower::Flower(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Goodie(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Flower::differentiation() {
    getWorld()->increaseScore(50); // increase score
    getWorld()->getPeach()->addPower(2, true);
    getWorld()->getPeach()->setHitpoints(2);
    return;
}


// ****************** LANDSCAPE Implementation ********************
Landscape::Landscape(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

bool Landscape::holdsGoodie() {
    return true;
}

void Landscape::doSomething() {
    return;
}

void Landscape::bonk() {
    return;
}
bool Landscape::isDamageable() {
    return false;
}
void Landscape::getDamaged() {
    return;
}

// ****************** PIPE Implementation ********************
Pipe::Pipe(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Landscape(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

// ****************** BLOCK Implementation ********************
Block::Block(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world, int goodie)
: Landscape(imageID, startX, startY, dir, depth, size, alive, world), m_goodie(goodie) {
    ;
}

void Block::bonk() {
    if (m_goodie == 0) {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    } else {
        // block contains an unreleased goodie
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        // TODO: introduce new goodie object
        if (m_goodie == 1) {
            // star goodie
            Star *starGoodie = new Star(IID_STAR, getX(), getY()+8, 0, 1, 1.0, true, getWorld());
            getWorld()->addActor(starGoodie);
        } else if (m_goodie == 2) {
            // mushroom goodie
            Mushroom *mushroomGoodie = new Mushroom(IID_MUSHROOM, getX(), getY()+8, 0, 1, 1.0, true, getWorld());
            getWorld()->addActor(mushroomGoodie);
        } else if (m_goodie == 3) {
            // flower goodie
            Flower *starGoodie = new Flower(IID_FLOWER, getX(), getY()+8, 0, 1, 1.0, true, getWorld());
            getWorld()->addActor(starGoodie);
        }
        m_goodie = 0;
    }
}

// ****************** ENDGAME Implementation ********************
Endgame::Endgame(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Endgame::bonk() {
    return;
}

void Endgame::doSomething() {
    Actor*peach = getWorld()->getPeach();
    if (!isAlive()) { return; }
    if (isOverlap(peach, this)) {
        getWorld()->increaseScore(1000);
        setAlive(false);
        end();
    }
}

bool Endgame::isDamageable() {
    return false;
}

bool Endgame::holdsGoodie() {
    return false;
}
void Endgame::getDamaged() {
    return;
}

// ****************** MARIO Implementation ********************
Mario::Mario(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Endgame(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Mario::end() {
    getWorld()->setGameOver();
}

// ****************** FLAG Implementation ********************

Flag::Flag(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Endgame(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Flag::end() {
    getWorld()->setLevelOver();
}

// ****************** PROJECTILE Implementation ********************
Projectile::Projectile(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Projectile::bonk() {
    return;
}

bool Projectile::isProjectile() {
    return true;
}

bool Projectile::holdsGoodie() {
    return false;
}

void Projectile::doSomething() {
    if (differentiation()) { return; }
    // common falling
    if (!getWorld()->isBlockingObjectAt(getX(), getY()-2)) {
        moveTo(getX(), getY()-2);
    }
    // common movement
    if (getDirection() == 0) {
        if (getWorld()->isBlockingObjectAt(getX()+2, getY())) {
            setAlive(false);
            return;
        } else {
            moveTo(getX()+2, getY());
        }
    } else if (getDirection() == 180) {
        if (getWorld()->isBlockingObjectAt(getX()-2, getY())) {
            setAlive(false);
            return;
        } else {
            moveTo(getX()-2, getY());
        }
    }
}

bool Projectile::isDamageable() {
    return false;
}
void Projectile::getDamaged() {
    return;
}


// ****************** PIRANHA FIREBALL Implementation ********************
PiranhaFireball::PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Projectile(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

bool PiranhaFireball::differentiation() {
    // check if it overlaps with peach
    if (isOverlap(getWorld()->getPeach(), this)) {
        // inform peach of damage
        getWorld()->getPeach()->getDamaged();
        setAlive(false);
        return true;
    } else {
        return false;
    }
}

// ****************** PEACH FIREBALL Implementation ********************
PeachFireball::PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Projectile(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

bool PeachFireball::differentiation() {
    Actor*a = nullptr;
    a = getWorld()->getBlockingObjectAt(getX(), getY());
    if (a != nullptr && a->isDamageable() && a != getWorld()->getPeach()) {
        a->getDamaged();
        setAlive(false);
        return true;
    } else {
        return false;
    }
}

// ****************** KOOPA SHELL Implementation ********************
Shell::Shell(int imageID, int startX, int startY, int dir, int depth, double size, bool alive, StudentWorld *world)
: Projectile(imageID, startX, startY, dir, depth, size, alive, world) {
    ;
}

void Shell::doSomething() {
    // check if shell overlaps with a damageable object
    Actor *a = getWorld()->getBlockingObjectAt(getX(), getY());
    if (a != nullptr && a != getWorld()->getPeach() && a->isDamageable()) {
        a->getDamaged();
        setAlive(false);
        return;
    }

    if (!getWorld()->isBlockingObjectAt(getX(), getY()-2)) {
        moveTo(getX(), getY()-2);
    }
    // common movement
    if (getDirection() == 0) {
        if (getWorld()->isBlockingObjectAt(getX()+2, getY())) {
            setAlive(false);
            return;
        } else {
            moveTo(getX()+2, getY());
        }
    } else if (getDirection() == 180) {
        if (getWorld()->isBlockingObjectAt(getX()-2, getY())) {
            setAlive(false);
            return;
        } else {
            moveTo(getX()-2, getY());
        }
    }
}

bool Shell::differentiation() {
    return true;
}
