#ifndef PEASHOOTER_HPP
#define PEASHOOTER_HPP

#include "Plant/ShooterPlant.hpp"
#include <string>

/**
 * @file Peashooter.hpp
 * @brief Concrete implementation of the basic Peashooter plant.
 *
 * Peashooter is the quintessential shooter plant:
 * - Identity: Is-a ShooterPlant (which is-a Plant)
 * - Ability: Inherited from ShooterPlant (implements IAttacker)
 *
 * Animation paths are cached via ResourceManager.
 *
 * Stats:
 * - Damage: 20 per pea
 * - Attack Interval: 1.5 seconds
 * - Projectile: Pea
 * - Sun Cost: 100
 * - Recharge: Fast (7.5s)
 */

class Peashooter : public ShooterPlant {
public:
    // Peashooter-specific constants
    static constexpr int PEASHOOTER_DAMAGE = 20;
    static constexpr float PEASHOOTER_ATTACK_INTERVAL = 1.5f;
    static constexpr int PEASHOOTER_SUN_COST = 100;
    static constexpr float PEASHOOTER_RECHARGE = 7.5f;

    Peashooter();
    virtual ~Peashooter() = default;

    // Initialize with animation from ResourceManager (frameDirectory ignored)
    void Initialize(const std::string& frameDirectory);

protected:
    // Override to spawn actual pea projectiles
    void OnAttack() override;
};

#endif // PEASHOOTER_HPP
