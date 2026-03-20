#ifndef SUNFLOWER_HPP
#define SUNFLOWER_HPP

#include "Plant/Plant.hpp"
#include "Plant/IProducer.hpp"
#include <functional>

/**
 * @file Sunflower.hpp
 * @brief Concrete implementation of the Sunflower plant.
 *
 * Sunflower demonstrates the interface separation:
 * - Identity: Is-a Plant (inherits core plant properties)
 * - Ability: Can produce (implements IProducer interface)
 *
 * Unlike ShooterPlants, Sunflower does NOT implement IAttacker.
 * Its role is purely economic - generating sun for the player.
 *
 * Stats:
 * - Production Amount: 25 sun per production
 * - Production Rate: 24 seconds between productions
 * - Sun Cost: 50
 * - Recharge: Fast (7.5s)
 */

class Sunflower : public Plant, public IProducer {
public:
    // Sunflower-specific constants
    static constexpr int SUNFLOWER_PRODUCTION_AMOUNT = 25;
    static constexpr float SUNFLOWER_PRODUCTION_RATE = 24.0f;  // seconds
    static constexpr int SUNFLOWER_SUN_COST = 50;
    static constexpr float SUNFLOWER_RECHARGE = 7.5f;
    static constexpr int SUNFLOWER_FRAME_COUNT = 29;

    // Callback type for when sun is produced
    using SunProducedCallback = std::function<void(int amount, glm::vec2 position)>;

    Sunflower();
    virtual ~Sunflower() = default;

    // Initialize with animation frames
    void Initialize(const std::string& frameDirectory);

    // Plant interface
    void Update(float deltaTime) override;

    // IProducer interface implementation
    void Produce() override;
    int GetProductionAmount() const override { return m_ProductionAmount; }
    float GetProductionRate() const override { return m_ProductionRate; }
    bool CanProduce() const override { return m_ProductionTimer >= m_ProductionRate; }

    // Set callback for when sun is produced
    void SetSunProducedCallback(SunProducedCallback callback) {
        m_OnSunProduced = std::move(callback);
    }

private:
    static std::vector<std::string> GetAnimationFrames(const std::string& frameDir);

    int m_ProductionAmount = SUNFLOWER_PRODUCTION_AMOUNT;
    float m_ProductionRate = SUNFLOWER_PRODUCTION_RATE;
    float m_ProductionTimer = 0.0f;

    SunProducedCallback m_OnSunProduced;
};

#endif // SUNFLOWER_HPP
