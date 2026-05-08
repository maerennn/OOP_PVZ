#include "LevelManager.hpp"

LevelConfig LevelManager::CreateLevel(int levelNum) {
    switch (levelNum) {
        case 1:  return CreateLevel1_1();
        case 2:  return CreateLevel1_2();
        case 3:  return CreateLevel1_3();
        case 4:  return CreateLevel1_4();
        default: return CreateLevel1_4();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-1  ─  Tutorial
// ══════════════════════════════════════════════════════════════════════════════
// Single middle lane (lane 3).  Peashooter only.  5 Basic Zombies,
// one at a time so the player can learn the core loop.
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_1() {
    LevelConfig cfg;
    cfg.activeLanes = {3};                      // middle lane only
    cfg.seedBank    = {PlantType::PEASHOOTER};  // Peashooter only
    cfg.startingSun = 150;                      // enough for 1 Peashooter + bonus

    using Z = ZombieType;

    // One wave: 5 Basics in lane 3, spaced 8 s apart so each arrives solo
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3,  0.0f},
        {Z::NORMAL, 3,  8.0f},
        {Z::NORMAL, 3, 16.0f},
        {Z::NORMAL, 3, 24.0f},
        {Z::NORMAL, 3, 32.0f}
    }, false});

    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-2  ─  Beginner
// ══════════════════════════════════════════════════════════════════════════════
// Center 3 lanes (2, 3, 4).  Sunflower added so the player learns
// sun economy.  Two normal waves + a flag wave finale.
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_2() {
    LevelConfig cfg;
    cfg.activeLanes = {2, 3, 4};
    cfg.seedBank    = {PlantType::SUNFLOWER, PlantType::PEASHOOTER};
    cfg.startingSun = 75;

    using Z = ZombieType;

    // Wave 1: one Basic per active lane, staggered
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 2, 0.0f},
        {Z::NORMAL, 3, 5.0f},
        {Z::NORMAL, 4, 10.0f}
    }, false});

    // Wave 2: 4 Basics, random within active lanes (lane 0 → random)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 0, 7.0f},
        {Z::NORMAL, 0, 10.0f}
    }, false});

    // Wave 3 (flag): first Conehead appearance + 4 Normals
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 3,  0.0f},
        {Z::CONEHEAD, 0,  2.0f},
        {Z::NORMAL,   0,  0.0f},
        {Z::NORMAL,   0,  3.0f},
        {Z::NORMAL,   0,  6.0f},
        {Z::NORMAL,   0,  9.0f}
    }, true});

    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-3  ─  Intermediate
// ══════════════════════════════════════════════════════════════════════════════
// Top 4 lanes (1–4).  Wall-nut introduced.  5 waves with increasing
// Conehead presence before the flag-wave finale.
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_3() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4};
    cfg.seedBank    = {PlantType::SUNFLOWER, PlantType::PEASHOOTER,
                       PlantType::WALLNUT};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // Wave 1: easy pair, random lanes
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 6.0f}
    }, false});

    // Wave 2: forced spread across 3 of the 4 active lanes
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 1, 0.0f},
        {Z::NORMAL, 3, 4.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // Wave 3: first Conehead in a forced lane
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 2, 0.0f},
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 4.0f},
        {Z::NORMAL,   0, 8.0f}
    }, false});

    // Wave 4: two Coneheads + two Normals
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 0, 0.0f},
        {Z::CONEHEAD, 0, 3.0f},
        {Z::NORMAL,   0, 1.0f},
        {Z::NORMAL,   0, 5.0f}
    }, false});

    // Wave 5 (flag): large wave to cap the level
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 1, 0.0f},
        {Z::CONEHEAD, 4, 0.0f},
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 1.5f},
        {Z::NORMAL,   0, 3.0f},
        {Z::NORMAL,   0, 4.5f},
        {Z::NORMAL,   0, 6.0f},
        {Z::NORMAL,   0, 7.5f}
    }, true});

    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-4  ─  Full Day 1
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes, all plants.  Original 11-wave script migrated from
// WaveManager::CreateLevel1_4() so that class stays data-agnostic.
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_4() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4, 5};
    cfg.seedBank    = {PlantType::SUNFLOWER, PlantType::PEASHOOTER,
                       PlantType::WALLNUT,   PlantType::CHERRYBOMB};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // Wave 1: 1 Normal, center lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3, 0.0f}
    }, false});

    // Wave 2: 2 Normals, lanes 2 & 4
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 2, 0.0f},
        {Z::NORMAL, 4, 5.0f}
    }, false});

    // Wave 3: 3 Normals, random, staggered
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // Wave 4: 3 Normals, forced top/middle/bottom
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 1, 0.0f},
        {Z::NORMAL, 3, 3.0f},
        {Z::NORMAL, 5, 6.0f}
    }, false});

    // Wave 5: 4 Normals, random, staggered
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 3.0f},
        {Z::NORMAL, 0, 6.0f},
        {Z::NORMAL, 0, 9.0f}
    }, false});

    // Wave 6: 4 Normals, mixed forced + random
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 2, 0.0f},
        {Z::NORMAL, 4, 2.0f},
        {Z::NORMAL, 0, 5.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // Wave 7: 5 Normals, one per lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 1, 0.0f},
        {Z::NORMAL, 2, 2.0f},
        {Z::NORMAL, 3, 4.0f},
        {Z::NORMAL, 4, 6.0f},
        {Z::NORMAL, 5, 8.0f}
    }, false});

    // Wave 8: 5 Normals, faster pacing
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 2.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 0, 5.0f},
        {Z::NORMAL, 0, 7.0f}
    }, false});

    // Wave 9: 6 Normals, dense random
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 2.0f},
        {Z::NORMAL, 0, 3.0f},
        {Z::NORMAL, 0, 5.0f},
        {Z::NORMAL, 0, 6.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // Wave 10: 1 Conehead (center) + 3 Normals
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 3, 0.0f},
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 3.0f},
        {Z::NORMAL,   0, 6.0f}
    }, false});

    // Wave 11 (FLAG): 2 Coneheads + 8 Normals — huge wave
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 2, 0.0f},
        {Z::CONEHEAD, 4, 0.0f},
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 1.0f},
        {Z::NORMAL,   0, 2.0f},
        {Z::NORMAL,   0, 3.0f},
        {Z::NORMAL,   0, 4.0f},
        {Z::NORMAL,   0, 5.0f},
        {Z::NORMAL,   0, 6.0f},
        {Z::NORMAL,   0, 7.0f}
    }, true});

    return cfg;
}
