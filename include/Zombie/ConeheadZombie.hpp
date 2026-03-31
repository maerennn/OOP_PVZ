#ifndef CONEHEAD_ZOMBIE_HPP
#define CONEHEAD_ZOMBIE_HPP

#include "Zombie/Zombie.hpp"

/**
 * @file ConeheadZombie.hpp
 * @brief Zombie with a traffic cone on its head for extra protection.
 *
 * ConeheadZombie is a tougher variant of the basic zombie:
 * - Health: 560 (200 base + 360 cone absorption)
 * - Speed: 20 pixels/second (same as basic)
 * - Damage: 100 per second
 *
 * The cone absorbs 360 damage before falling off, then the zombie
 * continues as a normal zombie.
 */

class ConeheadZombie : public Zombie {
public:
    // Conehead-specific constants
    static constexpr int CONEHEAD_HEALTH = 560;       // 200 base + 360 cone
    static constexpr float CONEHEAD_SPEED = 20.0f;
    static constexpr int CONEHEAD_DAMAGE = 100;

    ConeheadZombie();
    virtual ~ConeheadZombie() = default;

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

#endif // CONEHEAD_ZOMBIE_HPP
