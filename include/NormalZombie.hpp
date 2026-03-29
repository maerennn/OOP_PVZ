#ifndef NORMAL_ZOMBIE_HPP
#define NORMAL_ZOMBIE_HPP

#include "Zombie.hpp"

/**
 * @file NormalZombie.hpp
 * @brief Concrete implementation of the basic zombie.
 *
 * NormalZombie is the standard enemy:
 * - Identity: Is-a Zombie (inherits base zombie behavior)
 * - Stats: 200 HP, 20 speed, 100 damage/second
 *
 * Animations loaded via ResourceManager from "Resources/zombies/Basic Zombie/":
 * - Walking: basiczombie0047.png to basiczombie0137.png
 * - Attacking: basiczombie0138.png to basiczombie0178.png
 */

class NormalZombie : public Zombie {
public:
    // NormalZombie-specific constants
    static constexpr int NORMAL_ZOMBIE_HEALTH = 200;
    static constexpr float NORMAL_ZOMBIE_SPEED = 20.0f;
    static constexpr int NORMAL_ZOMBIE_DAMAGE = 100;

    NormalZombie();
    virtual ~NormalZombie() = default;

    /**
     * @brief Initialize zombie with animations from ResourceManager.
     */
    void Initialize() override;

protected:
    /**
     * @brief Called when state changes to swap animations.
     */
    void OnStateChanged(State newState) override;
};

#endif // NORMAL_ZOMBIE_HPP
