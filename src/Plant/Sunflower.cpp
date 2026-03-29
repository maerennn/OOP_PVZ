#include "Plant/Sunflower.hpp"
#include "ResourceManager.hpp"

Sunflower::Sunflower()
    : Plant("Sunflower",
            DEFAULT_HEALTH,
            SUNFLOWER_SUN_COST,
            SUNFLOWER_RECHARGE)
{
}

void Sunflower::Initialize(const std::string& /*frameDirectory*/) {
    // Get animation from ResourceManager (cached paths, new instance)
    auto animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_SUNFLOWER_IDLE, true);
    SetDrawable(animation);

    // Default scale for plant sprite
    m_Transform.scale = {0.25f, 0.25f};
}

void Sunflower::Update(float deltaTime) {
    Plant::Update(deltaTime);

    // Accumulate time for sun production
    m_ProductionTimer += deltaTime;

    // Produce sun when ready
    if (CanProduce()) {
        Produce();
    }
}

void Sunflower::Produce() {
    // Reset timer
    m_ProductionTimer = 0.0f;

    // Notify callback (typically spawns a collectible sun object)
    if (m_OnSunProduced) {
        m_OnSunProduced(m_ProductionAmount, m_Transform.translation);
    }
}
