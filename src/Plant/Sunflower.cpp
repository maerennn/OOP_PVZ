#include "Plant/Sunflower.hpp"
#include <cstdio>

Sunflower::Sunflower()
    : Plant("Sunflower",
            DEFAULT_HEALTH,
            SUNFLOWER_SUN_COST,
            SUNFLOWER_RECHARGE)
{
}

void Sunflower::Initialize(const std::string& frameDirectory) {
    auto frames = GetAnimationFrames(frameDirectory);
    SetupAnimation(frames, 50, true);

    // Default scale for plant sprite
    m_Transform.scale = {0.3f, 0.3f};
}

void Sunflower::Update(float deltaTime) {
    Plant::Update(deltaTime);

    // Accumulate time for sun production
    m_ProductionTimer += deltaTime;

    // Produce sun when ready
    if (CanProduce()) {
        Produce();
    }
}

void Sunflower::Produce() {
    // Reset timer
    m_ProductionTimer = 0.0f;

    // Notify callback (typically spawns a collectible sun object)
    if (m_OnSunProduced) {
        m_OnSunProduced(m_ProductionAmount, m_Transform.translation);
    }

    // TODO: The game world would handle:
    // - Creating a Sun object near this plant
    // - Animating it floating up slightly
    // - Making it clickable to collect
    // - Adding the sun value to player's total
}

std::vector<std::string> Sunflower::GetAnimationFrames(const std::string& frameDir) {
    std::vector<std::string> frames;
    frames.reserve(SUNFLOWER_FRAME_COUNT);

    // // First frame has different naming convention
    frames.push_back(frameDir + "/Sunflower0005.png");

    // Remaining frames: Sunflower0002.png through Sunflower0029.png
    for (int i = 5; i <= SUNFLOWER_FRAME_COUNT; ++i) {
        char filename[64];
        std::snprintf(filename, sizeof(filename), "/Sunflower%04d.png", i);
        frames.push_back(frameDir + filename);
    }

    return frames;
}
