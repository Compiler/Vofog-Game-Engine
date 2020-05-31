#include "vpch.h"
#include "VofogEntityManager.h"

int VofogEntityManager::COUNT = 0;

void VofogEntityManager::registerEntity(VofogEntity* entity) {
	COUNT++;
}

void VofogEntityManager::freeEntity(VofogEntity* entity) {
	COUNT--;
}