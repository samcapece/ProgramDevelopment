#ifndef PACKAGE_H
#define PACKAGE_H

#include <vector>

#include "IEntity.h"
#include "math/vector3.h"
#include "util/json.h"
#include "Warehouse.h"
#include "Boat.h"

class Robot;
class Warehouse;
class Boat;

class Package : public IEntity {
 public:
  /**
   * @brief Constructor
   * @param obj JSON object containing the package's information
   */
  Package(JsonObject& obj);

  /**
   * @brief Gets the Package's destination
   * @return The Package's destination
   */
  Vector3 getDestination() const;

  /**
   * @brief Returns the name of the strategy for this package
   * 
   * @returns String name of strategy
  */
  std::string getStrategyName() const;

  /**
   * @brief Set the Strategy Name
   *
   * @param strategyName_ Strategy name
   */
  void setStrategyName(std::string strategyName_);

  /**
   * @brief Updates the Package
   * 
   * @param dt difference in time since last update
  */
  void update(double dt);

  /**
   * @brief Sets the attributes for delivery
   * 
   * @param owner Robot for the package to be delivered to
  */
  void initDelivery(Robot* owner);

  /**
   * @brief Gives the robot/owner this package
  */
  void handOff();

  /**
   * @brief Sets the attributes for passing to warehouse
   * @param storage Warehouse for the package to be passed to
  */
  void setWarehouse(Warehouse* storage);

  /**
   * @brief Passes this package to warehouse
  */
  void passOff();

  /**
   * @brief Passes this package to boat
  */
  void giveToBoat();

  /**
   * @brief Sets the attributes for passing to boat
   * @param newBoat Boat for the package to be passed to
  */
  void setBoat(Boat* newBoat);

  /**
   * @brief get the time package has been moving 
   * @return the time the package has been moving
  */
  double getTimer();

  bool requiresDelivery = true;

 private:
  Vector3 destination;
  std::string strategyName;
  Robot* owner = nullptr;
  Warehouse* warehouse = nullptr;
  Boat* boatOwner = nullptr;
  double timer = 0;
  bool notifcationsent = false;
};

#endif  // PACKAGE_H
