#include "Plant/DefensePlant.hpp"

DefensePlant::DefensePlant(const std::string& name,
                           int health,
                           int sunCost,
                           float rechargeTime)
    : Plant(name, health, sunCost, rechargeTime)
{
}

void DefensePlant::TakeDamage(int amount) {
    Plant::TakeDamage(amount);
    UpdateDamageStage();
}

void DefensePlant::Update(float deltaTime) {
    Plant::Update(deltaTime);
}

void DefensePlant::UpdateDamageStage() {
    float healthPercent = GetHealthPercent();
    DamageStage newStage;

    if (healthPercent > 0.66f) {
        newStage = DamageStage::HEALTHY;
    } else if (healthPercent > 0.33f) {
        newStage = DamageStage::DAMAGED;
    } else {
        newStage = DamageStage::CRITICAL;
    }

    if (newStage != m_CurrentStage) {
        m_CurrentStage = newStage;
        OnDamageStageChanged(newStage);
    }
}

void DefensePlant::OnDamageStageChanged(DamageStage newStage) {
    // Switch to appropriate animation based on damage stage
    switch (newStage) {
        case DamageStage::HEALTHY:
            if (m_HealthyAnimation) {
                SetDrawable(m_HealthyAnimation);
            }
            break;
        case DamageStage::DAMAGED:
            if (m_DamagedAnimation) {
                SetDrawable(m_DamagedAnimation);
            }
            break;
        case DamageStage::CRITICAL:
            if (m_CriticalAnimation) {
                SetDrawable(m_CriticalAnimation);
            }
            break;
    }
}

void DefensePlant::SetupDamageStageAnimations(
    const std::vector<std::string>& healthyFrames,
    const std::vector<std::string>& damagedFrames,
    const std::vector<std::string>& criticalFrames,
    int frameInterval)
{
    if (!healthyFrames.empty()) {
        m_HealthyAnimation = std::make_shared<Util::Animation>(
            healthyFrames, true, frameInterval, true, 0);
    }
    if (!damagedFrames.empty()) {
        m_DamagedAnimation = std::make_shared<Util::Animation>(
            damagedFrames, true, frameInterval, true, 0);
    }
    if (!criticalFrames.empty()) {
        m_CriticalAnimation = std::make_shared<Util::Animation>(
            criticalFrames, true, frameInterval, true, 0);
    }

    // Start with healthy animation
    if (m_HealthyAnimation) {
        SetDrawable(m_HealthyAnimation);
        m_Animation = m_HealthyAnimation;
    }
}
