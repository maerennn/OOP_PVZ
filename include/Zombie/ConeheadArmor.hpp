#ifndef CONEHEAD_ARMOR_HPP
#define CONEHEAD_ARMOR_HPP

#include "Zombie/Armor.hpp"

/**
 * @file ConeheadArmor.hpp
 * @brief Traffic-cone armor component — 370 HP, 3 degradation states.
 *
 * Damage thresholds (cumulative damage absorbed):
 *   0   – 129  : INTACT       → Zombie_cone1.png
 *   130 – 259  : MINOR_DAMAGE → Zombie_cone2.png  (noticeable scuffs)
 *   260 – 369  : HEAVY_DAMAGE → Zombie_cone3.png  (heavily dented)
 *   370+       : DESTROYED    → overlay hidden, zombie reverts to basic sprite
 */
class ConeheadArmor : public Armor {
public:
    static constexpr int MAX_HEALTH = 370;

    // Cumulative-damage thresholds for visual state changes
    static constexpr int THRESHOLD_MINOR  = 130;
    static constexpr int THRESHOLD_HEAVY  = 260;

    ConeheadArmor() : Armor(MAX_HEALTH) {}
    ~ConeheadArmor() override = default;

    std::string GetCurrentSpritePath() const override;

    // Overlay position relative to the zombie's world centre.
    // Positive Y = up on screen (OpenGL convention used by this engine).
    float GetOverlayXOffset() const override { return 10.0f; }
    float GetOverlayYOffset() const override { return 65.0f; }

protected:
    void UpdateDegradationState() override;
};

#endif // CONEHEAD_ARMOR_HPP
