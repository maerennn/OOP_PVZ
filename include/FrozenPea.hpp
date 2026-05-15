#ifndef FROZENPEA_HPP
#define FROZENPEA_HPP

#include "Projectile.hpp"
#include <glm/glm.hpp>

/**
 * @file FrozenPea.hpp
 * @brief Concrete implementation of the frozen pea projectile.
 *
 * Fired by the Snow Pea plant. Behaves like a normal Pea but applies
 * a chill effect to the zombie it hits, halving movement and attack
 * speed for CHILL_DURATION seconds.
 */

class FrozenPea : public Projectile {
public:
    static constexpr float FROZEN_PEA_SPEED   = 300.0f;
    static constexpr float CHILL_DURATION     = 10.0f;

    FrozenPea(int damage, int row, const glm::vec2& position);
    virtual ~FrozenPea() = default;

    void Initialize() override;
    void OnHit() override;

    // Applies chill effect to the struck zombie
    void OnZombieHit(Zombie& zombie) override;
};

#endif // FROZENPEA_HPP
