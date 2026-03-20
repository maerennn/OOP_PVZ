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
    m_Transform.scale = {0.3f, 0.3f};
}

void Peashooter::OnAttack() {
    // TODO: Spawn a pea projectile in the game world
    // The projectile system would handle:
    // - Creating a Pea at this plant's position
    // - Moving it across the lane
    // - Collision detection with zombies
    // - Applying damage on hit
    //
    // Example pseudocode:
    // auto pea = std::make_shared<Projectile>(
    //     ProjectileType::PEA,
    //     m_Damage,
    //     GetRow(),
    //     m_Transform.translation
    // );
    // GameWorld::SpawnProjectile(pea);
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
