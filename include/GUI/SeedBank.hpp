#ifndef SEED_BANK_HPP
#define SEED_BANK_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <memory>
#include <vector>

class SeedPacket;

/**
 * @brief Container for seed packets, displayed at top of screen.
 * Manages seed packet layout and provides hit detection.
 */
class SeedBank : public Util::GameObject {
public:
    static constexpr int MAX_SLOTS = 6;

    SeedBank();
    ~SeedBank() = default;

    /**
     * @brief Initialize the seed bank with selected plants.
     * @param selectedPlants Plants to show in the bank
     */
    void Initialize(const std::vector<PlantType>& selectedPlants);

    /**
     * @brief Update all packets.
     * @param deltaTime Time since last frame
     * @param currentSun Player's current sun amount
     */
    void Update(float deltaTime, int currentSun);

    /**
     * @brief Get the packet at a screen position.
     * @param screenPos Position in screen coordinates
     * @return Pointer to packet if found, nullptr otherwise
     */
    SeedPacket* GetPacketAtPosition(const glm::vec2& screenPos);

    /**
     * @brief Start cooldown for a specific plant type.
     * @param type The plant type that was just planted
     */
    void StartCooldown(PlantType type);

    /**
     * @brief Get all GameObjects for adding to renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetAllObjects() const;

    const std::vector<std::shared_ptr<SeedPacket>>& GetPackets() const { return m_Packets; }

private:
    void LayoutPackets();

    std::shared_ptr<Util::GameObject> m_Background;  // SeedBankCoop.png
    std::vector<std::shared_ptr<SeedPacket>> m_Packets;
};

#endif // SEED_BANK_HPP
