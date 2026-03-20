#include "Plant/ShooterPlant.hpp"

ShooterPlant::ShooterPlant(const std::string& name,
                           int damage,
                           float attackInterval,
                           ProjectileType projectileType,
                           int health,
                           int sunCost,
                           float rechargeTime)
    : Plant(name, health, sunCost, rechargeTime)
    , m_Damage(damage)
    , m_AttackInterval(attackInterval)
    , m_ProjectileType(projectileType)
{
}

void ShooterPlant::Update(float deltaTime) {
    Plant::Update(deltaTime);

    // Only attack if there's a target in the lane
    if (m_HasTarget) {
        m_AttackTimer += deltaTime;
        if (m_AttackTimer >= m_AttackInterval) {
            PerformAttack();
            m_AttackTimer = 0.0f;
        }
    }
}

void ShooterPlant::PerformAttack() {
    OnAttack();
}

void ShooterPlant::OnAttack() {
    // Base implementation - subclasses override to spawn actual projectiles
    // This is where projectile creation logic would be implemented
    // Example: SpawnProjectile(m_ProjectileType, m_Damage, GetRow());
}
