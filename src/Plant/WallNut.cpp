#include "Plant/WallNut.hpp"
#include <cstdio>

WallNut::WallNut()
    : DefensePlant("Wall-nut",
                   WALLNUT_HEALTH,
                   WALLNUT_SUN_COST,
                   WALLNUT_RECHARGE)
{
}

void WallNut::Initialize(const std::string& frameDirectory) {
    // Load all three damage stage animations from the same directory
    // Assumes naming convention:
    // - WallNut0001.png ... (healthy)
    // - WallNutDamaged0001.png ... (damaged)
    // - WallNutCritical0001.png ... (critical)

    auto healthyFrames = GetAnimationFrames(frameDirectory, "WallNut", 16);
    auto damagedFrames = GetAnimationFrames(frameDirectory, "WallNutDamaged", 11);
    auto criticalFrames = GetAnimationFrames(frameDirectory, "WallNutCritical", 13);

    SetupDamageStageAnimations(healthyFrames, damagedFrames, criticalFrames, 80);

    // Default scale for plant sprite
    m_Transform.scale = {0.3f, 0.3f};
}

void WallNut::Initialize(const std::string& healthyFrameDir,
                         const std::string& damagedFrameDir,
                         const std::string& criticalFrameDir) {
    auto healthyFrames = GetAnimationFrames(healthyFrameDir, "WallNut", 16);
    auto damagedFrames = GetAnimationFrames(damagedFrameDir, "WallNutDamaged", 11);
    auto criticalFrames = GetAnimationFrames(criticalFrameDir, "WallNutCritical", 13);

    SetupDamageStageAnimations(healthyFrames, damagedFrames, criticalFrames, 80);

    m_Transform.scale = {0.3f, 0.3f};
}

void WallNut::OnDamageStageChanged(DamageStage newStage) {
    // Call parent implementation to handle animation switching
    DefensePlant::OnDamageStageChanged(newStage);

    // Additional effects could be added here:
    // - Play crunch sound effect
    // - Spawn particle effects (crumbs, etc.)
    // - Screen shake on critical
}

std::vector<std::string> WallNut::GetAnimationFrames(
    const std::string& frameDir,
    const std::string& prefix,
    int frameCount)
{
    std::vector<std::string> frames;
    frames.reserve(frameCount);

    for (int i = 1; i <= frameCount; ++i) {
        char filename[128];
        std::snprintf(filename, sizeof(filename), "/%s%04d.png", prefix.c_str(), i);
        frames.push_back(frameDir + filename);
    }

    return frames;
}
