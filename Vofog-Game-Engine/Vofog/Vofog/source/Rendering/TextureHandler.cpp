#include <vpch.h>
#include "TextureHandler.h"
namespace Vofog {
	std::unordered_map<std::string, std::shared_ptr<Vofog::Texture>> TextureHandler::_textureSlots;//DO NOT WRITE TO THIS 
	std::vector<std::string> ScriptHandler::scripts;
	std::vector<std::string> TextureHandler::imagePaths;
	std::vector<std::string> TextureHandler::imageReadableNames;

}