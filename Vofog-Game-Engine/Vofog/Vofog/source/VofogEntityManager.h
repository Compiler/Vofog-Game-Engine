#pragma once
#include <Vofog.h>
#include <VofogEntity.h>
#include <vector>


extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


}



	class VofogEntity;
class VOFOG_API VofogEntityManager{
private:
	static int COUNT;
	std::vector<VofogEntity*> _entities;
public:

	void registerEntity(VofogEntity* entity);
	void freeEntity(VofogEntity* entity);
	int getCount() { return COUNT; }
};

