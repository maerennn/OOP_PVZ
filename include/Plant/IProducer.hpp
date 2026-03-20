#ifndef IPRODUCER_HPP
#define IPRODUCER_HPP

/**
 * @file IProducer.hpp
 * @brief Interface for plants that generate resources (sun).
 *
 * === Interface Design Philosophy ===
 * Interfaces separate "identity" (what a plant IS) from "ability" (what it CAN DO).
 *
 * The IProducer interface allows any plant type to have resource generation
 * capability without being tied to a specific class hierarchy.
 *
 * Example: Both Sunflower (regular plant) and Twin Sunflower (upgraded plant)
 * can implement IProducer with different production rates.
 */

class IProducer {
public:
    virtual ~IProducer() = default;

    // Core production method - generates resources
    virtual void Produce() = 0;

    // Production properties
    virtual int GetProductionAmount() const = 0;
    virtual float GetProductionRate() const = 0;  // Time between productions (seconds)

    // Check if ready to produce
    virtual bool CanProduce() const = 0;
};

#endif // IPRODUCER_HPP
