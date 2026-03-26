#include "PlantType.hpp"
#include "Plants.hpp"

// Plant resource directories
#define PEASHOOTER_DIR RESOURCE_DIR "/plants/Peashooter-20260312T155048Z-3-001/Peashooter"
#define SUNFLOWER_DIR RESOURCE_DIR "/plants/Sunflower-20260312T155100Z-3-001/Sunflower"
#define WALLNUT_DIR RESOURCE_DIR "/plants/WallNut-20260312T155116Z-3-001/WallNut"
#define CHERRYBOMB_DIR RESOURCE_DIR "/plants/CherryBomb-20260312T155108Z-3-001/CherryBomb"

// Static array of plant info - order must match PlantType enum
static const PlantInfo s_PlantInfo[] = {
    // PEASHOOTER
    {
        PlantType::PEASHOOTER,
        "Peashooter",
        Peashooter::PEASHOOTER_SUN_COST,      // 100
        Peashooter::PEASHOOTER_RECHARGE,      // 7.5f
        PEASHOOTER_DIR "/Idle.png",
        PEASHOOTER_DIR "/Accurate Idle",
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<Peashooter>();
            plant->Initialize(PEASHOOTER_DIR "/Accurate Idle");
            return plant;
        }
    },
    // SUNFLOWER
    {
        PlantType::SUNFLOWER,
        "Sunflower",
        Sunflower::SUNFLOWER_SUN_COST,        // 50
        Sunflower::SUNFLOWER_RECHARGE,        // 7.5f
        SUNFLOWER_DIR "/Sunflower0005.png",
        SUNFLOWER_DIR,
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<Sunflower>();
            plant->Initialize(SUNFLOWER_DIR);
            return plant;
        }
    },
    // WALLNUT
    {
        PlantType::WALLNUT,
        "Wall-nut",
        WallNut::WALLNUT_SUN_COST,            // 50
        WallNut::WALLNUT_RECHARGE,            // 30.0f
        WALLNUT_DIR "/Wallnut0001.png",
        WALLNUT_DIR,
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<WallNut>();
            plant->Initialize(WALLNUT_DIR);
            return plant;
        }
    },
    // CHERRYBOMB
    {
        PlantType::CHERRYBOMB,
        "Cherry Bomb",
        CherryBomb::CHERRYBOMB_SUN_COST,      // 150
        CherryBomb::CHERRYBOMB_RECHARGE,      // 50.0f
        CHERRYBOMB_DIR "/CherryBomb0001.png",
        CHERRYBOMB_DIR,
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<CherryBomb>();
            plant->Initialize(CHERRYBOMB_DIR);
            return plant;
        }
    }
};

// Ensure array size matches enum count
static_assert(sizeof(s_PlantInfo) / sizeof(s_PlantInfo[0]) == static_cast<int>(PlantType::COUNT),
              "PlantInfo array size must match PlantType::COUNT");

const PlantInfo& PlantRegistry::Get(PlantType type) {
    return s_PlantInfo[static_cast<int>(type)];
}

std::shared_ptr<Plant> PlantRegistry::Create(PlantType type) {
    const auto& info = Get(type);
    if (info.factory) {
        return info.factory();
    }
    return nullptr;
}
