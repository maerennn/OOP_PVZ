#ifndef ARMOR_HPP
#define ARMOR_HPP

#include <string>

/**
 * @file Armor.hpp
 * @brief Abstract component for zombie head-armor (Composition over Inheritance).
 *
 * === Design ===
 * Armor is a pure-data component — it has no rendering or GameObject logic.
 * It is owned by a Zombie via unique_ptr<Armor> and drives two concerns:
 *
 *   1. Damage absorption — TakeDamage() absorbs incoming damage and returns
 *      any overflow that must flow through to the zombie's base HP.
 *
 *   2. Visual state — GetCurrentSpritePath() returns the correct overlay
 *      image path for the current degradation level so the Zombie can
 *      update its armor-overlay child GameObject.
 *
 * === Subclasses ===
 *   ConeheadArmor   — 370 HP, 3 visual states
 *   BucketheadArmor — 1100 HP, 3 visual states
 */
class Armor {
public:
    /**
     * @brief Degradation levels shared by all armor types.
     * The exact HP thresholds per level are defined by subclasses.
     */
    enum class DegradationState {
        INTACT,       ///< Full / light damage  (sprite 1)
        MINOR_DAMAGE, ///< Noticeable wear       (sprite 2)
        HEAVY_DAMAGE, ///< Heavily damaged        (sprite 3)
        DESTROYED     ///< HP depleted — armor gone
    };

    virtual ~Armor() = default;

    // Non-copyable — owned exclusively by one Zombie
    Armor(const Armor&) = delete;
    Armor& operator=(const Armor&) = delete;

    /**
     * @brief Absorb up to @p amount damage.
     *
     * Damage is applied to the armor first.  If the armor's remaining HP
     * cannot absorb everything, the remainder (overflow) is returned so the
     * caller can apply it to the zombie's base health.
     *
     * @param amount  Incoming damage (must be > 0).
     * @return        Overflow damage that bypasses the armor (0 if fully absorbed).
     */
    int TakeDamage(int amount);

    // ── Queries ────────────────────────────────────────────────────────────

    bool             IsDestroyed()          const { return m_Health <= 0; }
    int              GetHealth()            const { return m_Health; }
    int              GetMaxHealth()         const { return m_MaxHealth; }
    int              GetTotalDamageTaken()  const { return m_TotalDamageTaken; }
    DegradationState GetState()             const { return m_State; }

    /**
     * @brief Absolute path to the static overlay sprite for the current state.
     * Returns an empty string when the armor is DESTROYED.
     */
    virtual std::string GetCurrentSpritePath() const = 0;

    /**
     * @brief Pixel offsets (in game-world units) from the zombie's centre
     * to the correct position for the armor overlay sprite.
     * Must be tuned per armor type so the head-piece aligns with the body.
     */
    virtual float GetOverlayXOffset() const = 0;
    virtual float GetOverlayYOffset() const = 0;

protected:
    explicit Armor(int maxHealth)
        : m_MaxHealth(maxHealth)
        , m_Health(maxHealth)
    {}

    /**
     * @brief Recompute m_State after m_TotalDamageTaken has been updated.
     * Subclasses define the HP thresholds for each visual level.
     */
    virtual void UpdateDegradationState() = 0;

    int              m_MaxHealth;
    int              m_Health;
    int              m_TotalDamageTaken = 0;
    DegradationState m_State = DegradationState::INTACT;
};

#endif // ARMOR_HPP
