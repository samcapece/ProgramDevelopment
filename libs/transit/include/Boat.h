#ifndef BOAT_H_
#define BOAT_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "Package.h"
#include "Warehouse.h"

class Package;
/**
 * @class Boat
 * @brief Represents a boat for transporting packages in the simulation.
 */
class Boat : public IEntity {
 public:
  /**
   * @brief Constructor for the boat
   * @param obj JSON object containing the boat's information
   */
  Boat(JsonObject& obj);

  /**
   * @brief Destructor for the boat
   */
  ~Boat();

  /**
   * @brief Updates the Boat
   * @param dt difference in time since last update
   */
  void update(double dt);

  /**
   * @brief find the nearest warehoues and set up Beeline for finalDestination
   */
  void start();

  /**
   * @brief Receive a package for storage or forwarding
   * @param package Pointer to the package to be received
   */
  void getPackage(Package* package);

 private:
  Package* package;  // Packages currently on the boat
  IStrategy* toFinalDestination = nullptr;
  int notstarted = 1;
};

#endif  // BOAT_H_
