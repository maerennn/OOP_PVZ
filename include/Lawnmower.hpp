#ifndef LAWNMOWER_HPP
#define LAWNMOWER_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include <memory>

/**
 * @brief Lawnmower - Last line of defense for each lane.
 *
 * Each lane has one lawnmower at the left edge. When a zombie reaches it:
 * - Lawnmower triggers and drives to the right
 * - Instantly kills any zombie it touches
 * - Disappears off the right side of the screen
 * - Does NOT respawn (one-time use per lane)
 */
class Lawnmower : public Util::GameObject {
public:
    enum class State {
        IDLE,       // Waiting at left edge
        TRIGGERED   // Moving right, killing zombies
    };

    // Lawnmower constants
    static constexpr float SPEED = 300.0f;           // Movement speed when triggered
    static constexpr float HITBOX_WIDTH = 60.0f;
    static constexpr float HITBOX_HEIGHT = 60.0f;
    static constexpr float TRIGGER_X_OFFSET = 30.0f; // How far into the mower zombies must reach

    /**
     * @brief Construct a lawnmower for a specific row.
     * @param row Lane index (0-4)
     * @param position Starting position
     */
    Lawnmower(int row, const glm::vec2& position);
    virtual ~Lawnmower() = default;

    /**
     * @brief Initialize animation via ResourceManager.
     */
    void Initialize();

    /**
     * @brief Update lawnmower state (move if triggered).
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime);

    /**
     * @brief Trigger the lawnmower to start moving.
     */
    void Trigger();

    // Accessors
    int GetRow() const { return m_Row; }
    State GetState() const { return m_State; }
    bool IsIdle() const { return m_State == State::IDLE; }
    bool IsTriggered() const { return m_State == State::TRIGGERED; }

    // Check if off-screen (for removal)
    bool IsOffScreen() const;
    bool ShouldRemove() const { return IsOffScreen(); }

    // Collision helpers
    float GetLeftEdge() const { return m_Transform.translation.x - HITBOX_WIDTH / 2; }
    float GetRightEdge() const { return m_Transform.translation.x + HITBOX_WIDTH / 2; }

private:
    int m_Row;
    State m_State = State::IDLE;
    float m_Speed = SPEED;

    std::shared_ptr<Util::Animation> m_Animation;
};

#endif // LAWNMOWER_HPP
