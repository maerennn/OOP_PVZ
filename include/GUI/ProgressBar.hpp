#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"
#include <memory>
#include <vector>

/**
 * @file ProgressBar.hpp
 * @brief The zombie progress bar UI that shows how far through the level
 *        the player has advanced, built from 5 layered PNG assets.
 *
 * Asset layering (bottom to top):
 *   FlagMeter_BG.png               — trough / frame background
 *   FlagMeter_fill.png             — green fill bar (scaled by progress)
 *   markerLine_FlagMeterParts.png  — notch at each flag-wave boundary
 *   redFlag_FlagMeterParts.png     — placed at right end of fill (final wave)
 *   zombieHead_FlagMeterParts.png  — slides right-to-left as zombies die
 *
 * The bar is positioned at the bottom-right of the screen.
 * Progress goes from right (start) to left (level complete), matching the
 * original PvZ where the zombie head icon moves leftward.
 */
class ProgressBar {
public:
    ProgressBar() = default;
    ~ProgressBar() = default;

    /**
     * @brief Build all GameObjects for the progress bar.
     * @param totalWaves       Total number of waves in the level.
     * @param flagWaveIndices  Indices (0-based) of flag waves for marker placement.
     */
    void Initialize(int totalWaves, const std::vector<int>& flagWaveIndices);

    /**
     * @brief Update fill and zombie-head position.
     * @param progress 0.0 (start) → 1.0 (level complete).
     */
    void SetProgress(float progress);

    /**
     * @brief Show or hide the entire progress bar.
     */
    void SetVisible(bool visible);

    /**
     * @brief Check if the progress bar is currently visible.
     */
    bool IsVisible() const { return m_Visible; }

    /**
     * @brief Collect all GameObjects for App to add to the renderer.
     */
    std::vector<std::shared_ptr<Util::GameObject>> GetAllObjects() const;

private:
    // Layout constants (PTSD screen coordinates)
    static constexpr float BAR_X = 430.0f;      // center X of meter
    static constexpr float BAR_Y = -330.0f;     // center Y of meter
    static constexpr float BAR_SCALE = 1.0f;

    // Fill bar stretching range — measured from the asset dimensions.
    // We'll compute these dynamically in Initialize based on the BG size.
    float m_FillMaxWidth = 0.0f;   // full bar pixel width
    float m_BarLeftX = 0.0f;       // screen X of the left end of the fill area
    float m_BarRightX = 0.0f;      // screen X of the right end of the fill area

    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Fill;
    std::shared_ptr<Util::GameObject> m_ZombieHead;
    std::shared_ptr<Util::GameObject> m_RedFlag;
    std::vector<std::shared_ptr<Util::GameObject>> m_MarkerLines;

    bool m_Visible = false;
};

#endif // PROGRESS_BAR_HPP
