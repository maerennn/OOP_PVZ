#include "Zombie/ConeheadArmor.hpp"
#include "ResourceManager.hpp"

void ConeheadArmor::UpdateDegradationState() {
    if (m_TotalDamageTaken < THRESHOLD_MINOR) {
        m_State = DegradationState::INTACT;
    } else if (m_TotalDamageTaken < THRESHOLD_HEAVY) {
        m_State = DegradationState::MINOR_DAMAGE;
    } else {
        m_State = DegradationState::HEAVY_DAMAGE;
    }
}

std::string ConeheadArmor::GetCurrentSpritePath() const {
    switch (m_State) {
        case DegradationState::INTACT:       return ResourceManager::ARMOR_CONE_1;
        case DegradationState::MINOR_DAMAGE: return ResourceManager::ARMOR_CONE_2;
        case DegradationState::HEAVY_DAMAGE: return ResourceManager::ARMOR_CONE_3;
        case DegradationState::DESTROYED:
        default:                             return "";
    }
}
