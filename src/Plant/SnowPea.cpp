#include "Plant/SnowPea.hpp"
#include "ResourceManager.hpp"

SnowPea::SnowPea()
    : ShooterPlant("Snow Pea",
                   SNOWPEA_DAMAGE,
                   SNOWPEA_ATTACK_INTERVAL,
                   ProjectileType::FROZEN_PEA,
                   DEFAULT_HEALTH,
                   SNOWPEA_SUN_COST,
                   SNOWPEA_RECHARGE)
{
}

void SnowPea::Initialize(const std::string& /*frameDirectory*/) {
    auto animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_SNOWPEA_IDLE, true);
    SetDrawable(animation);
    m_Transform.scale = {0.28f, 0.28f};
}

void SnowPea::OnAttack() {
    // Base implementation fires the FROZEN_PEA projectile via callback
    ShooterPlant::OnAttack();
}
