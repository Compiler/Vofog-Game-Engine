#pragma once

#include <Vofog.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>
namespace Vofog{

	namespace ECS{
		typedef size_t EntityID;
		class VOFOG_API Entity{
		private:
			EntityID _ID;
			static size_t _ENTITY_COUNT;
			std::string _name;

		public:
			Entity():_ID(++_ENTITY_COUNT){
				_name = "Entity" + std::to_string(_ID);
			}

			Entity(std::string name):_ID(++_ENTITY_COUNT){
				_name = name;
			}
			
			
			void setName(std::string newName){
				_name = newName;
			}


			EntityID getID()const{ return _ID; }
			std::string getName()const{ return _name; }

			std::string toString() { return "Entity \"" + _name + "\" has ID: \'" + std::to_string(_ID) + "\'\n"; }
			template<class Archive>
			void save(Archive& ar)const{
				ar(_name, _ID);
				VOFOG_CORE_LOG("Saved entity with id: {}", _ID);
				VOFOG_CORE_LOG("Saved Entity Count: {}", _ENTITY_COUNT);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(_name, _ID);
				static int HIGHEST_COUNT_SEEN = 0;
				if(HIGHEST_COUNT_SEEN < _ID) HIGHEST_COUNT_SEEN = _ID;
				_ENTITY_COUNT = HIGHEST_COUNT_SEEN;
				VOFOG_CORE_LOG("Load entity with id: {}", _ID);
				VOFOG_CORE_LOG("Load Entity Count: {}", _ENTITY_COUNT);
			}
		};

	}

}