#include "Plant/InstantKillPlant.hpp"

InstantKillPlant::InstantKillPlant(const std::string& name,
                                   int damage,
                                   float areaRadius,
                                   int sunCost,
                                   float rechargeTime,
                                   float fuseTime)
    : Plant(name, DEFAULT_HEALTH, sunCost, rechargeTime)
    , m_Damage(damage)
    , m_AreaRadius(areaRadius)
    , m_FuseTime(fuseTime)
{
}

void InstantKillPlant::Update(float deltaTime) {
    Plant::Update(deltaTime);

    // Handle arming for plants that need it (like Potato Mine)
    if (!m_IsArmed) {
        m_ArmTimer += deltaTime;
        if (m_ArmTimer >= m_ArmTime) {
            m_IsArmed = true;
        }
    }

    // Handle fuse countdown after trigger
    if (m_HasTriggered && m_FuseTime > 0.0f) {
        m_FuseTimer += deltaTime;
        if (m_FuseTimer >= m_FuseTime) {
            OnTrigger();
            OnTriggerComplete();
        }
    }
}

void InstantKillPlant::PerformAttack() {
    if (!m_IsArmed || m_HasTriggered) {
        return;  // Can't attack if not armed or already triggered
    }

    Trigger();
}

void InstantKillPlant::Trigger() {
    if (m_HasTriggered) {
        return;
    }

    m_HasTriggered = true;

    // If no fuse time, trigger immediately
    if (m_FuseTime <= 0.0f) {
        OnTrigger();
        OnTriggerComplete();
    }
    // Otherwise, Update() will handle the fuse countdown
}

void InstantKillPlant::OnTrigger() {
    // Base implementation - subclasses override for specific effects
    // This is where area damage would be applied to all zombies in range
    // Example: ApplyAreaDamage(GetGridPosition(), m_AreaRadius, m_Damage);
}

void InstantKillPlant::OnTriggerComplete() {
    // Single-use: plant destroys itself after attack
    m_Health = 0;
}
