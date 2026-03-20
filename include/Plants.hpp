#ifndef PLANTS_HPP
#define PLANTS_HPP

/**
 * @file Plants.hpp
 * @brief Convenience header that includes all Plant types.
 *
 * Include this header to access all plant classes in one import.
 */

// Interfaces
#include "Plant/IAttacker.hpp"
#include "Plant/IProducer.hpp"

// Base Classes
#include "Plant/Plant.hpp"
#include "Plant/ShooterPlant.hpp"
#include "Plant/DefensePlant.hpp"
#include "Plant/InstantKillPlant.hpp"

// Concrete Plants
#include "Plant/Peashooter.hpp"
#include "Plant/Sunflower.hpp"
#include "Plant/WallNut.hpp"
#include "Plant/CherryBomb.hpp"

#endif // PLANTS_HPP
