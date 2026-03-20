#include "Plant/Plant.hpp"
#include "GameConfig.hpp"

Plant::Plant(const std::string& name, int health, int sunCost, float rechargeTime)
    : m_Name(name)
    , m_Health(health)
    , m_MaxHealth(health)
    , m_SunCost(sunCost)
    , m_RechargeTime(rechargeTime)
{
    SetZIndex(GameConfig::ZIndex::PLANT);
}

void Plant::Update(float /*deltaTime*/) {
    // Base implementation - subclasses override for specific behavior
}

void Plant::TakeDamage(int amount) {
    m_Health -= amount;
    if (m_Health < 0) {
        m_Health = 0;
    }
}

void Plant::SetupAnimation(const std::vector<std::string>& frames,
                           int frameInterval,
                           bool looping) {
    m_Animation = std::make_shared<Util::Animation>(
        frames,
        true,           // play immediately
        frameInterval,  // ms per frame
        looping,        // loop animation
        0               // no cooldown
    );
    SetDrawable(m_Animation);
}
