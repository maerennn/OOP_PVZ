#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

// Forward declaration
class Plant;

/**
 * @file Zombie.hpp
 * @brief Base class for all zombies in the game.
 *
 * === Design Philosophy ===
 * Zombie represents the "identity" of an enemy:
 * - Health: How much damage it can take before dying
 * - Speed: How fast it moves toward the house
 * - Row/Lane: Which horizontal lane it walks in
 * - State: Current behavior (WALKING, ATTACKING, DYING, DEAD)
 *
 * The state machine logic is ENCAPSULATED within the Zombie class.
 * Zombie::Update() handles all state transitions and behavior internally.
 *
 * Concrete zombies (NormalZombie, ConeheadZombie) inherit from this base.
 */

class Zombie : public Util::GameObject {
public:
    /**
     * @brief Zombie behavior states.
     */
    enum class State {
        WALKING,    // Moving toward the house
        JUMPING,    // Mid-air (pole vault / leap)
        ATTACKING,  // Eating a plant
        DYING,      // Playing death animation
        CHARRED,    // Burnt by explosion (Cherry Bomb)
        DEAD        // Ready for removal
    };

    // Death types for different death animations
    enum class DeathType {
        NORMAL,     // Standard death animation
        EXPLOSION   // Charred/burnt animation (Cherry Bomb)
    };

    // Default zombie constants
    static constexpr int DEFAULT_HEALTH = 200;
    static constexpr float DEFAULT_SPEED = 20.0f;  // Pixels per second
    static constexpr int DEFAULT_DAMAGE = 100;     // Damage per second to plants
    static constexpr float HITBOX_WIDTH = 40.0f;
    static constexpr float HITBOX_HEIGHT = 80.0f;

    /**
     * @brief Construct a new Zombie.
     * @param name Zombie type name
     * @param health Starting health
     * @param speed Movement speed (pixels/second)
     * @param damage Damage dealt to plants per second
     */
    Zombie(const std::string& name,
           int health = DEFAULT_HEALTH,
           float speed = DEFAULT_SPEED,
           int damage = DEFAULT_DAMAGE);

    virtual ~Zombie() = default;

    // Prevent copying, allow moving
    Zombie(const Zombie&) = delete;
    Zombie& operator=(const Zombie&) = delete;
    Zombie(Zombie&&) = default;
    Zombie& operator=(Zombie&&) = default;

    /**
     * @brief Initialize zombie animations.
     * Must be called after construction. Subclasses implement this.
     */
    virtual void Initialize() = 0;

    /**
     * @brief Update zombie state and behavior.
     * Handles state machine logic internally:
     * - WALKING: Move left
     * - ATTACKING: Stay in place, deal damage
     * - DYING: Play death animation
     * - DEAD: Mark for removal
     * @param deltaTime Time since last frame
     */
    virtual void Update(float deltaTime);

    /**
     * @brief Called when the zombie first contacts a plant while walking.
     * Default: set target plant and switch to ATTACKING.
     * Override (e.g. PoleVaultZombie) to jump instead.
     */
    virtual void OnPlantEncountered(std::shared_ptr<Plant> plant);

    /**
     * @brief Y-axis offset (OpenGL units) applied on spawn to align the sprite
     * with its lane.  Override when the zombie's visual centre is not at the
     * geometric centre of its canvas.  Positive = shift up on screen.
     */
    virtual float GetSpriteYOffset() const { return 0.0f; }

    /**
     * @brief Apply damage to the zombie.
     * @param amount Damage to apply
     */
    void TakeDamage(int amount);

    /**
     * @brief Apply explosion damage (Cherry Bomb) - uses charred death animation.
     * @param amount Damage to apply
     */
    void TakeExplosionDamage(int amount);

    // State management
    State GetState() const { return m_State; }
    void SetState(State newState);

    // Accessors
    const std::string& GetName() const { return m_Name; }
    int GetHealth() const { return m_Health; }
    int GetMaxHealth() const { return m_MaxHealth; }
    float GetSpeed() const { return m_Speed; }
    int GetDamage() const { return m_Damage; }
    int GetRow() const { return m_Row; }
    void SetRow(int row) { m_Row = row; }

    // State queries
    bool IsAlive() const { return m_Health > 0; }
    bool IsChilled() const { return m_ChillTimer > 0.0f; }

    /**
     * @brief Apply a chill/slow effect for the given duration.
     * While chilled the zombie moves and attacks at half speed.
     * Re-applying resets the timer.
     * @param duration Seconds to remain chilled
     */
    void ApplyChill(float duration);
    bool IsDead() const { return m_State == State::DEAD; }
    bool IsAttacking() const { return m_State == State::ATTACKING; }
    bool ShouldRemove() const { return m_State == State::DEAD; }

    // Target plant for attacking (set by App collision system)
    void SetTargetPlant(std::shared_ptr<Plant> plant) { m_TargetPlant = plant; }
    std::shared_ptr<Plant> GetTargetPlant() const { return m_TargetPlant.lock(); }
    void ClearTargetPlant() { m_TargetPlant.reset(); }

    // Collision helpers
    float GetLeftEdge() const { return m_Transform.translation.x - HITBOX_WIDTH / 2; }
    float GetRightEdge() const { return m_Transform.translation.x + HITBOX_WIDTH / 2; }

protected:
    /**
     * @brief Called when state changes. Override to swap animations.
     * @param newState The state being transitioned to
     */
    virtual void OnStateChanged(State newState);

    /**
     * @brief Set up an animation from frame paths.
     */
    void SetupAnimation(const std::vector<std::string>& frames,
                        int frameInterval = 50,
                        bool looping = true);

    // Identity
    std::string m_Name;

    // Stats
    int m_Health;
    int m_MaxHealth;
    float m_Speed;
    int m_Damage;

    // Position
    int m_Row = 0;

    // State machine
    State m_State = State::WALKING;
    DeathType m_DeathType = DeathType::NORMAL;

    // Target plant (for attacking state) — weak_ptr avoids dangling if plant is removed
    std::weak_ptr<Plant> m_TargetPlant;

    // Death timer (fallback if animation state detection fails)
    float m_DeathTimer = 0.0f;
    static constexpr float DEATH_ANIMATION_DURATION  = 4.5f;  // ~92 frames @ 50ms
    static constexpr float CHARRED_ANIMATION_DURATION = 1.9f;  // ~38 frames @ 50ms

    // Chill (Snow Pea) effect
    float m_ChillTimer = 0.0f;
    static constexpr float CHILL_SPEED_FACTOR = 0.5f;

    // Animation
    std::shared_ptr<Util::Animation> m_CurrentAnimation;
    std::shared_ptr<Util::Animation> m_WalkingAnimation;
    std::shared_ptr<Util::Animation> m_AttackingAnimation;
    std::shared_ptr<Util::Animation> m_DyingAnimation;
    std::shared_ptr<Util::Animation> m_CharredAnimation;
};

#endif // ZOMBIE_HPP
