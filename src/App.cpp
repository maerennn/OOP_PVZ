#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "ResourceManager.hpp"
#include "Sun.hpp"
#include "Pea.hpp"
#include "Zombie/NormalZombie.hpp"
#include "Plant/Sunflower.hpp"
#include "Plant/ShooterPlant.hpp"
#include "Plant/CherryBomb.hpp"
#include <algorithm>
#include <random>
#include <cmath>

void App::Start() {
    LOG_TRACE("Start");

    // ══════════════════════════════════════════════════════════════════════
    // Initialize ResourceManager FIRST - caches all animation paths
    // ══════════════════════════════════════════════════════════════════════
    ResourceManager::GetInstance().Initialize();

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

    // ── Initialize Lawnmowers ────────────────────────────────────────────
    SpawnLawnmowers();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // ── Check Game Over ─────────────────────────────────────────────────
    if (m_GameOver) {
        m_Root.Update();  // Still render
        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
            Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
        return;  // Skip gameplay updates
    }

    // ── Update GUI Systems ──────────────────────────────────────────────
    float deltaTime = 1.0f / 60.0f;  // Fixed timestep for now

    m_SunManager->Update();
    m_SeedBank->Update(deltaTime, m_SunManager->GetSun());

    // ── Handle Planting Input and State ─────────────────────────────────
    m_PlantingSystem->HandleInput();
    m_PlantingSystem->Update(deltaTime);

    // ── Update Lawnmower System ─────────────────────────────────────────
    UpdateLawnmowers(deltaTime);
    CheckLawnmowerCollisions();

    // ── Update Zombie System ────────────────────────────────────────────
    UpdateZombies(deltaTime);
    CheckGameOver();  // Check if zombie reached the house
    if (m_GameOver) return;  // Early exit if game just ended

    // ── Check Zombie-Plant Collisions ───────────────────────────────────
    CheckZombiePlantCollisions();

    // ── Update Shooter Target Detection ─────────────────────────────────
    UpdateShooterTargets();

    // ── Update All Plants ───────────────────────────────────────────────
    for (int r = 0; r < GameConfig::GRID_ROWS; ++r) {
        for (int c = 0; c < GameConfig::GRID_COLS; ++c) {
            if (m_PlantGrid[r][c] != nullptr) {
                m_PlantGrid[r][c]->Update(deltaTime);
            }
        }
    }

    // ── Remove Dead Plants ──────────────────────────────────────────────
    RemoveDeadPlants();

    // ── Update Projectile System ────────────────────────────────────────
    UpdateProjectiles(deltaTime);

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
                    // Sunflower suns don't fall - targetY = starting Y
                    SpawnSun(amount, sunPos, sunPos.y);
                }
            );
        }
    }

    // ── Wire up ShooterPlant callback ───────────────────────────────────
    auto shooter = std::dynamic_pointer_cast<ShooterPlant>(plant);
    if (shooter) {
        shooter->SetProjectileCallback(
            [this](ProjectileType ptype, int damage, int prow, glm::vec2 pos) {
                SpawnProjectile(ptype, damage, prow, pos);
            }
        );
    }

    // ── Wire up CherryBomb explosion callback ───────────────────────────
    if (type == PlantType::CHERRYBOMB) {
        auto cherryBomb = std::dynamic_pointer_cast<CherryBomb>(plant);
        if (cherryBomb) {
            cherryBomb->SetExplosionCallback(
                [this, row, col](glm::vec2 position, float radius, int damage) {
                    HandleCherryBombExplosion(row, col, damage);
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

void App::SpawnSun(int value, const glm::vec2& position, float targetY) {
    auto sun = std::make_shared<Sun>(position, targetY, value);
    sun->Initialize();

    // Set callback to add sun to manager when collected
    sun->SetCollectedCallback([this](int sunValue) {
        m_SunManager->AddSun(sunValue);
        LOG_DEBUG("Collected {} sun! Total: {}", sunValue, m_SunManager->GetSun());
    });

    m_Root.AddChild(sun);
    m_Suns.push_back(sun);

    LOG_DEBUG("Spawned sun at ({}, {}) -> target Y: {}", position.x, position.y, targetY);
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

        // Random target Y position in the lawn area (where sun will stop falling)
        std::uniform_real_distribution<float> yDist(
            GameConfig::LaneCenterY(3),  // Lower-middle lane
            GameConfig::LaneCenterY(1)   // Upper-middle lane
        );

        float targetY = yDist(gen);
        // Spawn at top of screen (above visible area in PTSD coordinates)
        constexpr float SKY_SPAWN_Y = 400.0f;  // Above top edge (360)
        glm::vec2 spawnPos = {xDist(gen), SKY_SPAWN_Y};

        SpawnSun(Sun::SUN_VALUE, spawnPos, targetY);
        LOG_DEBUG("Sky drop sun spawned at Y={}, falling to Y={}", SKY_SPAWN_Y, targetY);
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

// ══════════════════════════════════════════════════════════════════════════
// Projectile System Implementation
// ══════════════════════════════════════════════════════════════════════════

void App::SpawnProjectile(ProjectileType type, int damage, int row, const glm::vec2& position) {
    std::shared_ptr<Projectile> projectile;

    switch (type) {
        case ProjectileType::PEA:
            projectile = std::make_shared<Pea>(damage, row, position);
            break;

        // Future projectile types:
        // case ProjectileType::FROZEN_PEA:
        //     projectile = std::make_shared<FrozenPea>(damage, row, position);
        //     break;

        default:
            LOG_WARN("Unknown projectile type: {}", static_cast<int>(type));
            return;
    }

    projectile->Initialize();
    m_Root.AddChild(projectile);
    m_Projectiles.push_back(projectile);

    LOG_DEBUG("Spawned projectile at ({}, {}) in row {}",
              position.x, position.y, row);
}

void App::UpdateProjectiles(float deltaTime) {
    // ── Update all projectiles ──────────────────────────────────────────
    for (auto& projectile : m_Projectiles) {
        projectile->Update(deltaTime);
    }

    // ── Projectile-Zombie Collision Detection ───────────────────────────
    for (auto& projectile : m_Projectiles) {
        if (!projectile->IsActive()) continue;

        for (auto& zombie : m_Zombies) {
            if (zombie->IsDead() || zombie->GetState() == Zombie::State::DYING) continue;
            if (zombie->GetRow() != projectile->GetRow()) continue;

            // AABB Collision check
            float projX = projectile->m_Transform.translation.x;
            float zombieLeft = zombie->GetLeftEdge();
            float zombieRight = zombie->GetRightEdge();

            if (projX >= zombieLeft && projX <= zombieRight) {
                // Hit! Apply damage and deactivate projectile
                zombie->TakeDamage(projectile->GetDamage());
                projectile->OnHit();

                LOG_DEBUG("Projectile hit {} for {} damage! Zombie HP: {}",
                          zombie->GetName(), projectile->GetDamage(), zombie->GetHealth());
                break;  // One projectile hits one zombie
            }
        }
    }

    // ── Remove inactive or off-screen projectiles (safe erase) ──────────
    auto it = std::remove_if(m_Projectiles.begin(), m_Projectiles.end(),
        [this](const std::shared_ptr<Projectile>& proj) {
            if (!proj->IsActive() || proj->IsOffScreen(GameConfig::ZOMBIE_ZONE_RIGHT)) {
                m_Root.RemoveChild(proj);
                return true;
            }
            return false;
        });
    m_Projectiles.erase(it, m_Projectiles.end());
}

void App::UpdateShooterTargets() {
    // ══════════════════════════════════════════════════════════════════════
    // Detect zombies in each lane and update shooter targets accordingly
    // ══════════════════════════════════════════════════════════════════════

    // First, count zombies per row
    bool zombieInRow[GameConfig::GRID_ROWS] = {false};
    for (const auto& zombie : m_Zombies) {
        if (!zombie->IsDead() && zombie->GetRow() >= 0 && zombie->GetRow() < GameConfig::GRID_ROWS) {
            zombieInRow[zombie->GetRow()] = true;
        }
    }

    // Update all shooter plants
    for (int r = 0; r < GameConfig::GRID_ROWS; ++r) {
        for (int c = 0; c < GameConfig::GRID_COLS; ++c) {
            if (m_PlantGrid[r][c] != nullptr) {
                auto shooter = std::dynamic_pointer_cast<ShooterPlant>(m_PlantGrid[r][c]);
                if (shooter) {
                    shooter->SetHasTarget(zombieInRow[r]);
                }
            }
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════
// Zombie System Implementation
// ══════════════════════════════════════════════════════════════════════════

void App::SpawnZombie(int row) {
    auto zombie = std::make_shared<NormalZombie>();
    zombie->Initialize();
    zombie->SetRow(row);

    // Position at right edge of screen, centered in lane
    float spawnX = GameConfig::ZOMBIE_SPAWN_X;
    float spawnY = GameConfig::LaneCenterY(row);
    zombie->m_Transform.translation = {spawnX, spawnY};

    m_Root.AddChild(zombie);
    m_Zombies.push_back(zombie);

    LOG_DEBUG("Spawned NormalZombie at row {}, position ({}, {})", row, spawnX, spawnY);
}

void App::UpdateZombies(float deltaTime) {
    // ── Spawn Timer (temporary wave system for testing) ─────────────────
    m_ZombieSpawnTimer += deltaTime;

    if (m_ZombieSpawnTimer >= ZOMBIE_SPAWN_INTERVAL) {
        m_ZombieSpawnTimer = 0.0f;

        // Spawn zombie in random row
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> rowDist(0, GameConfig::GRID_ROWS - 1);

        int row = rowDist(gen);
        SpawnZombie(row);
    }

    // ── Update all zombies ──────────────────────────────────────────────
    for (auto& zombie : m_Zombies) {
        zombie->Update(deltaTime);
    }

    // ── Remove dead zombies (safe erase) ────────────────────────────────
    auto it = std::remove_if(m_Zombies.begin(), m_Zombies.end(),
        [this](const std::shared_ptr<Zombie>& zombie) {
            if (zombie->ShouldRemove()) {
                m_Root.RemoveChild(zombie);
                return true;
            }
            return false;
        });
    m_Zombies.erase(it, m_Zombies.end());
}

void App::CheckZombiePlantCollisions() {
    // ══════════════════════════════════════════════════════════════════════
    // For each zombie in WALKING state, check if it has reached a plant
    // ══════════════════════════════════════════════════════════════════════

    for (auto& zombie : m_Zombies) {
        // Only check walking zombies
        if (zombie->GetState() != Zombie::State::WALKING) continue;

        int row = zombie->GetRow();
        float zombieLeft = zombie->GetLeftEdge();

        // Check each column from right to left for plants in this row
        for (int col = GameConfig::GRID_COLS - 1; col >= 0; --col) {
            auto& plant = m_PlantGrid[row][col];
            if (!plant || !plant->IsAlive()) continue;

            // Get plant position and hitbox
            glm::vec2 plantPos = plant->m_Transform.translation;
            float plantHalfWidth = 30.0f;  // Approximate plant hitbox half-width
            float plantRight = plantPos.x + plantHalfWidth;

            // Check if zombie has reached this plant
            if (zombieLeft <= plantRight) {
                // Zombie reached plant - switch to ATTACKING
                zombie->SetTargetPlant(plant.get());
                zombie->SetState(Zombie::State::ATTACKING);
                LOG_DEBUG("Zombie {} attacking plant at row {} col {}",
                          zombie->GetName(), row, col);
                break;  // Only attack one plant at a time
            }
        }
    }
}

void App::RemoveDeadPlants() {
    // ══════════════════════════════════════════════════════════════════════
    // Remove plants that have been destroyed by zombies
    // ══════════════════════════════════════════════════════════════════════

    for (int r = 0; r < GameConfig::GRID_ROWS; ++r) {
        for (int c = 0; c < GameConfig::GRID_COLS; ++c) {
            auto& plant = m_PlantGrid[r][c];
            if (plant && !plant->IsAlive()) {
                LOG_DEBUG("Removing dead plant at row {} col {}", r, c);
                m_Root.RemoveChild(plant);
                plant.reset();  // Remove from grid
            }
        }
    }
}

void App::CheckGameOver() {
    // ═══════════════════════════════════════════════════════════════════════
    // Check if any zombie has reached the house (left edge of lawn)
    // ═══════════════════════════════════════════════════════════════════════
    
    for (const auto& zombie : m_Zombies) {
        // Skip dead, dying, or charred zombies (they can't eat brains)
        if (zombie->GetState() == Zombie::State::DEAD ||
            zombie->GetState() == Zombie::State::DYING ||
            zombie->GetState() == Zombie::State::CHARRED) {
            continue;
        }
        
        float zombieX = zombie->GetTransform().translation.x;
        if (zombieX < GameConfig::HOUSE_X) {
            LOG_ERROR("═══════════════════════════════════════════════");
            LOG_ERROR("           GAME OVER - Zombies ate your brains!");
            LOG_ERROR("═══════════════════════════════════════════════");
            m_GameOver = true;
            m_CurrentState = State::END;
            return;
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════
// Lawnmower System Implementation
// ══════════════════════════════════════════════════════════════════════════

void App::SpawnLawnmowers() {
    m_Lawnmowers.clear();

    for (int row = 0; row < GameConfig::GRID_ROWS; ++row) {
        // Position at left edge of lawn (mower zone)
        float x = GameConfig::MOWER_ZONE_RIGHT - 30.0f;  // Slightly inside the zone
        float y = GameConfig::LaneCenterY(row);

        auto mower = std::make_shared<Lawnmower>(row, glm::vec2{x, y});
        mower->Initialize();

        m_Root.AddChild(mower);
        m_Lawnmowers.push_back(mower);

        LOG_DEBUG("Spawned lawnmower at row {} position ({}, {})", row, x, y);
    }
}

void App::UpdateLawnmowers(float deltaTime) {
    // Update all lawnmowers
    for (auto& mower : m_Lawnmowers) {
        mower->Update(deltaTime);
    }

    // Remove off-screen lawnmowers (safe erase)
    auto it = std::remove_if(m_Lawnmowers.begin(), m_Lawnmowers.end(),
        [this](const std::shared_ptr<Lawnmower>& mower) {
            if (mower->ShouldRemove()) {
                LOG_DEBUG("Removing off-screen lawnmower from row {}", mower->GetRow());
                m_Root.RemoveChild(mower);
                return true;
            }
            return false;
        });
    m_Lawnmowers.erase(it, m_Lawnmowers.end());
}

void App::CheckLawnmowerCollisions() {
    for (auto& mower : m_Lawnmowers) {
        int mowerRow = mower->GetRow();
        float mowerX = mower->GetTransform().translation.x;

        for (auto& zombie : m_Zombies) {
            // Skip dead/dying/charred zombies
            if (zombie->GetState() == Zombie::State::DEAD ||
                zombie->GetState() == Zombie::State::DYING ||
                zombie->GetState() == Zombie::State::CHARRED) {
                continue;
            }

            // Only check zombies in the same row
            if (zombie->GetRow() != mowerRow) {
                continue;
            }

            float zombieX = zombie->GetTransform().translation.x;

            // Check collision
            float distance = std::abs(zombieX - mowerX);
            if (distance < 50.0f) {  // Collision threshold
                if (mower->IsIdle()) {
                    // Zombie triggered the mower!
                    mower->Trigger();
                    LOG_DEBUG("Zombie triggered lawnmower in row {}!", mowerRow);
                }

                if (mower->IsTriggered()) {
                    // Mower instantly kills the zombie
                    zombie->TakeDamage(9999);
                    LOG_DEBUG("Lawnmower destroyed zombie in row {}!", mowerRow);
                }
            }
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════
// Explosion System Implementation (CherryBomb)
// ══════════════════════════════════════════════════════════════════════════

void App::HandleCherryBombExplosion(int centerRow, int centerCol, int damage) {
    LOG_DEBUG("CherryBomb EXPLOSION at row {}, col {} with {} damage!", 
              centerRow, centerCol, damage);

    // Calculate the world position of the cherry bomb
    glm::vec2 centerPos = GameConfig::CellToPosition(centerRow, centerCol);

    // 3x3 grid area: check rows [centerRow-1, centerRow+1]
    //                       cols [centerCol-1, centerCol+1]
    // But for zombies, we calculate using world position + cell dimensions

    float explosionRadiusX = GameConfig::CELL_WIDTH * 1.5f;   // ~1.5 cells horizontally
    float explosionRadiusY = GameConfig::CELL_HEIGHT * 1.5f;  // ~1.5 cells vertically

    int zombiesKilled = 0;

    for (auto& zombie : m_Zombies) {
        // Skip already dead/dying zombies
        if (zombie->GetState() == Zombie::State::DEAD ||
            zombie->GetState() == Zombie::State::DYING ||
            zombie->GetState() == Zombie::State::CHARRED) {
            continue;
        }

        glm::vec2 zombiePos = zombie->GetTransform().translation;

        // Check if zombie is within 3x3 area (using Manhattan-style box collision)
        float dx = std::abs(zombiePos.x - centerPos.x);
        float dy = std::abs(zombiePos.y - centerPos.y);

        if (dx <= explosionRadiusX && dy <= explosionRadiusY) {
            // Zombie is in blast radius! Apply explosion damage
            zombie->TakeExplosionDamage(damage);
            zombiesKilled++;
            LOG_DEBUG("  - {} caught in explosion!", zombie->GetName());
        }
    }

    LOG_DEBUG("CherryBomb killed {} zombies in explosion!", zombiesKilled);
}
