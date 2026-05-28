#include "Zombie/Armor.hpp"
#include <algorithm>

int Armor::TakeDamage(int amount) {
    if (m_Health <= 0) {
        // Already destroyed — all incoming damage overflows to zombie base HP
        return amount;
    }

    if (amount >= m_Health) {
        // Armor is completely depleted; calculate overflow
        int overflow = amount - m_Health;
        m_TotalDamageTaken += m_Health;
        m_Health = 0;
        m_State = DegradationState::DESTROYED;
        return overflow;
    }

    // Armor absorbs everything
    m_Health -= amount;
    m_TotalDamageTaken += amount;
    UpdateDegradationState();
    return 0;
}
