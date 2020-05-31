#include <vpch.h>
#include "Component.h"

uint32_t Vofog::ECS::hash_name(const char* str){
	unsigned long hash = 5381;
	int c;

	while((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}