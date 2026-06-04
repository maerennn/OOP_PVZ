#include "GUI/ConveyorBelt.hpp"
#include "GUI/SeedPacket.hpp"
#include "Util/Image.hpp"
#include "GameConfig.hpp"
#include <algorithm>
#include <cmath>

#define CONVEYOR_BACKDROP RESOURCE_DIR "/gameGUI/ConveyorBelt_backdrop.png"

namespace {
    // Total belt width = 10 × 75 = 750 px; screen half-width = 400 px
    // First card centre = -400 + 25 (left margin) + 37.5 (half-slot) = -337.5
    constexpr float BELT_Y        = 305.0f;   // same row as SeedBank
    constexpr float FIRST_CARD_X  = -285.5f;
    constexpr float CARD_SPACING  = ConveyorBelt::CARD_SLOT_W;
    constexpr int   PREFILL_COUNT = 0;        // cards present at level start
}

void ConveyorBelt::Initialize(const std::vector<PlantType>& pool,
                               float intervalSec) {
    m_Pool         = pool;
    m_FillInterval = intervalSec;
    m_FillTimer    = 0.0f;
    m_Rng          = std::mt19937{std::random_device{}()};
    m_Cards.clear();

    // Background
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>(CONVEYOR_BACKDROP));
    m_Background->m_Transform.translation = {0.0f, BELT_Y};
    m_Background->m_Transform.scale       = {1.25f, 1.25f};
    m_Background->SetZIndex(GameConfig::ZIndex::SEED_BANK);

    // Pre-fill
    int fill = std::min(PREFILL_COUNT, static_cast<int>(m_Pool.size()));
    for (int i = 0; i < fill; ++i) {
        PushRandomCard();
    }
}

void ConveyorBelt::Update(float deltaTime) {
    if (m_Pool.empty()) return;

    m_FillTimer += deltaTime;
    if (m_FillTimer >= m_FillInterval &&
        static_cast<int>(m_Cards.size()) < MAX_CARDS) {
        m_FillTimer = 0.5f;
        PushRandomCard();
    }

    // Smoothly animate cards toward their target positions
    for (size_t i = 0; i < m_Cards.size(); ++i) {
        auto& card = m_Cards[i];
        float currentX = card->m_Transform.translation.x;
        float targetX = m_TargetX[i];
        
        if (std::abs(currentX - targetX) > 0.5f) {
            float step = SLIDE_SPEED * deltaTime;
            if (currentX < targetX) {
                currentX = std::min(currentX + step, targetX);
            } else {
                currentX = std::max(currentX - step, targetX);
            }
            card->m_Transform.translation.x = currentX;
        }
    }

    // Cards don't use sun — keep them perpetually READY by updating with
    // a very large sun value so the DISABLED state never triggers.
    for (auto& card : m_Cards) {
        card->Update(deltaTime, 9999);
    }
}

SeedPacket* ConveyorBelt::GetCardAtPosition(const glm::vec2& screenPos) {
    for (auto& card : m_Cards) {
        if (card->ContainsPoint(screenPos)) {
            return card.get();
        }
    }
    return nullptr;
}

int ConveyorBelt::GetCardIndex(const SeedPacket* ptr) const {
    for (int i = 0; i < static_cast<int>(m_Cards.size()); ++i) {
        if (m_Cards[i].get() == ptr) return i;
    }
    return -1;
}

void ConveyorBelt::RemoveCard(int index) {
    if (index < 0 || index >= static_cast<int>(m_Cards.size())) return;

    // Remove card's children from the renderer
    if (m_OnRemoveObject) {
        for (auto& child : m_Cards[index]->GetChildren()) {
            m_OnRemoveObject(child);
        }
    }

    m_Cards.erase(m_Cards.begin() + index);
    LayoutCards();
}

std::vector<std::shared_ptr<Util::GameObject>> ConveyorBelt::GetAllObjects() const {
    std::vector<std::shared_ptr<Util::GameObject>> objs;
    objs.push_back(m_Background);
    for (const auto& card : m_Cards) {
        auto children = card->GetChildren();
        objs.insert(objs.end(), children.begin(), children.end());
    }
    return objs;
}

// ── Private ───────────────────────────────────────────────────────────────────

void ConveyorBelt::PushRandomCard() {
    if (m_Pool.empty()) return;
    std::uniform_int_distribution<int> dist(0, static_cast<int>(m_Pool.size()) - 1);
    PlantType type = m_Pool[dist(m_Rng)];

    auto card = std::make_shared<SeedPacket>(type);
    card->Initialize();
    card->SetZIndex(GameConfig::ZIndex::SEED_BANK + 0.5f);
    
    // Start card off-screen to the right for smooth slide-in animation
    card->m_Transform.translation = {260.0f, BELT_Y};
    
    m_Cards.push_back(card);
    LayoutCards();

    // Add card's children to the renderer
    if (m_OnAddObject) {
        for (auto& child : card->GetChildren()) {
            m_OnAddObject(child);
        }
    }
}

void ConveyorBelt::LayoutCards() {
    m_TargetX.resize(m_Cards.size());
    for (int i = 0; i < static_cast<int>(m_Cards.size()); ++i) {
        m_TargetX[i] = FIRST_CARD_X + i * CARD_SPACING;
        m_Cards[i]->UpdateChildPositions();
    }
}
