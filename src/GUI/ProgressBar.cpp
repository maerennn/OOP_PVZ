#include "GUI/ProgressBar.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"

void ProgressBar::Initialize(int totalWaves,
                              const std::vector<int>& flagWaveIndices)
{
    // ── Background (trough) ─────────────────────────────────────────────
    m_Background = std::make_shared<Util::GameObject>();
    auto bgImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/gameGUI/FlagMeter_BG.png");
    m_Background->SetDrawable(bgImage);
    m_Background->m_Transform.translation = {BAR_X, BAR_Y};
    m_Background->m_Transform.scale = {BAR_SCALE, BAR_SCALE};
    m_Background->SetZIndex(GameConfig::ZIndex::UI_OVERLAY);

    // Determine fill area from the background image dimensions
    glm::vec2 bgSize = bgImage->GetSize() * BAR_SCALE;
    // The fill area has small margins on each side (~4px each side at 1x scale)
    float margin = 4.0f * BAR_SCALE;
    m_FillMaxWidth = bgSize.x - margin * 2.0f;
    m_BarLeftX = BAR_X - bgSize.x * 0.5f + margin;
    m_BarRightX = BAR_X + bgSize.x * 0.5f - margin;

    // ── Fill bar ────────────────────────────────────────────────────────
    m_Fill = std::make_shared<Util::GameObject>();
    auto fillImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/gameGUI/FlagMeter_fill.png");
    m_Fill->SetDrawable(fillImage);
    m_Fill->SetZIndex(GameConfig::ZIndex::UI_OVERLAY + 0.1f);
    // Start with zero fill — will be positioned in SetProgress
    m_Fill->m_Transform.scale = {0.0f, BAR_SCALE};
    m_Fill->m_Transform.translation = {m_BarRightX, BAR_Y};

    // ── Marker lines at flag-wave boundaries ────────────────────────────
    for (int flagIdx : flagWaveIndices) {
        auto marker = std::make_shared<Util::GameObject>();
        auto markerImage = std::make_shared<Util::Image>(
            RESOURCE_DIR "/gameGUI/markerLine_FlagMeterParts.png");
        marker->SetDrawable(markerImage);
        marker->SetZIndex(GameConfig::ZIndex::UI_OVERLAY + 0.2f);

        // Position the marker proportionally along the bar.
        // Flag wave index maps to the fraction of zombies killed up to that wave.
        // For simplicity, use wave index / totalWaves as a linear position.
        float fraction = static_cast<float>(flagIdx + 1) / static_cast<float>(totalWaves);
        float markerX = m_BarRightX - fraction * m_FillMaxWidth;
        marker->m_Transform.translation = {markerX, BAR_Y};
        marker->m_Transform.scale = {BAR_SCALE, BAR_SCALE};

        m_MarkerLines.push_back(marker);
    }

    // ── Red flag (at left end = level complete) ─────────────────────────
    m_RedFlag = std::make_shared<Util::GameObject>();
    auto flagImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/gameGUI/redFlag_FlagMeterParts.png");
    m_RedFlag->SetDrawable(flagImage);
    m_RedFlag->SetZIndex(GameConfig::ZIndex::UI_OVERLAY + 0.3f);
    m_RedFlag->m_Transform.translation = {m_BarLeftX, BAR_Y + 8.0f};
    m_RedFlag->m_Transform.scale = {BAR_SCALE, BAR_SCALE};

    // ── Zombie head indicator ───────────────────────────────────────────
    m_ZombieHead = std::make_shared<Util::GameObject>();
    auto headImage = std::make_shared<Util::Image>(
        RESOURCE_DIR "/gameGUI/zombieHead_FlagMeterParts.png");
    m_ZombieHead->SetDrawable(headImage);
    m_ZombieHead->SetZIndex(GameConfig::ZIndex::UI_OVERLAY + 0.4f);
    m_ZombieHead->m_Transform.scale = {BAR_SCALE, BAR_SCALE};
    // Starts at the right end of the bar
    m_ZombieHead->m_Transform.translation = {m_BarRightX, BAR_Y + 10.0f};

    // Start hidden — shown when the preparation phase ends
    SetVisible(false);

    LOG_DEBUG("ProgressBar initialized: barLeft={}, barRight={}, fillWidth={}",
              m_BarLeftX, m_BarRightX, m_FillMaxWidth);
}

void ProgressBar::SetProgress(float progress) {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    // ── Fill bar: grows from right to left ──────────────────────────────
    // Scale X proportionally. The fill image is centered, so we need to
    // offset its translation so it appears anchored to the right edge.
    float fillWidth = progress * m_FillMaxWidth;

    // Scale the fill image. The original image covers the full bar width,
    // so we scale it down to show partial progress.
    m_Fill->m_Transform.scale = {progress * BAR_SCALE, BAR_SCALE};

    // Anchor to right side: center of the scaled fill is at rightEdge - fillWidth/2
    m_Fill->m_Transform.translation = {m_BarRightX - fillWidth * 0.5f, BAR_Y};

    // ── Zombie head: slides from right to left ──────────────────────────
    float headX = m_BarRightX - progress * m_FillMaxWidth;
    m_ZombieHead->m_Transform.translation = {headX, BAR_Y + 10.0f};
}

std::vector<std::shared_ptr<Util::GameObject>> ProgressBar::GetAllObjects() const {
    std::vector<std::shared_ptr<Util::GameObject>> objects;
    objects.push_back(m_Background);
    objects.push_back(m_Fill);
    for (const auto& marker : m_MarkerLines) {
        objects.push_back(marker);
    }
    objects.push_back(m_RedFlag);
    objects.push_back(m_ZombieHead);
    return objects;
}

void ProgressBar::SetVisible(bool visible) {
    m_Visible = visible;
    m_Background->SetVisible(visible);
    m_Fill->SetVisible(visible);
    m_ZombieHead->SetVisible(visible);
    m_RedFlag->SetVisible(visible);
    for (const auto& marker : m_MarkerLines) {
        marker->SetVisible(visible);
    }
}
