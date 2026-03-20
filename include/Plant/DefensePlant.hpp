#ifndef DEFENSE_PLANT_HPP
#define DEFENSE_PLANT_HPP

#include "Plant/Plant.hpp"
#include <functional>

/**
 * @file DefensePlant.hpp
 * @brief Specialized plant subclass for defensive/wall plants.
 *
 * DefensePlant identity:
 * - High health pool (default 4000 HP vs standard 300 HP)
 * - Visual degradation stages as health decreases
 * - Primary purpose is blocking zombies, not attacking
 *
 * The class manages three visual states based on health percentage:
 * - HEALTHY: 67-100% health (full appearance)
 * - DAMAGED: 34-66% health (cracked/damaged appearance)
 * - CRITICAL: 0-33% health (severely damaged appearance)
 */

class DefensePlant : public Plant {
public:
    // Defense plants have much higher health than standard plants
    static constexpr int DEFAULT_DEFENSE_HEALTH = 4000;

    enum class DamageStage {
        HEALTHY,    // 67-100% HP
        DAMAGED,    // 34-66% HP
        CRITICAL    // 0-33% HP
    };

    DefensePlant(const std::string& name,
                 int health = DEFAULT_DEFENSE_HEALTH,
                 int sunCost = 50,
                 float rechargeTime = 30.0f);

    virtual ~DefensePlant() = default;

    // Override damage handling to update visual state
    void TakeDamage(int amount) override;

    // Plant interface
    void Update(float deltaTime) override;

    // Current damage stage
    DamageStage GetDamageStage() const { return m_CurrentStage; }

    // Get health percentage (0.0 - 1.0)
    float GetHealthPercent() const {
        return static_cast<float>(m_Health) / static_cast<float>(m_MaxHealth);
    }

protected:
    // Called when damage stage changes - override to update visuals
    virtual void OnDamageStageChanged(DamageStage newStage);

    // Setup animations for each damage stage
    void SetupDamageStageAnimations(
        const std::vector<std::string>& healthyFrames,
        const std::vector<std::string>& damagedFrames,
        const std::vector<std::string>& criticalFrames,
        int frameInterval = 50);

private:
    void UpdateDamageStage();

    DamageStage m_CurrentStage = DamageStage::HEALTHY;

    std::shared_ptr<Util::Animation> m_HealthyAnimation;
    std::shared_ptr<Util::Animation> m_DamagedAnimation;
    std::shared_ptr<Util::Animation> m_CriticalAnimation;
};

#endif // DEFENSE_PLANT_HPP
