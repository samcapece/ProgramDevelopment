#include "Boat.h"

#include "BeelineStrategy.h"
#include "SimulationModel.h"

Boat::Boat(JsonObject& obj) : IEntity(obj) {
  // Initialize boat attributes from the JSON object
}

Boat::~Boat() {}
void Boat ::getPackage(Package* package) {
  this->package = package;
  std ::cout << "boat has gotten package" << std ::endl;
}

void Boat ::start() {
  // find closest warehouse and set istrategy
  Vector3 finalDestination;
  double distance = 100000;
  Warehouse* closest_warehouse = nullptr;
  for (int i = 0; i < model->Warehouses.size(); ++i) {
    Warehouse* warehouse = model->Warehouses.at(i);
    if (warehouse->getPosition().dist(position) > 1 &&
        warehouse->getPosition().dist(position) < distance) {
      closest_warehouse = warehouse;
      distance = warehouse->getPosition().dist(position);
    }
  }
  Vector3 closest = closest_warehouse->getPosition();
  finalDestination = closest;

  toFinalDestination = new BeelineStrategy(position, finalDestination);
  model->notifyBoat(closest_warehouse->getName());
  std ::cout << "end of boat start" << std ::endl;
}

void Boat ::update(double dt) {
  // std :: cout << "In boat update" << std :: endl;
  if (package) {
    if (notstarted) {
      start();
      notstarted = 0;
    }
    if (toFinalDestination) {
      toFinalDestination->move(this, dt);

      if (package) {
        package->setPosition(Vector3(position.x, 250, position.z));
        package->setDirection(direction);
      }

      if (toFinalDestination->isCompleted()) {
        // Notify that the drone has dropped off the package
        model->notifyBoatComplete(this->getName());
        delete toFinalDestination;
        toFinalDestination = nullptr;
        // add package to the sim queue
        model->scheduledDeliveries.push_front(package);
        std ::cout << "added package back to sim" << std ::endl;
        // delete package;
        package = nullptr;

        // remove this from sim
        std ::cout << "removing package back to sim" << std ::endl;
        model->removeEntity(id);
        std ::cout << "removed from sim" << std ::endl;
      }
    }
  }
}
