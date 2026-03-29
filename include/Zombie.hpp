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
        ATTACKING,  // Eating a plant
        DYING,      // Playing death animation
        DEAD        // Ready for removal
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
     * @brief Apply damage to the zombie.
     * @param amount Damage to apply
     */
    void TakeDamage(int amount);

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
    bool IsDead() const { return m_State == State::DEAD; }
    bool IsAttacking() const { return m_State == State::ATTACKING; }
    bool ShouldRemove() const { return m_State == State::DEAD; }

    // Target plant for attacking (set by App collision system)
    void SetTargetPlant(Plant* plant) { m_TargetPlant = plant; }
    Plant* GetTargetPlant() const { return m_TargetPlant; }
    void ClearTargetPlant() { m_TargetPlant = nullptr; }

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

    // Target plant (for attacking state)
    Plant* m_TargetPlant = nullptr;

    // Death timer (fallback if animation state detection fails)
    float m_DeathTimer = 0.0f;
    static constexpr float DEATH_ANIMATION_DURATION = 4.5f;  // ~92 frames @ 50ms

    // Animation
    std::shared_ptr<Util::Animation> m_CurrentAnimation;
    std::shared_ptr<Util::Animation> m_WalkingAnimation;
    std::shared_ptr<Util::Animation> m_AttackingAnimation;
    std::shared_ptr<Util::Animation> m_DyingAnimation;
};

#endif // ZOMBIE_HPP
