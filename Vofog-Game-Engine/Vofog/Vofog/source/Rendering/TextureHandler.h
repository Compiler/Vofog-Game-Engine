#pragma once
#include <Vofog.h>
#include <vector>
#include <Rendering/Renderables/Texture.h>
#include <string>
#include <unordered_map>
#include <tools/Assets/TextLoader.h>
#include <filesystem>
namespace Vofog {



	struct VOFOG_API ScriptHandler{
	#define VOFOG_DEFAULT_SCRIPTING_PATH std::string(".//..//Vofog//resources//scripts//lua//")
		static std::vector<std::string> scripts;

		//TODO: only check the array if the files found are different
		static void populate_all_scripts(){
			for(const auto& entry : std::filesystem::directory_iterator(VOFOG_DEFAULT_SCRIPTING_PATH)){
				std::string fullPath = entry.path().string();
				bool found = false;
				for(int i = 0; i < scripts.size(); i++)if(scripts[i] == fullPath)found = true;
				if(!found)scripts.push_back(fullPath);
	
			}

		}
	};
	
	
	
	
	struct VOFOG_API TextureHandler {
#define VOFOG_DEFAULT_IMAGE_FILEPATH std::string(".//..//Vofog//resources//images//")

	private:
		static std::unordered_map<std::string, std::shared_ptr<Vofog::Texture>> _textureSlots;
	public:
		TextureHandler() = delete;
		TextureHandler(const TextureHandler&) = delete;
		TextureHandler operator=(const TextureHandler&) = delete;


		static std::vector<std::string> imagePaths;
		static std::vector<std::string> imageReadableNames;

		//TODO: only check the array if the files found are different
		static void populate_all_filepaths() {
			for (const auto& entry : std::filesystem::directory_iterator(VOFOG_DEFAULT_IMAGE_FILEPATH)) {
				std::string fullPath = entry.path().string();
				bool found = false;
				for (int i = 0; i < imagePaths.size(); i++)if (imagePaths[i] == fullPath)found = true;
				if (!found) {
					imagePaths.push_back(fullPath);
					imageReadableNames.push_back(fullPath.substr(fullPath.find_last_of('/') + 1));
				}
			}

		}


		static unsigned int getSize() { return _textureSlots.size(); }
		//do not call-- for startup code
		static void _init() {
			populate_all_filepaths();
			for (const auto& entry : std::filesystem::directory_iterator(VOFOG_DEFAULT_IMAGE_FILEPATH)) {
				std::string fullPath = entry.path().string();
				size_t fileTypePosition = fullPath.find_last_of('.') + 1;
				size_t nameStart = fullPath.find_last_of('/') + 1;

				std::string fileName = fullPath.substr(nameStart);
				std::string fileType = fullPath.substr(fileTypePosition);
				int type = GL_RGB;
				if (fileType == "png") {
					type = GL_RGBA;
				} else if (fileType == "jpg")
					type = GL_RGB;
				

				VOFOG_CORE_DEBUG("Sending in {} with {} type", fullPath, type == GL_RGBA ? "RGBA" : "RGB");
				_textureSlots[fullPath] = std::make_shared<Texture>((fullPath).c_str(), type, type);
			}
		
		}
		static std::shared_ptr<Texture> get(std::string handle) {
			std::unordered_map<std::string, std::shared_ptr<Texture>>::const_iterator foundVal = _textureSlots.find(handle);
			if (foundVal == _textureSlots.end()) {
				VOFOG_CORE_ERROR("Couldn't find {} in map", handle);
				return 0;
			} else {
				return _textureSlots[handle];//std::make_shared<Texture>(foundVal->first);
			}

		}

		static void bind(std::string handle) {
			std::unordered_map<std::string, std::shared_ptr<Texture>>::const_iterator foundVal = _textureSlots.find(handle);
			if (foundVal == _textureSlots.end()) {
				VOFOG_CORE_ERROR("Couldn't find {} in map", handle);
			} else {
				_textureSlots[handle]->bind();//std::make_shared<Texture>(foundVal->first);
			}

		}

		static bool doesExist(std::string handle){
			std::unordered_map<std::string, std::shared_ptr<Texture>>::const_iterator foundVal = _textureSlots.find(handle);
			if(foundVal == _textureSlots.end()){
				VOFOG_CORE_ERROR("Couldn't find {} in map", handle);
				return false;
			} else{
				return true;//std::make_shared<Texture>(foundVal->first);
			}

		}
#define VFG_NO_HANDLE_DEFAULT "VOFOG_NO_HANDLE_DEFAULT"
		static void add(std::string filepath, std::string handle = VFG_NO_HANDLE_DEFAULT, int internalFormat = GL_RGB, int outputFormat = GL_RGB,
			 unsigned int wrapTypeS = GL_REPEAT, unsigned int wrapTypeT = GL_REPEAT, unsigned int dataType = GL_UNSIGNED_BYTE) {
			if (handle == VFG_NO_HANDLE_DEFAULT)
				handle = filepath;
			auto foundVal = _textureSlots.find(handle);
			if (foundVal != _textureSlots.end()) {
				foundVal->second = std::make_shared<Texture>(filepath.c_str(), internalFormat, outputFormat, dataType, wrapTypeS, wrapTypeT);
				foundVal->second->setHandle(handle);
				VOFOG_CORE_LOG("Overwrite on {} texture T {}", handle, filepath);
			} else{
				_textureSlots[handle] = std::make_shared<Texture>(filepath.c_str(), internalFormat, outputFormat, dataType, wrapTypeS, wrapTypeT);
				_textureSlots[handle]->setHandle(handle);

			}



		}


	};
}

