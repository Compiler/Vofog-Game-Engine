#include <vpch.h>
#include<Entity Component System/EntityManager.h>

namespace Vofog{


	ECS::EntityID ECS::EntityManager::getEntityByName(std::string name) {
		for (int i = 0; i < _ids.size(); i++) {
			if (_entities[_ids[i]]->getName() == name) 
				return _entities[_ids[i]]->getID();
		}
		
	}


	void ECS::EntityManager::removeEntity(ECS::EntityID id) {
		for (int i = 0; i < _ids.size(); i++) {
			if (_ids[i] == id) {
				_entities.erase(_ids[i]);
				_data.erase(_ids[i]);
				_ids.erase(_ids.begin() + i);
				if (_currentEntity == id)
					_currentEntity = _ids[0];
			}
		}
	
	}


	void ECS::EntityManager::addEntity(Entity entity){
		_data[entity.getID()] = std::unordered_map<ComponentID, std::shared_ptr<Component>>();
		_ids.push_back(entity.getID());
		_entities[entity.getID()] = std::make_shared<Entity>(entity);
		if(!_entities[_currentEntity])_currentEntity = entity.getID();
	}
	ECS::EntityID ECS::EntityManager::addEntity(){
		Entity entity = Entity();
		_data[entity.getID()] = std::unordered_map<ComponentID, std::shared_ptr<Component>>();
		_ids.push_back(entity.getID());
		_entities[entity.getID()] = std::make_shared<Entity>(entity);
		attachComponent<ECS::TransformComponent>(entity.getID(), ECS::TransformComponent());
		if(!_entities[_currentEntity])_currentEntity = entity.getID();
		return entity.getID();
	}

	std::shared_ptr<ECS::TransformComponent> ECS::EntityManager::getTransformComponent(EntityID id) {
		return std::dynamic_pointer_cast<ECS::TransformComponent>(_data[id][ECS::TransformComponent().getID()]);
	}

	std::shared_ptr<ECS::PhysicsComponent> ECS::EntityManager::getPhysicsComponent(EntityID id) {
		return std::dynamic_pointer_cast<ECS::PhysicsComponent>(_data[id][ECS::PhysicsComponent().getID()]);
	}

	void ECS::EntityManager::updateSystems(){
		if(_transitionedStates){
			static btVector3 gravityOff(0, 0, 0);
			static btVector3 gravityOn(0, -9.81, 0);
			if(_mode == EngineStateTuple::APPLICATION) _world->setGravity(gravityOn); else _world->setGravity(gravityOff);
			_world->stepSimulation(1.0f / 30.0f);
			std::vector<EntityID> thing = getAssociatedEntities<TransformComponent, ECS::PhysicsComponent>();
			int count = 0;
			if(/*_mode == EngineStateTuple::EDITOR*/true){
				VOFOG_CORE_WARN("Checking {} bodies", thing.size());
				for(int i = 0; i < thing.size(); i++){
					auto phys = this->getComponent<PhysicsComponent>(thing[i]);
					auto body = phys->body;
					if(!body)continue;
					auto transform = getComponent<TransformComponent>(thing[i]);
					body->forceActivationState(1);
					btVector3 position1 = btVector3(transform->position.x, transform->position.y, transform->position.z);
					btVector3 position2 = body->getWorldTransform().getOrigin();
					/*if(position1 == position2){
						count++;
						continue;
					}*/
					auto motion = getComponent<PhysicsComponent>(thing[i])->motion;
					btTransform initialTransform;
					initialTransform.setIdentity();
					glm::vec3 rot(glm::radians(transform->rotation.x), glm::radians(transform->rotation.y), glm::radians(transform->rotation.z));
					initialTransform.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
					initialTransform.setRotation(btQuaternion(rot.y, rot.x, rot.z));
					body->setWorldTransform(initialTransform);
					body->getCollisionShape()->setLocalScaling(btVector3(transform->size.x, transform->size.y, transform->size.z ));
					body->clearForces();
					body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
					body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
					body->activate(true);
					_world->updateSingleAabb((btCollisionObject*)body.get());
				}
			}
			//world->stepSimulation(1.0f / 30.0f);
			_transitionedStates = false;
			VOFOG_CORE_LOG("{} body transformation operations saved", count);
		}
		if(_mode == EngineStateTuple::APPLICATION)
			for(int i = 0; i < _applicationSystems.size(); i++)
				_applicationSystems[i]->update(*this);
		if(_mode == EngineStateTuple::EDITOR)
			for(int i = 0; i < _editorSystems.size(); i++)
				_editorSystems[i]->update(*this);

	}

	void ECS::EntityManager::clearScriptedEntities() {
		for (int i = 0; i < _scriptIds.size(); i++) {
			removeEntity(_scriptIds[i]);
		}
		_scriptIds.clear();
	}


	void ECS::EntityManager::setState(EngineState state){
		VOFOG_ASSERT(state == 0 || state == 1);
		static std::vector<ECS::TransformComponent> transforms;

		//Application ends -> goes back to editor
		if(state == EngineStateTuple::EDITOR && _mode == EngineStateTuple::APPLICATION){
			VOFOG_CORE_DEBUG("App");
			auto transformEntities = getAssociatedEntities<ECS::TransformComponent>();
			for(int i = 0; i < transformEntities.size(); i++){
				auto comp = getComponent<ECS::TransformComponent>(transformEntities[i]);
				comp->position = comp->editorPosition;
				comp->rotation = comp->editorRotation;
			}
			_transitionedStates = true;
			//delete scripted entities
			clearScriptedEntities();
		}

		//Runs application, leaving editor
		else if(state == EngineStateTuple::APPLICATION && _mode == EngineStateTuple::EDITOR){
			VOFOG_CORE_DEBUG("Edit");
			auto transformEntities = getAssociatedEntities<ECS::TransformComponent>();
			for(int i = 0; i < transformEntities.size(); i++){
				auto comp = getComponent<ECS::TransformComponent>(transformEntities[i]);
				comp->editorPosition = comp->position;
				comp->editorRotation = comp->rotation;

			}

			_transitionedStates = true;


			
		}
		

		_mode = state;


	}


	void ECS::EntityManager::removeComponent(EntityID entity, ComponentID id){
		_data[entity].erase(id);
	}



	/* ========== LUA Methods ========== */
	ECS::Entity ECS::EntityManager::_LUA_addEntity() {
		auto id = addEntity();
		_scriptIds.push_back(id);
		return *_entities[id];
	}
	ECS::Entity ECS::EntityManager::_LUA_addRenderableEntity(float x, float y, float z) {
		auto id = addEntity();
		ECS::EntityManager::attachComponent<TransformComponent>(id, TransformComponent(glm::vec3(x,y,z), glm::vec3(1,1,1), glm::vec3(0)));
		ECS::EntityManager::attachComponent<ECS::MeshRendererComponent>(id, MeshRendererComponent());
		auto type = MeshTypeComponent(); type.meshType = MeshType::CUBE;
		ECS::EntityManager::attachComponent<ECS::MeshTypeComponent>(id, type);
		auto col = ECS::RenderableComponent(); col.color = glm::vec4((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 1);
		ECS::EntityManager::attachComponent<ECS::RenderableComponent>(id, col);
		_scriptIds.push_back(id);
		return *_entities[id];
	}
	/*ECS::Entity ECS::EntityManager::_LUA_addEntity(std::string name) {
		return ECS::Entity();

	}*/

}