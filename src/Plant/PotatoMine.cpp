#include "Plant/PotatoMine.hpp"
#include "ResourceManager.hpp"
#include "Util/Logger.hpp"

PotatoMine::PotatoMine()
    : InstantKillPlant("Potato Mine",
                       POTATOMINE_DAMAGE,
                       POTATOMINE_AREA_RADIUS,
                       POTATOMINE_SUN_COST,
                       POTATOMINE_RECHARGE,
                       0.0f)  // No fuse delay - instant on contact
{
    m_ArmTime  = POTATOMINE_ARM_TIME;
    m_IsArmed  = false;  // Starts unarmed (underground)
}

void PotatoMine::Initialize(const std::string& /*frameDirectory*/) {
    m_UnarmedAnimation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_POTATOMINE_UNARMED, true);
    m_ArmingAnimation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_POTATOMINE_ARMING, true);
    m_ArmedAnimation = ResourceManager::GetInstance().CreateAnimation(
        ResourceManager::PLANT_POTATOMINE_ARMED, true);

    // Start with dormant/unarmed sprite
    SetDrawable(m_UnarmedAnimation);
    m_Transform.scale = {0.28f, 0.28f};

    LOG_DEBUG("PotatoMine placed - arming in {}s", POTATOMINE_ARM_TIME);
}

void PotatoMine::Update(float deltaTime) {
    InstantKillPlant::Update(deltaTime);

    // When arming triggers, play the one-shot transition animation
    if (m_IsArmed && !m_WasArmed) {
        m_WasArmed = true;
        m_IsArming = true;
        SetDrawable(m_ArmingAnimation);
        LOG_DEBUG("PotatoMine arming transition started");
    }

    // Once the transition animation finishes, switch to the looping armed idle
    if (m_IsArming && m_ArmingAnimation->GetState() == Util::Animation::State::ENDED) {
        m_IsArming = false;
        SetDrawable(m_ArmedAnimation);
        LOG_DEBUG("PotatoMine is now ARMED and ready!");
    }
}

void PotatoMine::OnTrigger() {
    LOG_DEBUG("PotatoMine TRIGGERED - exploding!");
    if (m_OnExplosion) {
        m_OnExplosion(m_Transform.translation, m_AreaRadius, m_Damage);
    }
}
