#ifndef PLANTING_SYSTEM_HPP
#define PLANTING_SYSTEM_HPP

#include "PlantType.hpp"
#include <memory>
#include <functional>
#include <glm/vec2.hpp>

class SeedBank;
class SunManager;
class CursorItem;
class GhostPlant;
class Plant;
class ConveyorBelt;
class ShovelBank;

/**
 * @brief State machine managing the plant selection and placement workflow.
 *
 * States:
 * - IDLE: No seed selected, waiting for user to click a seed packet
 * - SEED_SELECTED: Seed clicked, plant icon follows cursor
 * - PLACING: Cursor over valid grid cell, showing ghost preview
 */
class PlantingSystem {
public:
    enum class State {
        IDLE,           // No seed selected
        SEED_SELECTED,  // Seed clicked, following cursor
        SHOVEL_SELECTED,// Shovel clicked, ready to remove plants
    };

    /**
     * @brief Callback when a plant should be placed.
     * Parameters: plant type, row, column
     */
    using PlantCallback = std::function<void(PlantType, int, int)>;

    /**
     * @brief Create the planting system.
     * @param seedBank Reference to the seed bank
     * @param sunManager Reference to the sun manager
     * @param cursorItem Reference to the cursor item display
     * @param ghostPlant Reference to the ghost plant preview
     */
    PlantingSystem(SeedBank& seedBank,
                   SunManager& sunManager,
                   CursorItem& cursorItem,
                   GhostPlant& ghostPlant);

    /**
     * @brief Set callback for when plant is placed.
     */
    void SetPlantCallback(PlantCallback callback) { m_OnPlant = std::move(callback); }

    /**
     * @brief Check if a grid cell is occupied.
     * Set this callback to check grid occupancy.
     */
    using OccupiedCallback = std::function<bool(int row, int col)>;
    void SetOccupiedCallback(OccupiedCallback callback) { m_IsOccupied = std::move(callback); }

    /**
     * @brief Attach a conveyor belt. When set, cards are taken from the belt
     *        instead of the seed bank, and placement is free (no sun cost).
     */
    void SetConveyorBelt(ConveyorBelt* belt) { m_ConveyorBelt = belt; }

    /**
     * @brief Attach a shovel bank. When set, players can remove plants from the grid.
     */
    void SetShovelBank(ShovelBank* shovelBank) { m_ShovelBank = shovelBank; }

    /**
     * @brief Set callback for when plant should be removed.
     * Parameters: row, column
     */
    using RemovePlantCallback = std::function<void(int, int)>;
    void SetRemovePlantCallback(RemovePlantCallback callback) { m_OnRemovePlant = std::move(callback); }

    /**
     * @brief Process input events.
     * Call this each frame to handle mouse clicks.
     */
    void HandleInput();

    /**
     * @brief Update the system state.
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime);

    State GetCurrentState() const { return m_CurrentState; }

private:
    void TransitionTo(State newState);
    void HandleIdleInput();
    void HandleSeedSelectedInput();
    void HandleShovelSelectedInput();

    glm::vec2 GetCursorPosition() const;
    bool IsLeftMouseClicked() const;
    bool IsRightMouseClicked() const;

    State m_CurrentState = State::IDLE;
    PlantType m_SelectedPlant = PlantType::PEASHOOTER;

    // References to collaborating objects
    SeedBank& m_SeedBank;
    SunManager& m_SunManager;
    CursorItem& m_CursorItem;
    GhostPlant& m_GhostPlant;

    PlantCallback m_OnPlant;
    RemovePlantCallback m_OnRemovePlant;
    OccupiedCallback m_IsOccupied;

    // Conveyor Belt (optional — null when using normal SeedBank)
    ConveyorBelt* m_ConveyorBelt          = nullptr;
    int           m_SelectedConveyorIndex = -1;

    // Shovel Bank (optional — null when not available)
    ShovelBank* m_ShovelBank = nullptr;

    // Track mouse state for edge detection
    bool m_WasLeftPressed = false;
    bool m_WasRightPressed = false;
};

#endif // PLANTING_SYSTEM_HPP
