#include "GUI/GhostPlant.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"

GhostPlant::GhostPlant() {
    // Render just below normal plants
    SetZIndex(GameConfig::ZIndex::PLANT - 0.1f);
    SetVisible(false);
}

void GhostPlant::SetPlantType(PlantType type) {
    m_CurrentType = type;
    m_Active = true;

    const auto& info = PlantRegistry::Get(type);

    // Create icon image
    auto iconImage = std::make_shared<Util::Image>(info.iconPath);
    SetDrawable(iconImage);

    // Scale to match placed plants
    m_Transform.scale = {0.25f, 0.25f};

    UpdateAppearance();
    SetVisible(true);
}

void GhostPlant::SetGridCell(int row, int col) {
    if (row < 0 || row >= GameConfig::GRID_ROWS ||
        col < 0 || col >= GameConfig::GRID_COLS) {
        return;
    }

    m_Row = row;
    m_Col = col;

    // Update position to cell center
    m_Transform.translation = GameConfig::CellToPosition(row, col);
}

void GhostPlant::SetValid(bool valid) {
    if (m_Valid != valid) {
        m_Valid = valid;
        UpdateAppearance();
    }
}

void GhostPlant::Clear() {
    m_Active = false;
    m_Row = -1;
    m_Col = -1;
    SetVisible(false);
    SetDrawable(nullptr);
}

void GhostPlant::UpdateAppearance() {
    // Note: PTSD framework doesn't have built-in tinting support
    // For now, we just show/hide. A proper implementation would
    // use a shader or tint the image.
    // Visual feedback could be enhanced with colored overlay if needed.
}
