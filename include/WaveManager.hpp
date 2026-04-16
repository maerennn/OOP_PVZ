#ifndef WAVE_MANAGER_HPP
#define WAVE_MANAGER_HPP

#include "ZombieType.hpp"
#include <functional>
#include <vector>
#include <random>

/**
 * @file WaveManager.hpp
 * @brief Manages wave-based zombie spawning for a level.
 *
 * WaveManager owns the LevelData and a timer. Each frame it checks whether
 * any ZombieSpawnEntry in the current wave has reached its delay and, if so,
 * fires the spawn callback. When all zombies in a wave are spawned AND killed,
 * it advances to the next wave (with a short inter-wave pause).
 *
 * App wires a callback: (ZombieType, lane) → void, which calls SpawnZombie.
 */
class WaveManager {
public:
    using SpawnCallback = std::function<void(ZombieType type, int lane)>;

    WaveManager();
    ~WaveManager() = default;

    /**
     * @brief Load a level definition. Resets all state.
     */
    void LoadLevel(const LevelData& level);

    /**
     * @brief Advance timers, spawn zombies whose delay has been reached.
     */
    void Update(float deltaTime);

    /**
     * @brief Notify the manager that a zombie has been killed.
     *        Used for progress bar and wave-transition logic.
     */
    void OnZombieKilled();

    /**
     * @brief Set the callback invoked when a zombie should be spawned.
     */
    void SetSpawnCallback(SpawnCallback callback) { m_OnSpawn = std::move(callback); }

    // ── Queries ────────────────────────────────────────────────────────

    bool IsLevelComplete() const;
    bool IsCurrentWaveFlagWave() const;

    /** @brief Progress 0.0 → 1.0 based on zombies killed / total. */
    float GetProgress() const;

    int GetCurrentWaveIndex() const { return m_CurrentWave; }
    int GetTotalWaves() const { return static_cast<int>(m_Level.waves.size()); }
    int GetTotalZombieCount() const { return m_TotalZombies; }
    int GetZombiesKilled() const { return m_ZombiesKilled; }

    /** @brief Indices of flag waves (for progress bar marker placement). */
    const std::vector<int>& GetFlagWaveIndices() const { return m_FlagWaveIndices; }

    // ── Level Data Factory ─────────────────────────────────────────────

    static LevelData CreateLevel1_4();

private:
    void StartNextWave();
    int ResolveLane(int lane);  // converts 0 → random 1-5

    LevelData m_Level;
    int m_CurrentWave = -1;
    float m_WaveTimer = 0.0f;

    // Per-wave tracking: which entries have already been spawned
    std::vector<bool> m_Spawned;

    // Global counters
    int m_TotalZombies = 0;
    int m_ZombiesSpawned = 0;
    int m_ZombiesKilled = 0;

    // Inter-wave pause
    float m_InterWaveTimer = 0.0f;
    bool m_WaitingForNextWave = false;
    static constexpr float INTER_WAVE_PAUSE = 3.0f;  // seconds between waves

    // Precomputed flag-wave indices
    std::vector<int> m_FlagWaveIndices;

    SpawnCallback m_OnSpawn;
    std::mt19937 m_Rng;
};

#endif // WAVE_MANAGER_HPP
