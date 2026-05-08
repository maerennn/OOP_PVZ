#include "WaveManager.hpp"
#include "Util/Logger.hpp"

WaveManager::WaveManager()
    : m_Rng(std::random_device{}())
{
}

void WaveManager::LoadLevel(const LevelData& level) {
    m_Level = level;
    m_CurrentWave = -1;
    m_WaveTimer = 0.0f;
    m_TotalZombies = level.TotalZombieCount();
    m_ZombiesSpawned = 0;
    m_ZombiesKilled = 0;
    m_InterWaveTimer = 0.0f;
    m_WaitingForNextWave = false;

    // Precompute flag wave indices
    m_FlagWaveIndices.clear();
    for (int i = 0; i < static_cast<int>(m_Level.waves.size()); ++i) {
        if (m_Level.waves[i].isFlagWave) {
            m_FlagWaveIndices.push_back(i);
        }
    }

    LOG_DEBUG("WaveManager: Loaded level with {} waves, {} total zombies",
              m_Level.waves.size(), m_TotalZombies);

    // Start the first wave
    StartNextWave();
}

void WaveManager::Update(float deltaTime) {
    if (IsLevelComplete()) return;

    // ── Inter-wave pause ────────────────────────────────────────────────
    if (m_WaitingForNextWave) {
        m_InterWaveTimer += deltaTime;
        if (m_InterWaveTimer >= INTER_WAVE_PAUSE) {
            m_WaitingForNextWave = false;
            StartNextWave();
        }
        return;
    }

    if (m_CurrentWave < 0 || m_CurrentWave >= static_cast<int>(m_Level.waves.size())) {
        return;
    }

    m_WaveTimer += deltaTime;

    const auto& wave = m_Level.waves[m_CurrentWave];

    // ── Spawn zombies whose delay has been reached ──────────────────────
    bool allSpawned = true;
    for (size_t i = 0; i < wave.zombies.size(); ++i) {
        if (m_Spawned[i]) continue;

        if (m_WaveTimer >= wave.zombies[i].delay) {
            int lane = ResolveLane(wave.zombies[i].lane);
            if (m_OnSpawn) {
                m_OnSpawn(wave.zombies[i].type, lane);
            }
            m_Spawned[i] = true;
            m_ZombiesSpawned++;
            LOG_DEBUG("WaveManager: Spawned zombie {} of wave {} (lane {})",
                      i + 1, m_CurrentWave + 1, lane);
        } else {
            allSpawned = false;
        }
    }

    // Recheck after spawning
    if (!allSpawned) return;

    // ── All spawned in this wave — check if all killed to advance ───────
    // We count kills globally. The number of zombies expected up to and
    // including this wave:
    int expectedKillsForAdvance = 0;
    for (int w = 0; w <= m_CurrentWave; ++w) {
        expectedKillsForAdvance += static_cast<int>(m_Level.waves[w].zombies.size());
    }

    if (m_ZombiesKilled >= expectedKillsForAdvance) {
        // All zombies from this wave are dead
        if (m_CurrentWave + 1 < static_cast<int>(m_Level.waves.size())) {
            m_WaitingForNextWave = true;
            m_InterWaveTimer = 0.0f;
            LOG_DEBUG("WaveManager: Wave {} complete, pausing before next wave",
                      m_CurrentWave + 1);
        } else {
            LOG_DEBUG("WaveManager: All waves complete! Level finished.");
        }
    }
}

void WaveManager::OnZombieKilled() {
    m_ZombiesKilled++;
    LOG_DEBUG("WaveManager: Zombie killed ({}/{})", m_ZombiesKilled, m_TotalZombies);
}

bool WaveManager::IsLevelComplete() const {
    return m_ZombiesKilled >= m_TotalZombies && m_TotalZombies > 0;
}

bool WaveManager::IsCurrentWaveFlagWave() const {
    if (m_CurrentWave < 0 || m_CurrentWave >= static_cast<int>(m_Level.waves.size())) {
        return false;
    }
    return m_Level.waves[m_CurrentWave].isFlagWave;
}

float WaveManager::GetProgress() const {
    if (m_TotalZombies <= 0) return 0.0f;
    return static_cast<float>(m_ZombiesKilled) / static_cast<float>(m_TotalZombies);
}

void WaveManager::StartNextWave() {
    m_CurrentWave++;
    m_WaveTimer = 0.0f;

    if (m_CurrentWave >= static_cast<int>(m_Level.waves.size())) {
        return;
    }

    const auto& wave = m_Level.waves[m_CurrentWave];
    m_Spawned.assign(wave.zombies.size(), false);

    if (wave.isFlagWave) {
        LOG_DEBUG("WaveManager: === WAVE {} (FLAG WAVE) === A HUGE WAVE OF ZOMBIES IS APPROACHING!",
                  m_CurrentWave + 1);
    } else {
        LOG_DEBUG("WaveManager: === WAVE {} ===", m_CurrentWave + 1);
    }
}

int WaveManager::ResolveLane(int lane) {
    if (lane >= 1 && lane <= 5) {
        return lane;
    }
    // lane == 0: pick a random lane from the active set
    if (!m_ActiveLanes.empty()) {
        std::uniform_int_distribution<int> dist(
            0, static_cast<int>(m_ActiveLanes.size()) - 1);
        return m_ActiveLanes[dist(m_Rng)];
    }
    // Fallback when no active-lane list is set
    std::uniform_int_distribution<int> dist(1, 5);
    return dist(m_Rng);
}

// Level data is now defined in LevelManager.cpp.
// WaveManager::CreateLevel1_4() has been removed — use LevelManager::CreateLevel().
