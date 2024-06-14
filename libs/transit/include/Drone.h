#ifndef DRONE_H_
#define DRONE_H_

#include <string>
#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "Warehouse.h"
#include "math/vector3.h"

class Package;
class Warehouse;

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Drone();

  /**
   * @brief Gets the next delivery in the scheduler
   */
  void getNextDelivery();

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);

  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied.
   */
  Drone(const Drone& drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   */
  Drone& operator=(const Drone& drone) = delete;

  /**
  @brief finds which side of the river a point is on
  @param pos the input position that you want to know what side it is on
  @return the side of the river the input is on
  */
  std ::string getSide(Vector3 pos);

 private:
  bool available = false;
  bool pickedUp = false;
  Package* package = nullptr;
  IStrategy* toPackage = nullptr;
  IStrategy* toFinalDestination = nullptr;
  std ::string tag;
  Warehouse* closest_warehouse = nullptr;
};

#endif
