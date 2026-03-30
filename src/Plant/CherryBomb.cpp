#include "Plant/CherryBomb.hpp"
#include "ResourceManager.hpp"

CherryBomb::CherryBomb()
    : InstantKillPlant("Cherry Bomb",
                       CHERRYBOMB_DAMAGE,
                       CHERRYBOMB_AREA_RADIUS,
                       CHERRYBOMB_SUN_COST,
                       CHERRYBOMB_RECHARGE,
                       CHERRYBOMB_FUSE_TIME)
{
}

void CherryBomb::Initialize(const std::string& /*frameDirectory*/) {
    // Get animation from ResourceManager (cached paths, new instance)
    auto animation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_CHERRYBOMB_FUSE, true);
    SetDrawable(animation);

    // Default scale for plant sprite
    m_Transform.scale = {0.3f, 0.3f};

    // Auto-trigger on placement (Cherry Bomb explodes shortly after planting)
    Trigger();
}

void CherryBomb::OnTrigger() {
    // Notify callback to create explosion effects and damage zombies
    if (m_OnExplosion) {
        m_OnExplosion(m_Transform.translation, m_AreaRadius, m_Damage);
    }
}
