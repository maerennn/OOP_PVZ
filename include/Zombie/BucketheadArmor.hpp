#ifndef BUCKETHEAD_ARMOR_HPP
#define BUCKETHEAD_ARMOR_HPP

#include "Zombie/Armor.hpp"

/**
 * @file BucketheadArmor.hpp
 * @brief Metal-bucket armor component — 1100 HP, 3 degradation states.
 *
 * Damage thresholds (cumulative damage absorbed):
 *   0   – 349  : INTACT       → Zombie_bucket1.png  (fully intact)
 *   350 – 699  : MINOR_DAMAGE → Zombie_bucket2.png  (small dent on top)
 *   700 – 1099 : HEAVY_DAMAGE → Zombie_bucket3.png  (heavy dent + red stain)
 *   1100+      : DESTROYED    → overlay hidden, zombie reverts to basic sprite
 *
 * Note: The zombie's total HP is 1300 (200 base + 1100 armor).
 */
class BucketheadArmor : public Armor {
public:
    static constexpr int MAX_HEALTH = 1100;

    // Cumulative-damage thresholds for visual state changes
    static constexpr int THRESHOLD_MINOR = 350;
    static constexpr int THRESHOLD_HEAVY = 700;

    BucketheadArmor() : Armor(MAX_HEALTH) {}
    ~BucketheadArmor() override = default;

    std::string GetCurrentSpritePath() const override;

    // Overlay position relative to the zombie's world centre.
    float GetOverlayXOffset() const override { return 10.0f; }
    float GetOverlayYOffset() const override { return 60.0f; }

protected:
    void UpdateDegradationState() override;
};

#endif // BUCKETHEAD_ARMOR_HPP
