#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

#include <vector>

#include "Boat.h"
#include "IEntity.h"
#include "Package.h"
#include "SimulationModel.h"

/**
 * @class Warehouse
 * @brief Represents a warehouse for storing and transferring packages.
 */
class Warehouse : public IEntity {
 public:
  /**
   * @brief Constructor for the warehouse
   * @param obj JSON object containing the warehouse's information
   */
  Warehouse(JsonObject& obj);

  /**
   * @brief Destructor for the warehouse
   */
  ~Warehouse();

  /**
   * @brief Creates a boat and transfers package to boat
   */
  void processPackageTransfer();

  /**
   * @brief Receive a package for storage or forwarding
   * @param package Pointer to the package to be received
   */
  void receivePackage(Package* package);

  /**
   * @brief Updates the Warehouse
   * @param dt difference in time since last update
   */
  void update(double dt);

 private:
  // std::vector<Boat*> boats;  // Boats currently docked at the warehouse
  Package* storedPackage = nullptr;  // Packages stored in the warehouse
};

#endif  // WAREHOUSE_H_
