#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"

// =========== CONSTRUCTOR =========== //
// Initializes StudentWorld, setting the asset path and ensuring no player is created yet.
StudentWorld::StudentWorld(std::string assetPath) : GameWorld(assetPath), player(nullptr) {}

// =========== DESTRUCTOR =========== //
// The cleanUp() function ensures that all actors and the player are properly deleted.
StudentWorld::~StudentWorld() {
    cleanUp();
}

// =========== INITIALIZE GAME WORLD =========== //
// Sets up the initial state of the game world. This involves:
// - Loadixng the level file that defines the layout of the game area.
// - Iterating through each position in the level grid and determining what type of object, if any, should be placed there.
// - Creating the appropriate actor objects (e.g., Player, Floor, Koopa) based on the level data and adding them to the game world.
int StudentWorld::init() {
    // Load level data
    Level lev(assetPath());
    std::string levelFile = "level01.txt"; // Name of the level file to be loaded

    // Check if level loads successfully; return an error code if it fails.
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result != Level::load_success) {
        return GWSTATUS_LEVEL_ERROR; // Return an error status if the level cannot be loaded
    }

    // Iterate through each grid position in the level and populate the world with the corresponding actors.
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            BaseClass* actor = nullptr;

            // Determine the type of actor to create based on the MazeEntry value at (x, y).
            switch (item) {
            case Level::player: // Place the player at this position.
                player = new Player(x, y, this);
                actor = player;
                break;
            case Level::floor: // Place a floor object.
                actor = new Floor(x, y, this);
                break;
            case Level::fireball: // Place a fireball object moving to the right.
                actor = new Fireball(x, y, this, GraphObject::right);
                break;
            case Level::koopa: // Place a Koopa enemy.
                actor = new Koopa(x, y, this);
                break;
            case Level::bonfire: // Place a bonfire that harms the player.
                actor = new Bonfire(x, y, this);
                break;
            case Level::extra_life: // Place an ExtraLifeGoodie.
                actor = new ExtraLifeGoodie(x, y, this);
                break;
            case Level::garlic: // Place a GarlicGoodie.
                actor = new GarlicGoodie(x, y, this);
                break;
            case Level::ladder: // PLace a ladder
                 actor = new Ladder(x, y, this);
                 break;
            case Level::left_kong:
                 actor = new Kong(x, y, this);
                 break;
            case Level::right_kong:
                actor = new Kong(x, y, this);
                break;
            default:
                // No valid actor to create; leave this grid cell empty.
                break;
            }

            // If an actor was created, add it to the list of actors in the game world.
            if (actor) {
                addActor(actor);
            }
        }
    }

    // Return a status indicating that the game should continue.
    return GWSTATUS_CONTINUE_GAME;
}

// =========== GAME LOOP =========== //
// This function is called repeatedly to advance the game state by one frame.
// - All live actors perform their actions
// - Dead actors are removed from the game world.
// - The game continues running unless a specific event (like player death) causes it to stop.
int StudentWorld::move() {

    // Process each actor’s action, ensuring they are still alive before acting.
    for (auto it = actors.begin(); it != actors.end(); ++it) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

            if (!player->isAlive()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }

        }
    }

    // Clean up any actors that have been marked as dead **AFTER** the loop.
    removeDeadActors();

    // Check if the player has died
    if (player && !player->isAlive()) {
        decLives();
        if (getLives() > 0) {
            return GWSTATUS_PLAYER_DIED;  // Player will respawn if lives remain
        }
        else {
            return GWSTATUS_FINISHED_LEVEL;  // End the game (or handle differently)
        }
    }

    // Update game status text.
    std::string statusText = "Score: " + std::to_string(getScore()) +
        " Lives: " + std::to_string(getLives()) +
        " Burps: " + std::to_string(player->getBurpCount());
    setGameStatText(statusText);

    // Return a status indicating that the game should continue.
    return GWSTATUS_CONTINUE_GAME;
}


// =========== CLEANUP FUNCTION =========== //
void StudentWorld::cleanUp() {

    // Iterate through the list of actors and delete each one.
    for (auto* actor : actors) {
        delete actor;
    }
    // Clear the list of actors since they have all been removed.
    actors.clear();
    player = nullptr;
}

// =========== CHECKS FOR OBSTACLES =========== //
// Determines if a specific (x, y) position is walkable or blocked by an obstacle.
bool StudentWorld::canMoveTo(int x, int y) const {
    for (const auto* actor : actors) {
        // If any actor at the position is not walkable, return false.
        if (actor->getX() == x && actor->getY() == y && !actor->isWalkable()) {
            return false;
        }
    }
    // If no blocking actors are found, the position is walkable.
    return true;
}

// Determines if there is a floor object at the specified position.
bool StudentWorld::isFloorAt(int x, int y) const {
    for (const auto* actor : actors) {
        if (actor->getX() == x && actor->getY() == y && dynamic_cast<const Floor*>(actor)) {
            return true;
        }
    }
    return false;
}

// Checks whether a ladder exists at a given position.
bool StudentWorld::isLadderAt(int x, int y) const {
    for (const auto* actor : actors) {
        if (dynamic_cast<const Ladder*>(actor) && actor->getX() == x && actor->getY() == y) {
            return true;
        }
    }
    return false;
}

// =========== DESTROY NEARBY ENEMIES =========== //
// Eliminates all enemy actors within one unit of the given position.
void StudentWorld::destroyEnemiesNear(int x, int y) {
    for (auto* actor : actors) {
        // Check if the actor is an enemy and close to the given position.
        if (actor->isEnemy() && abs(actor->getX() - x) <= 1 && abs(actor->getY() - y) <= 1) {
            actor->setDead();
        }
    }
}

// =========== PLAYER UTILITIES =========== //
// Increases the player's lives. This is often triggered by collecting an extra life goodie.
void StudentWorld::increasePlayerLives() {
    if (player) {
        player->incrementLives();
    }
}

// =========== ACTOR MANAGEMENT =========== //
// Adds a new actor to the game world’s list of active actors.
void StudentWorld::addActor(BaseClass* actor) {
    if (actor) actors.push_back(actor);
}

// Removes actors that are marked as “dead” from the game world.
void StudentWorld::removeDeadActors() {
    for (auto it = actors.begin(); it != actors.end();) {
        if (!(*it)->isAlive()) {
            delete* it;            // Free the memory for the dead actor.
            it = actors.erase(it); // Remove the actor from the list.
        }
        else {
            ++it;                  // Move to the next actor if it is still alive.
        }
    }
}



// =========== WHERE IS THE WALL =========== //
bool StudentWorld::isWallAt(int x, int y) const {
    for (const auto* actor : actors) {
        if (dynamic_cast<const Floor*>(actor) && actor->getX() == x && actor->getY() == y) {
            return true; // Found a wall (Floor object)
        }
    }
    return false; // No wall found
}


GameWorld* createStudentWorld(std::string assetPath) {
    return new StudentWorld(assetPath);
}
