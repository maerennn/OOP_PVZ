#ifndef ZOMBIE_TYPE_HPP
#define ZOMBIE_TYPE_HPP

#include <vector>

/**
 * @file ZombieType.hpp
 * @brief Enums and data structures for the wave/level spawning system.
 */

enum class ZombieType {
    NORMAL,
    CONEHEAD,
    BUCKETHEAD,
    POLEVAULT
};

/**
 * @brief One zombie to spawn within a wave.
 * @param type    Which zombie subclass.
 * @param lane    1-5 (1 = top, 5 = bottom). 0 = random lane.
 * @param delay   Seconds after wave start before this zombie appears.
 */
struct ZombieSpawnEntry {
    ZombieType type;
    int lane;           // 0 = random, 1-5 = forced
    float delay;        // seconds after wave begins
};

/**
 * @brief One wave within a level.
 */
struct WaveData {
    std::vector<ZombieSpawnEntry> zombies;
    bool isFlagWave = false;  // "A huge wave of zombies is approaching!"
};

/**
 * @brief A complete level definition (sequence of waves).
 */
struct LevelData {
    std::vector<WaveData> waves;

    int TotalZombieCount() const {
        int count = 0;
        for (const auto& wave : waves) {
            count += static_cast<int>(wave.zombies.size());
        }
        return count;
    }
};

#endif // ZOMBIE_TYPE_HPP
