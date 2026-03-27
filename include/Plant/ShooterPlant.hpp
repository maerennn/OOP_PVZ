#ifndef SHOOTER_PLANT_HPP
#define SHOOTER_PLANT_HPP

#include "Plant/Plant.hpp"
#include "Plant/IAttacker.hpp"
#include <functional>

/**
 * @file ShooterPlant.hpp
 * @brief Specialized plant subclass for ranged attacking plants.
 *
 * ShooterPlant combines:
 * - Identity: Is-a Plant (inherits core plant properties)
 * - Ability: Can attack (implements IAttacker interface)
 *
 * This class handles the common behavior of projectile-shooting plants:
 * - Timing between shots
 * - Projectile creation
 * - DPS calculation
 *
 * Concrete shooter plants (Peashooter, SnowPea, Repeater) inherit from this.
 */

class ShooterPlant : public Plant, public IAttacker {
public:
    /**
     * @brief Callback type for when a projectile should be spawned.
     * Parameters: ProjectileType, damage, row, position
     */
    using ProjectileCallback = std::function<void(ProjectileType type,
                                                   int damage,
                                                   int row,
                                                   glm::vec2 position)>;

    ShooterPlant(const std::string& name,
                 int damage,
                 float attackInterval,
                 ProjectileType projectileType,
                 int health = DEFAULT_HEALTH,
                 int sunCost = DEFAULT_SUN_COST,
                 float rechargeTime = DEFAULT_RECHARGE_TIME);

    virtual ~ShooterPlant() = default;

    // Plant interface
    void Update(float deltaTime) override;

    // IAttacker interface implementation
    void PerformAttack() override;
    int GetDamage() const override { return m_Damage; }
    float GetAttackInterval() const override { return m_AttackInterval; }
    ProjectileType GetProjectileType() const override { return m_ProjectileType; }

    // DPS is calculated dynamically (method override from IAttacker)
    float GetDPS() const override {
        if (m_AttackInterval <= 0.0f) return 0.0f;
        return static_cast<float>(m_Damage) / m_AttackInterval;
    }

    // Check if there's a target in this plant's lane
    bool HasTargetInLane() const { return m_HasTarget; }
    void SetHasTarget(bool hasTarget) { m_HasTarget = hasTarget; }

    /**
     * @brief Set callback for when projectile should be spawned.
     * The callback is invoked by OnAttack() with all necessary data.
     */
    void SetProjectileCallback(ProjectileCallback callback) {
        m_OnProjectile = std::move(callback);
    }

protected:
    // Called when attack is ready - fires the callback to spawn projectile
    virtual void OnAttack();

    int m_Damage;
    float m_AttackInterval;          // Seconds between attacks
    ProjectileType m_ProjectileType;

    float m_AttackTimer = 0.0f;      // Time since last attack
    bool m_HasTarget = false;        // Is there a zombie in this lane?

    ProjectileCallback m_OnProjectile;  // Callback to spawn projectile
};

#endif // SHOOTER_PLANT_HPP
