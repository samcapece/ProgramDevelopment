#include "Package.h"

#include "Robot.h"
#include "SimulationModel.h"

Package::Package(JsonObject& obj) : IEntity(obj) {}

Vector3 Package::getDestination() const { return destination; }

std::string Package::getStrategyName() const { return strategyName; }

void Package::setStrategyName(std::string strategyName_) {
  strategyName = strategyName_;
}

void Package::update(double dt) {
  this->timer += dt;
  if (notifcationsent != true && this->timer > 100.0) {
    notifcationsent = true;
    std ::cout << "timer has reached limit" << std ::endl;
    if (!owner->isRecieved()) {
      std ::cout << "PACKAGE IS LATE" << std ::endl;
      model->notifyLate(this->getName());
    }
  }
}

void Package::initDelivery(Robot* owner) {
  this->owner = owner;
  owner->requestedDelivery = false;
  requiresDelivery = false;
  destination = owner->getPosition();
}

void Package::handOff() {
  if (owner) {
    owner->receive(this);
  }
}

void Package::setWarehouse(Warehouse* storage) { this->warehouse = storage; }

void Package::passOff() {
  if (warehouse) {
    warehouse->receivePackage(this);
  }
}

void Package ::setBoat(Boat* newBoat) { this->boatOwner = newBoat; }

void Package::giveToBoat() {
  if (boatOwner) {
    boatOwner->getPackage(this);
  }
}
