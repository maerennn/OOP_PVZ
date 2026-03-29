#include "Zombie.hpp"
#include "GameConfig.hpp"
#include "Util/Logger.hpp"
#include "Plant/Plant.hpp"

Zombie::Zombie(const std::string& name, int health, float speed, int damage)
    : m_Name(name)
    , m_Health(health)
    , m_MaxHealth(health)
    , m_Speed(speed)
    , m_Damage(damage)
    , m_State(State::WALKING)
{
    SetZIndex(GameConfig::ZIndex::ZOMBIE);
}

void Zombie::Update(float deltaTime) {
    // ══════════════════════════════════════════════════════════════════════
    // State Machine Logic - All behavior is encapsulated here
    // ══════════════════════════════════════════════════════════════════════

    switch (m_State) {
        case State::WALKING:
            // Move left toward the house
            m_Transform.translation.x -= m_Speed * deltaTime;
            break;

        case State::ATTACKING:
            // Deal damage to target plant over time
            if (m_TargetPlant && m_TargetPlant->IsAlive()) {
                int damageThisFrame = static_cast<int>(m_Damage * deltaTime);
                if (damageThisFrame < 1) damageThisFrame = 1;  // Minimum 1 damage per frame
                m_TargetPlant->TakeDamage(damageThisFrame);

                // Check if plant died
                if (!m_TargetPlant->IsAlive()) {
                    LOG_DEBUG("{} killed plant, resuming walk", m_Name);
                    ClearTargetPlant();
                    SetState(State::WALKING);
                }
            } else {
                // No target or target died - resume walking
                ClearTargetPlant();
                SetState(State::WALKING);
            }
            break;

        case State::DYING:
            // Accumulate death timer
            m_DeathTimer += deltaTime;

            // Check if death animation is complete (use timer as primary check)
            if (m_DeathTimer >= DEATH_ANIMATION_DURATION) {
                LOG_DEBUG("{} death animation complete (timer)", m_Name);
                SetState(State::DEAD);
            }
            // Also check animation state as backup
            else if (m_DyingAnimation && 
                     m_DyingAnimation->GetState() == Util::Animation::State::ENDED) {
                LOG_DEBUG("{} death animation complete (animation state)", m_Name);
                SetState(State::DEAD);
            }
            break;

        case State::DEAD:
            // Do nothing - waiting for removal by App
            break;
    }
}

void Zombie::TakeDamage(int amount) {
    if (m_State == State::DEAD || m_State == State::DYING) {
        return;  // Can't damage dead/dying zombies
    }

    m_Health -= amount;
    LOG_DEBUG("{} took {} damage, health: {}/{}", m_Name, amount, m_Health, m_MaxHealth);

    if (m_Health <= 0) {
        m_Health = 0;
        SetState(State::DYING);
    }
}

void Zombie::SetState(State newState) {
    if (m_State == newState) return;

    State oldState = m_State;
    m_State = newState;

    LOG_DEBUG("{} state: {} -> {}",
              m_Name,
              static_cast<int>(oldState),
              static_cast<int>(newState));

    OnStateChanged(newState);
}

void Zombie::OnStateChanged(State newState) {
    // Switch animation based on new state
    switch (newState) {
        case State::WALKING:
            if (m_WalkingAnimation) {
                m_CurrentAnimation = m_WalkingAnimation;
                SetDrawable(m_CurrentAnimation);
                m_WalkingAnimation->Play();
            }
            break;

        case State::ATTACKING:
            if (m_AttackingAnimation) {
                m_CurrentAnimation = m_AttackingAnimation;
                SetDrawable(m_CurrentAnimation);
                m_AttackingAnimation->Play();
            }
            break;

        case State::DYING:
            // Reset death timer
            m_DeathTimer = 0.0f;

            if (m_DyingAnimation) {
                m_CurrentAnimation = m_DyingAnimation;
                SetDrawable(m_CurrentAnimation);
                m_DyingAnimation->Play();
                LOG_DEBUG("{} playing death animation", m_Name);
            } else {
                // No dying animation - go straight to dead after brief delay
                LOG_DEBUG("{} has no dying animation, will die after timer", m_Name);
            }
            break;

        case State::DEAD:
            // Mark for removal - App will handle cleanup
            LOG_DEBUG("{} is now DEAD, ready for removal", m_Name);
            break;
    }
}

void Zombie::SetupAnimation(const std::vector<std::string>& frames,
                            int frameInterval,
                            bool looping) {
    m_CurrentAnimation = std::make_shared<Util::Animation>(
        frames,
        true,           // play immediately
        frameInterval,  // ms per frame
        looping,        // loop animation
        0               // no cooldown
    );
    SetDrawable(m_CurrentAnimation);
}
