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
    OccupiedCallback m_IsOccupied;

    // Track mouse state for edge detection
    bool m_WasLeftPressed = false;
    bool m_WasRightPressed = false;
};

#endif // PLANTING_SYSTEM_HPP
