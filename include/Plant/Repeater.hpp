#ifndef REPEATER_HPP
#define REPEATER_HPP

#include "Plant/ShooterPlant.hpp"
#include <string>

/**
 * @file Repeater.hpp
 * @brief Concrete implementation of the Repeater plant.
 *
 * Repeater fires two peas per attack cycle, doubling the firepower of a Peashooter.
 *
 * Stats:
 * - Damage: 20 per pea (40 total per cycle)
 * - Attack Interval: 1.5 seconds
 * - Projectile: Pea (x2)
 * - Sun Cost: 200
 * - Recharge: Fast (7.5s)
 */

class Repeater : public ShooterPlant {
public:
    static constexpr int   REPEATER_DAMAGE           = 20;
    static constexpr float REPEATER_ATTACK_INTERVAL  = 1.5f;
    static constexpr int   REPEATER_SUN_COST         = 200;
    static constexpr float REPEATER_RECHARGE         = 7.5f;

    Repeater();
    virtual ~Repeater() = default;

    void Initialize(const std::string& frameDirectory);

protected:
    // Fires two peas per attack instead of one
    void OnAttack() override;
};

#endif // REPEATER_HPP
