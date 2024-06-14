#include "SimulationModel.h"

#include "BoatFactory.h"
#include "DroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "PackageFactory.h"
#include "RobotFactory.h"
#include "WarehouseFactory.h"

SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  entityFactory.AddFactory(new DroneFactory());
  entityFactory.AddFactory(new PackageFactory());
  entityFactory.AddFactory(new RobotFactory());
  entityFactory.AddFactory(new HumanFactory());
  entityFactory.AddFactory(new HelicopterFactory());
  entityFactory.AddFactory(new WarehouseFactory());
  entityFactory.AddFactory(new BoatFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto& [id, entity] : entities) {
    delete entity;
  }
  delete graph;
}

IEntity* SimulationModel::createEntity(JsonObject& entity) {
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;
  std::string type = entity["type"];

  IEntity* myNewEntity = nullptr;
  if (myNewEntity = entityFactory.CreateEntity(entity)) {
    // Call AddEntity to add it to the view
    myNewEntity->linkModel(this);
    controller.addEntity(*myNewEntity);
    entities[myNewEntity->getId()] = myNewEntity;
    if (type == "warehouse") {
      Warehouses.push_back(dynamic_cast<Warehouse*>(myNewEntity));
      std ::cout << "added warehouse to queue" << std ::endl;
    }
  }

  return myNewEntity;
}

void SimulationModel::removeEntity(int id) { removed.insert(id); }

/// Schedules a Delivery for an object in the scene
void SimulationModel::scheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  Robot* receiver = nullptr;

  for (auto& [id, entity] : entities) {
    if (name == entity->getName()) {
      if (Robot* r = dynamic_cast<Robot*>(entity)) {
        if (r->requestedDelivery) {
          receiver = r;
          break;
        }
      }
    }
  }

  Package* package = nullptr;

  for (auto& [id, entity] : entities) {
    if (name + "_package" == entity->getName()) {
      if (Package* p = dynamic_cast<Package*>(entity)) {
        if (p->requiresDelivery) {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package) {
    package->initDelivery(receiver);
    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    controller.sendEventToView("DeliveryScheduled", details);
  }
}

const routing::IGraph* SimulationModel::getGraph() { return graph; }

/// Updates the simulation
void SimulationModel::update(double dt) {
  for (auto& [id, entity] : entities) {
    entity->update(dt);
    controller.updateEntity(*entity);
  }
  for (int id : removed) {
    removeFromSim(id);
  }
  removed.clear();
}

void SimulationModel::stop(void) { controller.stop(); }

void SimulationModel::removeFromSim(int id) {
  IEntity* entity = entities[id];
  if (entity) {
    for (auto i = scheduledDeliveries.begin(); i != scheduledDeliveries.end();
         ++i) {
      if (*i == entity) {
        scheduledDeliveries.erase(i);
        break;
      }
    }
    controller.removeEntity(*entity);
    entities.erase(id);
    delete entity;
  }
}

// Notification methods below

void SimulationModel::sendNotification(const std::string& message) {
  // Log the message being sent
  std::cout << "Sending notification: " << message << std::endl;
  JsonObject details;
  details["message"] = message;
  // Use the controller to send the event
  controller.sendEventToView("NotificationEvent", details);
}

void SimulationModel::notifyDeliveryReceived(const std::string& droneName, const std::string& packageName) {
  sendNotification(droneName + " has received a delivery task for " + packageName);
}

void SimulationModel::notifyPickUp(const std::string& droneName, const std::string& packageName) {
  sendNotification(droneName + " has picked up " + packageName);
}

void SimulationModel::notifyDropOff(const std::string& droneName, const std::string& packageName) {
  sendNotification(droneName + " has dropped off " + packageName + " to the client.");
}

void SimulationModel::notifyWarehouse(const std::string& droneName, const std::string& warehouseName) {
  sendNotification(droneName + " has dropped off the package at " + warehouseName + ".");
}

void SimulationModel::notifyBoat(const std::string& warehouseName) {
  sendNotification("Package is heading to " + warehouseName + " by boat.");
}

void SimulationModel::notifyBoatComplete(const std::string& boatName) {
  sendNotification(boatName + " has arrived at warehouse with package.");
}

void SimulationModel::notifyLate(const std::string& packageName) {
  sendNotification(packageName + " is running late.");
}
