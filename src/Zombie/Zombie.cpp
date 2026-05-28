#include "Zombie/Zombie.hpp"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
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
            // Move left toward the house (at half speed when chilled)
            if (m_ChillTimer > 0.0f) {
                m_Transform.translation.x -= m_Speed * CHILL_SPEED_FACTOR * deltaTime;
            } else {
                m_Transform.translation.x -= m_Speed * deltaTime;
            }
            break;

        case State::JUMPING:
            // Movement during jump is handled entirely by the subclass (PoleVaultZombie).
            // Base class does nothing here so non-jumping zombies are unaffected.
            break;

        case State::ATTACKING: {
            // Lock the weak_ptr — plant may have been removed by another system
            auto target = m_TargetPlant.lock();
            if (target && target->IsAlive()) {
                // Halve damage rate when chilled
                float attackMultiplier = (m_ChillTimer > 0.0f) ? CHILL_SPEED_FACTOR : 1.0f;
                int damageThisFrame = static_cast<int>(m_Damage * attackMultiplier * deltaTime);
                if (damageThisFrame < 1) damageThisFrame = 1;  // Minimum 1 damage per frame
                target->TakeDamage(damageThisFrame);

                // Check if plant died
                if (!target->IsAlive()) {
                    LOG_DEBUG("{} killed plant, resuming walk", m_Name);
                    ClearTargetPlant();
                    SetState(State::WALKING);
                }
            } else {
                // No target or target died/removed - resume walking
                ClearTargetPlant();
                SetState(State::WALKING);
            }
            break;
        }

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

        case State::CHARRED:
            // Accumulate death timer for charred animation
            m_DeathTimer += deltaTime;

            // Charred animation is shorter
            if (m_DeathTimer >= CHARRED_ANIMATION_DURATION) {
                LOG_DEBUG("{} charred animation complete (timer)", m_Name);
                SetState(State::DEAD);
            }
            else if (m_CharredAnimation && 
                     m_CharredAnimation->GetState() == Util::Animation::State::ENDED) {
                LOG_DEBUG("{} charred animation complete (animation state)", m_Name);
                SetState(State::DEAD);
            }
            break;

        case State::DEAD:
            // Do nothing - waiting for removal by App
            break;
    }

    // Sync armor overlay world-position to follow this zombie every frame.
    // The PTSD renderer does NOT propagate parent transforms to children,
    // so we must set the child's world transform manually.
    if (m_ArmorOverlay && m_Armor) {
        m_ArmorOverlay->m_Transform.translation = {
            m_Transform.translation.x + m_Armor->GetOverlayXOffset(),
            m_Transform.translation.y + m_Armor->GetOverlayYOffset()
        };
        m_ArmorOverlay->m_Transform.scale = m_Transform.scale * 4.0f;
    }

    // Tick chill timer
    if (m_ChillTimer > 0.0f) {
        m_ChillTimer -= deltaTime;
        if (m_ChillTimer < 0.0f) m_ChillTimer = 0.0f;
    }
}

void Zombie::TakeDamage(int amount) {
    if (m_State == State::DEAD || m_State == State::DYING || m_State == State::CHARRED) {
        return;
    }

    // ── Route damage through armor first ──────────────────────────────────
    int baseAmount = amount;
    if (m_Armor) {
        Armor::DegradationState stateBefore = m_Armor->GetState();
        baseAmount = m_Armor->TakeDamage(amount);

        if (m_Armor->IsDestroyed()) {
            LOG_DEBUG("{} armor destroyed (overflow {} dmg)", m_Name, baseAmount);
            OnArmorDestroyed();
        } else if (m_Armor->GetState() != stateBefore) {
            OnArmorStateChanged(m_Armor->GetState());
        }
    }

    // ── Apply overflow (or full damage if unarmed) to base HP ─────────────
    if (baseAmount > 0) {
        m_Health -= baseAmount;
        LOG_DEBUG("{} took {} base damage, health: {}/{}", m_Name, baseAmount, m_Health, m_MaxHealth);

        if (m_Health <= 0) {
            m_Health = 0;
            m_DeathType = DeathType::NORMAL;
            SetState(State::DYING);
        }
    }
}

void Zombie::TakeExplosionDamage(int amount) {
    if (m_State == State::DEAD || m_State == State::DYING || m_State == State::CHARRED) {
        return;
    }

    // ── Route through armor first (explosion ignores armor less efficiently) ─
    int baseAmount = amount;
    if (m_Armor) {
        Armor::DegradationState stateBefore = m_Armor->GetState();
        baseAmount = m_Armor->TakeDamage(amount);

        if (m_Armor->IsDestroyed()) {
            OnArmorDestroyed();
        } else if (m_Armor->GetState() != stateBefore) {
            OnArmorStateChanged(m_Armor->GetState());
        }
    }

    if (baseAmount > 0) {
        m_Health -= baseAmount;
        LOG_DEBUG("{} took {} EXPLOSION base damage, health: {}/{}", m_Name, baseAmount, m_Health, m_MaxHealth);

        if (m_Health <= 0) {
            m_Health = 0;
            m_DeathType = DeathType::EXPLOSION;
            SetState(State::CHARRED);
        }
    }
}

// ── EquipArmor ─────────────────────────────────────────────────────────────

void Zombie::EquipArmor(std::unique_ptr<Armor> armor) {
    // Remove any existing overlay from the child list
    if (m_ArmorOverlay) {
        RemoveChild(m_ArmorOverlay);
        m_ArmorOverlay.reset();
    }

    m_Armor = std::move(armor);

    if (!m_Armor) return;

    // Build the overlay child — starts with the INTACT sprite
    const std::string key = m_Armor->GetCurrentSpritePath();
    if (!key.empty()) {
        auto image = ResourceManager::GetInstance().GetImage(key);
        m_ArmorOverlay = std::make_shared<Util::GameObject>(
            image,
            GetZIndex() + 0.1f  // render just above the zombie body
        );
        AddChild(m_ArmorOverlay);
    }

    m_LastArmorState = m_Armor->GetState();
    LOG_DEBUG("{} equipped armor ({} HP)", m_Name, m_Armor->GetMaxHealth());
}

// ── Armor event hooks ──────────────────────────────────────────────────────

void Zombie::OnArmorStateChanged(Armor::DegradationState newState) {
    if (!m_Armor || !m_ArmorOverlay) return;

    const std::string key = m_Armor->GetCurrentSpritePath();
    if (!key.empty()) {
        auto image = ResourceManager::GetInstance().GetImage(key);
        if (image) {
            m_ArmorOverlay->SetDrawable(image);
            LOG_DEBUG("{} armor sprite updated (state {})", m_Name, static_cast<int>(newState));
        }
    }
}

void Zombie::OnArmorDestroyed() {
    // Hide the overlay — zombie body continues its current animation uninterrupted
    if (m_ArmorOverlay) {
        m_ArmorOverlay->SetVisible(false);
    }
    // Release the armor component
    m_Armor.reset();

    LOG_DEBUG("{} armor destroyed — reverting to basic sprite", m_Name);
}

void Zombie::ApplyChill(float duration) {
    if (m_State == State::DEAD || m_State == State::DYING || m_State == State::CHARRED) {
        return;
    }
    // Reset (or extend) the chill timer
    m_ChillTimer = duration;
    LOG_DEBUG("{} chilled for {:.1f}s", m_Name, duration);
}

void Zombie::OnPlantEncountered(std::shared_ptr<Plant> plant) {
    SetTargetPlant(plant);
    SetState(State::ATTACKING);
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

        case State::JUMPING:
            // Animation handled by subclass override; base does nothing.
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

        case State::CHARRED:
            // Reset death timer for charred animation
            m_DeathTimer = 0.0f;

            // Load charred animation from ResourceManager (shared across all zombie types)
            m_CharredAnimation = ResourceManager::GetInstance().CreateAnimation(
                ResourceManager::ZOMBIE_CHARRED, true);
            
            if (m_CharredAnimation) {
                m_CurrentAnimation = m_CharredAnimation;
                SetDrawable(m_CurrentAnimation);
                LOG_DEBUG("{} playing charred animation (Cherry Bomb)", m_Name);
            } else {
                // No charred animation - go straight to dead
                LOG_DEBUG("{} has no charred animation, dying immediately", m_Name);
                SetState(State::DEAD);
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
