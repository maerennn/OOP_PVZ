#ifndef CHOMPER_HPP
#define CHOMPER_HPP

#include "Plant/Plant.hpp"
#include "Plant/IAttacker.hpp"
#include <functional>
#include <memory>

// Forward declaration — avoids pulling the full Zombie header into every TU
class Zombie;

/**
 * @file Chomper.hpp
 * @brief Close-range instant-kill plant with a 42-second chewing reload.
 *
 * === State Machine ===
 *
 *   IDLE  ──(zombie in range)──►  CAPTURE  ──(anim ends)──►  EATING
 *    ▲                                                            │
 *    └──────────────(anim ends)──── EATING_DONE ◄──(timer)───────┘
 *
 *   IDLE        Looping idle animation. Accepts a target via SetNearestZombie().
 *   CAPTURE     One-shot snap animation. The target zombie is INSTANTLY killed
 *               via m_OnChomp the moment this state is entered.
 *   EATING      Looping chew animation. 42-second timer. Cannot attack.
 *   EATING_DONE One-shot burp/reset animation. Transitions back to IDLE.
 *
 * === Integration with App ===
 *   App::UpdateChomperTargets() → SetNearestZombie(zombie)
 *   App::PlacePlant()           → SetChompCallback([](zombie){ zombie->TakeDamage(99999); })
 *
 * === Stats ===
 *   Sun Cost  : 150
 *   Health    : 300 (normal toughness)
 *   Recharge  : 30 s (seed-packet cooldown)
 *   Chew Time : 42 s
 *   Range     : ~2 cells to the right (~184 px)
 */
class Chomper : public Plant, public IAttacker {
public:
    // ── State ────────────────────────────────────────────────────────────
    enum class ChomperState {
        IDLE,
        CAPTURE,
        EATING,
        EATING_DONE
    };

    // ── Constants ────────────────────────────────────────────────────────
    static constexpr int   CHOMPER_SUN_COST  = 150;
    static constexpr int   CHOMPER_HEALTH    = DEFAULT_HEALTH;  // 300
    static constexpr float CHOMPER_RECHARGE  = 30.0f;           // seconds
    static constexpr float CHOMPER_CHEW_TIME = 42.0f;           // seconds
    static constexpr float CHOMPER_RANGE     = 184.0f;          // ~2 cell widths

    // Callback fired immediately when the IDLE→CAPTURE transition is made.
    // App should call zombie->TakeDamage(99999) to instantly remove the target.
    using ChompCallback = std::function<void(std::shared_ptr<Zombie>)>;

    // ── Lifecycle ────────────────────────────────────────────────────────
    explicit Chomper();
    ~Chomper() override = default;

    Chomper(const Chomper&)            = delete;
    Chomper& operator=(const Chomper&) = delete;

    /**
     * @brief Load animations from the ResourceManager.
     * @param frameDirectory Unused — animations are loaded by name constant.
     */
    void Initialize(const std::string& frameDirectory);

    // ── Plant interface ──────────────────────────────────────────────────
    void Update(float deltaTime) override;

    // ── IAttacker interface ──────────────────────────────────────────────
    void PerformAttack() override;
    int GetDamage() const override { return 9999; }
    float GetAttackInterval() const override { return CHOMPER_CHEW_TIME; }
    ProjectileType GetProjectileType() const override { return ProjectileType::NONE; }

    // ── Chomper-specific API (called by App) ─────────────────────────────

    /**
     * @brief Offer the nearest eligible zombie to the Chomper.
     * Accepted only in IDLE state; immediately transitions to CAPTURE
     * and fires the chomp callback.
     * @param zombie Nearest zombie in range (must not be nullptr).
     */
    void SetNearestZombie(std::shared_ptr<Zombie> zombie);

    /**
     * @brief Register the callback invoked at the start of CAPTURE.
     * App should call zombie->TakeDamage(99999) inside it.
     */
    void SetChompCallback(ChompCallback cb) { m_OnChomp = std::move(cb); }

    // ── Queries ──────────────────────────────────────────────────────────
    ChomperState GetChomperState() const { return m_State; }

    /** True while chewing (cannot accept a new target). */
    bool IsEating() const {
        return m_State == ChomperState::EATING ||
               m_State == ChomperState::EATING_DONE;
    }

private:
    void TransitionTo(ChomperState next);

    // ── State ────────────────────────────────────────────────────────────
    ChomperState m_State = ChomperState::IDLE;

    // ── Animations ───────────────────────────────────────────────────────
    std::shared_ptr<Util::Animation> m_IdleAnim;
    std::shared_ptr<Util::Animation> m_CaptureAnim;
    std::shared_ptr<Util::Animation> m_EatingAnim;
    std::shared_ptr<Util::Animation> m_EatingDoneAnim;

    // ── Timing ───────────────────────────────────────────────────────────
    float m_ChewTimer = 0.0f;

    // ── Target tracking ──────────────────────────────────────────────────
    std::shared_ptr<Zombie> m_CapturedZombie; // Held during CAPTURE animation

    // ── Callback ─────────────────────────────────────────────────────────
    ChompCallback m_OnChomp;
};

#endif // CHOMPER_HPP
