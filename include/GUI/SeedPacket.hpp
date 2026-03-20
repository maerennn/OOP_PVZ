#ifndef SEED_PACKET_HPP
#define SEED_PACKET_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <memory>
#include <vector>

namespace Util {
    class Image;
    class Text;
}

/**
 * @brief Individual seed packet UI widget.
 * Displays plant icon, sun cost, and handles cooldown state.
 */
class SeedPacket : public Util::GameObject {
public:
    enum class State {
        READY,      // Can be selected (cooldown complete, enough sun)
        COOLDOWN,   // Recharging after placement
        DISABLED    // Not enough sun
    };

    /**
     * @brief Create a seed packet for a plant type.
     * @param type The plant type this packet represents
     */
    explicit SeedPacket(PlantType type);
    ~SeedPacket() = default;

    /**
     * @brief Initialize visual components.
     * Must be called after construction.
     */
    void Initialize();

    /**
     * @brief Update packet state based on current sun and cooldown.
     * @param deltaTime Time since last frame
     * @param currentSun Player's current sun amount
     */
    void Update(float deltaTime, int currentSun);

    /**
     * @brief Check if a screen position is within this packet.
     * @param screenPos Position in screen coordinates
     * @return true if point is within packet bounds
     */
    bool ContainsPoint(const glm::vec2& screenPos) const;

    /**
     * @brief Check if this packet can be selected.
     * @param currentSun Player's current sun amount
     * @return true if ready and affordable
     */
    bool CanSelect(int currentSun) const;

    /**
     * @brief Start the cooldown timer after planting.
     */
    void StartCooldown();

    // Accessors
    PlantType GetPlantType() const { return m_PlantType; }
    State GetState() const { return m_State; }
    int GetSunCost() const;
    float GetCooldownProgress() const;  // 0.0 (full cooldown) to 1.0 (ready)

    // Packet dimensions (for layout)
    static constexpr float WIDTH = 50.0f;
    static constexpr float HEIGHT = 70.0f;

    /**
     * @brief Get all child GameObjects for adding to renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const;

    /**
     * @brief Update children positions based on packet transform.
     * Call this after changing the packet's m_Transform.translation.
     */
    void UpdateChildPositions();

private:
    void UpdateVisuals();
    void CreateComponents();

    PlantType m_PlantType;
    State m_State = State::READY;

    // Cooldown tracking
    float m_CooldownRemaining = 0.0f;
    float m_TotalCooldown = 0.0f;

    // Visual components
    std::shared_ptr<Util::GameObject> m_Background;     // SeedPacket_Larger.png
    std::shared_ptr<Util::GameObject> m_PlantIcon;      // Plant icon
    std::shared_ptr<Util::GameObject> m_SunCostObject;  // Text showing cost
    std::shared_ptr<Util::Text> m_SunCostText;
    std::shared_ptr<Util::GameObject> m_CooldownOverlay; // Gray overlay during cooldown
};

#endif // SEED_PACKET_HPP
