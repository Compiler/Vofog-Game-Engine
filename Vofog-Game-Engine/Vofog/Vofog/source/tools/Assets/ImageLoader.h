#pragma once
#include <Vofog.h>


namespace Vofog{
	class VOFOG_API ImageLoader{
	public:
		ImageLoader() = delete;

		static unsigned char* loadImage(const char name[], int* width, int* height, int* numOfColChannels);
		static void free(void* data);
	};


}



