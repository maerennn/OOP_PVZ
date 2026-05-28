#include "Plant/Chomper.hpp"
#include "Zombie/Zombie.hpp"
#include "ResourceManager.hpp"
#include "Util/Logger.hpp"

// ── Constructor ──────────────────────────────────────────────────────────────

Chomper::Chomper()
    : Plant("Chomper", CHOMPER_HEALTH, CHOMPER_SUN_COST, CHOMPER_RECHARGE)
{
}

// ── Initialization ───────────────────────────────────────────────────────────

void Chomper::Initialize(const std::string& /*frameDirectory*/) {
    auto& rm = ResourceManager::GetInstance();

    m_IdleAnim       = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_IDLE,       true);
    m_CaptureAnim    = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_CAPTURE,    false);
    m_EatingAnim     = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_EATING,     true);
    m_EatingDoneAnim = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_EATING_DONE, false);

    SetDrawable(m_IdleAnim);
    m_Transform.scale = {0.25f, 0.25f};


    LOG_DEBUG("Chomper initialized and in IDLE state");
}

// ── IAttacker::PerformAttack ─────────────────────────────────────────────────
// Detection is App-driven via SetNearestZombie(); this is a no-op stub.

void Chomper::PerformAttack() {}

// ── Target handoff (called by App::UpdateChomperTargets) ────────────────────

void Chomper::SetNearestZombie(std::shared_ptr<Zombie> zombie) {
    if (m_State != ChomperState::IDLE) return;
    if (!zombie) return;

    m_CapturedZombie = std::move(zombie);
    TransitionTo(ChomperState::CAPTURE);
}

// ── Update ───────────────────────────────────────────────────────────────────

void Chomper::Update(float deltaTime) {
    Plant::Update(deltaTime);

    switch (m_State) {

        case ChomperState::IDLE:
            // Waiting — App drives target injection via SetNearestZombie().
            break;

        case ChomperState::CAPTURE:
            // The zombie was already killed in TransitionTo(CAPTURE).
            // Wait for the one-shot snap animation to finish.
            if (m_CaptureAnim &&
                m_CaptureAnim->GetState() == Util::Animation::State::ENDED) {
                TransitionTo(ChomperState::EATING);
            }
            break;

        case ChomperState::EATING:
            // Accumulate chew time.
            m_ChewTimer += deltaTime;
            if (m_ChewTimer >= CHOMPER_CHEW_TIME) {
                m_ChewTimer = 0.0f;
                TransitionTo(ChomperState::EATING_DONE);
            }
            break;

        case ChomperState::EATING_DONE:
            // Wait for the one-shot burp/reset animation to finish.
            if (m_EatingDoneAnim &&
                m_EatingDoneAnim->GetState() == Util::Animation::State::ENDED) {
                TransitionTo(ChomperState::IDLE);
            }
            break;
    }
}

// ── State machine ────────────────────────────────────────────────────────────

void Chomper::TransitionTo(ChomperState next) {
    m_State = next;
    auto& rm = ResourceManager::GetInstance();

    switch (next) {

        case ChomperState::IDLE:
            // Obtain a fresh looping idle animation.
            m_IdleAnim = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_IDLE, true);
            SetDrawable(m_IdleAnim);
            LOG_DEBUG("Chomper [{}:{}] → IDLE", GetRow(), GetCol());
            break;

        case ChomperState::CAPTURE:
            // ── Instant kill: fire callback BEFORE the animation plays ──
            // The zombie is removed from the game this same frame, so it
            // can never start attacking the Chomper.
            if (m_OnChomp && m_CapturedZombie) {
                m_OnChomp(m_CapturedZombie);
            }
            m_CapturedZombie.reset();  // Release ownership

            // Obtain a fresh one-shot capture animation.
            m_CaptureAnim = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_CAPTURE, true);
            SetDrawable(m_CaptureAnim);
            LOG_DEBUG("Chomper [{}:{}] → CAPTURE (zombie instantly killed)", GetRow(), GetCol());
            break;

        case ChomperState::EATING:
            m_ChewTimer = 0.0f;
            // Obtain a fresh looping eating animation.
            m_EatingAnim = rm.CreateAnimation(ResourceManager::PLANT_CHOMPER_EATING, true);
            SetDrawable(m_EatingAnim);
            LOG_DEBUG("Chomper [{}:{}] → EATING (42 s chew timer started)", GetRow(), GetCol());
            break;

        case ChomperState::EATING_DONE:
            // Obtain a fresh one-shot eating-done animation.
            m_EatingDoneAnim = rm.CreateAnimation(
                ResourceManager::PLANT_CHOMPER_EATING_DONE, true);
            SetDrawable(m_EatingDoneAnim);
            LOG_DEBUG("Chomper [{}:{}] → EATING_DONE (transitioning back)", GetRow(), GetCol());
            break;
    }
}
