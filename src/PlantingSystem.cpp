#include "PlantingSystem.hpp"
#include "GUI/SeedBank.hpp"
#include "GUI/SeedPacket.hpp"
#include "GUI/CursorItem.hpp"
#include "GUI/GhostPlant.hpp"
#include "SunManager.hpp"
#include "GameConfig.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

PlantingSystem::PlantingSystem(SeedBank& seedBank,
                               SunManager& sunManager,
                               CursorItem& cursorItem,
                               GhostPlant& ghostPlant)
    : m_SeedBank(seedBank)
    , m_SunManager(sunManager)
    , m_CursorItem(cursorItem)
    , m_GhostPlant(ghostPlant)
{
}

void PlantingSystem::HandleInput() {
    switch (m_CurrentState) {
        case State::IDLE:
            HandleIdleInput();
            break;
        case State::SEED_SELECTED:
            HandleSeedSelectedInput();
            break;
    }

    // Update mouse state for next frame
    m_WasLeftPressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
    m_WasRightPressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB);
}

void PlantingSystem::Update(float /*deltaTime*/) {
    if (m_CurrentState == State::SEED_SELECTED) {
        glm::vec2 cursorPos = GetCursorPosition();

        // Update cursor item position
        m_CursorItem.UpdatePosition(cursorPos);

        // Check if cursor is over grid
        int row, col;
        if (GameConfig::PositionToCell(cursorPos, row, col)) {
            // Check if cell is occupied
            bool occupied = m_IsOccupied ? m_IsOccupied(row, col) : false;

            // Show ghost plant at cell
            if (!m_GhostPlant.IsActive() ||
                m_GhostPlant.GetRow() != row ||
                m_GhostPlant.GetCol() != col) {
                m_GhostPlant.SetPlantType(m_SelectedPlant);
                m_GhostPlant.SetGridCell(row, col);
            }
            m_GhostPlant.SetValid(!occupied);
        } else {
            // Cursor not over grid, hide ghost
            m_GhostPlant.Clear();
        }
    }
}

void PlantingSystem::HandleIdleInput() {
    // Check for left click on seed packet
    if (IsLeftMouseClicked()) {
        glm::vec2 cursorPos = GetCursorPosition();

        SeedPacket* packet = m_SeedBank.GetPacketAtPosition(cursorPos);
        if (packet && packet->CanSelect(m_SunManager.GetSun())) {
            m_SelectedPlant = packet->GetPlantType();
            TransitionTo(State::SEED_SELECTED);
            LOG_DEBUG("Selected plant: {}", PlantRegistry::Get(m_SelectedPlant).name);
        }
    }
}

void PlantingSystem::HandleSeedSelectedInput() {
    glm::vec2 cursorPos = GetCursorPosition();

    // Right click cancels selection
    if (IsRightMouseClicked()) {
        TransitionTo(State::IDLE);
        LOG_DEBUG("Plant selection cancelled");
        return;
    }

    // Left click attempts to place plant
    if (IsLeftMouseClicked()) {
        int row, col;
        if (GameConfig::PositionToCell(cursorPos, row, col)) {
            // Check if cell is occupied
            bool occupied = m_IsOccupied ? m_IsOccupied(row, col) : false;

            if (!occupied) {
                // Place the plant
                int cost = PlantRegistry::Get(m_SelectedPlant).sunCost;
                if (m_SunManager.SpendSun(cost)) {
                    // Notify callback to create the plant
                    if (m_OnPlant) {
                        m_OnPlant(m_SelectedPlant, row, col);
                    }

                    // Start cooldown on the seed packet
                    m_SeedBank.StartCooldown(m_SelectedPlant);

                    LOG_DEBUG("Placed {} at ({}, {})",
                              PlantRegistry::Get(m_SelectedPlant).name, row, col);

                    // Return to idle
                    TransitionTo(State::IDLE);
                }
            }
        }
    }
}

void PlantingSystem::TransitionTo(State newState) {
    // Exit current state
    switch (m_CurrentState) {
        case State::IDLE:
            break;
        case State::SEED_SELECTED:
            m_CursorItem.Clear();
            m_GhostPlant.Clear();
            break;
    }

    m_CurrentState = newState;

    // Enter new state
    switch (m_CurrentState) {
        case State::IDLE:
            break;
        case State::SEED_SELECTED:
            m_CursorItem.SetPlantType(m_SelectedPlant);
            break;
    }
}

glm::vec2 PlantingSystem::GetCursorPosition() const {
    return Util::Input::GetCursorPosition();
}

bool PlantingSystem::IsLeftMouseClicked() const {
    // Edge detection: pressed this frame but not last frame
    bool pressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
    return pressed && !m_WasLeftPressed;
}

bool PlantingSystem::IsRightMouseClicked() const {
    bool pressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB);
    return pressed && !m_WasRightPressed;
}
