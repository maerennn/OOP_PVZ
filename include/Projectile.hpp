#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Util/GameObject.hpp"
#include "Plant/IAttacker.hpp"
#include <glm/glm.hpp>
#include <memory>

/**
 * @file Projectile.hpp
 * @brief Base class for all projectiles in the game.
 *
 * === Design Philosophy ===
 * Projectile represents the "identity" of a flying object:
 * - What type it is (Pea, Frozen Pea, etc.)
 * - How much damage it deals
 * - What lane it travels in
 * - How fast it moves
 *
 * Concrete projectiles (Pea, FrozenPea, Cabbage) inherit from this base.
 * The projectile system in App handles movement, collision, and removal.
 */

class Projectile : public Util::GameObject {
public:
    // Default projectile constants
    static constexpr float DEFAULT_SPEED = 300.0f;  // Pixels per second

    /**
     * @brief Construct a new Projectile.
     * @param type The projectile type (from IAttacker enum)
     * @param damage Damage dealt on hit
     * @param row The lane/row this projectile travels in
     * @param position Starting position
     * @param speed Movement speed (pixels/second)
     */
    Projectile(ProjectileType type,
               int damage,
               int row,
               const glm::vec2& position,
               float speed = DEFAULT_SPEED);

    virtual ~Projectile() = default;

    // Prevent copying, allow moving
    Projectile(const Projectile&) = delete;
    Projectile& operator=(const Projectile&) = delete;
    Projectile(Projectile&&) = default;
    Projectile& operator=(Projectile&&) = default;

    /**
     * @brief Initialize the projectile (load sprite/animation).
     * Must be called after construction.
     */
    virtual void Initialize() = 0;

    /**
     * @brief Update projectile state (movement).
     * @param deltaTime Time since last frame
     */
    virtual void Update(float deltaTime);

    /**
     * @brief Called when projectile hits a zombie.
     * Override for special effects (freeze, splash, etc.)
     */
    virtual void OnHit();

    // Accessors
    ProjectileType GetType() const { return m_Type; }
    int GetDamage() const { return m_Damage; }
    int GetRow() const { return m_Row; }
    float GetSpeed() const { return m_Speed; }

    // State
    bool IsActive() const { return m_IsActive; }
    void Deactivate() { m_IsActive = false; }

    /**
     * @brief Check if projectile is off-screen (should be removed).
     * @param screenRight Right edge of screen in world coordinates
     */
    bool IsOffScreen(float screenRight = 700.0f) const;

protected:
    ProjectileType m_Type;
    int m_Damage;
    int m_Row;
    float m_Speed;
    bool m_IsActive = true;
};

#endif // PROJECTILE_HPP
