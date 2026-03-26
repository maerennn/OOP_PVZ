#include "SunManager.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "GameConfig.hpp"

// Font path (using PTSD's bundled font)
#define SUN_FONT_PATH RESOURCE_DIR "/../PTSD/assets/fonts/Inter.ttf"
#define SUN_ICON_PATH RESOURCE_DIR "/Sun-20260312T155248Z-3-001/Sun/Sun0001.png"

SunManager::SunManager()
    : m_CurrentSun(STARTING_SUN)
{
}

void SunManager::Initialize() {
    // Create root container
    m_Root = std::make_shared<Util::GameObject>();
    m_Root->SetZIndex(GameConfig::ZIndex::SEED_BANK);

    // Create sun icon
    // m_SunIcon = std::make_shared<Util::GameObject>();
    // auto sunImage = std::make_shared<Util::Image>(SUN_ICON_PATH);
    // m_SunIcon->SetDrawable(sunImage);
    // m_SunIcon->m_Transform.translation = {-575.0f, 300.0f};
    // m_SunIcon->m_Transform.scale = {0.5f, 0.5f};
    // m_SunIcon->SetZIndex(GameConfig::ZIndex::SEED_BANK + 0.1f);

    // Create sun count text (wrapped in GameObject)
    m_SunText = std::make_shared<Util::Text>(
        SUN_FONT_PATH,
        24,
        std::to_string(m_CurrentSun),
        Util::Color(0, 0, 0)  // Black text
    );

    m_SunTextObject = std::make_shared<Util::GameObject>();
    m_SunTextObject->SetDrawable(m_SunText);
    m_SunTextObject->m_Transform.translation = {-576.0f, 267.0f};
    m_SunTextObject->SetZIndex(GameConfig::ZIndex::SEED_BANK + 0.1f);

    UpdateDisplay();
}

void SunManager::Update() {
    // Currently just updates display
    // Future: Could animate sun collection, etc.
}

bool SunManager::SpendSun(int amount) {
    if (!CanAfford(amount)) {
        return false;
    }
    m_CurrentSun -= amount;
    UpdateDisplay();
    return true;
}

void SunManager::AddSun(int amount) {
    m_CurrentSun += amount;
    UpdateDisplay();
}

void SunManager::UpdateDisplay() {
    if (m_SunText) {
        m_SunText->SetText(std::to_string(m_CurrentSun));
    }
}

std::vector<std::shared_ptr<Util::GameObject>> SunManager::GetUIElements() const {
    // return {m_SunIcon, m_SunTextObject};
    return {m_SunTextObject};
}
