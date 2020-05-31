#pragma once

#include <Vofog.h>
#include <vector>
#include <unordered_map>
#include <Entity Component System/Entity.h>
#include <Entity Component System/Systems/EntitySystem.h>
#include <Entity Component System/Systems/Application/PhysicsSystem.h>
#include <Entity Component System/Components/PhysicsComponent.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Components/RenderableComponent.h>
#include <Entity Component System/Components/Component.h>
#include "btBulletDynamicsCommon.h"


#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>
namespace Vofog{
	namespace ECS{

		namespace EngineStateTuple{
			constexpr int EDITOR = 0;
			constexpr int APPLICATION = 1;
		}

		typedef size_t ComponentID;
		typedef size_t EntityID;
		typedef int EngineState;

		class VOFOG_API EntityManager{
		private:

			std::unordered_map<EntityID, std::unordered_map<ComponentID, std::shared_ptr<Component>>> _data;
			std::unordered_map<EntityID,std::shared_ptr<Entity>> _entities;
			std::vector<EntityID> _ids;
			std::vector<ComponentID> _componentIDs;
			std::vector<Component> _components;
			EntityID _currentEntity;

			std::vector<std::shared_ptr<Vofog::ECS::EntitySystem>> _editorSystems; 
			std::vector<std::shared_ptr<Vofog::ECS::EntitySystem>> _applicationSystems;

			std::vector<EntityID> _scriptIds;

			EngineState _mode = EngineStateTuple::EDITOR;

			std::shared_ptr<btDiscreteDynamicsWorld> _world;

			bool _transitionedStates = false;
		public:
			
			EntityID getEntityByName(std::string name);

			bool hasTransitioned() const{ return _transitionedStates; }
			EntityManager() = default;
			EntityManager(std::shared_ptr<btDiscreteDynamicsWorld> world):_world(world){}


			void clearScriptedEntities();
			ECS::Entity _LUA_addRenderableEntity(float x, float y, float z);
			ECS::Entity _LUA_addEntity();
		//	ECS::Entity _LUA_addEntity(std::string name);

			

			void setWorld(std::shared_ptr<btDiscreteDynamicsWorld> world){ _world = world; }
			std::shared_ptr<btDiscreteDynamicsWorld> getWorld(){ return _world; }
			bool entityExists(EntityID id){ return !_entities[id] ? false : true; }
			EngineState getState()const{ return _mode; }

			void setCurrentEntity(EntityID id){ _currentEntity = id; }
			void updateSystems();


			void removeEntity(EntityID id);


			EntityID getCurrentEntity(){ return _currentEntity; }
			const std::vector<ComponentID>& getComponentIDs()const{
				return _componentIDs;
			}

			const std::vector<ComponentID> getAssociatedComponentIDs(EntityID id)const{
				std::vector<ComponentID> associatedIDs;
				for(int i = 0; i < _componentIDs.size(); i++){
					bool seen = false;
					for(int k = 0; k < associatedIDs.size(); k++) if(_componentIDs[i] == associatedIDs[k]) seen = true;
					if(seen) continue;
					associatedIDs.push_back(_componentIDs[i]);
				}
				return associatedIDs;

			}


			std::shared_ptr<Entity> getEntity(EntityID id){
				bool thing;
				if(!_entities[id]){
					thing = true;
				}
				return _entities[id];
			}
			size_t getEntityCount()const{ return _ids.size(); }



			const std::vector<EntityID>& getIDS(){
				return _ids;
			}
			template<typename T>
			void addSystem(std::shared_ptr<T> system, EngineState state = EngineStateTuple::APPLICATION){
				if(state == EngineStateTuple::APPLICATION)
					_applicationSystems.push_back((system));
				if(state == EngineStateTuple::EDITOR)
					_editorSystems.push_back((system));
				
			}
			
			void addEntity(Entity entity);
			ECS::EntityID addEntity();

			void removeComponent(EntityID entity, ComponentID id);
			template<typename T>
			void attachComponent(EntityID entity, std::shared_ptr<T> component){
				
				//_data[entity][type_id<T>()] = component;
				_data[entity][component->getID()] = component;
				_components.push_back(*component);
				VOFOG_ASSERT(_entities[entity]);
			//	_entities[entity] = std::make_shared<Entity>(Entity());
				//if(!_entities[entity.getID()]) _entities[entity.getID()] = std::make_shared<Entity>(entity);
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component->getID())seen = true;
				if(!seen)_componentIDs.push_back(component->getID());
			}

			template<typename T>
			void attachComponent(EntityID entity, T component){
				VOFOG_ASSERT(_entities[entity]);
				//_data[entity][type_id<T>()] = std::make_shared<T>(component);
				_data[entity][component.getID()] = std::make_shared<T>(component);
				_components.push_back(component);
				//_entities[entity] = std::make_shared<Entity>(Entity());
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component.getID())seen = true;
				if(!seen)_componentIDs.push_back(component.getID());
			}

			template<typename T>
			void attachComponent(const Entity& entity, std::shared_ptr<T> component){
				VOFOG_ASSERT(_entities[entity.getID()]);
				//_data[entity.getID()][type_id<T>()] = component;
				_data[entity][component->getID()] = component;
				_components.push_back(*component);
				//_entities[entity.getID()] = std::make_shared<Entity>(entity);
				//if(!_entities[entity.getID()]) _entities[entity.getID()] = std::make_shared<Entity>(entity);
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component->getID())seen = true;
				if(!seen)_componentIDs.push_back(component->getID());
			}
			template<typename T, typename... Types>
			void attachComponent(const Entity& entity, std::shared_ptr<T> component, std::shared_ptr<Types> ... types){
			//	_data[entity.getID()][type_id<T>()] = component;
				_data[entity][component->getID()] = component;
				_components.push_back(*component);
				VOFOG_ASSERT(_entities[entity.getID()]);
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component->getID())seen = true;
				if(!seen)_componentIDs.push_back(component->getID());
				attachComponent(entity, types...);
			}
			template<typename T>
			void attachComponent(const Entity& entity, T component){
			//	_data[entity.getID()][type_id<T>()] = std::make_shared<T>(component);
				_data[entity.getID()][component.getID()] = std::make_shared<T>(component);
				_components.push_back(component);
				VOFOG_ASSERT(_entities[entity.getID()]);
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component.getID())seen = true;
				if(!seen)_componentIDs.push_back(component.getID());
			}
			template<typename T, typename... Types>
			void attachComponent(const Entity& entity, T component, Types ... types){
				//_data[entity.getID()][type_id<T>()] = std::make_shared<T>(component);
				uint32_t id = component.getID();
				_data[entity.getID()][component.getID()] = std::make_shared<T>(component);
				_components.push_back(component);
				VOFOG_ASSERT(_entities[entity.getID()]);
				bool seen = false;
				for(int i = 0; i < _componentIDs.size(); i++)
					if(_componentIDs[i] == component.getID())seen = true;
				if(!seen)_componentIDs.push_back(component.getID());
				attachComponent(entity, types...);
			}

			
			//


			



			template<typename T>
			std::shared_ptr<T> getComponent(const Entity& entity)/*const*/{
				//return std::dynamic_pointer_cast<T>(_data[entity.getID()][type_id<T>()]);
				T comp;
				return std::dynamic_pointer_cast<T>(_data[entity.getID()][comp.getID()]);
			}
			template<typename T>
			std::shared_ptr<T> getComponent(EntityID id)/*const*/{
			//	size_t compID = type_id<T>();
				size_t compID = T().getID();
				return std::dynamic_pointer_cast<T>(_data[id][compID]);
			}

			std::shared_ptr<Component> getComponent(EntityID id, ComponentID compID)/*const*/{
				return (_data[id][compID]);
			}

			template<typename T>
			std::vector<EntityID> getAssociatedEntities()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				uint32_t currentCompId = T().getID();
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if(_data[currentEntity][currentCompId]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename T, typename N>
			std::vector<EntityID> getAssociatedEntities()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if(_data[currentEntity][T().getID()] && _data[currentEntity][N().getID()]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename T, typename N, typename M>
			std::vector<EntityID> getAssociatedEntities()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if(_data[currentEntity][T().getID()] && _data[currentEntity][N().getID()] && _data[currentEntity][M().getID()]){
						associatedIDs.push_back(currentEntity);
					}
									
				}
				return associatedIDs;
			}

			template<typename A, typename B, typename C, typename D>
			std::vector<EntityID> getAssociatedEntities()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if (_data[currentEntity][A().getID()] &&
						_data[currentEntity][B().getID()] &&
						_data[currentEntity][C().getID()] &&
						_data[currentEntity][D().getID()]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename A, typename B, typename C, typename D, typename E>
			std::vector<EntityID> getAssociatedEntities()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if( _data[currentEntity][A().getID()] &&
						_data[currentEntity][B().getID()] &&
						_data[currentEntity][C().getID()] &&
						_data[currentEntity][D().getID()] &&
						_data[currentEntity][E().getID()]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename A, typename B, typename C, typename D, typename E, typename F>
			std::vector<EntityID> getAssociatedEntities() /*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if(_data[currentEntity][A().getID()] &&
						_data[currentEntity][B().getID()] &&
						_data[currentEntity][C().getID()] &&
						_data[currentEntity][D().getID()] &&
						_data[currentEntity][E().getID()] &&
						_data[currentEntity][F().getID()]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename T, typename N>
			std::vector<EntityID> getAssociatedEntitiesExcludingOne()/*const*/{
				std::vector<EntityID> associatedIDs;
				Vofog::ECS::EntityID currentEntity;
				for(int i = 0; i < _ids.size(); i++){
					currentEntity = _ids[i];
					if(_data[currentEntity][type_id<T>()] && !_data[currentEntity][type_id<N>()]){
						associatedIDs.push_back(currentEntity);
					}

				}
				return associatedIDs;
			}

			template<typename T>
			std::vector<EntityID>& filterType(){


			}


			std::shared_ptr<ECS::TransformComponent> getTransformComponent(EntityID id);
			std::shared_ptr<ECS::PhysicsComponent> getPhysicsComponent(EntityID id);

			std::string getEntityName(EntityID id){ return _entities[id]->getName(); }
			void setEntityName(EntityID id, std::string name){ return _entities[id]->setName(name); }

			void setState(EngineState state);


			template<class Archive>
			void save(Archive& ar)const {
				ar(_data, _entities, _currentEntity, _componentIDs, _ids, _applicationSystems, _editorSystems);
			}

			template<class Archive>
			void load(Archive& ar) {
				ar(_data, _entities, _currentEntity, _componentIDs, _ids, _applicationSystems, _editorSystems);
				_mode = EngineStateTuple::EDITOR;

			}
		};






	}



}