#include "Zombie/NormalZombie.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"

NormalZombie::NormalZombie()
    : Zombie("Normal Zombie",
             NORMAL_ZOMBIE_HEALTH,
             NORMAL_ZOMBIE_SPEED,
             NORMAL_ZOMBIE_DAMAGE)
{
}

void NormalZombie::Initialize() {
    auto& resourceManager = ResourceManager::GetInstance();

    // Load animations from ResourceManager (cached paths, new instances)
    m_WalkingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BASIC_WALKING, true);

    m_AttackingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BASIC_ATTACKING, false);

    m_DyingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BASIC_DYING, false);

    // Start with walking animation
    m_CurrentAnimation = m_WalkingAnimation;
    SetDrawable(m_CurrentAnimation);

    // Scale the zombie sprite
    m_Transform.scale = {0.25f, 0.25f};

    LOG_DEBUG("NormalZombie initialized with walking, attacking, and dying animations");
}

void NormalZombie::OnStateChanged(State newState) {
    // Call base implementation for animation switching
    Zombie::OnStateChanged(newState);
}
