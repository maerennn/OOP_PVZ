#include "Plant/Peashooter.hpp"
#include "ResourceManager.hpp"

Peashooter::Peashooter()
    : ShooterPlant("Peashooter",
                   PEASHOOTER_DAMAGE,
                   PEASHOOTER_ATTACK_INTERVAL,
                   ProjectileType::PEA,
                   DEFAULT_HEALTH,
                   PEASHOOTER_SUN_COST,
                   PEASHOOTER_RECHARGE)
{
}

void Peashooter::Initialize(const std::string& /*frameDirectory*/) {
    // Get animation from ResourceManager (cached paths, new instance)
    auto animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_PEASHOOTER_IDLE, true);
    SetDrawable(animation);

    // Default scale for plant sprite
    m_Transform.scale = {0.28f, 0.28f};
}

void Peashooter::OnAttack() {
    // Use base class implementation which fires the callback
    ShooterPlant::OnAttack();
}
