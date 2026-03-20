#ifndef WALLNUT_HPP
#define WALLNUT_HPP

#include "Plant/DefensePlant.hpp"

/**
 * @file WallNut.hpp
 * @brief Concrete implementation of the Wall-nut defensive plant.
 *
 * Wall-nut demonstrates the DefensePlant pattern:
 * - Identity: Is-a DefensePlant (which is-a Plant)
 * - Ability: None (no interfaces implemented - pure defense)
 *
 * Wall-nut does NOT implement IAttacker or IProducer.
 * Its purpose is solely to block zombies with its high HP.
 *
 * Stats:
 * - Health: 4000 (very high)
 * - Sun Cost: 50
 * - Recharge: Slow (30s)
 *
 * Visual States:
 * - Healthy (67-100%): Full, undamaged appearance
 * - Damaged (34-66%): Cracked shell, showing bite marks
 * - Critical (0-33%): Severely damaged, almost eaten through
 */

class WallNut : public DefensePlant {
public:
    // WallNut-specific constants
    static constexpr int WALLNUT_HEALTH = 4000;
    static constexpr int WALLNUT_SUN_COST = 50;
    static constexpr float WALLNUT_RECHARGE = 30.0f;

    WallNut();
    virtual ~WallNut() = default;

    // Initialize with animation frames for all damage stages
    void Initialize(const std::string& frameDirectory);

    // Alternative: Initialize with separate frame directories for each stage
    void Initialize(const std::string& healthyFrameDir,
                    const std::string& damagedFrameDir,
                    const std::string& criticalFrameDir);

protected:
    // Override to customize visual transition effects
    void OnDamageStageChanged(DamageStage newStage) override;

private:
    static std::vector<std::string> GetAnimationFrames(
        const std::string& frameDir,
        const std::string& prefix,
        int frameCount);
};

#endif // WALLNUT_HPP
