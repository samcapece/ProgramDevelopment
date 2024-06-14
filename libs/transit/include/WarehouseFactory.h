#ifndef WarehouseFACTORY_H_
#define Warehouse_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Warehouse.h"

/**
 *@brief Warehouse Factory to produce Warehouse class.
 **/
class WarehouseFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for WarehouseFactory class.
   **/
  virtual ~WarehouseFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);
};

#endif
