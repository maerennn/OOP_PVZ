#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // ── Background ──────────────────────────────────────────────────────
    m_Background = std::make_shared<Util::GameObject>();
    auto bgImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/Background/background1.png");
    m_Background->SetDrawable(bgImage);
    m_Background->SetZIndex(GameConfig::ZIndex::BACKGROUND);

    // Zoom and pan to fit the lawn area nicely in the window
    m_Background->m_Transform.scale = {1.18f, 1.18f};
    m_Background->m_Transform.translation = {250.0f, -15.0f};

    m_Root.AddChild(m_Background);

    // ── Initialize Grid ─────────────────────────────────────────────────
    for (int r = 0; r < GameConfig::GRID_ROWS; ++r) {
        for (int c = 0; c < GameConfig::GRID_COLS; ++c) {
            m_PlantGrid[r][c] = nullptr;
        }
    }

    // ── Initialize Sun Manager ──────────────────────────────────────────
    m_SunManager = std::make_unique<SunManager>();
    m_SunManager->Initialize();
    for (auto& obj : m_SunManager->GetUIElements()) {
        m_Root.AddChild(obj);
    }

    // ── Initialize Seed Bank ────────────────────────────────────────────
    m_SeedBank = std::make_unique<SeedBank>();
    m_SeedBank->Initialize({
        PlantType::SUNFLOWER,
        PlantType::PEASHOOTER,
        PlantType::WALLNUT,
        PlantType::CHERRYBOMB
    });
    for (auto& obj : m_SeedBank->GetAllObjects()) {
        m_Root.AddChild(obj);
    }

    // ── Initialize Cursor Item ──────────────────────────────────────────
    m_CursorItem = std::make_shared<CursorItem>();
    m_Root.AddChild(m_CursorItem);

    // ── Initialize Ghost Plant ──────────────────────────────────────────
    m_GhostPlant = std::make_shared<GhostPlant>();
    m_Root.AddChild(m_GhostPlant);

    // ── Initialize Planting System ──────────────────────────────────────
    m_PlantingSystem = std::make_unique<PlantingSystem>(
        *m_SeedBank, *m_SunManager, *m_CursorItem, *m_GhostPlant
    );

    m_PlantingSystem->SetPlantCallback(
        [this](PlantType type, int row, int col) {
            PlacePlant(type, row, col);
        }
    );

    m_PlantingSystem->SetOccupiedCallback(
        [this](int row, int col) {
            return IsCellOccupied(row, col);
        }
    );

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // ── Update GUI Systems ──────────────────────────────────────────────
    float deltaTime = 1.0f / 60.0f;  // Fixed timestep for now

    m_SunManager->Update();
    m_SeedBank->Update(deltaTime, m_SunManager->GetSun());

    // ── Handle Planting Input and State ─────────────────────────────────
    m_PlantingSystem->HandleInput();
    m_PlantingSystem->Update(deltaTime);

    // ── Render ──────────────────────────────────────────────────────────
    m_Root.Update();

    // ── Exit Handling ───────────────────────────────────────────────────
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {
    LOG_TRACE("End");
}

void App::PlacePlant(PlantType type, int row, int col) {
    // Check cell is empty (double-check)
    if (m_PlantGrid[row][col] != nullptr) {
        return;
    }

    // Create plant from registry
    auto plant = PlantRegistry::Create(type);
    if (!plant) {
        LOG_ERROR("Failed to create plant of type {}", static_cast<int>(type));
        return;
    }

    // Position the plant
    plant->SetZIndex(GameConfig::ZIndex::PLANT);
    plant->SetGridPosition(row, col);
    plant->m_Transform.translation = GameConfig::CellToPosition(row, col);

    // Add to scene and grid
    m_Root.AddChild(plant);
    m_PlantGrid[row][col] = plant;

    LOG_DEBUG("Placed {} at cell ({}, {})",
              PlantRegistry::Get(type).name, row, col);
}

bool App::IsCellOccupied(int row, int col) const {
    if (row < 0 || row >= GameConfig::GRID_ROWS ||
        col < 0 || col >= GameConfig::GRID_COLS) {
        return true;  // Out of bounds = occupied
    }
    return m_PlantGrid[row][col] != nullptr;
}
