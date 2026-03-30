#ifndef BUCKETHEAD_ZOMBIE_HPP
#define BUCKETHEAD_ZOMBIE_HPP

#include "Zombie.hpp"

/**
 * @file BucketheadZombie.hpp
 * @brief Zombie with a bucket on its head for maximum protection.
 *
 * BucketheadZombie is the toughest basic zombie variant:
 * - Health: 1300 (200 base + 1100 bucket absorption)
 * - Speed: 20 pixels/second (same as basic)
 * - Damage: 100 per second
 *
 * The bucket absorbs 1100 damage before falling off, making this
 * zombie very difficult to kill without multiple attackers.
 */

class BucketheadZombie : public Zombie {
public:
    // Buckethead-specific constants
    static constexpr int BUCKETHEAD_HEALTH = 1300;    // 200 base + 1100 bucket
    static constexpr float BUCKETHEAD_SPEED = 20.0f;
    static constexpr int BUCKETHEAD_DAMAGE = 100;

    BucketheadZombie();
    virtual ~BucketheadZombie() = default;

    /**
     * @brief Initialize zombie with animations from ResourceManager.
     */
    void Initialize() override;

protected:
    /**
     * @brief Called when state changes to swap animations.
     */
    void OnStateChanged(State newState) override;
};

#endif // BUCKETHEAD_ZOMBIE_HPP
