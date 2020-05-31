#pragma once

#include <Vofog.h>
#include <tools/Scenes/Scene.h>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>



namespace Vofog {
	class VOFOG_API SceneManager{
		
	private:
		//no need for unordered_map cuz not many scenes ~<10000 99% of the time
		std::vector<std::shared_ptr<Scene>> _scenes;

		std::shared_ptr<Scene> _currentScene;
		std::shared_ptr<Scene> _nextScene;


	public:
		SceneManager() {  }

		void addScene(Scene newScene);
		void addScene(const Scene& newScene);
		void addScene(std::unique_ptr<Scene>& newScene);
		void addScene(std::shared_ptr<Scene>& newScene);

		void render();
		void renderEditorTools();
		void update();

		void onEvent();
		void onDetach();
		
		void setScene(const Scene& newScene);
		void setScene(const std::shared_ptr<Scene>& newScene);
		void setScene(std::string newSceneName);

		const std::shared_ptr<Scene>& getCurrentScene()const { return _currentScene; }
		std::pair<std::vector<std::shared_ptr<Scene>>::iterator,
			std::vector<std::shared_ptr<Scene>>::iterator> scenes()
		{
			return std::make_pair(_scenes.begin(), _scenes.end());
		}


		template<class Archive>
		void save(Archive& ar)const{
			ar(_currentScene, _nextScene, _scenes);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(_currentScene, _nextScene, _scenes);
			update();
		}

	};
}

