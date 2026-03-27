#ifndef PEA_HPP
#define PEA_HPP

#include "Projectile.hpp"
#include <string>

/**
 * @file Pea.hpp
 * @brief Concrete implementation of the standard pea projectile.
 *
 * Pea is the basic projectile fired by Peashooters:
 * - Identity: Is-a Projectile
 * - Deals 20 damage on hit (inherited from shooter)
 * - Travels in a straight line across the lane
 *
 * Future projectile types (FrozenPea, FirePea) will follow this pattern.
 */

class Pea : public Projectile {
public:
    // Pea-specific constants
    static constexpr float PEA_SPEED = 300.0f;  // Pixels per second

    /**
     * @brief Construct a new Pea projectile.
     * @param damage Damage dealt on hit
     * @param row The lane/row this pea travels in
     * @param position Starting position
     */
    Pea(int damage, int row, const glm::vec2& position);

    virtual ~Pea() = default;

    /**
     * @brief Initialize the pea sprite.
     */
    void Initialize() override;

    /**
     * @brief Called when pea hits a zombie.
     * Standard pea just deals damage and disappears.
     */
    void OnHit() override;
};

#endif // PEA_HPP
