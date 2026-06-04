#ifndef SHOVEL_BANK_HPP
#define SHOVEL_BANK_HPP

#include "Util/GameObject.hpp"
#include <memory>

/**
 * @brief Container for the shovel tool, displayed near the seed bank.
 * The shovel allows players to remove plants from the grid.
 * Available only on levels 1-6 through 1-10.
 */
class ShovelBank : public Util::GameObject {
public:
    ShovelBank();
    ~ShovelBank() = default;

    /**
     * @brief Initialize the shovel bank with positioning based on level.
     * @param levelNum Current level number (affects positioning)
     */
    void Initialize(int levelNum);

    /**
     * @brief Check if the shovel area was clicked.
     * @param screenPos Position in screen coordinates
     * @return true if shovel was clicked
     */
    bool IsClicked(const glm::vec2& screenPos) const;

    /**
     * @brief Get all GameObjects for adding to renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetAllObjects() const;

    /**
     * @brief Set whether the shovel has been picked up.
     * @param pickedUp true when shovel is being dragged
     */
    void SetPickedUp(bool pickedUp);

    bool IsPickedUp() const { return m_PickedUp; }

private:
    std::shared_ptr<Util::GameObject> m_BankBackground;  // ShovelBank.png
    std::shared_ptr<Util::GameObject> m_Shovel;          // Shovel.png
    bool m_PickedUp = false;
    int m_LevelNum = 0;
};

#endif // SHOVEL_BANK_HPP
