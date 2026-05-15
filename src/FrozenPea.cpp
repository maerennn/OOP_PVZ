#include "FrozenPea.hpp"
#include "Zombie/Zombie.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"

FrozenPea::FrozenPea(int damage, int row, const glm::vec2& position)
    : Projectile(ProjectileType::FROZEN_PEA, damage, row, position, FROZEN_PEA_SPEED)
{
}

void FrozenPea::Initialize() {
    auto image = std::make_shared<Util::Image>(
        RESOURCE_DIR "/images/ProjectileSnowPea.png"
    );
    SetDrawable(image);
    m_Transform.scale = {1.25f, 1.25f};
}

void FrozenPea::OnHit() {
    // Deactivate the projectile (same as base Pea)
    Projectile::OnHit();
}

void FrozenPea::OnZombieHit(Zombie& zombie) {
    zombie.ApplyChill(CHILL_DURATION);
}
