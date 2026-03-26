set(SRC_FILES
    App.cpp
    Util/ColorRect.cpp

    # Plant System
    Plant/Plant.cpp
    Plant/ShooterPlant.cpp
    Plant/DefensePlant.cpp
    Plant/InstantKillPlant.cpp

    # Concrete Plants
    Plant/Peashooter.cpp
    Plant/Sunflower.cpp
    Plant/WallNut.cpp
    Plant/CherryBomb.cpp

    # Game Systems
    PlantType.cpp
    SunManager.cpp
    PlantingSystem.cpp
    Sun.cpp

    # GUI System
    GUI/SeedPacket.cpp
    GUI/SeedBank.cpp
    GUI/CursorItem.cpp
    GUI/GhostPlant.cpp
)

set(INCLUDE_FILES
    App.hpp
    GameConfig.hpp
    Util/ColorRect.hpp

    # Plant System - Interfaces
    Plant/IAttacker.hpp
    Plant/IProducer.hpp

    # Plant System - Base Classes
    Plant/Plant.hpp
    Plant/ShooterPlant.hpp
    Plant/DefensePlant.hpp
    Plant/InstantKillPlant.hpp

    # Concrete Plants
    Plant/Peashooter.hpp
    Plant/Sunflower.hpp
    Plant/WallNut.hpp
    Plant/CherryBomb.hpp

    # Game Systems
    PlantType.hpp
    SunManager.hpp
    PlantingSystem.hpp
    Sun.hpp

    # GUI System
    GUI/SeedPacket.hpp
    GUI/SeedBank.hpp
    GUI/CursorItem.hpp
    GUI/GhostPlant.hpp
)

set(TEST_FILES
)
