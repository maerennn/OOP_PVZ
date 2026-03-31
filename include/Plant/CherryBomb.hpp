#ifndef CHERRYBOMB_HPP
#define CHERRYBOMB_HPP

#include "Plant/InstantKillPlant.hpp"
#include "Util/Animation.hpp"
#include <functional>
#include <memory>

/**
 * @file CherryBomb.hpp
 * @brief Concrete implementation of the Cherry Bomb explosive plant.
 *
 * Cherry Bomb demonstrates the InstantKillPlant pattern:
 * - Identity: Is-a InstantKillPlant (which is-a Plant)
 * - Ability: Inherited from InstantKillPlant (implements IAttacker)
 *
 * Cherry Bomb has a 3x3 area of effect and kills all zombies in range.
 * After exploding, the plant is destroyed (health = 0).
 *
 * Explosion Behavior:
 * 1. Plays fuse/countdown animation (~1.2 seconds)
 * 2. When animation ends, triggers explosion
 * 3. Applies 1800 damage to all zombies in 3x3 grid area
 * 4. Zombies killed by explosion use charred death animation
 * 5. Cherry Bomb destroys itself (health = 0)
 *
 * Animations loaded via ResourceManager.
 *
 * Stats:
 * - Damage: 1800 (instant kill for most zombies)
 * - Area: 3x3 cells
 * - Fuse Time: ~1.2 seconds (animation plays before explosion)
 * - Sun Cost: 150
 * - Recharge: Very Slow (50s)
 */

class CherryBomb : public InstantKillPlant {
public:
    // CherryBomb-specific constants
    static constexpr int CHERRYBOMB_DAMAGE = 1800;
    static constexpr float CHERRYBOMB_AREA_RADIUS = 1.5f;  // 3x3 cells
    static constexpr int CHERRYBOMB_SUN_COST = 150;
    static constexpr float CHERRYBOMB_RECHARGE = 50.0f;
    static constexpr float CHERRYBOMB_FUSE_TIME = 1.2f;

    // Callback for explosion effect
    using ExplosionCallback = std::function<void(glm::vec2 position, float radius, int damage)>;

    CherryBomb();
    virtual ~CherryBomb() = default;

    /**
     * @brief Initialize animation via ResourceManager.
     * @param frameDirectory Ignored - animation loaded from ResourceManager
     */
    void Initialize(const std::string& frameDirectory);

    /**
     * @brief Update fuse timer and check for animation completion.
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime) override;

    // Set callback for when explosion occurs
    void SetExplosionCallback(ExplosionCallback callback) {
        m_OnExplosion = std::move(callback);
    }

protected:
    // Override to create explosion effect
    void OnTrigger() override;

private:
    /**
     * @brief Trigger the explosion, damage zombies, and destroy self.
     */
    void Explode();

    ExplosionCallback m_OnExplosion;
    std::shared_ptr<Util::Animation> m_FuseAnimation;
    bool m_HasExploded = false;
    float m_FuseTimer = 0.0f;
};

#endif // CHERRYBOMB_HPP
