#include "SeedPacketDrop.hpp"
#include "PlantType.hpp"
#include "GameConfig.hpp"
#include "Util/Image.hpp"
#include <cmath>

SeedPacketDrop::SeedPacketDrop(PlantType type, glm::vec2 spawnPos)
    : m_Type(type)
    , m_BaseY(spawnPos.y)
{
    m_Transform.translation = spawnPos;
}

void SeedPacketDrop::Initialize() {
    const PlantInfo& info = PlantRegistry::Get(m_Type);
    SetDrawable(std::make_shared<Util::Image>(info.iconPath));

    // Draw above plants/zombies but below the cursor
    SetZIndex(GameConfig::ZIndex::CURSOR_ITEM - 0.5f);

    // Match the seed bank packet scale (SeedPacket uses ~50×70 px)
    m_Transform.scale = {0.2f, 0.2f};
}

void SeedPacketDrop::Update(float deltaTime) {
    m_BobTime += deltaTime * BOB_SPEED;
    m_Transform.translation.y = m_BaseY + std::sin(m_BobTime) * BOB_AMPLITUDE;
}

bool SeedPacketDrop::IsClicked(const glm::vec2& cursorPos) const {
    const glm::vec2& pos = m_Transform.translation;
    return (std::abs(cursorPos.x - pos.x) <= HIT_HALF_W &&
            std::abs(cursorPos.y - pos.y) <= HIT_HALF_H);
}
