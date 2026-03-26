#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Sun.hpp"
#include "Plant/Sunflower.hpp"
#include <algorithm>
#include <random>

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

    // ── Update All Plants ───────────────────────────────────────────────
    for (int r = 0; r < GameConfig::GRID_ROWS; ++r) {
        for (int c = 0; c < GameConfig::GRID_COLS; ++c) {
            if (m_PlantGrid[r][c] != nullptr) {
                m_PlantGrid[r][c]->Update(deltaTime);
            }
        }
    }

    // ── Update Sun Collection System ────────────────────────────────────
    UpdateSuns(deltaTime);
    CheckSunCollection();

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        LOG_DEBUG("Mouse Clicked at: x={}, y={}", mousePos.x, mousePos.y);
    }
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

    // ── Wire up Sunflower callback ──────────────────────────────────────
    if (type == PlantType::SUNFLOWER) {
        auto sunflower = std::dynamic_pointer_cast<Sunflower>(plant);
        if (sunflower) {
            sunflower->SetSunProducedCallback(
                [this](int amount, glm::vec2 position) {
                    // Offset sun spawn slightly above the plant
                    glm::vec2 sunPos = position + glm::vec2(0.0f, 30.0f);
                    SpawnSun(amount, sunPos);
                }
            );
        }
    }

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

// ══════════════════════════════════════════════════════════════════════════
// Sun System Implementation
// ══════════════════════════════════════════════════════════════════════════

void App::SpawnSun(int value, const glm::vec2& position) {
    auto sun = std::make_shared<Sun>(position, value);
    sun->Initialize();

    // Set callback to add sun to manager when collected
    sun->SetCollectedCallback([this](int sunValue) {
        m_SunManager->AddSun(sunValue);
        LOG_DEBUG("Collected {} sun! Total: {}", sunValue, m_SunManager->GetSun());
    });

    m_Root.AddChild(sun);
    m_Suns.push_back(sun);

    LOG_DEBUG("Spawned sun at ({}, {})", position.x, position.y);
}

void App::UpdateSuns(float deltaTime) {
    // ── Update all existing suns ────────────────────────────────────────
    for (auto& sun : m_Suns) {
        sun->Update(deltaTime);
    }

    // ── Remove collected or expired suns (safe erase) ───────────────────
    auto it = std::remove_if(m_Suns.begin(), m_Suns.end(),
        [this](const std::shared_ptr<Sun>& sun) {
            if (sun->ShouldRemove()) {
                m_Root.RemoveChild(sun);
                return true;
            }
            return false;
        });
    m_Suns.erase(it, m_Suns.end());

    // ── Sky Drop Timer ──────────────────────────────────────────────────
    m_SkyDropTimer += deltaTime;

    if (m_SkyDropTimer >= SKY_DROP_INTERVAL) {
        m_SkyDropTimer = 0.0f;

        // Random X position within the lawn grid area
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xDist(
            GameConfig::GRID_ORIGIN_X + 50.0f,
            GameConfig::GRID_RIGHT - 50.0f
        );

        // Random Y position in the upper lawn area
        std::uniform_real_distribution<float> yDist(
            GameConfig::LaneCenterY(2),  // Middle lane
            GameConfig::LaneCenterY(0)   // Top lane
        );

        glm::vec2 skyDropPos = {xDist(gen), yDist(gen)};
        SpawnSun(Sun::SUN_VALUE, skyDropPos);
        LOG_DEBUG("Sky drop sun at ({}, {})", skyDropPos.x, skyDropPos.y);
    }
}

void App::CheckSunCollection() {
    // Only check on mouse click
    if (!Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        return;
    }

    glm::vec2 cursorPos = Util::Input::GetCursorPosition();

    // Check each sun for click
    for (auto& sun : m_Suns) {
        if (sun->IsClicked(cursorPos)) {
            sun->Collect();
            break;  // Only collect one sun per click
        }
    }
}
