#pragma once


#include <vector>
#include <unordered_map>
#include <Vofog.h>
#include <tools/Events/Event.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

#include <imgui/imgui.h>
#define DEFINE_COMPONENT(component_name) \
	struct VOFOG_API component_name : public Vofog::ECS::Component { \
		virtual uint32_t getID(){return hash_name(#component_name);}\

namespace Vofog{
	
	namespace ECS{

		// djb2 hashing algorithm
		extern uint32_t hash_name(const char* str);

		template<typename T>
		struct type{ static void id(){} };

		template<typename T>
		size_t type_id(){ return reinterpret_cast<size_t>(&type<T>::id); }
		#define MAX_COMPONENTS 64

		class VOFOG_API Component{

		public:
			static int count;

			virtual void _display_gui_information_(){}
			virtual ~Component(){}
			virtual uint32_t getID(){ return -1; }

			template<class Archive>
			void save(Archive& ar)const{
				int thing = 0; ar(thing);
			}

			template<class Archive>
			void load(Archive& ar){
				int thing = 0; ar(thing);
			}
		};

		

		

	}

	

}