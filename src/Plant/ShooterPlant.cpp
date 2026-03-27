#include "Plant/ShooterPlant.hpp"
#include "Util/Logger.hpp"

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
    // Fire callback to spawn projectile via App
    if (m_OnProjectile) {
        // Offset projectile spawn slightly to the right of the plant
        glm::vec2 spawnPos = m_Transform.translation + glm::vec2(30.0f, 10.0f);

        m_OnProjectile(m_ProjectileType, m_Damage, GetRow(), spawnPos);

        LOG_DEBUG("{} fired {} projectile at row {}",
                  GetName(), static_cast<int>(m_ProjectileType), GetRow());
    }
}
