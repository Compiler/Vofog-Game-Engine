#pragma once
#include <Vofog.h>
#include <tools/Layers/TestLayer.h>
#include <tools/Layers/Layer.h>
#include <string>
#include <vector>
#include <tools/Scenes/Scene.h>
#include <time.h>       /* time */

namespace Vofog{


	class VOFOG_API TestScene: public Scene{

		public:

			TestScene():Scene(){}
			TestScene(std::string sceneName):Scene(sceneName){}

			virtual void initialize(){

				Scene::initialize();
				
				std::shared_ptr<Layer> testLayer = std::make_shared<TestLayer>();
				m_sceneLayers.push_back(testLayer);
				m_sceneLayers[0]->init(0, 0);
				m_sceneLayers[0]->setName("Creation Name");


			}

			virtual void update(){
				Scene::update();


			}
			virtual void render(){
				Scene::render();

			}
			virtual void unload(){

				VOFOG_CORE_DEBUG("Scene: {}\tUnloading", m_sceneName);
				//cache some shit upon reload

			}

			virtual void onDetach(){
				
			
			}

			virtual void onEvent(){
				Scene::onEvent();

			}

			void onLoad()override{
			
			}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<Scene>(this));
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<Scene>(this));
				onLoad();

			}





	};


}
CEREAL_REGISTER_TYPE(Vofog::TestScene);

