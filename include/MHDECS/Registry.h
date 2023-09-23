

#ifndef MHDECS_REGISTRY_H
#define MHDECS_REGISTRY_H

#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <any>

#include "Component.h"
#include "IEntity.h"

namespace MHDECS {


    enum class ECS_SIGNAL {
        UPDATE
    };



    class Registry {
        using SignalFunction = std::function<void(IEntity*)>;
        inline static std::unordered_map<size_t, std::unordered_map<ECS_SIGNAL, SignalFunction>> signalFunctions;
        inline static std::vector<IEntity*> entities;

        template <typename C>
        static SignalFunction getFunc(ECS_SIGNAL signal) {
            CHECK_COMPONENT_TYPE_VALID

            auto type_id = typeid(C).hash_code();
            auto it = signalFunctions.find(type_id);
            if (it != signalFunctions.end()) {
                auto& type_map = it->second;
                auto signal_it = type_map.find(signal);
                if (signal_it != type_map.end()) {
                    return signal_it->second;
                }
            }
            return nullptr;
        }


        public:
            template <typename C>
            static void connect(ECS_SIGNAL signal, SignalFunction func) {
                CHECK_COMPONENT_TYPE_VALID

                auto type_id = typeid(C).hash_code();
                signalFunctions[type_id][signal] = func;
            }

            template<typename T, typename... Args>
            static T* registerEntity(Args... args) {
                if (!std::is_base_of<IEntity, T>::value) {
                    throw std::runtime_error("[MHDECS] Entity must be inherited from MHDECS::IEntity");
                }

                IEntity* entity = new T(args...);

                entities.push_back(entity);
                return (T*) entity;

            }

            static void registerEntity(IEntity* entity) {
                auto it = std::find(entities.begin(), entities.end(), entity);

                if (it == entities.end()) {
                    entities.push_back(entity);
                } else {
                    throw std::runtime_error("[MHDECS] Entity has already been registered");
                }

            }

            static bool unregisterEntity(IEntity* entity) {
                int indexToDelete = -1;

                for (int i = 0; i < entities.size(); i++) {
                    IEntity* d = entities[i];
                    if (entity == d) {
                        indexToDelete = i;
                    }
                }

                if (indexToDelete == -1) return false;

                entities.erase(entities.begin() + indexToDelete);
                return true;
            }

            static void unregisterAll() {
                entities.clear();
            }

            template <typename C>
            static std::vector<IEntity*> getEntitiesWithComponent() {
                CHECK_COMPONENT_TYPE_VALID

                std::vector<IEntity*> ret;
                for (IEntity* e : entities) {
                    if (e->HasComponent<C>()) {
                        ret.push_back(e);
                    }
                }

                return ret;
            }

            template <typename C>
            static std::vector<IEntity*> getEntitiesWithComponent(std::vector<IEntity*> ents) {
                CHECK_COMPONENT_TYPE_VALID

                std::vector<IEntity*> ret;
                for (IEntity* e : ents) {
                    if (e->HasComponent<C>()) {
                        ret.push_back(e);
                    }
                }

                return ret;
            }

            template <typename C>
            static void update() {
                CHECK_COMPONENT_TYPE_VALID

                for (IEntity* e : entities) {
                    if (e->HasComponent<C>()) {
                        if (getFunc<C>(ECS_SIGNAL::UPDATE) != nullptr) {
                            getFunc<C>(ECS_SIGNAL::UPDATE)(e);
                        }
                    }
                }
            }


    };

}

#endif //MHDECS_REGISTRY_H
