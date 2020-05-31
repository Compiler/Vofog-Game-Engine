#pragma once

#include <Vofog.h>
#include <tools/Logging.h>
char* vfg_open_file(std::string filename = "Vofog project", std::string extensions = "(*.vfgp)\0*.vofog\0");
char* vfg_save_file(std::string filename = "Vofog project", std::string extensions = "(*.vfgp)\0*.vofog\0");

char* vfg_open_text_file(std::string filename = "Vofog project", std::string extensions = "(*.vfgp)\0*.vofog\0");
char* vfg_save_text_file(std::string filename = "Vofog project", std::string extensions = "(*.vfgp)\0*.vofog\0");
#ifdef VFG_WIN
/*for opening file*/
#include <windows.h>
#include <Commdlg.h>


std::string vfg_get_executable_path (){
	char result[MAX_PATH];
	return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}


char* vfg_save_file(std::string filename, std::string extensions){
	char* modelFileName;
	char bufFileName[MAX_PATH * 2] = "";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	//ofn.lpstrFilter = (LPCSTR)TEXT(std::string(filename + extensions + " All files (*.*)\0*.*\0").c_str());
	ofn.lpstrFilter = (LPCSTR)TEXT("Vofog project (*.vfg)\0*.vofog\0 All files (*.*)\0*.*\0");
	ofn.lpstrFile = (LPSTR)bufFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCSTR)TEXT("Choose Vofog Project file");
	ofn.Flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)TEXT("*.vfg");

	if(GetSaveFileName(&ofn)){
		//delete_zero_symbols(bufFileName);
		modelFileName = (char*)calloc(strlen(bufFileName) + 1, sizeof(char));
		if(modelFileName != 0)
			strcpy(modelFileName, bufFileName);
		else
			VOFOG_CORE_ERROR("MFN: {} = 0", modelFileName);
		return modelFileName;
	} else{
		VOFOG_CORE_ERROR("Error");
		return NULL;
	}
}

char* vfg_open_file(std::string filename, std::string extensions){
	char* modelFileName;
	char bufFileName[MAX_PATH * 2] = "";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	//ofn.lpstrFilter = (LPCSTR)TEXT(std::string(filename + extensions + " All files (*.*)\0*.*\0").c_str());
	ofn.lpstrFilter = (LPCSTR)TEXT("Vofog project (*.vfg)\0*.vofog\0 All files (*.*)\0*.*\0");
	ofn.lpstrFile = (LPSTR)bufFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCSTR)TEXT("Choose Vofog Project file");
	ofn.Flags = OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)TEXT("*.vfg");

	if(GetOpenFileName(&ofn)){
		//delete_zero_symbols(bufFileName);
		modelFileName = (char*)calloc(strlen(bufFileName) + 1, sizeof(char));
		if(modelFileName != 0)
			strcpy(modelFileName, bufFileName);
		else
			VOFOG_CORE_ERROR("SceneFileName: {} = 0", modelFileName);
		return modelFileName;
	} else{
		VOFOG_CORE_ERROR("Error");
		return NULL;
	}
}

char* vfg_save_text_file(std::string filename, std::string extensions){
	char* modelFileName;
	char bufFileName[MAX_PATH * 2] = "";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	//ofn.lpstrFilter = (LPCSTR)TEXT(std::string(filename + extensions + " All files (*.*)\0*.*\0").c_str());
	ofn.lpstrFilter = (LPCSTR)TEXT("Scripting File(*.lua)\0*.lua\0 All files (*.*)\0*.*\0");
	ofn.lpstrFile = (LPSTR)bufFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCSTR)TEXT("Choose Vofog Project file");
	ofn.Flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)TEXT("*.lua");

	if(GetSaveFileName(&ofn)){
		//delete_zero_symbols(bufFileName);
		modelFileName = (char*)calloc(strlen(bufFileName) + 1, sizeof(char));
		if(modelFileName != 0)
			strcpy(modelFileName, bufFileName);
		else
			VOFOG_CORE_ERROR("LuaFileName: {} = 0", modelFileName);
		return modelFileName;
	} else{
		VOFOG_CORE_ERROR("Error");
		return NULL;
	}
}

char* vfg_open_text_file(std::string filename, std::string extensions){
	char* modelFileName;
	char bufFileName[MAX_PATH * 2] = "";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	//ofn.lpstrFilter = (LPCSTR)TEXT(std::string(filename + extensions + " All files (*.*)\0*.*\0").c_str());
	ofn.lpstrFilter = (LPCSTR)TEXT("Scripting File(*.lua)\0*.txt\0 All files (*.*)\0*.*\0");
	ofn.lpstrFile = (LPSTR)bufFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCSTR)TEXT("Choose Vofog Project file");
	ofn.Flags = OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)TEXT("*.lua");

	if(GetOpenFileName(&ofn)){
		//delete_zero_symbols(bufFileName);
		modelFileName = (char*)calloc(strlen(bufFileName) + 1, sizeof(char));
		if(modelFileName != 0)
			strcpy(modelFileName, bufFileName);
		else
			VOFOG_CORE_ERROR("LuaFileName: {} = 0", modelFileName);
		return modelFileName;
	} else{
		VOFOG_CORE_ERROR("Error");
		return NULL;
	}
}
#endif