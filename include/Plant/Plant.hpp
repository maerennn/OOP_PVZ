#ifndef PLANT_HPP
#define PLANT_HPP

#include <string>
#include <memory>
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

/**
 * @file Plant.hpp
 * @brief Base class for all plants in the game.
 *
 * The Plant class defines the core identity of all plants:
 * - Name: Identifies the plant type
 * - Health: How much damage the plant can take
 * - Sun Cost: Resource cost to plant
 * - Recharge Time: Cooldown before planting again
 *
 * This is the "identity" layer - what a plant fundamentally IS.
 * Abilities (attacking, producing) are added via interfaces.
 */

class Plant : public Util::GameObject {
public:
    // Default plant stats
    static constexpr int DEFAULT_HEALTH = 300;
    static constexpr int DEFAULT_SUN_COST = 100;
    static constexpr float DEFAULT_RECHARGE_TIME = 7.5f;

    Plant(const std::string& name,
          int health = DEFAULT_HEALTH,
          int sunCost = DEFAULT_SUN_COST,
          float rechargeTime = DEFAULT_RECHARGE_TIME);

    virtual ~Plant() = default;

    // Prevent copying, allow moving
    Plant(const Plant&) = delete;
    Plant& operator=(const Plant&) = delete;
    Plant(Plant&&) = default;
    Plant& operator=(Plant&&) = default;

    // Core update method - called each frame
    virtual void Update(float deltaTime);

    // Damage handling
    virtual void TakeDamage(int amount);
    bool IsAlive() const { return m_Health > 0; }

    // Accessors
    const std::string& GetName() const { return m_Name; }
    int GetHealth() const { return m_Health; }
    int GetMaxHealth() const { return m_MaxHealth; }
    int GetSunCost() const { return m_SunCost; }
    float GetRechargeTime() const { return m_RechargeTime; }

    // Grid position
    void SetGridPosition(int row, int col) { m_Row = row; m_Col = col; }
    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }

protected:
    // Animation setup helper
    void SetupAnimation(const std::vector<std::string>& frames,
                        int frameInterval = 50,
                        bool looping = true);

    std::string m_Name;
    int m_Health;
    int m_MaxHealth;
    int m_SunCost;
    float m_RechargeTime;

    int m_Row = -1;
    int m_Col = -1;

    std::shared_ptr<Util::Animation> m_Animation;
};

#endif // PLANT_HPP
