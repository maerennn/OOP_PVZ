#include "Plant/CherryBomb.hpp"
#include "ResourceManager.hpp"
#include "Util/Logger.hpp"

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
    m_FuseAnimation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_CHERRYBOMB_FUSE, true);
    SetDrawable(m_FuseAnimation);

    // Default scale for plant sprite
    m_Transform.scale = {0.3f, 0.3f};

    LOG_DEBUG("CherryBomb initialized with fuse animation");
}

void CherryBomb::Update(float deltaTime) {
    // Call base class update (handles arming, fuse timer, etc.)
    InstantKillPlant::Update(deltaTime);

    // Check if we're planted and animation hasn't exploded yet
    if (!m_HasExploded && m_FuseAnimation) {
        // Check if fuse animation has completed
        if (m_FuseAnimation->GetState() == Util::Animation::State::ENDED) {
            Explode();
        }
        
        // Fallback: check timer in case animation state detection fails
        m_FuseTimer += deltaTime;
        if (m_FuseTimer >= CHERRYBOMB_FUSE_TIME) {
            Explode();
        }
    }
}

void CherryBomb::Explode() {
    if (m_HasExploded) {
        return;  // Already exploded
    }

    m_HasExploded = true;
    LOG_DEBUG("CherryBomb EXPLODING!");

    // Trigger the explosion callback to damage zombies
    if (m_OnExplosion) {
        m_OnExplosion(m_Transform.translation, m_AreaRadius, m_Damage);
    }

    // Destroy self (health = 0 marks for removal)
    m_Health = 0;
    LOG_DEBUG("CherryBomb destroyed itself after explosion");
}

void CherryBomb::OnTrigger() {
    // This is called by base class when Trigger() is invoked
    // For CherryBomb, we don't explode immediately - we wait for animation
    LOG_DEBUG("CherryBomb triggered, waiting for fuse animation to complete...");
}
