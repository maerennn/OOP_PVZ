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
#include "Zombie.hpp"
#include <memory>
#include <vector>

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    void PlacePlant(PlantType type, int row, int col);
    bool IsCellOccupied(int row, int col) const;

    // Sun system methods
    void SpawnSun(int value, const glm::vec2& position);
    void UpdateSuns(float deltaTime);
    void CheckSunCollection();

    // Projectile system methods
    void SpawnProjectile(ProjectileType type, int damage, int row, const glm::vec2& position);
    void UpdateProjectiles(float deltaTime);
    void UpdateShooterTargets();

    // Zombie system methods
    void SpawnZombie(int row);
    void UpdateZombies(float deltaTime);
    void CheckZombiePlantCollisions();
    void RemoveDeadPlants();

    State m_CurrentState = State::START;

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
    float m_ZombieSpawnTimer = 0.0f;
    static constexpr float ZOMBIE_SPAWN_INTERVAL = 8.0f;  // Seconds between spawns (for testing)
};

#endif
