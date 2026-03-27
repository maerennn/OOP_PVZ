#include "Pea.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"

Pea::Pea(int damage, int row, const glm::vec2& position)
    : Projectile(ProjectileType::PEA, damage, row, position, PEA_SPEED)
{
}

void Pea::Initialize() {
    // Load pea sprite
    auto peaImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/images/ProjectilePea.png"
    );
    SetDrawable(peaImage);

    // Scale the pea sprite appropriately
    m_Transform.scale = {1.25f, 1.25f};
}

void Pea::OnHit() {
    // Standard pea: just deactivate (damage is applied by collision system)
    Projectile::OnHit();

    // Future: Could add hit particles/sound here
}
