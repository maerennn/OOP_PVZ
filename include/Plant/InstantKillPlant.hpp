#ifndef INSTANT_KILL_PLANT_HPP
#define INSTANT_KILL_PLANT_HPP

#include "Plant/Plant.hpp"
#include "Plant/IAttacker.hpp"

/**
 * @file InstantKillPlant.hpp
 * @brief Specialized plant subclass for single-use explosive/instant-kill plants.
 *
 * InstantKillPlant combines:
 * - Identity: Is-a Plant (inherits core plant properties)
 * - Ability: Can attack (implements IAttacker interface)
 *
 * Key behavior: Single-use plants that destroy themselves after attacking.
 * After PerformAttack() is called:
 * 1. The attack effect is triggered (explosion, area damage, etc.)
 * 2. The plant's health is set to 0
 * 3. The plant should be removed from the game
 *
 * Examples: Cherry Bomb, Jalapeno, Squash, Potato Mine
 */

class InstantKillPlant : public Plant, public IAttacker {
public:
    InstantKillPlant(const std::string& name,
                     int damage,
                     float areaRadius,
                     int sunCost = 150,
                     float rechargeTime = 50.0f,
                     float fuseTime = 0.0f);

    virtual ~InstantKillPlant() = default;

    // Plant interface
    void Update(float deltaTime) override;

    // IAttacker interface implementation
    void PerformAttack() override;
    int GetDamage() const override { return m_Damage; }
    float GetAttackInterval() const override { return 0.0f; }  // Single use
    ProjectileType GetProjectileType() const override { return ProjectileType::AREA_EFFECT; }

    // Single-use plants have effectively infinite DPS for their one attack
    float GetDPS() const override { return static_cast<float>(m_Damage); }

    // Area of effect radius
    float GetAreaRadius() const { return m_AreaRadius; }

    // Check if plant has exploded/attacked
    bool HasTriggered() const { return m_HasTriggered; }

    // Manual trigger (e.g., for Potato Mine when zombie steps on it)
    void Trigger();

    // Some instant plants need arming time (e.g., Potato Mine)
    bool IsArmed() const { return m_IsArmed; }
    float GetArmTime() const { return m_ArmTime; }

protected:
    // Called when the attack effect should happen - override for custom effects
    virtual void OnTrigger();

    // Called after trigger animation completes - for cleanup
    virtual void OnTriggerComplete();

    int m_Damage;
    float m_AreaRadius;              // Radius of area effect
    float m_FuseTime;                // Delay before explosion after trigger
    float m_ArmTime = 0.0f;          // Time needed to become active (e.g., Potato Mine)

    bool m_HasTriggered = false;
    bool m_IsArmed = true;           // Most instant plants are armed immediately
    float m_FuseTimer = 0.0f;
    float m_ArmTimer = 0.0f;
};

#endif // INSTANT_KILL_PLANT_HPP
