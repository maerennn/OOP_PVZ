#include "Plant/Peashooter.hpp"
#include <cstdio>

Peashooter::Peashooter()
    : ShooterPlant("Peashooter",
                   PEASHOOTER_DAMAGE,
                   PEASHOOTER_ATTACK_INTERVAL,
                   ProjectileType::PEA,
                   DEFAULT_HEALTH,
                   PEASHOOTER_SUN_COST,
                   PEASHOOTER_RECHARGE)
{
}

void Peashooter::Initialize(const std::string& frameDirectory) {
    auto frames = GetAnimationFrames(frameDirectory);
    SetupAnimation(frames, 50, true);

    // Default scale for plant sprite
    m_Transform.scale = {0.28f, 0.28f};
}

void Peashooter::OnAttack() {
    // Use base class implementation which fires the callback
    // This allows App to spawn the actual projectile
    ShooterPlant::OnAttack();

    // Peashooter-specific behavior could be added here:
    // - Play shooting sound
    // - Trigger shooting animation
}

std::vector<std::string> Peashooter::GetAnimationFrames(const std::string& frameDir) {
    std::vector<std::string> frames;
    frames.reserve(PEASHOOTER_FRAME_COUNT);

    for (int i = 1; i <= PEASHOOTER_FRAME_COUNT; ++i) {
        char filename[64];
        std::snprintf(filename, sizeof(filename), "/PeashooterIdle%04d.png", i);
        frames.push_back(frameDir + filename);
    }

    return frames;
}
