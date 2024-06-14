#ifndef Boat_FACTORY_H_
#define Boat_FACTORY_H_

#include <vector>

#include "Boat.h"
#include "IEntityFactory.h"

/**
 *@brief Boat Factory to produce Boat class.
 **/
class BoatFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for BoatFactory class.
   **/
  virtual ~BoatFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);
};

#endif
