#ifndef POTATOMINE_HPP
#define POTATOMINE_HPP

#include "Plant/InstantKillPlant.hpp"
#include "Util/Animation.hpp"
#include <functional>
#include <memory>
#include <string>

/**
 * @file PotatoMine.hpp
 * @brief Concrete implementation of the Potato Mine plant.
 *
 * Potato Mine is an inexpensive instant-kill plant with an arming delay.
 * After being planted it spends 15 seconds underground (unarmed).
 * Once armed, the first zombie that steps on it triggers an explosion
 * that instantly kills zombies in a small area.
 *
 * Arming Behavior:
 * 1. Planted: shows dormant sprite, m_IsArmed = false
 * 2. After 15 seconds: pops up, m_IsArmed = true, swaps to armed animation
 * 3. Zombie contact: App calls Trigger() → OnTrigger() → fires explosion callback
 * 4. Explosion kills nearby zombies and destroys the mine
 *
 * Stats:
 * - Damage: 1800 (instant kill for most zombies)
 * - Area: ~0.6 cell radius (contact kill)
 * - Arm Time: 15 seconds
 * - Sun Cost: 25
 * - Recharge: Very Slow (30s)
 */

class PotatoMine : public InstantKillPlant {
public:
    static constexpr int   POTATOMINE_DAMAGE      = 1800;
    static constexpr float POTATOMINE_AREA_RADIUS = 0.6f;   // contact-range blast
    static constexpr int   POTATOMINE_SUN_COST    = 25;
    static constexpr float POTATOMINE_RECHARGE    = 30.0f;
    static constexpr float POTATOMINE_ARM_TIME    = 15.0f;

    using ExplosionCallback = std::function<void(glm::vec2 position, float radius, int damage)>;

    PotatoMine();
    virtual ~PotatoMine() = default;

    void Initialize(const std::string& frameDirectory);
    void Update(float deltaTime) override;

    void SetExplosionCallback(ExplosionCallback callback) {
        m_OnExplosion = std::move(callback);
    }

protected:
    void OnTrigger() override;

private:
    bool m_WasArmed  = false;   // Tracks arm-state transition to swap animation once
    bool m_IsArming  = false;   // True while the one-shot arming transition plays

    ExplosionCallback m_OnExplosion;
    std::shared_ptr<Util::Animation> m_UnarmedAnimation;
    std::shared_ptr<Util::Animation> m_ArmingAnimation;
    std::shared_ptr<Util::Animation> m_ArmedAnimation;
};

#endif // POTATOMINE_HPP
