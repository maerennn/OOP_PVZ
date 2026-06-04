#include "LevelManager.hpp"

LevelConfig LevelManager::CreateLevel(int levelNum) {
    switch (levelNum) {
        case 1:  return CreateLevel1_1();
        case 2:  return CreateLevel1_2();
        case 3:  return CreateLevel1_3();
        case 4:  return CreateLevel1_4();
        case 6:  return CreateLevel1_6();
        case 7:  return CreateLevel1_7();
        case 8:  return CreateLevel1_8();
        case 9:  return CreateLevel1_9();
        case 10: return CreateLevel1_10();
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
    cfg.startingSun = 1000;                      // enough for 1 Peashooter + bonus

    using Z = ZombieType;

    // One wave: 5 Basics in lane 3, spaced 8 s apart so each arrives solo
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3,  0.0f},
        {Z::CONEHEAD, 3,  8.0f},
        {Z::BUCKETHEAD, 3, 16.0f},
        {Z::NORMAL, 3, 24.0f},
        {Z::NORMAL, 3, 32.0f}
    }, false});

    cfg.rewardPlant  = PlantType::SUNFLOWER;
    cfg.nextLevelNum = 2;

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

    cfg.rewardPlant  = PlantType::CHERRYBOMB;
    cfg.nextLevelNum = 3;

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
    cfg.activeLanes = {2, 3, 4};
    cfg.seedBank    = {PlantType::SUNFLOWER, PlantType::PEASHOOTER,
                       PlantType::CHERRYBOMB};
    // cfg.seedBank    = {PlantType::SUNFLOWER, PlantType::SNOWPEA, PlantType::REPEATER,
    //                    PlantType::POTATOMINE};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // Wave 1: easy pair, random lanes
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 15.0f}
    }, false});

    // Wave 2: forced spread across 3 of the 4 active lanes
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 2, 0.0f},
        {Z::NORMAL, 4, 4.0f},
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
        {Z::CONEHEAD, 2, 0.0f},
        {Z::CONEHEAD, 3, 0.0f},
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 1.5f},
        {Z::NORMAL,   0, 3.0f},
        {Z::NORMAL,   0, 4.5f},
        {Z::NORMAL,   0, 6.0f},
        {Z::NORMAL,   0, 7.5f}
    }, true});

    cfg.rewardPlant  = PlantType::WALLNUT;
    cfg.nextLevelNum = 4;

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

    cfg.rewardPlant  = PlantType::POTATOMINE;
    cfg.nextLevelNum = 6;
    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-6  ─  Pole Vaulter Debut
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes.  First appearance of the Pole Vaulting Zombie.
// 10 waves with escalating threat, culminating in a flag wave that
// introduces exactly one PoleVaultZombie.
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_6() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4, 5};
    cfg.seedBank    = {PlantType::PEASHOOTER,
                       PlantType::SUNFLOWER,
                       PlantType::CHERRYBOMB,
                       PlantType::POTATOMINE,
                       PlantType::WALLNUT};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // Wave 1: 1 Normal
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 2: 1 Normal
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 3: 1 Normal
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 4: 2 threat points → 2 Normals (gentle multi-zombie intro)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 4.0f}
    }, false});

    // Wave 5: 2 threat points → 1 Conehead (first random-lane Conehead)
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 0, 0.0f}
    }, false});

    // Wave 6: 1 Conehead (scripted, per authentic data)
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 0, 0.0f}
    }, false});

    // Wave 7: 1 Normal + 2 threat points → 3 Normals (volume pressure)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 3.0f},
        {Z::NORMAL, 0, 6.0f}
    }, false});

    // Wave 8: 1 Normal + 2 threat points → 1 Normal + 1 Conehead (mixed threat)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 3.0f},
        {Z::CONEHEAD, 0, 5.0f}
    }, false});

    // Wave 9: 1 Normal + 2 threat points → 3 Normals, tighter delays (urgency)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 2.0f},
        {Z::NORMAL, 0, 4.0f}
    }, false});

    // Wave 10 (FLAG): Conehead stand-in for Flag Zombie + 5 Normals + 1 Conehead
    //                 + 1 PoleVaultZombie at delay 5.0f for dramatic first encounter
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD,  0,  0.0f},   // flag-bearer stand-in
        {Z::NORMAL,    0,  0.0f},
        {Z::NORMAL,    0,  1.5f},
        {Z::NORMAL,    0,  3.0f},
        {Z::NORMAL,    0,  4.5f},
        {Z::NORMAL,    0,  6.0f},
        {Z::NORMAL,    0,  7.5f},
        {Z::CONEHEAD,  0,  2.0f},
        {Z::POLEVAULT, 0,  5.0f}    // first PoleVaultZombie appearance
    }, true});

    cfg.rewardPlant  = PlantType::SNOWPEA;
    cfg.nextLevelNum = 6;
    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-7  ─  "The Vaulter's Arrival"
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes.  20 waves (Big Waves at 10 & 20).
// Plants: Peashooter, Sunflower, Cherry Bomb, Potato Mine, Wall-nut, Snow Pea
// Zombies: Normal, Conehead, Pole Vaulter
//
// Point-modifier schedule (governs zombie budget per wave):
//   Waves  1– 3 : no modifier   → pure Normal Zombie introduction
//   Waves  4– 9 : +2 pts        → Normal volume ramp-up
//   Waves 10–15 : +4 pts        → Big Wave 10 (Normals only) then Conehead debut
//   Waves 16–19 : +6 pts        → Pole Vaulter escalation
//   Wave  20    : +5 pts (flag) → All three types, max intensity finale
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_7() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4, 5};
    cfg.seedBank    = {PlantType::PEASHOOTER,
                       PlantType::SUNFLOWER,
                       PlantType::CHERRYBOMB,
                       PlantType::POTATOMINE,
                       PlantType::WALLNUT,
                       PlantType::SNOWPEA};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // ── Waves 1-3 : No modifier ─ single/pair Normal introduction ─────────
    // Wave 1: 1 Normal (centre lane, ultra-soft opener)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3, 0.0f}
    }, false});

    // Wave 2: 1 Normal, random lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 3: 2 Normals, spread
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // ── Waves 4-9 : +2 pts ─ Normal volume ramp ───────────────────────────
    // Wave 4: 2 Normals, forced outer lanes
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 1, 0.0f},
        {Z::NORMAL, 5, 5.0f}
    }, false});

    // Wave 5: 3 Normals, random, staggered
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // Wave 6: 3 Normals (main) + 1 Normal ambush ~15 s later
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0,  0.0f},
        {Z::NORMAL, 0,  3.0f},
        {Z::NORMAL, 0,  6.0f},
        {Z::NORMAL, 0, 15.0f}   // ambush: lone straggler after main group
    }, false});

    // Wave 7: 4 Normals, one per priority lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 1, 0.0f},
        {Z::NORMAL, 3, 3.0f},
        {Z::NORMAL, 5, 6.0f},
        {Z::NORMAL, 0, 9.0f}
    }, false});

    // Wave 8: 4 Normals (main) + 1 Normal ambush
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0,  0.0f},
        {Z::NORMAL, 0,  2.5f},
        {Z::NORMAL, 0,  5.0f},
        {Z::NORMAL, 0,  7.5f},
        {Z::NORMAL, 0, 16.0f}   // ambush
    }, false});

    // Wave 9: 5 Normals, tighter — final prep before Big Wave
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 2.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 0, 6.0f},
        {Z::NORMAL, 0, 8.0f}
    }, false});

    // ── Wave 10 : +4 pts ─ BIG WAVE — Normals only flood ─────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 1.0f},
        {Z::NORMAL, 0, 2.0f},
        {Z::NORMAL, 0, 3.0f},
        {Z::NORMAL, 0, 4.0f},
        {Z::NORMAL, 1, 4.0f},
        {Z::NORMAL, 0, 5.0f},
        {Z::NORMAL, 0, 6.0f},
        {Z::NORMAL, 5, 6.0f}
    }, true});

    // ── Waves 11-15 : +4 pts ─ Conehead debut, Pole Vaulter lurks ─────────
    // Wave 11: first Conehead appearance — forced centre lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 3.0f},
        {Z::NORMAL,   0, 6.0f},
        {Z::CONEHEAD, 3, 0.0f}
    }, false});

    // Wave 12: 3 Normals + 2 Coneheads
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 4.0f},
        {Z::NORMAL,   0, 8.0f},
        {Z::CONEHEAD, 0, 2.0f},
        {Z::CONEHEAD, 0, 6.0f}
    }, false});

    // Wave 13: 2 Normals + 2 Coneheads (main) + first Pole Vaulter as ambush
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0,  0.0f},
        {Z::NORMAL,   0,  5.0f},
        {Z::CONEHEAD, 0,  2.0f},
        {Z::CONEHEAD, 0,  7.0f},
        {Z::POLEVAULT, 0, 15.0f}  // ambush: surprise first Pole Vaulter
    }, false});

    // Wave 14: Pole Vaulter enters main rotation
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 4.0f},
        {Z::CONEHEAD,  0, 2.0f},
        {Z::CONEHEAD,  0, 6.0f},
        {Z::POLEVAULT, 0, 0.0f}
    }, false});

    // Wave 15: 2 Normals + 3 Coneheads + 1 Pole Vaulter — peak +4 tier
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 5.0f},
        {Z::CONEHEAD,  0, 0.0f},
        {Z::CONEHEAD,  0, 3.0f},
        {Z::CONEHEAD,  0, 6.0f},
        {Z::POLEVAULT, 0, 2.0f}
    }, false});

    // ── Waves 16-19 : +6 pts ─ Two Pole Vaulters, heavy mixed pressure ─────
    // Wave 16: 2 Normals + 2 Coneheads + 2 Pole Vaulters
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 5.0f},
        {Z::CONEHEAD,  0, 1.5f},
        {Z::CONEHEAD,  0, 7.0f},
        {Z::POLEVAULT, 0, 0.0f},
        {Z::POLEVAULT, 0, 4.0f}
    }, false});

    // Wave 17: 2 Normals + 3 Coneheads + 2 Pole Vaulters
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 6.0f},
        {Z::CONEHEAD,  0, 0.0f},
        {Z::CONEHEAD,  0, 3.0f},
        {Z::CONEHEAD,  0, 6.0f},
        {Z::POLEVAULT, 0, 1.5f},
        {Z::POLEVAULT, 0, 5.0f}
    }, false});

    // Wave 18: 1 Normal + 3 Coneheads + 2 Pole Vaulters + 1 Normal ambush
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0,  0.0f},
        {Z::CONEHEAD,  0,  0.0f},
        {Z::CONEHEAD,  0,  3.0f},
        {Z::CONEHEAD,  0,  7.0f},
        {Z::POLEVAULT, 0,  2.0f},
        {Z::POLEVAULT, 0,  5.0f},
        {Z::NORMAL,    0, 14.0f}   // ambush straggler
    }, false});

    // Wave 19: 2 Normals + 3 Coneheads + 3 Pole Vaulters — save Cherry Bomb!
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 7.0f},
        {Z::CONEHEAD,  0, 0.0f},
        {Z::CONEHEAD,  0, 3.0f},
        {Z::CONEHEAD,  0, 6.0f},
        {Z::POLEVAULT, 0, 1.0f},
        {Z::POLEVAULT, 0, 4.0f},
        {Z::POLEVAULT, 0, 7.0f}
    }, false});

    // ── Wave 20 : +5 pts ─ FINAL BIG WAVE — all three types ───────────────
    // 4 Normals + 4 Coneheads + 3 Pole Vaulters (main burst)
    // + 1 Conehead ambush 18 s in for the final sting
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    1,  0.0f},
        {Z::NORMAL,    3,  1.0f},
        {Z::NORMAL,    5,  2.0f},
        {Z::NORMAL,    0,  4.0f},
        {Z::CONEHEAD,  0,  0.0f},
        {Z::CONEHEAD,  0,  1.5f},
        {Z::CONEHEAD,  0,  3.0f},
        {Z::CONEHEAD,  0,  5.0f},
        {Z::POLEVAULT, 2,  0.0f},
        {Z::POLEVAULT, 4,  2.0f},
        {Z::POLEVAULT, 0,  5.0f},
        {Z::CONEHEAD,  0, 18.0f}   // ambush: final sting after main cluster
    }, true});
    cfg.rewardPlant  = PlantType::CHOMPER;
    cfg.nextLevelNum = 8;
    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-8  ─  "Bucket Brigade"
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes.  10 waves (one Final Big Wave at Wave 10).
// Plants: Peashooter, Sunflower, Cherry Bomb, Potato Mine, Wall-nut, Snow Pea
// Zombies: Normal, Conehead, Buckethead (debut)
//
// Point-modifier schedule:
//   Waves 1–3 : no modifier  → solo Regular Zombie intro
//   Waves 4–5 : +2 pts, no guaranteed spawn (dynamic only)
//   Wave  6   : no modifier  → scripted breather (1 Regular)
//   Waves 7–9 : +2 pts       → Buckethead introduced gradually
//   Wave  10  : final rush   → all three types
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_8() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4, 5};
    cfg.seedBank    = {PlantType::PEASHOOTER,
                       PlantType::SUNFLOWER,
                       PlantType::CHERRYBOMB,
                       PlantType::POTATOMINE,
                       PlantType::WALLNUT,
                       PlantType::SNOWPEA};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // ── Waves 1-3 : No modifier ─ solo Regular opener ─────────────────────
    // Wave 1: 1 Normal, centre lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3, 0.0f}
    }, false});

    // Wave 2: 1 Normal, random lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 3: 1 Normal, random lane — last no-modifier wave
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // ── Waves 4-5 : +2 pts, no guaranteed spawn (dynamic pool) ────────────
    // Wave 4: dynamic — 2 Normals in random lanes (first multi-zombie pressure)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 5.0f}
    }, false});

    // Wave 5: dynamic — 1 Normal + 1 Conehead (Conehead via dynamic spawn)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::CONEHEAD, 0, 4.0f}
    }, false});

    // ── Wave 6 : No modifier ─ scripted breather ──────────────────────────
    // Wave 6: 1 Normal, scripted — pause to rearm Potato Mines / repair Wall-nuts
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // ── Waves 7-9 : +2 pts ─ Buckethead debut ─────────────────────────────
    // Wave 7: 2 Normals (main) + 1 Conehead (ambush, random lane)
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0,  0.0f},
        {Z::NORMAL,   0,  5.0f},
        {Z::CONEHEAD, 0, 12.0f}   // ambush: trails the main pair
    }, false});

    // Wave 8: 1 Normal (main) + 1 Buckethead (ambush) — FIRST BUCKETHEAD
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::BUCKETHEAD, 0, 10.0f}  // ambush: Buckethead arrives after Normal draws attention
    }, false});

    // Wave 9: 1 Normal + 1 Conehead (main) + 1 Buckethead (ambush)
    //         All three types present — full preview before finale
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::CONEHEAD,   0,  4.0f},
        {Z::BUCKETHEAD, 0, 12.0f}  // ambush: lone Buckethead after main duo
    }, false});

    // ── Wave 10 : Final Big Wave ─ all three types ─────────────────────────
    // 4 Normals + 3 Coneheads + 2 Bucketheads (main burst)
    // + 1 Normal ambush trailing the cluster
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     1,  0.0f},
        {Z::NORMAL,     3,  1.0f},
        {Z::NORMAL,     5,  2.0f},
        {Z::NORMAL,     0,  4.0f},
        {Z::CONEHEAD,   0,  0.0f},
        {Z::CONEHEAD,   0,  2.0f},
        {Z::CONEHEAD,   0,  5.0f},
        {Z::BUCKETHEAD, 0,  1.0f},
        {Z::BUCKETHEAD, 0,  4.0f},
        {Z::NORMAL,     0, 16.0f}   // ambush: straggler after main cluster
    }, true});

    cfg.rewardPlant  = PlantType::REPEATER;
    cfg.nextLevelNum = 9;
    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-9  ─  "All Hands on Deck"
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes.  20 waves (Big Waves at 10 & 20).
// Plants: Sunflower, Peashooter, Snow Pea, Wall-nut, Potato Mine, Repeater
// Zombies: Normal, Conehead, Pole Vaulter, Buckethead (all four types)
//
// Point-modifier schedule:
//   Waves  1– 3 : no modifier   → solo Regular intro
//   Waves  4– 6 : +2 pts, None Guaranteed → Conehead debut via dynamic pool
//   Waves  7– 9 : +2 pts, 1 Guaranteed   → Pole Vaulter first appears (Wave 9)
//   Wave  10    : BIG WAVE, +4 pts        → Buckethead shock debut inside huge wave
//   Waves 11–12 : +4 pts, None Guaranteed → Buckethead enters regular rotation
//   Waves 13–15 : +4 pts, 1 Guaranteed   → all four types; each type gets a guaranteed slot
//   Waves 16–18 : +6 pts, None Guaranteed → dual Bucketheads, dual Pole Vaulters
//   Wave  19    : +6 pts, 1 Guaranteed   → near-finale intensity; dual Bucketheads
//   Wave  20    : FINAL BIG WAVE         → all four types, 12-zombie chaos
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_9() {
    LevelConfig cfg;
    cfg.activeLanes = {1, 2, 3, 4, 5};
    cfg.seedBank    = {PlantType::SUNFLOWER,
                       PlantType::PEASHOOTER,
                       PlantType::SNOWPEA,
                       PlantType::WALLNUT,
                       PlantType::POTATOMINE,
                       PlantType::REPEATER};
    cfg.startingSun = 50;

    using Z = ZombieType;

    // ── Waves 1-3 : No modifier ─ solo Regular intro ──────────────────────
    // Wave 1: 1 Normal, centre lane — ultra-soft opener
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 3, 0.0f}
    }, false});

    // Wave 2: 1 Normal, random lane
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // Wave 3: 1 Normal, random lane — last no-modifier wave
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // ── Waves 4-6 : +2 pts, None Guaranteed ─ Conehead debut ──────────────
    // Wave 4: 2 Normals, random lanes staggered — first multi-zombie pressure
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f},
        {Z::NORMAL, 0, 5.0f}
    }, false});

    // Wave 5: 1 Normal + 1 Conehead — FIRST CONEHEAD APPEARANCE
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::CONEHEAD, 0, 4.0f}
    }, false});

    // Wave 6: 2 Coneheads — armor is now the standard threat
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 0, 0.0f},
        {Z::CONEHEAD, 0, 5.0f}
    }, false});

    // ── Waves 7-9 : +2 pts, 1 Guaranteed ─ Conehead baseline; PV teaser ───
    // Wave 7: 1 Conehead (guaranteed) + 2 Normals — FIRST GUARANTEED CONEHEAD
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD, 0, 0.0f},   // guaranteed
        {Z::NORMAL,   0, 2.0f},
        {Z::NORMAL,   0, 6.0f}
    }, false});

    // Wave 8: 1 Normal (guaranteed) + 1 Conehead + 1 Normal
    //         Conehead trails the opener by ~4 s — "second punch" timing
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},   // guaranteed
        {Z::NORMAL,   0, 3.0f},
        {Z::CONEHEAD, 0, 7.0f}
    }, false});

    // Wave 9: 1 Normal (guaranteed) + 1 Conehead + 1 Pole Vaulter
    //         FIRST POLE VAULTING ZOMBIE — arrives ~5 s after the pair
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},   // guaranteed
        {Z::CONEHEAD,  0, 3.0f},
        {Z::POLEVAULT, 0, 8.0f}   // first PoleVault appearance
    }, false});

    // ── Wave 10 : BIG WAVE, +4 pts ─ Buckethead shock debut ───────────────
    // 3 Normals (outer lanes) + 3 Coneheads (random) + 2 Pole Vaulters (flanks)
    // + 1 Buckethead (random, delayed 4 s) — FIRST BUCKETHEAD
    // "A huge wave of zombies is approaching!"
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     1,  0.0f},
        {Z::NORMAL,     3,  1.0f},
        {Z::NORMAL,     5,  2.0f},
        {Z::CONEHEAD,   0,  0.0f},
        {Z::CONEHEAD,   0,  2.0f},
        {Z::CONEHEAD,   0,  4.0f},
        {Z::POLEVAULT,  2,  0.0f},
        {Z::POLEVAULT,  4,  1.5f},
        {Z::BUCKETHEAD, 0,  4.0f}   // first Buckethead — arrives mid-chaos
    }, true});

    // ── Waves 11-12 : +4 pts, None Guaranteed ─ Buckethead integration ─────
    // Wave 11: 1 Normal + 2 Coneheads + 1 Buckethead — Buckethead encore, solo
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::CONEHEAD,   0, 2.0f},
        {Z::CONEHEAD,   0, 6.0f},
        {Z::BUCKETHEAD, 0, 4.0f}
    }, false});

    // Wave 12: 2 Coneheads + 1 Pole Vaulter + 1 Buckethead
    //          Both heavyweights share the wave for the first time
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD,   0, 0.0f},
        {Z::CONEHEAD,   0, 4.0f},
        {Z::POLEVAULT,  0, 2.0f},
        {Z::BUCKETHEAD, 0, 1.0f}
    }, false});

    // ── Waves 13-15 : +4 pts, 1 Guaranteed ─ all four types; escalating slots
    // Wave 13: 1 Conehead (guaranteed) + 1 Conehead + 1 Pole Vaulter + 1 Buckethead
    //          ALL FOUR ZOMBIE TYPES ACTIVE SIMULTANEOUSLY FOR THE FIRST TIME
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD,   0, 0.0f},   // guaranteed
        {Z::CONEHEAD,   0, 5.0f},
        {Z::POLEVAULT,  0, 2.0f},
        {Z::BUCKETHEAD, 0, 3.0f}
    }, false});

    // Wave 14: 1 Pole Vaulter (guaranteed) + 2 Coneheads + 1 Buckethead
    //          FIRST GUARANTEED POLE VAULTER — agility is now a constant baseline
    cfg.zombieWaves.waves.push_back({{
        {Z::POLEVAULT,  0, 0.0f},   // guaranteed
        {Z::CONEHEAD,   0, 1.5f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::BUCKETHEAD, 0, 3.0f}
    }, false});

    // Wave 15: 1 Buckethead (guaranteed) + 2 Coneheads + 1 Pole Vaulter
    //          FIRST GUARANTEED BUCKETHEAD — heavy armor is now permanent baseline
    cfg.zombieWaves.waves.push_back({{
        {Z::BUCKETHEAD, 0, 0.0f},   // guaranteed
        {Z::CONEHEAD,   0, 1.0f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::POLEVAULT,  0, 3.0f}
    }, false});

    // ── Waves 16-18 : +6 pts, None Guaranteed ─ dual Bucketheads, dual PVs ─
    // Wave 16: 2 Coneheads + 2 Pole Vaulters + 1 Buckethead
    //          Dual Pole Vaulters create simultaneous split-vault across flanks
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD,   0, 0.0f},
        {Z::CONEHEAD,   0, 4.0f},
        {Z::POLEVAULT,  2, 0.0f},
        {Z::POLEVAULT,  4, 2.0f},
        {Z::BUCKETHEAD, 0, 1.5f}
    }, false});

    // Wave 17: 1 Normal + 2 Coneheads + 1 Pole Vaulter + 2 Bucketheads
    //          FIRST DUAL BUCKETHEAD WAVE — double armor drain
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::CONEHEAD,   0, 1.0f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::POLEVAULT,  0, 3.0f},
        {Z::BUCKETHEAD, 0, 0.0f},
        {Z::BUCKETHEAD, 0, 4.0f}
    }, false});

    // Wave 18: 3 Coneheads + 2 Pole Vaulters + 1 Buckethead
    //          Speed + armor density at maximum for a None Guaranteed wave
    cfg.zombieWaves.waves.push_back({{
        {Z::CONEHEAD,   0, 0.0f},
        {Z::CONEHEAD,   0, 3.0f},
        {Z::CONEHEAD,   0, 6.0f},
        {Z::POLEVAULT,  0, 1.0f},
        {Z::POLEVAULT,  0, 4.5f},
        {Z::BUCKETHEAD, 0, 2.0f}
    }, false});

    // ── Wave 19 : +6 pts, 1 Guaranteed ─ near-finale; dual Bucketheads ─────
    // 1 Buckethead (guaranteed) + 2 Coneheads + 2 Pole Vaulters + 1 Buckethead
    cfg.zombieWaves.waves.push_back({{
        {Z::BUCKETHEAD, 0, 0.0f},   // guaranteed
        {Z::CONEHEAD,   0, 1.5f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::POLEVAULT,  0, 2.0f},
        {Z::POLEVAULT,  0, 6.0f},
        {Z::BUCKETHEAD, 0, 3.5f}
    }, false});

    // ── Wave 20 : FINAL BIG WAVE ─ all four types, total chaos ────────────
    // 2 Normals (outer lanes) + 4 Coneheads (random) +
    // 3 Pole Vaulters (lanes 2, 3, 4) + 3 Bucketheads (random)
    // "A huge wave of zombies is approaching!"
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     1,  0.0f},
        {Z::NORMAL,     5,  1.5f},
        {Z::CONEHEAD,   0,  0.0f},
        {Z::CONEHEAD,   0,  2.0f},
        {Z::CONEHEAD,   0,  4.0f},
        {Z::CONEHEAD,   0,  6.0f},
        {Z::POLEVAULT,  2,  0.0f},
        {Z::POLEVAULT,  3,  1.0f},
        {Z::POLEVAULT,  4,  2.5f},
        {Z::BUCKETHEAD, 0,  0.0f},
        {Z::BUCKETHEAD, 0,  2.0f},
        {Z::BUCKETHEAD, 0,  5.0f}
    }, true});
    
    cfg.nextLevelNum = 10;
    return cfg;
}

// ══════════════════════════════════════════════════════════════════════════════
// Level 1-10  ─  Conveyor Belt
// ══════════════════════════════════════════════════════════════════════════════
// All 5 lanes.  No sun mechanic — plants are delivered by a moving belt.
// 20 waves (flag waves at W10 and W20).
// Zombies: Normal → Conehead → PoleVault → Buckethead (introduced progressively)
// ══════════════════════════════════════════════════════════════════════════════
LevelConfig LevelManager::CreateLevel1_10() {
    LevelConfig cfg;
    cfg.activeLanes    = {1, 2, 3, 4, 5};
    cfg.startingSun    = 0;          // sun unused — conveyor belt provides plants
    cfg.useConveyorBelt  = true;
    cfg.conveyorInterval = 7.5f;
    cfg.conveyorPool   = {
        PlantType::PEASHOOTER,
        PlantType::WALLNUT,
        PlantType::CHERRYBOMB,
        PlantType::REPEATER,
        PlantType::SNOWPEA,
        PlantType::POTATOMINE,
        PlantType::CHOMPER,
    };
    cfg.rewardPlant  = std::nullopt;
    cfg.nextLevelNum = 0;

    using Z = ZombieType;

    // ── W1–3 : Single Normal ──────────────────────────────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL, 0, 0.0f}
    }, false});

    // ── W4 : 1 Normal + 1 Conehead (Conehead debut) ───────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::CONEHEAD, 0, 5.0f}
    }, false});

    // ── W5 : 2 Normals + 1 Conehead ──────────────────────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 4.0f},
        {Z::CONEHEAD, 0, 2.0f}
    }, false});

    // ── W6 : 2 Normals + 2 Coneheads ─────────────────────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,   0, 0.0f},
        {Z::NORMAL,   0, 5.0f},
        {Z::CONEHEAD, 0, 2.0f},
        {Z::CONEHEAD, 0, 7.0f}
    }, false});

    // ── W7 : 2 Normals + 1 Conehead + 1 PoleVault (PoleVault debut) ──────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 5.0f},
        {Z::CONEHEAD,  0, 3.0f},
        {Z::POLEVAULT, 0, 0.0f}
    }, false});

    // ── W8 : 3 Normals + 1 Conehead + 1 PoleVault ────────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 3.0f},
        {Z::NORMAL,    0, 6.0f},
        {Z::CONEHEAD,  0, 2.0f},
        {Z::POLEVAULT, 0, 4.0f}
    }, false});

    // ── W9 : 3 Normals + 2 Coneheads + 1 PoleVault ───────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    0, 0.0f},
        {Z::NORMAL,    0, 3.0f},
        {Z::NORMAL,    0, 6.0f},
        {Z::CONEHEAD,  0, 1.5f},
        {Z::CONEHEAD,  0, 5.0f},
        {Z::POLEVAULT, 0, 3.0f}
    }, false});

    // ── W10 (FLAG) : 5 Normals (one per lane) + 1 Conehead + 1 PoleVault ─
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,    1, 0.0f},
        {Z::NORMAL,    2, 2.0f},
        {Z::NORMAL,    3, 4.0f},
        {Z::NORMAL,    4, 6.0f},
        {Z::NORMAL,    5, 8.0f},
        {Z::CONEHEAD,  0, 0.0f},
        {Z::POLEVAULT, 0, 3.0f}
    }, true});

    // ── W11 : 3 Normals + 1 Conehead + 1 Buckethead (Buckethead debut) ───
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::NORMAL,     0, 3.0f},
        {Z::NORMAL,     0, 6.0f},
        {Z::CONEHEAD,   0, 2.0f},
        {Z::BUCKETHEAD, 0, 0.0f}
    }, false});

    // ── W12 : 4 Normals + 2 Coneheads + 1 Buckethead ─────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::NORMAL,     0, 2.5f},
        {Z::NORMAL,     0, 5.0f},
        {Z::NORMAL,     0, 7.5f},
        {Z::CONEHEAD,   0, 1.0f},
        {Z::CONEHEAD,   0, 5.5f},
        {Z::BUCKETHEAD, 0, 3.0f}
    }, false});

    // ── W13 : 4 Normals + 2 Coneheads + 2 Bucketheads ────────────────────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::NORMAL,     0, 3.0f},
        {Z::NORMAL,     0, 6.0f},
        {Z::NORMAL,     0, 9.0f},
        {Z::CONEHEAD,   0, 1.5f},
        {Z::CONEHEAD,   0, 7.5f},
        {Z::BUCKETHEAD, 0, 0.0f},
        {Z::BUCKETHEAD, 0, 5.0f}
    }, false});

    // ── W14 : 5 Normals + 2 Coneheads + 1 PoleVault + 1 Buckethead ──────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::NORMAL,     0, 2.0f},
        {Z::NORMAL,     0, 4.0f},
        {Z::NORMAL,     0, 6.0f},
        {Z::NORMAL,     0, 8.0f},
        {Z::CONEHEAD,   0, 1.0f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::POLEVAULT,  0, 3.0f},
        {Z::BUCKETHEAD, 0, 2.0f}
    }, false});

    // ── W15 : 5 Normals + 3 Coneheads + 2 PoleVaults + 2 Bucketheads ────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::NORMAL,     0,  2.5f},
        {Z::NORMAL,     0,  5.0f},
        {Z::NORMAL,     0,  7.5f},
        {Z::NORMAL,     0, 10.0f},
        {Z::CONEHEAD,   0,  1.0f},
        {Z::CONEHEAD,   0,  4.0f},
        {Z::CONEHEAD,   0,  8.0f},
        {Z::POLEVAULT,  0,  0.0f},
        {Z::POLEVAULT,  0,  5.0f},
        {Z::BUCKETHEAD, 0,  2.0f},
        {Z::BUCKETHEAD, 0,  7.0f}
    }, false});

    // ── W16 : 5 Normals + 3 Coneheads + 2 PoleVaults + 2 Bucketheads ────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0, 0.0f},
        {Z::NORMAL,     0, 2.0f},
        {Z::NORMAL,     0, 4.0f},
        {Z::NORMAL,     0, 7.0f},
        {Z::NORMAL,     0, 9.0f},
        {Z::CONEHEAD,   0, 1.0f},
        {Z::CONEHEAD,   0, 5.0f},
        {Z::CONEHEAD,   0, 8.0f},
        {Z::POLEVAULT,  0, 2.0f},
        {Z::POLEVAULT,  0, 6.0f},
        {Z::BUCKETHEAD, 0, 0.0f},
        {Z::BUCKETHEAD, 0, 5.5f}
    }, false});

    // ── W17 : 6 Normals + 3 Coneheads + 3 PoleVaults + 2 Bucketheads ────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::NORMAL,     0,  2.0f},
        {Z::NORMAL,     0,  4.0f},
        {Z::NORMAL,     0,  6.0f},
        {Z::NORMAL,     0,  8.5f},
        {Z::NORMAL,     0, 11.0f},
        {Z::CONEHEAD,   0,  1.0f},
        {Z::CONEHEAD,   0,  5.0f},
        {Z::CONEHEAD,   0,  9.0f},
        {Z::POLEVAULT,  0,  0.0f},
        {Z::POLEVAULT,  0,  3.5f},
        {Z::POLEVAULT,  0,  7.0f},
        {Z::BUCKETHEAD, 0,  2.0f},
        {Z::BUCKETHEAD, 0,  7.5f}
    }, false});

    // ── W18 : 6 Normals + 4 Coneheads + 3 PoleVaults + 3 Bucketheads ────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::NORMAL,     0,  2.0f},
        {Z::NORMAL,     0,  4.5f},
        {Z::NORMAL,     0,  7.0f},
        {Z::NORMAL,     0,  9.0f},
        {Z::NORMAL,     0, 11.5f},
        {Z::CONEHEAD,   0,  1.0f},
        {Z::CONEHEAD,   0,  3.5f},
        {Z::CONEHEAD,   0,  6.0f},
        {Z::CONEHEAD,   0,  9.5f},
        {Z::POLEVAULT,  0,  0.0f},
        {Z::POLEVAULT,  0,  4.0f},
        {Z::POLEVAULT,  0,  8.0f},
        {Z::BUCKETHEAD, 0,  1.5f},
        {Z::BUCKETHEAD, 0,  5.5f},
        {Z::BUCKETHEAD, 0, 10.0f}
    }, false});

    // ── W19 : 7 Normals + 4 Coneheads + 3 PoleVaults + 3 Bucketheads ────
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     0,  0.0f},
        {Z::NORMAL,     0,  2.0f},
        {Z::NORMAL,     0,  3.5f},
        {Z::NORMAL,     0,  5.5f},
        {Z::NORMAL,     0,  7.0f},
        {Z::NORMAL,     0,  9.0f},
        {Z::NORMAL,     0, 11.0f},
        {Z::CONEHEAD,   0,  1.0f},
        {Z::CONEHEAD,   0,  3.0f},
        {Z::CONEHEAD,   0,  6.5f},
        {Z::CONEHEAD,   0, 10.0f},
        {Z::POLEVAULT,  0,  0.0f},
        {Z::POLEVAULT,  0,  4.0f},
        {Z::POLEVAULT,  0,  8.5f},
        {Z::BUCKETHEAD, 0,  1.5f},
        {Z::BUCKETHEAD, 0,  6.0f},
        {Z::BUCKETHEAD, 0, 10.5f}
    }, false});

    // ── W20 (FLAG / FINAL) : 8 Normals + 4 Coneheads + 3 PoleVaults + 2 Bucketheads ─
    cfg.zombieWaves.waves.push_back({{
        {Z::NORMAL,     1,  0.0f},
        {Z::NORMAL,     2,  1.0f},
        {Z::NORMAL,     3,  2.0f},
        {Z::NORMAL,     4,  3.0f},
        {Z::NORMAL,     5,  4.0f},
        {Z::NORMAL,     0,  1.5f},
        {Z::NORMAL,     0,  3.5f},
        {Z::NORMAL,     0,  6.0f},
        {Z::CONEHEAD,   0,  0.0f},
        {Z::CONEHEAD,   0,  2.5f},
        {Z::CONEHEAD,   0,  5.0f},
        {Z::CONEHEAD,   0,  7.5f},
        {Z::POLEVAULT,  2,  0.0f},
        {Z::POLEVAULT,  3,  2.0f},
        {Z::POLEVAULT,  4,  4.0f},
        {Z::BUCKETHEAD, 0,  1.0f},
        {Z::BUCKETHEAD, 0,  5.5f}
    }, true});

    return cfg;
}
