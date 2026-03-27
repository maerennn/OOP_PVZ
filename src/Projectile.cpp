#include "Projectile.hpp"
#include "GameConfig.hpp"

Projectile::Projectile(ProjectileType type,
                       int damage,
                       int row,
                       const glm::vec2& position,
                       float speed)
    : m_Type(type)
    , m_Damage(damage)
    , m_Row(row)
    , m_Speed(speed)
{
    m_Transform.translation = position;
    SetZIndex(GameConfig::ZIndex::PROJECTILE);
}

void Projectile::Update(float deltaTime) {
    if (!m_IsActive) return;

    // Move right across the screen
    m_Transform.translation.x += m_Speed * deltaTime;
}

void Projectile::OnHit() {
    // Base implementation: just deactivate
    // Subclasses can override for special effects
    Deactivate();
}

bool Projectile::IsOffScreen(float screenRight) const {
    return m_Transform.translation.x > screenRight;
}
