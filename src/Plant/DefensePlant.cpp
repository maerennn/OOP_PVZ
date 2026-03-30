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
    std::shared_ptr<Util::Animation> healthyAnim,
    std::shared_ptr<Util::Animation> damagedAnim,
    std::shared_ptr<Util::Animation> criticalAnim)
{
    m_HealthyAnimation = std::move(healthyAnim);
    m_DamagedAnimation = std::move(damagedAnim);
    m_CriticalAnimation = std::move(criticalAnim);

    // Start with healthy animation
    if (m_HealthyAnimation) {
        SetDrawable(m_HealthyAnimation);
        m_Animation = m_HealthyAnimation;
    }
}
