# 2026 OOPL Final Report

## Group Information

**Group Number:** 
**Members:** 
**Replicated Game:** 

---

## Project Overview

### Game Introduction
*(Briefly describe the game here)*

### Group Division of Labor
*(Outline who did what)*

---

## Game Features & Mechanics

### Game Rules
### Game Screenshots / UI

---

## Software Design & Architecture

### System Architecture

The game employs a **Object-Oriented Programming (OOP) architecture** with clear separation of concerns across multiple subsystems, leveraging core OOP principles:

#### Object-Oriented Principles

1. **Encapsulation**
   - All game entities (Plants, Zombies, Projectiles) encapsulate their state and behavior within classes
   - Private member variables (health, position, timers) are accessed through public interfaces
   - Example: `SunManager` encapsulates sun economy with controlled `AddSun()` and `SpendSun()` methods

2. **Inheritance**
   - **Plant Hierarchy**: `Plant` base class → `ShooterPlant`, `DefensePlant`, `InstantKillPlant` specialized classes
   - **Zombie Hierarchy**: `Zombie` base class → `NormalZombie`, `PoleVaultZombie` with specialized behaviors
   - **Projectile Hierarchy**: `Projectile` base class → `Pea`, `FrozenPea` with unique collision effects

3. **Polymorphism**
   - Virtual methods enable dynamic dispatch: `Plant::Update()`, `Zombie::OnPlantEncountered()`, `Projectile::OnZombieHit()`
   - Interface-based polymorphism: `IAttacker` and `IProducer` allow plants to have multiple capabilities
   - Example: `PoleVaultZombie` overrides `OnPlantEncountered()` to jump instead of attack

4. **Abstraction**
   - Abstract interfaces (`IAttacker`, `IProducer`) define contracts without implementation details
   - Base classes provide common functionality while derived classes implement specifics
   - Framework abstractions (`Util::GameObject`, `Util::Animation`) hide low-level rendering complexity

#### Core Design Patterns

1. **Factory Pattern**
   - `PlantRegistry`: Centralizes plant metadata and creation logic, storing sun costs, recharge times, animation paths, and factory methods for instantiating plant objects
   - `LevelManager`: Acts as a stateless factory for `LevelConfig` objects, defining nine hard-coded levels with wave configurations, seed banks, and reward plants

2. **Composition over Inheritance**
   - Plants use **interface segregation** with `IAttacker` and `IProducer` interfaces to grant abilities independently
   - Examples: `Sunflower` implements `IProducer` for sun generation; `Chomper` implements `IAttacker` for eating zombies; `WallNut` is a pure defense plant with neither interface

3. **State Machine Pattern**
   - `App::State`: Manages game flow through six states (BOOT → MAIN_MENU → LEVEL_SELECT → START → UPDATE → END)
   - `Zombie::State`: Handles AI behaviors (WALKING → ATTACKING → DYING → DEAD)
   - `PlantingSystem`: Controls planting workflow (IDLE → SEED_SELECTED)

4. **Singleton Pattern**
   - `ResourceManager`: Ensures single instance for centralized asset loading and caching

5. **Observer Pattern (via Callbacks)**
   - Plants notify the App through callback functions (`OnSunProduced`, `OnPlantExplode`)
   - Zombies use `OnStateChanged()` and `OnArmorDestroyed()` callbacks for event-driven behavior

#### System Components

**Game Flow Layer**
- **App**: Main orchestrator managing the game loop, coordinate transformations, collision detection, and subsystem updates
- **LevelManager**: Provides level data including active lanes, zombie waves, seed bank contents, and starting sun
- **WaveManager**: Controls zombie spawn timing with 3-second inter-wave pauses and tracks wave completion

**Entity Systems**
- **Plant System**: Three-tier hierarchy
  - `ShooterPlant` (Peashooter, Repeater, SnowPea) with damage and attack intervals
  - `DefensePlant` (WallNut) with 4000 HP and three damage stages
  - `InstantKillPlant` (CherryBomb, PotatoMine) with area-of-effect damage
- **Zombie System**: Base class with armor composition
  - `NormalZombie`, `PoleVaultZombie` with special vault mechanics
  - Separate `Armor` components (ConeheadArmor, BucketheadArmor) for damage absorption
- **Projectile System**: Abstract `Projectile` base with concrete implementations (Pea, FrozenPea) handling row-based movement and collision

**GUI & Economy**
- **SeedBank/SeedPacket**: Manages plant selection with cooldown timers and sun cost checks
- **PlantingSystem**: Handles mouse input with ghost plant previews and grid occupancy validation
- **SunManager**: Tracks economy with sun collection and spending
- **ProgressBar**: Displays wave progression with flag-wave markers

**Resource Management**
- **ResourceManager**: Pre-loads all animations and images during boot phase using GPU texture caching
- **Lazy Animation Creation**: Returns new `Animation` instances from cached texture data on demand

#### Grid & Spatial Organization
- **5×9 Grid**: 92px × 115px cells with origin at (-269, 241)
- **Z-Index Layering**: Background (0) → Plants (5) → Projectiles (6) → Zombies (7) → UI (11)
- **Occupancy Tracking**: 2D array `m_PlantGrid[row][col]` prevents overlapping plants

### Technical Implementation

#### Programming Language & Standards
- **C++17**: Modern language features including `std::optional`, structured bindings, and `std::shared_ptr`/`std::weak_ptr` for memory management
- **CMake 3.16+**: Cross-platform build system with FetchContent for dependency management

#### Core Libraries & Frameworks

**PTSD Framework** (Practical Tools for Simple Design)
- Custom game engine built on top of industry-standard libraries
- Provides core abstractions:
  - `Util::GameObject`: Transform hierarchy with parent-child relationships
  - `Util::Renderer`: Scene graph root for drawable management
  - `Util::Animation`: Frame-sequence player with looping states
  - `Util::Image`: GPU texture wrapper for static sprites
  - `Util::Text`: TTF font rendering
  - `Util::Input`: Keyboard and mouse input polling with edge detection

**Graphics & Media**
- **SDL2**: Cross-platform multimedia framework for window management, input handling, and OpenGL context creation
- **SDL2_image**: Texture loading from PNG/JPG assets
- **SDL2_ttf**: TrueType font rendering
- **SDL2_mixer**: Audio playback (background music and sound effects)
- **OpenGL**: Hardware-accelerated 2D rendering via `glTexImage2D` for texture uploads
- **GLEW**: OpenGL extension loading
- **GLM**: Mathematics library for vector/matrix operations

**Utilities**
- **spdlog**: Fast logging system with multiple severity levels (LOG_DEBUG, LOG_ERROR)
- **ImGui**: Immediate-mode GUI for debug overlays and development tools

#### Key Programming Techniques

1. **Resource Pre-caching**: All textures loaded during `App::Boot()` to eliminate runtime GPU stalls
2. **Weak Pointer Management**: Zombies store target plants as `std::weak_ptr` to prevent circular references
3. **Grid-Based Collision**: O(1) lookup for plant-zombie interactions using 2D array indexing
4. **Callback-Driven Events**: Decouples game objects from the main App class
5. **Data-Driven Design**: Level configurations stored as structured data (`LevelConfig`, `WaveData`, `ZombieSpawnEntry`)
6. **State Enum + Callbacks**: Every stateful object follows `OnStateChanged()` pattern for animations and behavior changes
7. **Frame-Rate Independence**: All movement and timers use `deltaTime` for consistent behavior across different hardware

#### Project Structure
```
include/ + src/        # Mirrored header/implementation files
├── Plant/             # Plant hierarchy and interfaces
├── Zombie/            # Zombie types and armor system
├── GUI/               # UI components (SeedBank, ProgressBar, etc.)
├── Util/              # Helper classes (ColorRect, etc.)
└── [Core systems]     # App, managers, and game logic
Resources/             # Asset files (PNG sequences, fonts, audio)
PTSD/                  # Framework library (engine abstractions)
CMakeLists.txt         # Build configuration
```

### AI / AI Agent Integration
*(Only fill out this section if you integrated AI/AI Agents into your project; otherwise, feel free to delete it)*

---

## Conclusion & Reflection

### Challenges & Solutions
*(Describe the bugs or obstacles you faced and how you fixed them)*

### Self-Assessment

| No. | Criteria | Completed |
|:---:|:---|:---:|
| 1   | This is an example | V |
| 2   | Changed the repository privacy/permissions to **Public** |  |
| 3   | Features a functional **Debug Mode** |  |
| 4   | Resolved all **Memory Leak** issues in the project |  |
| 5   | No typos, grammatical errors, or missing sections in this report |  |
| 6   | Report maintains a clean aesthetic and is highly human-readable |  |

### Reflections / Takeaways
*(Individual or collective thoughts on what you learned)*

### Contribution Percentage
*(e.g., Member A: 50%, Member B: 50%)*