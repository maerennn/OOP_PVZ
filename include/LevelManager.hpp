#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "ZombieType.hpp"
#include "PlantType.hpp"
#include <vector>

/**
 * @brief Per-level configuration passed to App::Start() and WaveManager.
 *
 * LevelConfig is the single source of truth for all level-specific settings.
 * App reads it once in Start() and distributes the relevant parts to each
 * subsystem (SunManager, SeedBank, WaveManager, Lawnmower spawner).
 */
struct LevelConfig {
    /** 1-based lane numbers that are active for this level (e.g. {3} for
     *  middle-only, {1,2,3,4,5} for the full 5-lane layout). */
    std::vector<int>       activeLanes;

    /** Plants available in the Seed Bank for this level. */
    std::vector<PlantType> seedBank;

    /** Sun the player starts with (overrides SunManager::STARTING_SUN). */
    int                    startingSun;

    /** All wave definitions for this level. */
    LevelData              zombieWaves;
};

/**
 * @brief Stateless factory that builds LevelConfig objects.
 *
 * All methods are static — no instance is ever needed.
 * This is the only place that owns level-specific wave scripting; App.cpp and
 * WaveManager.cpp contain no hard-coded level data.
 */
class LevelManager {
public:
    LevelManager() = delete;

    /**
     * @brief Dispatch by level number (1–4).
     * @param levelNum  1 = Level 1-1, 2 = Level 1-2, 3 = Level 1-3, 4 = Level 1-4.
     *                  Values outside [1,4] fall back to Level 1-4.
     */
    static LevelConfig CreateLevel(int levelNum);

    // ── Individual level factories ─────────────────────────────────────
    static LevelConfig CreateLevel1_1();  ///< Tutorial: 1 lane, Peashooter, 5 Basics
    static LevelConfig CreateLevel1_2();  ///< Beginner:  3 lanes, +Sunflower
    static LevelConfig CreateLevel1_3();  ///< Intermediate: 4 lanes, +Wall-nut
    static LevelConfig CreateLevel1_4();  ///< Full day 1: 5 lanes, all plants
};

#endif // LEVEL_MANAGER_HPP
