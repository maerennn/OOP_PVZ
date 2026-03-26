#include "GUI/SeedPacket.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/ColorRect.hpp"
#include "GameConfig.hpp"

#define SEED_PACKET_BG RESOURCE_DIR "/gameGUI/SeedPacket_Larger.png"
#define FONT_PATH RESOURCE_DIR "/../PTSD/assets/fonts/Inter.ttf"

SeedPacket::SeedPacket(PlantType type)
    : m_PlantType(type)
    , m_TotalCooldown(PlantRegistry::Get(type).rechargeTime)
{
}

void SeedPacket::Initialize() {
    CreateComponents();
    UpdateVisuals();
}

void SeedPacket::CreateComponents() {
    const auto& info = PlantRegistry::Get(m_PlantType);

    // Background (seed packet template)
    m_Background = std::make_shared<Util::GameObject>();
    auto bgImage = std::make_shared<Util::Image>(SEED_PACKET_BG);
    m_Background->SetDrawable(bgImage);
    m_Background->m_Transform.scale = {0.7f, 0.7f};  // Scale to fit
    m_Background->SetZIndex(0.0f);

    // Plant icon
    m_PlantIcon = std::make_shared<Util::GameObject>();
    auto iconImage = std::make_shared<Util::Image>(info.iconPath);
    m_PlantIcon->SetDrawable(iconImage);
    m_PlantIcon->m_Transform.translation = {0.0f, 8.0f};  // Slightly up
    m_PlantIcon->m_Transform.scale = {0.15f, 0.15f};  // Scale icon to fit packet
    m_PlantIcon->SetZIndex(0.1f);

    // Sun cost text
    m_SunCostText = std::make_shared<Util::Text>(
        FONT_PATH,
        12,
        std::to_string(info.sunCost),
        Util::Color(0, 0, 0)
    );
    m_SunCostObject = std::make_shared<Util::GameObject>();
    m_SunCostObject->SetDrawable(m_SunCostText);
    m_SunCostObject->m_Transform.translation = {0.0f, -35.0f};  // Bottom of packet
    m_SunCostObject->SetZIndex(0.1f);

    // Cooldown overlay (semi-transparent gray rectangle)
    m_CooldownOverlay = std::make_shared<Util::GameObject>();
    auto overlay = std::make_shared<Util::ColorRect>(
        glm::vec2{WIDTH * 0.7f, HEIGHT * 0.7f}, 
        glm::vec4{0.2f, 0.2f, 0.2f, 0.6f}  // Dark gray, semi-transparent
    );
    m_CooldownOverlay->SetDrawable(overlay);
    m_CooldownOverlay->SetZIndex(0.2f);
    m_CooldownOverlay->SetVisible(false);  // Hidden by default
}

void SeedPacket::Update(float deltaTime, int currentSun) {
    // Update cooldown
    if (m_CooldownRemaining > 0.0f) {
        m_CooldownRemaining -= deltaTime;
        if (m_CooldownRemaining <= 0.0f) {
            m_CooldownRemaining = 0.0f;
        }
    }

    // Determine state
    State newState;
    if (m_CooldownRemaining > 0.0f) {
        newState = State::COOLDOWN;
    } else if (currentSun < GetSunCost()) {
        newState = State::DISABLED;
    } else {
        newState = State::READY;
    }

    if (newState != m_State) {
        m_State = newState;
        UpdateVisuals();
    }
}

void SeedPacket::UpdateVisuals() {
    switch (m_State) {
        case State::READY:
            m_CooldownOverlay->SetVisible(false);
            // Icon at full brightness (no tint needed)
            break;

        case State::COOLDOWN:
            m_CooldownOverlay->SetVisible(true);
            break;

        case State::DISABLED:
            // Show a subtle overlay for unaffordable
            m_CooldownOverlay->SetVisible(true);
            break;
    }
}

bool SeedPacket::ContainsPoint(const glm::vec2& screenPos) const {
    // Get packet's world position
    glm::vec2 packetPos = m_Transform.translation;

    // AABB collision check
    float halfW = (WIDTH * 0.7f) / 2.0f;
    float halfH = (HEIGHT * 0.7f) / 2.0f;

    return (screenPos.x >= packetPos.x - halfW &&
            screenPos.x <= packetPos.x + halfW &&
            screenPos.y >= packetPos.y - halfH &&
            screenPos.y <= packetPos.y + halfH);
}

bool SeedPacket::CanSelect(int currentSun) const {
    return m_State == State::READY && currentSun >= GetSunCost();
}

void SeedPacket::StartCooldown() {
    m_CooldownRemaining = m_TotalCooldown;
    m_State = State::COOLDOWN;
    UpdateVisuals();
}

int SeedPacket::GetSunCost() const {
    return PlantRegistry::Get(m_PlantType).sunCost;
}

float SeedPacket::GetCooldownProgress() const {
    if (m_TotalCooldown <= 0.0f) return 1.0f;
    return 1.0f - (m_CooldownRemaining / m_TotalCooldown);
}

std::vector<std::shared_ptr<Util::GameObject>> SeedPacket::GetChildren() const {
    return {m_Background, m_PlantIcon, m_SunCostObject, m_CooldownOverlay};
}

void SeedPacket::UpdateChildPositions() {
    // Store local offsets for each child
    glm::vec2 bgOffset = {0.0f, 0.0f};
    glm::vec2 iconOffset = {0.0f, 8.0f};
    glm::vec2 textOffset = {0.0f, -35.0f};
    glm::vec2 overlayOffset = {0.0f, 0.0f};

    // Update positions based on packet's world position
    glm::vec2 basePos = m_Transform.translation;
    float baseZ = GetZIndex();

    m_Background->m_Transform.translation = basePos + bgOffset;
    m_Background->SetZIndex(baseZ);

    m_PlantIcon->m_Transform.translation = basePos + iconOffset;
    m_PlantIcon->SetZIndex(baseZ + 0.1f);

    m_SunCostObject->m_Transform.translation = basePos + textOffset;
    m_SunCostObject->SetZIndex(baseZ + 0.1f);

    m_CooldownOverlay->m_Transform.translation = basePos + overlayOffset;
    m_CooldownOverlay->SetZIndex(baseZ + 0.2f);
}
