#include "WarehouseFactory.h"

IEntity* WarehouseFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("warehouse") == 0) {
    std::cout << "Warehouse Created" << std::endl;
    return new Warehouse(entity);
  }
  return nullptr;
}
