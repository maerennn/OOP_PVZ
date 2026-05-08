#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/GameObject.hpp"
#include "GameConfig.hpp"
#include "Plants.hpp"
#include "PlantType.hpp"
#include "SunManager.hpp"
#include "PlantingSystem.hpp"
#include "GUI/SeedBank.hpp"
#include "GUI/CursorItem.hpp"
#include "GUI/GhostPlant.hpp"
#include "Sun.hpp"
#include "Projectile.hpp"
#include "Zombie/Zombie.hpp"
#include "Lawnmower.hpp"
#include "WaveManager.hpp"
#include "GUI/ProgressBar.hpp"
#include "LevelManager.hpp"
#include <memory>
#include <vector>

class App {
public:
    enum class State {
        LEVEL_SELECT,
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void LevelSelect();
    void Start();
    void Update();
    void End();

private:
    void PlacePlant(PlantType type, int row, int col);
    bool IsCellOccupied(int row, int col) const;

    // Sun system methods
    void SpawnSun(int value, const glm::vec2& position, float targetY);
    void UpdateSuns(float deltaTime);
    void CheckSunCollection();

    // Projectile system methods
    void SpawnProjectile(ProjectileType type, int damage, int row, const glm::vec2& position);
    void UpdateProjectiles(float deltaTime);
    void UpdateShooterTargets();

    // Zombie system methods
    void SpawnZombie(ZombieType type, int lane);
    void UpdateZombies(float deltaTime);
    void CheckZombiePlantCollisions();
    void RemoveDeadPlants();

    // Lawnmower system methods
    void SpawnLawnmowers();
    void UpdateLawnmowers(float deltaTime);
    void CheckLawnmowerCollisions();

    // Explosion system methods
    void HandleCherryBombExplosion(int centerRow, int centerCol, int damage);

    State m_CurrentState = State::LEVEL_SELECT;

    // ── Level selection state ─────────────────────────────────────────
    int  m_SelectedLevel           = 1;    ///< 1–4, set in LevelSelect()
    bool m_LevelSelectInitialized  = false;
    std::vector<std::shared_ptr<Util::GameObject>> m_LevelSelectObjects;

    // ── Active lane list (set from LevelConfig) ───────────────────────
    std::vector<int> m_ActiveLanes;

    Util::Renderer m_Root;

    // Full-screen lawn background
    std::shared_ptr<Util::GameObject> m_Background;

    // 5x9 grid of plant slots (nullptr = empty cell)
    std::shared_ptr<Plant> m_PlantGrid[GameConfig::GRID_ROWS][GameConfig::GRID_COLS];

    // GUI Systems
    std::unique_ptr<SunManager> m_SunManager;
    std::unique_ptr<SeedBank> m_SeedBank;
    std::shared_ptr<CursorItem> m_CursorItem;
    std::shared_ptr<GhostPlant> m_GhostPlant;
    std::unique_ptr<PlantingSystem> m_PlantingSystem;

    // Sun collection system
    std::vector<std::shared_ptr<Sun>> m_Suns;
    float m_SkyDropTimer = 0.0f;
    static constexpr float SKY_DROP_INTERVAL = 12.0f;  // Seconds between sky drops

    // Projectile system
    std::vector<std::shared_ptr<Projectile>> m_Projectiles;

    // Zombie system
    std::vector<std::shared_ptr<Zombie>> m_Zombies;

    // Wave system
    std::unique_ptr<WaveManager> m_WaveManager;
    std::unique_ptr<ProgressBar> m_ProgressBar;

    // Lawnmower system
    std::vector<std::shared_ptr<Lawnmower>> m_Lawnmowers;

    // Game state
    bool m_GameOver = false;
    void CheckGameOver();

    // Grace period: 20s after first visible frame before zombies spawn.
    // Ensures the blank loading screen is gone and the player can prepare.
    bool m_FirstFrameRendered = false;
    float m_PreparationTimer = 0.0f;
    static constexpr float PREPARATION_DURATION = 20.0f;
};

#endif
