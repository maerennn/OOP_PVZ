#include "PlantType.hpp"
#include "Plants.hpp"

// Plant resource directories
#define PEASHOOTER_DIR  RESOURCE_DIR "/plants/Peashooter"
#define SUNFLOWER_DIR   RESOURCE_DIR "/plants/Sunflower"
#define WALLNUT_DIR     RESOURCE_DIR "/plants/WallNut"
#define CHERRYBOMB_DIR  RESOURCE_DIR "/plants/CherryBomb"
#define REPEATER_DIR    RESOURCE_DIR "/plants/Repeater"
#define SNOWPEA_DIR     RESOURCE_DIR "/plants/SnowPea"
#define POTATOMINE_DIR  RESOURCE_DIR "/plants/PotatoMine"
#define CHOMPER_DIR     RESOURCE_DIR "/plants/Chomper"

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
    },
    // REPEATER
    {
        PlantType::REPEATER,
        "Repeater",
        Repeater::REPEATER_SUN_COST,          // 200
        Repeater::REPEATER_RECHARGE,          // 7.5f
        REPEATER_DIR "/Idle.png",
        REPEATER_DIR "/AccurateIdle",
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<Repeater>();
            plant->Initialize(REPEATER_DIR "/AccurateIdle");
            return plant;
        }
    },
    // SNOWPEA
    {
        PlantType::SNOWPEA,
        "Snow Pea",
        SnowPea::SNOWPEA_SUN_COST,            // 175
        SnowPea::SNOWPEA_RECHARGE,            // 7.5f
        SNOWPEA_DIR "/Idle.png",
        SNOWPEA_DIR "/SnowaccurateIdle",
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<SnowPea>();
            plant->Initialize(SNOWPEA_DIR "/SnowaccurateIdle");
            return plant;
        }
    },
    // POTATOMINE
    {
        PlantType::POTATOMINE,
        "Potato Mine",
        PotatoMine::POTATOMINE_SUN_COST,      // 25
        PotatoMine::POTATOMINE_RECHARGE,      // 30.0f
        POTATOMINE_DIR "/Potatomine0001.png",
        POTATOMINE_DIR,
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<PotatoMine>();
            plant->Initialize(POTATOMINE_DIR);
            return plant;
        }
    },
    // CHOMPER
    {
        PlantType::CHOMPER,
        "Chomper",
        Chomper::CHOMPER_SUN_COST,            // 150
        Chomper::CHOMPER_RECHARGE,            // 30.0f
        CHOMPER_DIR "/idle/Chomper.png0001.png",
        CHOMPER_DIR,
        []() -> std::shared_ptr<Plant> {
            auto plant = std::make_shared<Chomper>();
            plant->Initialize(CHOMPER_DIR);
            return plant;
        },
        {30.0f, 30.0f},   // spriteOffset: nudge in-game sprite (x, y) — tune as needed
        {35.0f, 25.0f},   // iconOffset:   nudge seed packet icon (x, y) — tune as needed
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
