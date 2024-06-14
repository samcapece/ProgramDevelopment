#include "Warehouse.h"

#include "Boat.h"
#include "util/json.h"

Warehouse::Warehouse(JsonObject& obj) : IEntity(obj) {
  // Initialize warehouse attributes from the JSON object
}

Warehouse::~Warehouse() {}

void Warehouse::processPackageTransfer() {
  std ::cout << "creating boat" << std ::endl;
  JsonObject myEntity;
  myEntity["name"] = "Boat";
  myEntity["type"] = "boat";
  myEntity["mesh"] = "assets/model/car1.glb";

  JsonArray pos = {position.x, position.y, position.z};
  myEntity["position"] = pos;

  JsonArray sc = {0.005, 0.005, 0.005};
  myEntity["scale"] = sc;

  JsonArray rot = {0, 0, 0};
  myEntity["rotation"] = rot;

  JsonArray dir = {0, 0, 0};
  myEntity["direction"] = dir;

  myEntity["speed"] = 25.0;

  JsonArray off = {0, 1.5, 0};
  myEntity["offset"] = off;

  IEntity* newBoat = model->createEntity(myEntity);

  std ::cout << "Warehoues : Boat created" << std ::endl;

  storedPackage->setBoat(dynamic_cast<Boat*>(newBoat));
  storedPackage->giveToBoat();
  storedPackage = nullptr;
}

void Warehouse::receivePackage(Package* package) {
  storedPackage = package;
  std ::cout << "warehouse has received package" << std ::endl;
}

void Warehouse::update(double dt) {
  if (storedPackage) {
    processPackageTransfer();
    storedPackage = nullptr;
    std ::cout << "Warehouse has removed package" << std ::endl;
  }
}
