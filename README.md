# MHDECS

![Logo](logo.PNG)

## MHDECS - Simple cross-platform header-only C++ Entity Component System

## Brief description of objects:

### Component

The main structure from which all components should be inherited

Example:
```cpp
  struct MyComponent : public MHDECS::Component {
    float a, b, c;
    bool d, e, f;
    ...
  }

```

### IEntity

The class-interface from which all entities should be inherited. It can also be used without inheritance.

Example with inheritance:
```cpp
  class MyEntity : public MyBaseClass, public MHDECS::IEntity, ... {
      ...
  }

  auto entity = MHDECS::Registry::registerEntity<MyEntity>(constructor args...); // Returns pointer to new entity
```

Example without inheritance:
```cpp
  auto entity = MHDECS::Registry::registerEntity<MHDECS::IEntity>(); // Returns pointer to new entity
```

If you already have an existing entity:
```cpp
    MHDECS::Registry::registerEntity(entity);
```

You can manipulate with components:

```cpp
  entity->AddComponent<MyComponent>(); // Add component to the entity. Each component can only be added once. Returns reference to the created component.
  entity->GetComponent<MyComponent>(); // Returns pointer to specified component if exists, otherwise nullptr
  entity->HasComponent<MyComponent>(); // Returns bool - does the entity has component
  entity->RemoveComponent<MyComponent>(); // Removes component and frees memory
```


### Registry

Static class required for working with components and registering entities.

Example:

```cpp
  // Do some actions when the component update signal is called

  
  MHDECS::Registry::connect<MyComponent>(MHDECS::ECS_SIGNAL::UPDATE, [] (MHDECS::IEntity* entity) {
      // Do something
      // MHDECS::IEntity* entity is the entity to which the current component is attached
      // MyComponent is the current type of component
      ...
  });

  // Call an update signal for specified type of component
  MHDECS::Registry::update<MyComponent>();

  // Create new entity
  auto entity = MHDECS::Registry::registerEntity<MyEntity>(constructor args...); // Returns pointer to new entity

  // Register existing entity
  MHDECS::Registry::registerEntity(entity);

  // Unregister entity
  MHDECS::Registry::unregisterEntity(entity);

  // Get all entities with specified component
  std::vector<MHDECS::IEntity*> entities = MHDECS::Registry::getEntitiesWithComponent<MyComponent>();

  // You can also require 2 and more components
  std::vector<MHDECS::IEntity*> entities = MHDECS::Registry::getEntitiesWithComponent<MyOtherComponent>(MHDECS::Registry::getEntitiesWithComponent<MyComponent>());

```

