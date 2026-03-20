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
#include <memory>

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
};

#endif
