#include "vpch.h"
#include "ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <external code/stbimage/stb_image.h>


unsigned char * Vofog::ImageLoader::loadImage(const char name[], int* width, int* height, int* numOfColChannels){
	stbi_set_flip_vertically_on_load(true);
	return stbi_load(name, width, height, numOfColChannels, 0);
}

void Vofog::ImageLoader::free(void* data){ stbi_image_free(data); }

