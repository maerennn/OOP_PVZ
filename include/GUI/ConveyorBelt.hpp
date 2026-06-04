#ifndef CONVEYOR_BELT_HPP
#define CONVEYOR_BELT_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <deque>
#include <memory>
#include <random>
#include <vector>

class SeedPacket;

/**
 * @brief Conveyor Belt UI widget — replaces SeedBank for conveyor-belt levels.
 *
 * Automatically pushes a random plant card to the tail every [interval] seconds
 * when the belt is not full (capped at MAX_CARDS = 10). When a card is used,
 * it is removed and the remaining cards shift left.
 *
 * Uses the existing SeedPacket class as card widgets (sun-cost and cooldown
 * mechanics are bypassed — every card is always "ready").
 */
class ConveyorBelt : public Util::GameObject {
public:
    static constexpr int   MAX_CARDS   = 10;
    static constexpr float CARD_SLOT_W = 75.0f;
    static constexpr float CARD_SLOT_H = 130.0f;

    ConveyorBelt() = default;
    ~ConveyorBelt() = default;

    /**
     * @brief Set up pool, interval, and pre-fill the belt with a few cards.
     * @param pool        Plant types to draw from randomly.
     * @param intervalSec Seconds between automatic card pushes.
     */
    void Initialize(const std::vector<PlantType>& pool, float intervalSec);

    /**
     * @brief Advance fill timer; push new cards when interval elapses and belt
     *        is not full.
     */
    void Update(float deltaTime);

    /**
     * @brief Hit-test: returns the card widget under screenPos, or nullptr.
     */
    SeedPacket* GetCardAtPosition(const glm::vec2& screenPos);

    /**
     * @brief Returns the 0-based index of a card in the belt, or -1.
     */
    int GetCardIndex(const SeedPacket* ptr) const;

    /**
     * @brief Remove the card at index and shift remaining cards left.
     */
    void RemoveCard(int index);

    /**
     * @brief Collect all child GameObjects (backdrop + card children) for
     *        adding to the renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetAllObjects() const;

    /**
     * @brief Set callback invoked when a new card's children should be added
     *        to the renderer.
     */
    using AddObjectCallback = std::function<void(std::shared_ptr<Util::GameObject>)>;
    void SetAddObjectCallback(AddObjectCallback callback) { m_OnAddObject = std::move(callback); }

    /**
     * @brief Set callback invoked when a card's children should be removed
     *        from the renderer.
     */
    using RemoveObjectCallback = std::function<void(std::shared_ptr<Util::GameObject>)>;
    void SetRemoveObjectCallback(RemoveObjectCallback callback) { m_OnRemoveObject = std::move(callback); }

private:
    void PushRandomCard();
    void LayoutCards();

    std::shared_ptr<Util::GameObject>       m_Background;
    std::deque<std::shared_ptr<SeedPacket>> m_Cards;
    std::vector<PlantType>                  m_Pool;
    float    m_FillInterval = 7.5f;
    float    m_FillTimer    = 0.0f;
    std::mt19937 m_Rng;

    // Animation for smooth card sliding
    std::vector<float> m_TargetX;         // Target X position for each card
    static constexpr float SLIDE_SPEED = 500.0f;  // pixels/second

    AddObjectCallback    m_OnAddObject;
    RemoveObjectCallback m_OnRemoveObject;
};

#endif // CONVEYOR_BELT_HPP
