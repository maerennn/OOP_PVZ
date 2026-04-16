#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Util/Animation.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @file ResourceManager.hpp
 * @brief Singleton class that caches GPU-ready Image objects to eliminate spawn-time stutter.
 *
 * === Design Philosophy ===
 * Since Util::Animation stores internal state (m_Index, m_State), we CANNOT share
 * Animation instances across game objects. Instead, we:
 * 1. Build all Util::Image objects (SDL surface load + glTexImage2D) ONCE in Initialize().
 * 2. Cache the resulting shared_ptr<Image> vectors.
 * 3. Provide a factory that creates NEW Animation instances by shallow-copying those
 *    shared_ptrs — no disk I/O or GPU uploads happen at spawn time.
 *
 * This eliminates the frame-stutter that occurred when a zombie spawned because
 * every Animation constructor previously re-uploaded all textures to the GPU.
 */

class ResourceManager {
public:
    /**
     * @brief Animation metadata with pre-built GPU-ready Image objects.
     */
    struct AnimationData {
        std::vector<std::shared_ptr<Util::Image>> frames;  // textures already in VRAM
        int frameInterval = 50;   // ms per frame
        bool looping = true;
    };

    /**
     * @brief Get the singleton instance.
     */
    static ResourceManager& GetInstance();

    // Delete copy/move for singleton
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    /**
     * @brief Load all game images into VRAM and cache them.
     * Call this once during App::Start() before any game objects are created.
     * This is intentionally slow — all GPU uploads happen here so spawn events are free.
     */
    void Initialize();

    /**
     * @brief Create a NEW Animation instance from cached Image objects.
     * No disk I/O or GPU uploads occur — only shared_ptr copies.
     * @param animationName The registered animation name (e.g., "zombie.basic.walking")
     * @param playImmediately Whether to start playing immediately
     * @return A new Animation instance, or nullptr if animation not found
     */
    std::shared_ptr<Util::Animation> CreateAnimation(
        const std::string& animationName,
        bool playImmediately = true);

    /**
     * @brief Check if an animation is registered.
     */
    bool HasAnimation(const std::string& animationName) const;

    // ══════════════════════════════════════════════════════════════════════
    // Animation name constants (use these instead of magic strings)
    // ══════════════════════════════════════════════════════════════════════

    // Zombie animations
    static constexpr const char* ZOMBIE_BASIC_WALKING = "zombie.basic.walking";
    static constexpr const char* ZOMBIE_BASIC_ATTACKING = "zombie.basic.attacking";
    static constexpr const char* ZOMBIE_BASIC_STANDING = "zombie.basic.standing";
    static constexpr const char* ZOMBIE_BASIC_DYING = "zombie.basic.dying";
    
    static constexpr const char* ZOMBIE_CONEHEAD_WALKING = "zombie.conehead.walking";
    static constexpr const char* ZOMBIE_CONEHEAD_ATTACKING = "zombie.conehead.attacking";
    static constexpr const char* ZOMBIE_CONEHEAD_DYING = "zombie.conehead.dying";
    
    static constexpr const char* ZOMBIE_BUCKETHEAD_WALKING = "zombie.buckethead.walking";
    static constexpr const char* ZOMBIE_BUCKETHEAD_ATTACKING = "zombie.buckethead.attacking";
    static constexpr const char* ZOMBIE_BUCKETHEAD_DYING = "zombie.buckethead.dying";
    
    static constexpr const char* ZOMBIE_POLEVAULT_WALKING = "zombie.polevault.walking";
    static constexpr const char* ZOMBIE_POLEVAULT_ATTACKING = "zombie.polevault.attacking";
    static constexpr const char* ZOMBIE_POLEVAULT_DYING = "zombie.polevault.dying";
    static constexpr const char* ZOMBIE_POLEVAULT_JUMP = "zombie.polevault.jump";

    // Shared zombie animations
    static constexpr const char* ZOMBIE_CHARRED = "zombie.charred";  // Burnt by Cherry Bomb

    // Plant animations
    static constexpr const char* PLANT_SUNFLOWER_IDLE = "plant.sunflower.idle";
    static constexpr const char* PLANT_PEASHOOTER_IDLE = "plant.peashooter.idle";
    static constexpr const char* PLANT_WALLNUT_HEALTHY = "plant.wallnut.healthy";
    static constexpr const char* PLANT_WALLNUT_DAMAGED = "plant.wallnut.damaged";
    static constexpr const char* PLANT_WALLNUT_CRITICAL = "plant.wallnut.critical";
    static constexpr const char* PLANT_CHERRYBOMB_FUSE = "plant.cherrybomb.fuse";

    // Other game objects
    static constexpr const char* SUN_FLOATING = "sun.floating";
    static constexpr const char* LAWNMOWER_IDLE = "lawnmower.idle";

private:
    ResourceManager() = default;

    /**
     * @brief Build Image objects from paths and register them under name.
     */
    void RegisterAnimation(const std::string& name,
                           const std::vector<std::string>& paths,
                           int frameInterval = 50,
                           bool looping = true);

    /**
     * @brief Helper to generate numbered frame paths.
     */
    static std::vector<std::string> GenerateFramePaths(
        const std::string& pathFormat,
        int startFrame,
        int endFrame);

    // Cache storage
    std::unordered_map<std::string, AnimationData> m_AnimationCache;
    bool m_Initialized = false;
};

#endif // RESOURCE_MANAGER_HPP
