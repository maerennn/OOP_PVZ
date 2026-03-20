#include "GUI/CursorItem.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"

CursorItem::CursorItem() {
    SetZIndex(GameConfig::ZIndex::CURSOR_ITEM);
    SetVisible(false);
}

void CursorItem::SetPlantType(PlantType type) {
    m_CurrentType = type;
    m_Active = true;

    const auto& info = PlantRegistry::Get(type);

    // Create icon image
    auto iconImage = std::make_shared<Util::Image>(info.iconPath);
    SetDrawable(iconImage);

    // Scale and set semi-transparent appearance
    m_Transform.scale = {0.4f, 0.4f};

    SetVisible(true);
}

void CursorItem::Clear() {
    m_Active = false;
    SetVisible(false);
    SetDrawable(nullptr);
}

void CursorItem::UpdatePosition(const glm::vec2& cursorPos) {
    if (m_Active) {
        // Offset slightly so icon doesn't obscure cursor
        m_Transform.translation = cursorPos + glm::vec2{20.0f, -20.0f};
    }
}
