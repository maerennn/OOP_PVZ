#include "ConeheadZombie.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"

ConeheadZombie::ConeheadZombie()
    : Zombie("Conehead Zombie",
             CONEHEAD_HEALTH,
             CONEHEAD_SPEED,
             CONEHEAD_DAMAGE)
{
}

void ConeheadZombie::Initialize() {
    auto& resourceManager = ResourceManager::GetInstance();

    // Load animations from ResourceManager (cached paths, new instances)
    m_WalkingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_CONEHEAD_WALKING, true);

    m_AttackingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_CONEHEAD_ATTACKING, false);

    m_DyingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_CONEHEAD_DYING, false);

    // Start with walking animation
    m_CurrentAnimation = m_WalkingAnimation;
    SetDrawable(m_CurrentAnimation);

    // Scale the zombie sprite
    m_Transform.scale = {0.25f, 0.25f};

    LOG_DEBUG("ConeheadZombie initialized with {} HP", CONEHEAD_HEALTH);
}

void ConeheadZombie::OnStateChanged(State newState) {
    // Call base implementation for animation switching
    Zombie::OnStateChanged(newState);
}
