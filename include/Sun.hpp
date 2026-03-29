#ifndef SUN_HPP
#define SUN_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <functional>

/**
 * @brief Collectible sun object that players click to gather resources.
 * 
 * Suns can be produced by:
 * - Sunflowers (appear near the plant)
 * - Sky drops (fall from above)
 * 
 * Animation paths are cached via ResourceManager.
 */
class Sun : public Util::GameObject {
public:
    // Sun constants
    static constexpr int SUN_VALUE = 25;
    static constexpr float SUN_LIFETIME = 10.0f;        // Seconds before disappearing
    static constexpr float CLICK_RADIUS = 40.0f;        // Click detection radius
    static constexpr float HOVER_AMPLITUDE = 5.0f;      // Hover animation amplitude
    static constexpr float HOVER_SPEED = 3.0f;          // Hover animation speed

    // Callback type for when sun is collected
    using CollectedCallback = std::function<void(int value)>;

    /**
     * @brief Construct a new Sun object.
     * @param position Initial position in screen coordinates
     * @param value Sun value (default 25)
     */
    explicit Sun(const glm::vec2& position, int value = SUN_VALUE);
    virtual ~Sun() = default;

    /**
     * @brief Initialize the sun animation via ResourceManager.
     */
    void Initialize();

    /**
     * @brief Update sun state (hover animation, lifetime).
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime);

    /**
     * @brief Check if a position (cursor) is within click range.
     * @param cursorPos Position to check
     * @return true if within click radius
     */
    bool IsClicked(const glm::vec2& cursorPos) const;

    /**
     * @brief Collect this sun (triggers callback and marks for removal).
     */
    void Collect();

    /**
     * @brief Set callback for when sun is collected.
     */
    void SetCollectedCallback(CollectedCallback callback) {
        m_OnCollected = std::move(callback);
    }

    // State queries
    bool IsCollected() const { return m_IsCollected; }
    bool IsExpired() const { return m_Lifetime <= 0.0f; }
    bool ShouldRemove() const { return m_IsCollected || IsExpired(); }
    int GetValue() const { return m_Value; }

private:
    int m_Value;
    float m_Lifetime;
    float m_HoverTime = 0.0f;
    glm::vec2 m_BasePosition;       // Original position (for hover effect)
    bool m_IsCollected = false;

    std::shared_ptr<Util::Animation> m_Animation;
    CollectedCallback m_OnCollected;
};

#endif // SUN_HPP
