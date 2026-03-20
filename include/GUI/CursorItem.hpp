#ifndef CURSOR_ITEM_HPP
#define CURSOR_ITEM_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <memory>

/**
 * @brief Visual representation of selected plant following the mouse cursor.
 * Shows the plant icon at cursor position during drag-and-drop.
 */
class CursorItem : public Util::GameObject {
public:
    CursorItem();
    ~CursorItem() = default;

    /**
     * @brief Set the plant type to display.
     * @param type Plant type to show at cursor
     */
    void SetPlantType(PlantType type);

    /**
     * @brief Clear the cursor item (hide it).
     */
    void Clear();

    /**
     * @brief Update position to follow cursor.
     * @param cursorPos Current cursor position in screen coordinates
     */
    void UpdatePosition(const glm::vec2& cursorPos);

    bool IsActive() const { return m_Active; }
    PlantType GetPlantType() const { return m_CurrentType; }

private:
    bool m_Active = false;
    PlantType m_CurrentType = PlantType::PEASHOOTER;
};

#endif // CURSOR_ITEM_HPP
