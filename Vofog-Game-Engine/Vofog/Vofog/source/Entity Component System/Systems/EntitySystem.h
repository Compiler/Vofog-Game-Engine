#pragma once

#include <Vofog.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog{
	namespace ECS{

		class EntityManager;

		class VOFOG_API EntitySystem{

		public:
			EntitySystem(){}
			virtual void update(EntityManager& manager) = 0;

			template<class Archive>
			void save(Archive& ar)const{
				
			}

			template<class Archive>
			void load(Archive& ar){
				
			}

			virtual ~EntitySystem(){}
		};


		

		

	}
}