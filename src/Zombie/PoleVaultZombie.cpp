#include "Zombie/PoleVaultZombie.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Plant/Plant.hpp"
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

    m_WalkingSlowAnimation = resourceManager.CreateAnimation(
        ResourceManager::ZOMBIE_POLEVAULT_WALKING_SLOW, true);

    // Start with walking animation (fast, with pole)
    m_CurrentAnimation = m_WalkingAnimation;
    SetDrawable(m_CurrentAnimation);

    // Scale the zombie sprite
    m_Transform.scale = {0.25f, 0.25f};

    LOG_DEBUG("PoleVaultZombie initialized with {} HP, speed {}", 
              POLEVAULT_HEALTH, POLEVAULT_SPEED_FAST);
}

void PoleVaultZombie::OnPlantEncountered(std::shared_ptr<Plant> plant) {
    if (m_HasPole) {
        // Record the plant's X so we can snap the landing position precisely
        m_VaultedPlantX = plant->m_Transform.translation.x;
        LOG_DEBUG("PoleVaultZombie vaulting over plant at x={:.1f}", m_VaultedPlantX);
        SetState(State::JUMPING);
    } else {
        // No pole left — behave like a normal zombie
        Zombie::OnPlantEncountered(plant);
    }
}

void PoleVaultZombie::Update(float deltaTime) {
    if (m_State == State::JUMPING) {
        // Advance the timer and move left at fast speed
        m_JumpTimer += deltaTime;
        m_Transform.translation.x -= m_Speed * deltaTime;

        if (m_JumpTimer >= JUMP_DURATION) {
            m_JumpTimer = 0.0f;
            m_HasPole   = false;
            m_Speed     = POLEVAULT_SPEED_SLOW;
            // Snap to exactly one tile past the vaulted plant.
            // The collision guard (plantPos.x > zombie.x) will always skip the
            // jumped-over plant, and any plant one tile further left is detected
            // correctly.
            m_Transform.translation.x = m_VaultedPlantX - GameConfig::CELL_WIDTH;
            SetState(State::WALKING);
            LOG_DEBUG("PoleVaultZombie landed at x={:.1f}, walking slow",
                      m_Transform.translation.x);
        }
        return;  // Don't run base Update while jumping
    }
    Zombie::Update(deltaTime);
}

void PoleVaultZombie::OnStateChanged(State newState) {
    if (newState == State::JUMPING) {
        if (m_JumpAnimation) {
            m_CurrentAnimation = m_JumpAnimation;
            SetDrawable(m_CurrentAnimation);
            m_JumpAnimation->Play();
        }
        return;
    }

    // ── Walking after jump ────────────────────────────────────────────────
    // Walk_After_Jump canvas is 961×723 (resized to match basic zombie).
    // The zombie was spawned with a +64 Y offset for the 2058×1295 with-pole
    // canvas.  Remove that offset so the body lands on the correct lane line.
    if (newState == State::WALKING && !m_HasPole) {
        m_Transform.translation.y = GameConfig::LaneCenterY(GetRow());

        // Guard: only swap drawable if not already the slow-walk animation —
        // prevents frame-0 restart on every ATTACKING→WALKING cycle.
        if (m_CurrentAnimation != m_WalkingSlowAnimation) {
            m_CurrentAnimation = m_WalkingSlowAnimation;
            SetDrawable(m_CurrentAnimation);
            if (m_WalkingSlowAnimation) m_WalkingSlowAnimation->Play();
        }
        return;
    }

    // ── Attacking after losing pole ───────────────────────────────────────
    // Attacking canvas is now 961×723 (body Y ≈ 0, same as walk-slow).
    // No Y offset needed — same lane centre as the walk-slow state.
    if (!m_HasPole && newState == State::ATTACKING) {
        m_Transform.translation.y = GameConfig::LaneCenterY(GetRow());
    }

    // ── Dying / Charred after losing pole ────────────────────────────────
    // Dying canvas is still 2058×1295 (body Y = +85.5px below canvas centre).
    if (!m_HasPole &&
        (newState == State::DYING || newState == State::CHARRED)) {
        m_Transform.translation.y = GameConfig::LaneCenterY(GetRow()) + 85.5f;
    }

    // Delegate animation swap to base class
    Zombie::OnStateChanged(newState);
}
