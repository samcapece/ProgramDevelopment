#include "BoatFactory.h"

IEntity* BoatFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("boat") == 0) {
    std::cout << "Boat Created" << std::endl;
    return new Boat(entity);
  }
  return nullptr;
}
