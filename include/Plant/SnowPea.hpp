#ifndef SNOWPEA_HPP
#define SNOWPEA_HPP

#include "Plant/ShooterPlant.hpp"
#include <string>

/**
 * @file SnowPea.hpp
 * @brief Concrete implementation of the Snow Pea plant.
 *
 * Snow Pea fires frozen peas that slow and chill zombies on hit.
 * Chilled zombies move and attack at half speed for 10 seconds.
 *
 * Stats:
 * - Damage: 20 per pea
 * - Attack Interval: 1.5 seconds
 * - Projectile: FrozenPea (applies chill on hit)
 * - Sun Cost: 175
 * - Recharge: Fast (7.5s)
 */

class SnowPea : public ShooterPlant {
public:
    static constexpr int   SNOWPEA_DAMAGE           = 20;
    static constexpr float SNOWPEA_ATTACK_INTERVAL  = 1.5f;
    static constexpr int   SNOWPEA_SUN_COST         = 175;
    static constexpr float SNOWPEA_RECHARGE         = 7.5f;

    SnowPea();
    virtual ~SnowPea() = default;

    void Initialize(const std::string& frameDirectory);

protected:
    void OnAttack() override;
};

#endif // SNOWPEA_HPP
