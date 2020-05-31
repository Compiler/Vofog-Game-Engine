#pragma once
#include <Vofog.h>
#include <string>
#include <vector>
#include <memory>

#include <Rendering/Renderables/Cube.h>
#include <Rendering/Renderables/Texture.h>
#include <Rendering/Renderables/InternalTexture.h>

#include <tools/Layers/Layer.h>
#include <Rendering/Window.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Entity.h>
#include <Entity Component System/EntityManager.h>

namespace Vofog {
	class VOFOG_API Scene {

	protected:
		std::string m_sceneName;

		std::vector<std::shared_ptr<Vofog::Layer>> m_sceneLayers;

		Vofog::InternalTexture m_frameTexture;
		unsigned int m_frameBuffer;
		unsigned int m_frameTextureID;

		bool m_sceneLoaded = false;
		static int COUNT;
	public:
		friend class VofogCore;

		Scene();
		Scene(std::string sceneName);
		Scene(const Scene& old);

		//can only run if the scene hasnt been loaded or the flag for loaded scene has been reset via resetScene
		virtual void initialize();

		virtual void onAttach();
		virtual void update();
		virtual void render();
		virtual void onLoad();
		virtual void onEvent();
		virtual void unload(){}
		virtual void onDetach(){ 
			//for(int i = 0; i < m_sceneLayers.size(); i++) m_sceneLayers[i]->onDetach();
		
		}


		inline const std::string& getName()const { return m_sceneName; }
		inline const bool sceneLoaded() const { return m_sceneLoaded; }
		virtual void resetScene() {m_sceneLoaded = false;}

		
		void bindFrame() { glBindTextureUnit(m_frameTextureID, m_frameTexture.getTextureID()); }
		int getFrameTextureID() const { return m_frameTextureID; }
		int getFrameTexture() const { return m_frameTexture.getTextureID(); }
		

		
		
		
		template<class Archive>
		void save(Archive& ar)const{
			VOFOG_CORE_LOG("Save: Frame tex attribs: ({}, {})", m_frameTexture.getWidth(), m_frameTexture.getHeight());
			ar(m_sceneName, m_frameTexture, m_sceneLayers, m_frameTextureID);
			
		}

		template<class Archive>
		void load(Archive& ar){
			std::string handle;//nullptr to current selected cube
			VOFOG_CORE_LOG("Load: Frame tex attribs: ({}, {})", m_frameTexture.getWidth(), m_frameTexture.getHeight());
			ar(m_sceneName, m_frameTexture, m_sceneLayers, m_frameTextureID);
			onLoad();
		}


		
	};

}