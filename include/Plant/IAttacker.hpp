#ifndef IATTACKER_HPP
#define IATTACKER_HPP

#include <string>

/**
 * @file IAttacker.hpp
 * @brief Interface for plants that deal damage to zombies.
 *
 * === Interface Design Philosophy ===
 * Interfaces separate "identity" (what a plant IS) from "ability" (what it CAN DO).
 *
 * - Identity: Defined by the class hierarchy (Plant -> ShooterPlant -> Peashooter)
 * - Ability:  Defined by interfaces (IAttacker provides attacking capability)
 *
 * This allows:
 * - A Peashooter IS a ShooterPlant, and it CAN attack (IAttacker)
 * - A Sunflower IS a Plant, and it CAN produce (IProducer)
 * - A Cherry Bomb IS an InstantKillPlant, and it CAN attack (IAttacker)
 *
 * Different plant types can share the same ability without sharing identity.
 */

enum class ProjectileType {
    NONE,
    PEA,
    FROZEN_PEA,
    CABBAGE,
    FIRE_PEA,
    FUME,          // Pass-through attack
    AREA_EFFECT    // Explosive/instant damage
};

class IAttacker {
public:
    virtual ~IAttacker() = default;

    // Core attack method - performs the plant's attack action
    virtual void PerformAttack() = 0;

    // Attack properties
    virtual int GetDamage() const = 0;
    virtual float GetAttackInterval() const = 0;
    virtual ProjectileType GetProjectileType() const = 0;

    // Calculated property: damage per second
    virtual float GetDPS() const {
        float interval = GetAttackInterval();
        if (interval <= 0.0f) return 0.0f;
        return static_cast<float>(GetDamage()) / interval;
    }
};

#endif // IATTACKER_HPP
