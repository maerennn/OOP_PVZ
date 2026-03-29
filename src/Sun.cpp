#include "Sun.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include <cmath>

Sun::Sun(const glm::vec2& position, int value)
    : m_Value(value)
    , m_Lifetime(SUN_LIFETIME)
    , m_BasePosition(position)
{
    m_Transform.translation = position;
    SetZIndex(GameConfig::ZIndex::SUN);
}

void Sun::Initialize() {
    // Get animation from ResourceManager (cached paths, new instance)
    m_Animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::SUN_FLOATING, true);
    SetDrawable(m_Animation);

    // Scale the sun sprite
    m_Transform.scale = {0.2f, 0.2f};
}

void Sun::Update(float deltaTime) {
    if (m_IsCollected) return;

    // Decrease lifetime
    m_Lifetime -= deltaTime;

    // Hover animation (gentle up/down bobbing)
    m_HoverTime += deltaTime * HOVER_SPEED;
    float hoverOffset = std::sin(m_HoverTime) * HOVER_AMPLITUDE;
    m_Transform.translation.y = m_BasePosition.y + hoverOffset;
}

bool Sun::IsClicked(const glm::vec2& cursorPos) const {
    if (m_IsCollected) return false;

    // Simple distance check
    glm::vec2 diff = cursorPos - m_Transform.translation;
    float distSq = diff.x * diff.x + diff.y * diff.y;
    return distSq <= (CLICK_RADIUS * CLICK_RADIUS);
}

void Sun::Collect() {
    if (m_IsCollected) return;

    m_IsCollected = true;

    // Trigger callback to add sun to player's total
    if (m_OnCollected) {
        m_OnCollected(m_Value);
    }
}
