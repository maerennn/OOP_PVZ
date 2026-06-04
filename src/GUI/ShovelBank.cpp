#include "GUI/ShovelBank.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "GameConfig.hpp"

#define SHOVEL_BANK_BG RESOURCE_DIR "/gameGUI/ShovelBank.png"
#define SHOVEL_IMG RESOURCE_DIR "/gameGUI/Shovel.png"

// Layout constants
namespace {
    // Position for levels 1-6 to 1-9 (right side of seed bank)
    constexpr float BANK_X_RIGHT = 400.0f;
    constexpr float BANK_Y_RIGHT = 305.0f;
    
    // Position for level 1-10 (left side of seed bank)
    constexpr float BANK_X_LEFT = -565.0f;
    constexpr float BANK_Y_LEFT = 305.0f;
    
    constexpr float SHOVEL_OFFSET_X = 0.0f;
    constexpr float SHOVEL_OFFSET_Y = 0.0f;
    
    // Hit detection size
    constexpr float HIT_WIDTH = 50.0f;
    constexpr float HIT_HEIGHT = 50.0f;
}

ShovelBank::ShovelBank() {
}

void ShovelBank::Initialize(int levelNum) {
    m_LevelNum = levelNum;
    
    // Determine position based on level
    float bankX, bankY;
    if (levelNum == 10) {
        // Level 1-10: left side
        bankX = BANK_X_LEFT;
        bankY = BANK_Y_LEFT;
    } else {
        // Levels 1-6 to 1-9: right side
        bankX = BANK_X_RIGHT;
        bankY = BANK_Y_RIGHT;
    }
    
    // Create background
    m_BankBackground = std::make_shared<Util::GameObject>();
    auto bgImage = std::make_shared<Util::Image>(SHOVEL_BANK_BG);
    m_BankBackground->SetDrawable(bgImage);
    m_BankBackground->m_Transform.translation = {bankX, bankY};
    m_BankBackground->m_Transform.scale = {1.0f, 1.0f};
    m_BankBackground->SetZIndex(GameConfig::ZIndex::SEED_BANK);
    
    // Create shovel
    m_Shovel = std::make_shared<Util::GameObject>();
    auto shovelImage = std::make_shared<Util::Image>(SHOVEL_IMG);
    m_Shovel->SetDrawable(shovelImage);
    m_Shovel->m_Transform.translation = {bankX + SHOVEL_OFFSET_X, bankY + SHOVEL_OFFSET_Y};
    m_Shovel->m_Transform.scale = {1.0f, 1.0f};
    m_Shovel->SetZIndex(GameConfig::ZIndex::SEED_BANK + 0.5f);
    m_Shovel->SetVisible(true);
}

bool ShovelBank::IsClicked(const glm::vec2& screenPos) const {
    if (!m_Shovel || m_PickedUp) {
        return false;
    }
    
    glm::vec2 shovelPos = m_Shovel->m_Transform.translation;
    float dx = std::abs(screenPos.x - shovelPos.x);
    float dy = std::abs(screenPos.y - shovelPos.y);
    
    return (dx <= HIT_WIDTH && dy <= HIT_HEIGHT);
}

void ShovelBank::SetPickedUp(bool pickedUp) {
    m_PickedUp = pickedUp;
    if (m_Shovel) {
        m_Shovel->SetVisible(!pickedUp);
    }
}

std::vector<std::shared_ptr<Util::GameObject>> ShovelBank::GetAllObjects() const {
    std::vector<std::shared_ptr<Util::GameObject>> objects;
    
    if (m_BankBackground) {
        objects.push_back(m_BankBackground);
    }
    
    if (m_Shovel) {
        objects.push_back(m_Shovel);
    }
    
    return objects;
}
