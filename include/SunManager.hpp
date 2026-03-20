#ifndef SUN_MANAGER_HPP
#define SUN_MANAGER_HPP

#include "Util/GameObject.hpp"
#include <memory>
#include <vector>

namespace Util {
    class Text;
}

/**
 * @brief Manages the player's sun resource.
 * Handles sun tracking, spending, and UI display.
 */
class SunManager {
public:
    static constexpr int STARTING_SUN = 50;

    SunManager();
    ~SunManager() = default;

    /**
     * @brief Initialize UI components.
     * Must be called before using the manager.
     */
    void Initialize();

    /**
     * @brief Update sun display (called each frame).
     */
    void Update();

    // Sun operations
    int GetSun() const { return m_CurrentSun; }
    bool CanAfford(int cost) const { return m_CurrentSun >= cost; }

    /**
     * @brief Attempt to spend sun.
     * @param amount Amount to spend
     * @return true if successful, false if insufficient sun
     */
    bool SpendSun(int amount);

    /**
     * @brief Add sun to the player's total.
     * @param amount Amount to add
     */
    void AddSun(int amount);

    /**
     * @brief Get all UI GameObjects for adding to renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetUIElements() const;

private:
    void UpdateDisplay();

    int m_CurrentSun = STARTING_SUN;

    // UI Components
    std::shared_ptr<Util::GameObject> m_Root;
    std::shared_ptr<Util::GameObject> m_SunIcon;
    std::shared_ptr<Util::GameObject> m_SunTextObject;
    std::shared_ptr<Util::Text> m_SunText;
};

#endif // SUN_MANAGER_HPP
