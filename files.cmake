set(SRC_FILES
    App.cpp
    main.cpp
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
    Plant/Repeater.cpp
    Plant/SnowPea.cpp
    Plant/PotatoMine.cpp

    # Game Systems
    PlantType.cpp
    SunManager.cpp
    PlantingSystem.cpp
    Sun.cpp
    Projectile.cpp
    Pea.cpp
    FrozenPea.cpp
    ResourceManager.cpp
    WaveManager.cpp
    LevelManager.cpp

    # Zombie System
    Zombie/Zombie.cpp
    Zombie/NormalZombie.cpp
    Zombie/ConeheadZombie.cpp
    Zombie/BucketheadZombie.cpp
    Zombie/PoleVaultZombie.cpp

    # Lawnmower System
    Lawnmower.cpp

    # GUI System
    GUI/SeedPacket.cpp
    GUI/SeedBank.cpp
    GUI/CursorItem.cpp
    GUI/GhostPlant.cpp
    GUI/ProgressBar.cpp
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
    Plant/Repeater.hpp
    Plant/SnowPea.hpp
    Plant/PotatoMine.hpp

    # Game Systems
    PlantType.hpp
    SunManager.hpp
    PlantingSystem.hpp
    Sun.hpp
    Projectile.hpp
    Pea.hpp
    FrozenPea.hpp
    ResourceManager.hpp
    ZombieType.hpp
    WaveManager.hpp

    # Zombie System
    Zombie/Zombie.hpp
    Zombie/NormalZombie.hpp
    Zombie/ConeheadZombie.hpp
    Zombie/BucketheadZombie.hpp
    Zombie/PoleVaultZombie.hpp

    # Lawnmower System
    Lawnmower.hpp

    # GUI System
    GUI/SeedPacket.hpp
    GUI/SeedBank.hpp
    GUI/CursorItem.hpp
    GUI/GhostPlant.hpp
    GUI/ProgressBar.hpp
)

set(TEST_FILES
)
