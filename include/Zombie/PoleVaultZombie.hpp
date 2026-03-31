#ifndef POLEVAULT_ZOMBIE_HPP
#define POLEVAULT_ZOMBIE_HPP

#include "Zombie/Zombie.hpp"

/**
 * @file PoleVaultZombie.hpp
 * @brief Zombie that pole vaults over the first plant it encounters.
 *
 * PoleVaultZombie is a fast zombie with a unique ability:
 * - Health: 340 (same as conehead without cone bonus)
 * - Speed: 40 pixels/second (FAST) while carrying pole, 20 after
 * - Damage: 100 per second
 *
 * Special behavior: On first plant contact, plays jump animation
 * and lands behind the plant. Then becomes a normal slow zombie.
 * 
 * States: WALKING (fast with pole), JUMPING, WALKING_SLOW (no pole), ATTACKING, DYING, DEAD
 */

class PoleVaultZombie : public Zombie {
public:
    // PoleVaulter-specific constants
    static constexpr int POLEVAULT_HEALTH = 340;
    static constexpr float POLEVAULT_SPEED_FAST = 40.0f;   // With pole
    static constexpr float POLEVAULT_SPEED_SLOW = 20.0f;   // After jump
    static constexpr int POLEVAULT_DAMAGE = 100;

    PoleVaultZombie();
    virtual ~PoleVaultZombie() = default;

    /**
     * @brief Initialize zombie with animations from ResourceManager.
     */
    void Initialize() override;

    /**
     * @brief Update with special pole vault logic.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Check if the pole vaulter still has its pole.
     */
    bool HasPole() const { return m_HasPole; }

protected:
    /**
     * @brief Called when state changes to swap animations.
     */
    void OnStateChanged(State newState) override;

private:
    bool m_HasPole = true;
    float m_JumpTimer = 0.0f;
    static constexpr float JUMP_DURATION = 1.7f;  // ~35 frames @ 50ms
    
    std::shared_ptr<Util::Animation> m_JumpAnimation;
};

#endif // POLEVAULT_ZOMBIE_HPP
