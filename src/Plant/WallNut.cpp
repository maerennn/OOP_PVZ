#include "Plant/WallNut.hpp"
#include "ResourceManager.hpp"

WallNut::WallNut()
    : DefensePlant("Wall-nut",
                   WALLNUT_HEALTH,
                   WALLNUT_SUN_COST,
                   WALLNUT_RECHARGE)
{
}

void WallNut::Initialize(const std::string& /*frameDirectory*/) {
    auto& resourceManager = ResourceManager::GetInstance();

    // Load all three damage stage animations from ResourceManager
    auto healthyAnim = resourceManager.CreateAnimation(
        ResourceManager::PLANT_WALLNUT_HEALTHY, true);
    auto damagedAnim = resourceManager.CreateAnimation(
        ResourceManager::PLANT_WALLNUT_DAMAGED, true);
    auto criticalAnim = resourceManager.CreateAnimation(
        ResourceManager::PLANT_WALLNUT_CRITICAL, true);

    SetupDamageStageAnimations(healthyAnim, damagedAnim, criticalAnim);

    // Default scale for plant sprite
    m_Transform.scale = {0.25f, 0.25f};
}

void WallNut::OnDamageStageChanged(DamageStage newStage) {
    // Call parent implementation to handle animation switching
    DefensePlant::OnDamageStageChanged(newStage);
}
