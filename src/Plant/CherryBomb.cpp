#include "Plant/CherryBomb.hpp"
#include <cstdio>

CherryBomb::CherryBomb()
    : InstantKillPlant("Cherry Bomb",
                       CHERRYBOMB_DAMAGE,
                       CHERRYBOMB_AREA_RADIUS,
                       CHERRYBOMB_SUN_COST,
                       CHERRYBOMB_RECHARGE,
                       CHERRYBOMB_FUSE_TIME)
{
    // Cherry Bomb triggers immediately when planted
    // The fuse time is for the explosion animation
}

void CherryBomb::Initialize(const std::string& frameDirectory) {
    auto frames = GetAnimationFrames(frameDirectory);
    SetupAnimation(frames, 50, false);  // Don't loop - plays once then explodes

    // Default scale for plant sprite
    m_Transform.scale = {0.4f, 0.4f};

    // Auto-trigger on placement (Cherry Bomb explodes shortly after planting)
    Trigger();
}

void CherryBomb::OnTrigger() {
    // Notify callback to create explosion effects and damage zombies
    if (m_OnExplosion) {
        m_OnExplosion(m_Transform.translation, m_AreaRadius, m_Damage);
    }

    // The base class OnTriggerComplete() will set health to 0
    // The game world should then remove this plant

    // TODO: The game world would handle:
    // - Creating explosion visual effect (screen flash, particle burst)
    // - Playing explosion sound
    // - Finding all zombies within CHERRYBOMB_AREA_RADIUS cells
    // - Applying CHERRYBOMB_DAMAGE to each (instant kill)
    // - Removing this plant from the grid
}

std::vector<std::string> CherryBomb::GetAnimationFrames(const std::string& frameDir) {
    std::vector<std::string> frames;

    // Cherry Bomb has a short animation before explosion
    // Typical frame count: 13 frames
    constexpr int FRAME_COUNT = 13;
    frames.reserve(FRAME_COUNT);

    for (int i = 1; i <= FRAME_COUNT; ++i) {
        char filename[64];
        std::snprintf(filename, sizeof(filename), "/CherryBomb%04d.png", i);
        frames.push_back(frameDir + filename);
    }

    return frames;
}
