#include "GUI/SeedBank.hpp"
#include "GUI/SeedPacket.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"

#define SEED_BANK_BG RESOURCE_DIR "/gameGUI/SeedBankCoop.png"

// Layout constants
namespace {
    constexpr float BANK_X = -420.0f;           // Position from center
    constexpr float BANK_Y = 305.0f;            // Near top
    constexpr float PACKET_START_X = -499.0f;   // First packet X position
    constexpr float PACKET_Y = 305.0f;          // Packets Y position
    constexpr float PACKET_SPACING = 70.0f;     // Space between packet centers
}

SeedBank::SeedBank() {
}

void SeedBank::Initialize(const std::vector<PlantType>& selectedPlants) {
    // Create background
    m_Background = std::make_shared<Util::GameObject>();
    auto bgImage = std::make_shared<Util::Image>(SEED_BANK_BG);
    m_Background->SetDrawable(bgImage);
    m_Background->m_Transform.translation = {BANK_X, BANK_Y};
    m_Background->m_Transform.scale = {1.25f, 1.25f};
    m_Background->SetZIndex(GameConfig::ZIndex::SEED_BANK);

    // Create packets for each selected plant
    int numPackets = std::min(static_cast<int>(selectedPlants.size()), MAX_SLOTS);
    m_Packets.reserve(numPackets);

    for (int i = 0; i < numPackets; ++i) {
        auto packet = std::make_shared<SeedPacket>(selectedPlants[i]);
        packet->Initialize();
        packet->SetZIndex(GameConfig::ZIndex::SEED_BANK + 0.5f);
        m_Packets.push_back(packet);
    }

    LayoutPackets();
}

void SeedBank::LayoutPackets() {
    for (size_t i = 0; i < m_Packets.size(); ++i) {
        float x = PACKET_START_X + i * PACKET_SPACING;
        m_Packets[i]->m_Transform.translation = {x, PACKET_Y};
        m_Packets[i]->UpdateChildPositions();
    }
}

void SeedBank::Update(float deltaTime, int currentSun) {
    for (auto& packet : m_Packets) {
        packet->Update(deltaTime, currentSun);
    }
}

SeedPacket* SeedBank::GetPacketAtPosition(const glm::vec2& screenPos) {
    for (auto& packet : m_Packets) {
        if (packet->ContainsPoint(screenPos)) {
            return packet.get();
        }
    }
    return nullptr;
}

void SeedBank::StartCooldown(PlantType type) {
    for (auto& packet : m_Packets) {
        if (packet->GetPlantType() == type) {
            packet->StartCooldown();
            break;
        }
    }
}

std::vector<std::shared_ptr<Util::GameObject>> SeedBank::GetAllObjects() const {
    std::vector<std::shared_ptr<Util::GameObject>> objects;

    // Add background
    objects.push_back(m_Background);

    // Add all packet children
    for (const auto& packet : m_Packets) {
        auto children = packet->GetChildren();
        objects.insert(objects.end(), children.begin(), children.end());
    }

    return objects;
}
