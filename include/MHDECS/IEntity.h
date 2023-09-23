

#ifndef MHDECS_IENTITY_H
#define MHDECS_IENTITY_H

#define CHECK_COMPONENT_TYPE_VALID if (!std::is_base_of<Component, C>::value) {throw std::runtime_error("[MHDECS] Component must be inherited from MHDECS::Component");}

#include <type_traits>
#include <iostream>
#include <vector>
#include <functional>

#include "Component.h"

namespace MHDECS {

    class IEntity {
        public:

            template<typename C>
            C& AddComponent();

            template<typename C>
            C* GetComponent();

            template<typename C>
            bool HasComponent();

            template<typename C>
            bool RemoveComponent();

        private:
            std::vector<Component*> components;
    };

    template<typename C>
    bool IEntity::RemoveComponent() {
        CHECK_COMPONENT_TYPE_VALID

        int indexToDelete = -1;
        Component* compToDelete = nullptr;

        for (int i = 0; i < components.size(); i++) {
            Component* comp = components[i];
            if (dynamic_cast<C*>(comp) != nullptr) {
                indexToDelete = i;
                compToDelete = comp;
            }
        }

        if (indexToDelete == -1) return false;

        components.erase(components.begin() + indexToDelete);
        delete compToDelete;
        return true;
    }

    template<typename C>
    C* IEntity::GetComponent() {
        CHECK_COMPONENT_TYPE_VALID
        for (Component* comp : components) {
            if (dynamic_cast<C*>(comp) != nullptr) {
                return (C*) comp;
            }
        }

        return nullptr;
    }

    template<typename C>
    bool IEntity::HasComponent() {
        CHECK_COMPONENT_TYPE_VALID
        for (Component* comp : components) {
            if (dynamic_cast<C*>(comp) != nullptr) {
                return true;
            }
        }

        return false;
    }


    template<typename C>
    C& IEntity::AddComponent() {
        CHECK_COMPONENT_TYPE_VALID

        if (!HasComponent<C>()) {
            C *comp = new C();
            components.push_back(comp);
            return *comp;
        } else {
            throw std::runtime_error("[MHDECS] Entity already has component");
        }
    }

}

#endif //MHDECS_IENTITY_H
