#include "ResourceManager.hpp"
#include "config.hpp"
#include "Util/Logger.hpp"
#include <cstdio>

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::Initialize() {
    if (m_Initialized) {
        LOG_DEBUG("ResourceManager already initialized");
        return;
    }

    LOG_DEBUG("ResourceManager: Loading all animation paths...");

    // ══════════════════════════════════════════════════════════════════════
    // ZOMBIE ANIMATIONS
    // ══════════════════════════════════════════════════════════════════════

    // Basic Zombie - Walking (frames 47-137)
    RegisterAnimation(
        ZOMBIE_BASIC_WALKING,
        GenerateFramePaths(
            RESOURCE_DIR "/zombies/Basic Zombie/Walking/basiczombie%04d.png",
            47, 137),
        50,   // 50ms per frame
        true  // looping
    );

    // Basic Zombie - Attacking (frames 138-178)
    RegisterAnimation(
        ZOMBIE_BASIC_ATTACKING,
        GenerateFramePaths(
            RESOURCE_DIR "/zombies/Basic Zombie/Attacking/basiczombie%04d.png",
            138, 178),
        50,
        true
    );

    // Basic Zombie - Standing (frames 1-46) - optional idle state
    RegisterAnimation(
        ZOMBIE_BASIC_STANDING,
        GenerateFramePaths(
            RESOURCE_DIR "/zombies/Basic Zombie/Standing/basiczombie%04d.png",
            1, 46),
        50,
        true
    );

    // Basic Zombie - Dying (noheadzombie frames 46-137, non-looping)
    RegisterAnimation(
        ZOMBIE_BASIC_DYING,
        GenerateFramePaths(
            RESOURCE_DIR "/zombies/Basic Zombie/Dead/noheadzombie%04d.png",
            179, 217),
        50,
        false  // plays once then stops
    );

    // ══════════════════════════════════════════════════════════════════════
    // PLANT ANIMATIONS
    // ══════════════════════════════════════════════════════════════════════

    // Sunflower - Idle (frames 5-29)
    RegisterAnimation(
        PLANT_SUNFLOWER_IDLE,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/Sunflower/Sunflower%04d.png",
            5, 29),
        50,
        true
    );

    // Peashooter - Idle (frames 1-25)
    RegisterAnimation(
        PLANT_PEASHOOTER_IDLE,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/Peashooter/AccurateIdle/PeashooterIdle%04d.png",
            1, 25),
        50,
        true
    );

    // WallNut - Healthy (frames 1-16)
    RegisterAnimation(
        PLANT_WALLNUT_HEALTHY,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/WallNut/Wallnut%04d.png",
            1, 22),
        80,
        true
    );

    // WallNut - Damaged (frames 1-11)
    RegisterAnimation(
        PLANT_WALLNUT_DAMAGED,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/WallNut/degrade1/walldegrade1%04d.png",
            1, 22),
        80,
        true
    );

    // WallNut - Critical (frames 1-13)
    RegisterAnimation(
        PLANT_WALLNUT_CRITICAL,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/WallNut/degrade2/walldegrade2%04d.png",
            1, 13),
        80,
        true
    );

    // CherryBomb - Fuse/Explosion (frames 1-13, non-looping)
    RegisterAnimation(
        PLANT_CHERRYBOMB_FUSE,
        GenerateFramePaths(
            RESOURCE_DIR "/plants/CherryBomb/CherryBomb%04d.png",
            1, 14),
        50,
        false  // plays once then stops
    );

    // ══════════════════════════════════════════════════════════════════════
    // OTHER GAME OBJECTS
    // ══════════════════════════════════════════════════════════════════════

    // Sun - Floating animation (frames 1-13)
    RegisterAnimation(
        SUN_FLOATING,
        GenerateFramePaths(
            RESOURCE_DIR "/Sun-20260312T155248Z-3-001/Sun/Sun%04d.png",
            1, 13),
        50,
        true
    );

    m_Initialized = true;
    LOG_DEBUG("ResourceManager: Cached {} animations", m_AnimationCache.size());
}

std::shared_ptr<Util::Animation> ResourceManager::CreateAnimation(
    const std::string& animationName,
    bool playImmediately)
{
    auto it = m_AnimationCache.find(animationName);
    if (it == m_AnimationCache.end()) {
        LOG_ERROR("ResourceManager: Animation '{}' not found!", animationName);
        return nullptr;
    }

    const AnimationData& data = it->second;

    // Create a BRAND NEW Animation instance with the cached paths
    return std::make_shared<Util::Animation>(
        data.framePaths,
        playImmediately,
        data.frameInterval,
        data.looping,
        0  // no cooldown
    );
}

bool ResourceManager::HasAnimation(const std::string& animationName) const {
    return m_AnimationCache.find(animationName) != m_AnimationCache.end();
}

const std::vector<std::string>* ResourceManager::GetFramePaths(
    const std::string& animationName) const
{
    auto it = m_AnimationCache.find(animationName);
    if (it == m_AnimationCache.end()) {
        return nullptr;
    }
    return &it->second.framePaths;
}

void ResourceManager::RegisterAnimation(const std::string& name,
                                         std::vector<std::string> paths,
                                         int frameInterval,
                                         bool looping)
{
    AnimationData data;
    data.framePaths = std::move(paths);
    data.frameInterval = frameInterval;
    data.looping = looping;

    m_AnimationCache[name] = std::move(data);

    LOG_DEBUG("ResourceManager: Registered '{}' with {} frames",
              name, m_AnimationCache[name].framePaths.size());
}

std::vector<std::string> ResourceManager::GenerateFramePaths(
    const std::string& pathFormat,
    int startFrame,
    int endFrame)
{
    std::vector<std::string> paths;
    paths.reserve(endFrame - startFrame + 1);

    for (int i = startFrame; i <= endFrame; ++i) {
        char filename[256];
        std::snprintf(filename, sizeof(filename), pathFormat.c_str(), i);
        paths.push_back(filename);
    }

    return paths;
}
