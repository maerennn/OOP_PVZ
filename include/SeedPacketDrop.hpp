#ifndef SEED_PACKET_DROP_HPP
#define SEED_PACKET_DROP_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <glm/glm.hpp>
#include <memory>

/**
 * @brief World-space seed packet that the last zombie drops on death.
 *
 * Spawns at the zombie's death position, bobs up and down to attract the
 * player's attention, and is collected when clicked. Once collected the
 * App unlocks the plant and transitions to the next level.
 *
 * Lifecycle:
 *   1. App::SpawnRewardDrop() creates and adds this to the renderer.
 *   2. App::Update() calls Update() and then IsClicked() each frame.
 *   3. On a positive click, App removes it from the renderer and resets.
 */
class SeedPacketDrop : public Util::GameObject {
public:
    static constexpr float BOB_AMPLITUDE = 8.0f;   ///< pixels above/below base Y
    static constexpr float BOB_SPEED     = 4.0f;   ///< radians/second
    static constexpr float HIT_HALF_W    = 28.0f;  ///< click hitbox half-width
    static constexpr float HIT_HALF_H    = 36.0f;  ///< click hitbox half-height

    /**
     * @param type      Plant type whose icon is displayed.
     * @param spawnPos  World-space position of the zombie's death (drop lands here).
     */
    SeedPacketDrop(PlantType type, glm::vec2 spawnPos);
    ~SeedPacketDrop() = default;

    /** Load the icon image from PlantRegistry and set the initial transform. */
    void Initialize();

    /**
     * @brief Advance the bob animation.
     * @param deltaTime Seconds since last frame.
     */
    void Update(float deltaTime);

    /**
     * @brief AABB hit-test for mouse clicks.
     * @param cursorPos Current cursor position in PTSD screen coordinates.
     * @return true if the cursor is inside the packet's click area.
     */
    bool IsClicked(const glm::vec2& cursorPos) const;

    PlantType GetPlantType() const { return m_Type; }

private:
    PlantType m_Type;
    float     m_BaseY   = 0.0f;
    float     m_BobTime = 0.0f;
};

#endif // SEED_PACKET_DROP_HPP
