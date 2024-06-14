#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "Package.h"
#include "SimulationModel.h"
#include "SpinDecorator.h"

Drone::Drone(JsonObject& obj) : IEntity(obj) {
  available = true;
  if (this->getPosition()[0] > 400) {
    tag = "east";
  } else {
    tag = "west";
  }
}

Drone::~Drone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
}

// returns which side of the river the input vector is in
std ::string Drone ::getSide(Vector3 pos) {
  Vector3 topleft = Vector3(-1000, pos[1], -738);
  Vector3 bottomright = Vector3(756, pos[1], 876.86);

  // calculates wheter package is on west or east side
  double A = bottomright.z - topleft.z;
  double B = topleft.x - bottomright.x;
  double C = bottomright.x * topleft.z - topleft.x * bottomright.z;

  double result = A * pos.x + B * pos.z + C;
  if (result > 0) {
    return "east";
  } else {
    return "west";
  }
}

void Drone::getNextDelivery() {
  if (model && model->scheduledDeliveries.size() > 0) {
    Package* first = model->scheduledDeliveries.front();
    Vector3 packPos = first->getPosition();
    std ::string package_side = getSide(packPos);
    // package->setTag(package_side);
    if (package_side == this->tag) {
      std ::cout << "package_side is: " << package_side << std ::endl;
      package = model->scheduledDeliveries.front();
      model->scheduledDeliveries.pop_front();
    } else {
      package == nullptr;
    }

    if (package) {
      // Notify that the drone has received a delivery task
      model->notifyDeliveryReceived(this->getName(), package->getName());

      std ::cout << "drone moving: " << this->tag << std ::endl;
      available = false;
      pickedUp = false;

      Vector3 packagePosition = package->getPosition();
      Vector3 finalDestination = package->getDestination();

      toPackage = new BeelineStrategy(position, packagePosition);

      // calculate if robot is on same side as package
      std ::string robot_side = getSide(finalDestination);

      if (robot_side != package_side) {
        std ::cout << "package and robot are on opposite sides " << std ::endl;
        double distance = 100000;
        // find the closest warehouse to go to
        for (int i = 0; i < model->Warehouses.size(); ++i) {
          Warehouse* warehouse = model->Warehouses.at(i);
          std ::string warehouseTag = getSide(warehouse->getPosition());
          if (warehouseTag == tag &&
              warehouse->getPosition().dist(packagePosition) < distance) {
            closest_warehouse = warehouse;
            distance = warehouse->getPosition().dist(packagePosition);
          }
        }
        Vector3 closest = closest_warehouse->getPosition();
        finalDestination = closest;
      } else {
        std ::cout << "package and robot are on the same side " << std ::endl;
      }

      std::string strat = package->getStrategyName();
      if (strat == "astar") {
        toFinalDestination = new JumpDecorator(new AstarStrategy(
            packagePosition, finalDestination, model->getGraph()));
      } else if (strat == "dfs") {
        toFinalDestination =
            new SpinDecorator(new JumpDecorator(new DfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "bfs") {
        toFinalDestination =
            new SpinDecorator(new SpinDecorator(new BfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "dijkstra") {
        toFinalDestination =
            new JumpDecorator(new SpinDecorator(new DijkstraStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else {
        toFinalDestination =
            new BeelineStrategy(packagePosition, finalDestination);
      }
    }
  }
}

void Drone::update(double dt) {
  if (available) getNextDelivery();

  if (toPackage) {
    toPackage->move(this, dt);

    if (toPackage->isCompleted()) {
      // Notify that the drone has picked up the package
      model->notifyPickUp(this->getName(), package->getName());

      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination) {
    toFinalDestination->move(this, dt);

    if (package && pickedUp) {
      package->setPosition(position);
      package->setDirection(direction);
    }

    if (toFinalDestination->isCompleted()) {
      pickedUp = false;
      // if drone has a closest warehouse then package is not done moving
      if (closest_warehouse) {
        model->notifyWarehouse(this->getName(), closest_warehouse->getName());
        delete toFinalDestination;
        toFinalDestination = nullptr;
        package->setWarehouse(closest_warehouse);
        package->passOff();
        package = nullptr;
        available = true;
        pickedUp = false;
        closest_warehouse = nullptr;
      } 
      else {
        model->notifyDropOff(this->getName(), package->getName());
        delete toFinalDestination;
        toFinalDestination = nullptr;
        package->handOff();
        package = nullptr;
        available = true;
        pickedUp = false;
      }
    }
  }
}

