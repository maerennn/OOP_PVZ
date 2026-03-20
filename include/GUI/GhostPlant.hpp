#ifndef GHOST_PLANT_HPP
#define GHOST_PLANT_HPP

#include "Util/GameObject.hpp"
#include "PlantType.hpp"
#include <memory>

/**
 * @brief Semi-transparent preview showing where a plant will be placed.
 * Displayed on the grid during plant placement.
 */
class GhostPlant : public Util::GameObject {
public:
    GhostPlant();
    ~GhostPlant() = default;

    /**
     * @brief Set the plant type to preview.
     * @param type Plant type to show
     */
    void SetPlantType(PlantType type);

    /**
     * @brief Set the grid cell for preview.
     * @param row Grid row
     * @param col Grid column
     */
    void SetGridCell(int row, int col);

    /**
     * @brief Set whether the placement is valid.
     * @param valid true for valid placement (green tint), false for invalid (red tint)
     */
    void SetValid(bool valid);

    /**
     * @brief Clear/hide the ghost plant.
     */
    void Clear();

    bool IsActive() const { return m_Active; }
    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }

private:
    void UpdateAppearance();

    bool m_Active = false;
    bool m_Valid = true;
    PlantType m_CurrentType = PlantType::PEASHOOTER;
    int m_Row = -1;
    int m_Col = -1;
};

#endif // GHOST_PLANT_HPP
