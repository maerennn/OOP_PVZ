#include "Sun.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include <cmath>

Sun::Sun(const glm::vec2& position, float targetY, int value)
    : m_Value(value)
    , m_Lifetime(SUN_LIFETIME)
    , m_BasePosition(position)
    , m_TargetY(targetY)
{
    m_Transform.translation = position;
    SetZIndex(GameConfig::ZIndex::SUN);

    // If starting Y differs from target Y, this sun needs to fall
    // In PTSD coordinate system, Y goes UP, so falling means decreasing Y
    if (std::abs(position.y - targetY) > 1.0f) {
        m_IsFalling = true;
    }
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

    // Handle falling behavior (for sky drops)
    if (m_IsFalling) {
        // In PTSD, Y goes UP, so falling = decreasing Y toward m_TargetY
        m_Transform.translation.y -= m_FallSpeed * deltaTime;

        // Check if we've reached or passed the target
        if (m_Transform.translation.y <= m_TargetY) {
            m_Transform.translation.y = m_TargetY;
            m_BasePosition.y = m_TargetY;
            m_IsFalling = false;
            m_HoverTime = 0.0f;  // Reset hover animation
        }
        return;  // Don't hover while falling
    }

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
