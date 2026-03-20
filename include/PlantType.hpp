#ifndef PLANT_TYPE_HPP
#define PLANT_TYPE_HPP

#include <string>
#include <memory>
#include <functional>

class Plant;

/**
 * @brief Enumeration of all plant types in the game.
 */
enum class PlantType {
    PEASHOOTER,
    SUNFLOWER,
    WALLNUT,
    CHERRYBOMB,
    COUNT  // Keep last - represents number of plant types
};

/**
 * @brief Metadata for a plant type.
 * Contains all static information needed to display and create plants.
 */
struct PlantInfo {
    PlantType type;
    std::string name;
    int sunCost;
    float rechargeTime;
    std::string iconPath;       // Path to icon image for seed packet
    std::string animationDir;   // Path to animation frames directory
    std::function<std::shared_ptr<Plant>()> factory;  // Creates plant instance
};

/**
 * @brief Central registry for plant metadata.
 * Provides access to plant info and factory methods without modifying Plant classes.
 */
class PlantRegistry {
public:
    /**
     * @brief Get metadata for a plant type.
     * @param type The plant type to query
     * @return Reference to PlantInfo struct
     */
    static const PlantInfo& Get(PlantType type);

    /**
     * @brief Create a new plant instance of the given type.
     * @param type The plant type to create
     * @return Shared pointer to the new plant (already initialized)
     */
    static std::shared_ptr<Plant> Create(PlantType type);

    /**
     * @brief Get the number of registered plant types.
     */
    static constexpr int GetCount() { return static_cast<int>(PlantType::COUNT); }
};

#endif // PLANT_TYPE_HPP
