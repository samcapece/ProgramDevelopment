#ifndef SIMULATION_MODEL_H_
#define SIMULATION_MODEL_H_

#include <deque>
#include <map>
#include <set>

#include "CompositeFactory.h"
#include "Drone.h"
#include "IController.h"
#include "IEntity.h"
#include "Robot.h"
#include "Warehouse.h"
#include "graph.h"

//--------------------  Model ----------------------------

/// Simulation Model handling the transit simulation.  The model can communicate
/// with the controller.
/**
 * @brief Class SimulationModel handling the transit simulation. it can
 * communicate with the controller
 **/
class SimulationModel {
 public:
  /**
   * @brief Default constructor that create the SimulationModel object
   **/
  SimulationModel(IController& controller);

  /**
   * @brief Destructor
   */
  ~SimulationModel();

  /**
   * @brief Set the Graph for the SimulationModel
   * @param graph Type IGraph* contain the new graph for SimulationModel
   **/
  void setGraph(const routing::IGraph* graph) { this->graph = graph; }

  /**
   * @brief Creates a new simulation entity
   * @param entity Type JsonObject contain the entity's reference to decide
   *which entity to create
   **/
  IEntity* createEntity(JsonObject& entity);

  /**
   * @brief Removes entity with given ID from the simulation
   *
   * @param id of the entity to be removed
   */
  void removeEntity(int id);

  /**
   * @brief Schedule a trip for an object in the scene
   * @param detail Type JsonObject contain the entity's reference to schedule
   *the detail of the trip being scheduled
   **/
  void scheduleTrip(JsonObject& details);

  /**
   * @brief Update the simulation
   * @param dt Type double contain the time since update was last called.
   **/
  void update(double dt);

  /**
   * @brief Stops the simulation
   * @return Void
   **/
  void stop();

  /**
   * @brief Sends a notification message to the controller, which will forward
   *it to the view.
   * @param message The notification message to be sent.
   **/
  void sendNotification(const std::string& message);

  /**
   * @brief Notifies that a drone has received a delivery task.
   * @param droneName The name of the drone that received the delivery task.
   **/
  void notifyDeliveryReceived(const std::string& droneName, const std::string& packageName);

  /**
   * @brief Notifies that a drone has picked up a package.
   * @param droneName The name of the drone that picked up the package.
   **/
  void notifyPickUp(const std::string& droneName, const std::string& packageName);

  /**
   * @brief Notifies that a drone has dropped off a package.
   * @param droneName The name of the drone that dropped off the package.
   **/
  void notifyDropOff(const std::string& droneName, const std::string& packageName);

  /**
   * @brief Notifies that a drone has dropped off a package at a warehouse.
   * @param warehouseName The name of the warehouse.
   **/
  void notifyWarehouse(const std::string& droneName,
                       const std::string& warehouseName);

  /**
   * @brief Notifies that a boat has picked up a package.
   * @param warehouseName The name of the warehouse the boat is heading to.
   **/
  void notifyBoat(const std::string& warehouseName);

  /**
   * @brief Notifies that a boat has completed a delivery.
   * @param boatName The name of the boat.
   **/
  void notifyBoatComplete(const std::string& boatName);

  /**
   * @brief Notifies the client that a package is late.
   * @param packageName The name of the package.
   **/
  void notifyLate(const std::string& packageName);

  /**
   * @brief Returns the graph of the map
   *
   * @returns IGraph* graph pointer
   */
  const routing::IGraph* getGraph();

  std::deque<Package*> scheduledDeliveries;
  std::deque<Warehouse*> Warehouses;

 protected:
  IController& controller;
  std::map<int, IEntity*> entities;
  std::set<int> removed;
  void removeFromSim(int id);
  const routing::IGraph* graph;
  CompositeFactory entityFactory;
};

#endif
