#include "Zombie/BucketheadZombie.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"

BucketheadZombie::BucketheadZombie()
    : Zombie("Buckethead Zombie",
             BUCKETHEAD_HEALTH,
             BUCKETHEAD_SPEED,
             BUCKETHEAD_DAMAGE)
{
}

void BucketheadZombie::Initialize() {
    auto& resourceManager = ResourceManager::GetInstance();

    // Load animations from ResourceManager (cached paths, new instances)
    m_WalkingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BUCKETHEAD_WALKING, true);

    m_AttackingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BUCKETHEAD_ATTACKING, false);

    m_DyingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_BUCKETHEAD_DYING, false);

    // Start with walking animation
    m_CurrentAnimation = m_WalkingAnimation;
    SetDrawable(m_CurrentAnimation);

    // Scale the zombie sprite
    m_Transform.scale = {0.25f, 0.25f};

    LOG_DEBUG("BucketheadZombie initialized with {} HP", BUCKETHEAD_HEALTH);
}

void BucketheadZombie::OnStateChanged(State newState) {
    // Call base implementation for animation switching
    Zombie::OnStateChanged(newState);
}
