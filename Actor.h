#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// =========== BASE CLASS =========== //
// Parent class for all game objects.
class BaseClass : public GraphObject {
public:
    BaseClass(int imgID, int startX, int startY, StudentWorld* world, int dir = right, double size = 1.0, int depth = 0);
    virtual ~BaseClass(); // Virtual destructor

    virtual void doSomething() = 0;
    bool isAlive() const { return m_alive; } // Checks if the object is still in play
    void setDead() { m_alive = false; } // Marks object for removal
    StudentWorld* getWorld() const { return m_world; } // Provides access to the game world

    virtual bool isWalkable() const { return false; } // Determines if an object can be walked on

    // Virtual methods for enemies and interactions
    virtual bool isEnemy() const { return false; } // Determines if the object is an enemy
    virtual void interactWithPlayer() {} // Allows enemies to interact with the player

protected:
    bool m_alive; // Tracks if the object is active
    StudentWorld* m_world; // Reference to the game world
};

// =========== PLAYER CLASS =========== //
class Player : public BaseClass {
public:
    Player(int startX, int startY, StudentWorld* world);
    void doSomething() override; // Handles player input and movement
    void jump(); // Handles jumping
    void fall(); // Handles falling due to gravity
    bool isOnFloor() const; // Checks if the player is standing on solid ground

    // New abilities
    void burp(); // Allows the player to eliminate nearby enemies
    void addBurp() { m_burpCount++; } // Increases the number of burps available
    void freeze(int ticks); // Freezes the player for a duration
    void incrementLives(); //  Correctly declared

    int getLives() const { return m_lives; }  // Returns player lives
    int getBurpCount() const { return m_burpCount; } // Returns burp count

private:
    int m_lives;  // Number of lives remaining
    bool m_jumping; // Tells us if the player is jumping
    int jumpHeight; // Tracks jump progression
    int m_burpCount; // Tracks available burps
    int m_frozenTicks; // Countdown timer for freeze effect
};

// =========== ENEMY CLASSES =========== //
// Koopa: Moves back and forth and freezes the player upon collision
class Koopa : public BaseClass {
public:
    Koopa(int startX, int startY, StudentWorld* world);
    void doSomething() override;
    bool isEnemy() const override { return true; }
    void interactWithPlayer() override; // Causes the player to freeze

private:
    int movementCooldown; //Conrrols moevement speed (every 10 ticks)
};

// =========== FLOOR CLASS =========== //
class Floor : public BaseClass {
public:
    Floor(int startX, int startY, StudentWorld* world);
    void doSomething() override {} // Floors do nothing each tick
    bool isWalkable() const override { return true; } // Can be walked on
};

// =========== FIREBALL CLASS =========== //
class Fireball : public BaseClass {
public:
    Fireball(int startX, int startY, StudentWorld* world, int dir); //  Added direction parameter
    void doSomething() override;
    bool isEnemy() const override { return true; }

private:
    int m_direction; //  Stores the direction the fireball moves
    int m_lifeSpan;
};


// =========== SPECIAL OBJECTS =========== //
// Bonfire: Burns the player if they touch it
class Bonfire : public BaseClass {
public:
    Bonfire(int startX, int startY, StudentWorld* world);
    void doSomething() override;
};

// Power-ups: Extra life and Garlic Goodie
class Goodie : public BaseClass {
public:
    Goodie(int startX, int startY, StudentWorld* world, int imageID, int scoreValue);
    virtual ~Goodie() {}  // Ensure Goodie has a defined destructor
    virtual void collect(Player* player) = 0;
    void doSomething() override;
protected:
    int m_scoreValue; // Score bonus for collecting this item
};

// Extra Life Goodie: Increases the player's lives
class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld* world);
    void collect(Player* player) override;
};

// Garlic Goodie: Gives the player extra burping ability
class GarlicGoodie : public Goodie {
public:
    GarlicGoodie(int startX, int startY, StudentWorld* world);
    void collect(Player* player) override;
};


// =========== KONG CLASS =========== //
class Kong : public BaseClass {
public:
    Kong(int startX, int startY, StudentWorld* world);
    void doSomething() override;
private:
    int m_throwCooldown;
};

// =========== BARREL CLASS =========== //
class Barrel : public BaseClass {
public:
    Barrel(int startX, int startY, StudentWorld* world, int dir);
    void doSomething() override;
private:
    int m_direction;
};

// =========== BURP CLASS ===========
class Burp : public BaseClass {
public:
    Burp(int startX, int startY, StudentWorld* world);
    void doSomething() override;
private:
    int m_lifeSpan = 10; //  an appropriate lifespan
};

// =========== LADDER CLASS ===========
class Ladder : public BaseClass {
public:
    Ladder(int startX, int startY, StudentWorld* world);
    void doSomething() override {} // Ladders do nothing each tick
    bool isWalkable() const override { return true; } // Ladders are walkable
};


#endif // ACTOR_H_
