#include "Plant/Repeater.hpp"
#include "ResourceManager.hpp"
#include "Util/Logger.hpp"

Repeater::Repeater()
    : ShooterPlant("Repeater",
                   REPEATER_DAMAGE,
                   REPEATER_ATTACK_INTERVAL,
                   ProjectileType::PEA,
                   DEFAULT_HEALTH,
                   REPEATER_SUN_COST,
                   REPEATER_RECHARGE)
{
}

void Repeater::Initialize(const std::string& /*frameDirectory*/) {
    auto animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_REPEATER_IDLE, true);
    SetDrawable(animation);
    m_Transform.scale = {0.28f, 0.28f};
}

void Repeater::OnAttack() {
    if (!m_OnProjectile) return;

    // First pea - standard offset
    glm::vec2 pos1 = m_Transform.translation + glm::vec2(30.0f, 10.0f);
    m_OnProjectile(m_ProjectileType, m_Damage, GetRow(), pos1);

    // Second pea - slightly further right for visual separation
    glm::vec2 pos2 = m_Transform.translation + glm::vec2(50.0f, 10.0f);
    m_OnProjectile(m_ProjectileType, m_Damage, GetRow(), pos2);

    LOG_DEBUG("Repeater fired 2 peas at row {}", GetRow());
}
