#include "PoleVaultZombie.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"

PoleVaultZombie::PoleVaultZombie()
    : Zombie("Pole Vaulting Zombie",
             POLEVAULT_HEALTH,
             POLEVAULT_SPEED_FAST,
             POLEVAULT_DAMAGE)
{
}

void PoleVaultZombie::Initialize() {
    auto& resourceManager = ResourceManager::GetInstance();

    // Load animations from ResourceManager (cached paths, new instances)
    m_WalkingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_POLEVAULT_WALKING, true);

    m_AttackingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_POLEVAULT_ATTACKING, false);

    m_DyingAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_POLEVAULT_DYING, false);

    m_JumpAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_POLEVAULT_JUMP, false);

    // Start with walking animation (fast, with pole)
    m_CurrentAnimation = m_WalkingAnimation;
    SetDrawable(m_CurrentAnimation);

    // Scale the zombie sprite
    m_Transform.scale = {0.25f, 0.25f};

    LOG_DEBUG("PoleVaultZombie initialized with {} HP, speed {}", 
              POLEVAULT_HEALTH, POLEVAULT_SPEED_FAST);
}

void PoleVaultZombie::Update(float deltaTime) {
    // TODO: Implement pole vault jump logic
    // When m_HasPole is true and first plant is encountered:
    // 1. Play m_JumpAnimation
    // 2. Move zombie forward past the plant
    // 3. Set m_HasPole = false
    // 4. Reduce speed to POLEVAULT_SPEED_SLOW
    // 5. Continue as normal zombie

    // For now, use base class behavior
    Zombie::Update(deltaTime);
}

void PoleVaultZombie::OnStateChanged(State newState) {
    // Call base implementation for animation switching
    Zombie::OnStateChanged(newState);
}
