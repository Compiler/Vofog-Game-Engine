#pragma once
#define CEREAL_THREAD_SAFE 1
/*Cereal*/
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>


#include <iostream>
#include <vector>
#include <string>

#include <memory>


/*Vofog includes*/
#include <tools/Logging.h>
#include <Vofog.h>

/*Platform specific*/
#ifdef VFG_WIN
#include <Windows.h>
#endif

/*GLM*/
#include <glm/glm.hpp>


/*Physics*/
#include "btBulletDynamicsCommon.h"
