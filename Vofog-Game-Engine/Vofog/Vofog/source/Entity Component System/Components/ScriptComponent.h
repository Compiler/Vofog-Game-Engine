#pragma once
#include <Vofog.h>
#include <Entity Component System/Components/Component.h>

namespace Vofog {
	namespace ECS {
		DEFINE_COMPONENT(ScriptComponent)
			std::string filepath = "none";

		void _display_gui_information_()override {
			count++;
			static std::string readableName;
			if(!filepath.empty() && filepath != "none")
				readableName = filepath.substr(filepath.find_last_of('/') + 1);
			if (ImGui::CollapsingHeader("Script Componenet", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Text(readableName.c_str());
			}
		}

		template<class Archive>
		void save(Archive& ar)const {
			ar(cereal::base_class<ECS::Component>(this), filepath);
		}

		template<class Archive>
		void load(Archive& ar) {
			ar(cereal::base_class<ECS::Component>(this), filepath);
		}
		};
	}
}
CEREAL_REGISTER_TYPE(Vofog::ECS::ScriptComponent);