#include "vpch.h"
#include "SceneManager.h"

namespace Vofog {



	void SceneManager::onEvent(){ _currentScene->onEvent(); }



	void SceneManager::render() {
		_currentScene->render();

	}

	void SceneManager::renderEditorTools(){
	
	
	}

	void SceneManager::update(){
		//swap scene call
		if(_nextScene != _currentScene){
			_currentScene->unload();
			_currentScene = _nextScene;
			_currentScene->onAttach();
		}
		_currentScene->update();
	}



	void SceneManager::addScene(Scene newScene) {
		_scenes.emplace_back(std::shared_ptr<Scene>(&newScene));
		if(!_currentScene) _currentScene = _scenes[0];

	}
	void SceneManager::addScene(const Scene& newScene) {
		_scenes.emplace_back(std::make_shared<Scene>(newScene));
		if(!_currentScene) _currentScene = _scenes[0];


	}
	void SceneManager::addScene(std::unique_ptr<Scene>& newScene) {
		_scenes.push_back(std::move(newScene));
		if(!_currentScene) _currentScene = _scenes[0];

	}
	void SceneManager::addScene(std::shared_ptr<Scene>& newScene){
		_scenes.push_back(newScene);
		if(!_currentScene){ _currentScene = _scenes[0]; _nextScene = _currentScene; }
	}
	

	void SceneManager::setScene(const Scene& newScene) {
		bool newSceneAttached = false;
		for (int i = 0; i < _scenes.size(); i++) {
			if (newScene.getName() == _scenes[i]->getName()) {
				_nextScene = _scenes[i];
				newSceneAttached = true;
				break;
			}
		}
		#ifdef VOFOG_DEBUG_BUILD
		if (!newSceneAttached)
			VOFOG_CORE_WARN("Couldn't swap to scene {}", newScene.getName());
		#endif
	}
	void SceneManager::setScene(const std::shared_ptr<Scene>& newScene) {
		if (newScene == _currentScene) return;
		bool newSceneAttached = false;
		for (int i = 0; i < _scenes.size(); i++) {
			if (newScene->getName() == _scenes[i]->getName()) {
				_nextScene = _scenes[i];
				newSceneAttached = true;
				break;
			}
		}
		#ifdef VOFOG_DEBUG_BUILD
		if (!newSceneAttached)
			VOFOG_CORE_WARN("Couldn't swap to scene {}", newScene->getName());
		#endif
	}
	
	void SceneManager::setScene(std::string newSceneName) {
		bool newSceneAttached = false;
		
		for (int i = 0; i < _scenes.size(); i++) {
			if (newSceneName == _scenes[i]->getName()) {
				_nextScene = _scenes[i];
				newSceneAttached = true;
				break;
			}
		}
		#ifdef VOFOG_DEBUG_BUILD
		if (!newSceneAttached)
			VOFOG_CORE_WARN("Couldn't swap to scene {}", newSceneName);
		#endif
	}


	void SceneManager::onDetach(){
		for(int i = 0; i < _scenes.size(); i++)
			_scenes[i]->onDetach();
		
	
	}



}