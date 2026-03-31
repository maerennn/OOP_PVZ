#include "Lawnmower.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"

Lawnmower::Lawnmower(int row, const glm::vec2& position)
    : m_Row(row)
    , m_State(State::IDLE)
{
    m_Transform.translation = position;
    SetZIndex(GameConfig::ZIndex::LAWN_MOWER);
}

void Lawnmower::Initialize() {
    // Load animation from ResourceManager - start paused (false = don't play immediately)
    m_Animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::LAWNMOWER_IDLE, false);
    SetDrawable(m_Animation);

    // Ensure animation is paused on first frame while IDLE
    m_Animation->SetCurrentFrame(0);
    m_Animation->Pause();

    // Scale the lawnmower sprite
    m_Transform.scale = {0.15f, 0.15f};

    LOG_DEBUG("Lawnmower initialized for row {}", m_Row);
}

void Lawnmower::Update(float deltaTime) {
    if (m_State == State::TRIGGERED) {
        // Move to the right
        m_Transform.translation.x += m_Speed * deltaTime;
    }
}

void Lawnmower::Trigger() {
    if (m_State == State::IDLE) {
        m_State = State::TRIGGERED;
        
        // Start the animation (spinning blades) when triggered
        m_Animation->Play();
        
        LOG_DEBUG("Lawnmower triggered in row {}!", m_Row);
    }
}

bool Lawnmower::IsOffScreen() const {
    // Off the right side of the screen
    return m_Transform.translation.x > GameConfig::ZOMBIE_ZONE_RIGHT + 100.0f;
}
