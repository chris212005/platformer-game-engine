#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>

class Actor;
class Player;

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath); // Constructor
    virtual ~StudentWorld(); // Destructor

    virtual int init(); // Initializes the game world
    virtual int move(); // Updates game objects each tick
    virtual void cleanUp(); // Cleans up memory after a level ends

    bool getKey(int& key) { return GameWorld::getKey(key); } // Retrieves player input

    bool canMoveTo(int x, int y) const; // Checks if a position is walkable
    bool isFloorAt(int x, int y) const; // Checks if there is a floor at (x, y)
    bool isLadderAt(int x, int y) const; // Checks if there is a ladder at (x, y)

    void destroyEnemiesNear(int x, int y); // Eliminates enemies near a position
    Player* getPlayer() const { return player; } // Returns a pointer to the player
    void increasePlayerLives();  // Increases player lives
    void addActor(BaseClass* actor); // Adds an actor to the world
    void removeDeadActors(); // Cleans up dead actors from the world

    const std::vector<BaseClass*>& getActors() const { return actors; }

    bool isEnemyAt(int x, int y) const {
        for (const auto* actor : actors) {
            if (actor->isEnemy() && actor->getX() == x && actor->getY() == y) {
                return true;
            }
        }
        return false;
    }

    bool isWallAt(int x, int y) const;



private:
    std::vector<BaseClass*> actors; // Stores all game objects
    Player* player; // Pointer to the player object
};

// Implemented `createStudentWorld()`
GameWorld* createStudentWorld(std::string assetPath);

#endif // STUDENTWORLD_H_
