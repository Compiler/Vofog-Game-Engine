#pragma once
#include <Vofog.h>
#include <imgui/imgui.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tools/Logging.h>

namespace Vofog{

	class VOFOG_API ImGuiEngine{

	public:

		ImGuiEngine();


		/*static void EDIT_MODEL_POSITION(Cube* cubeToEdit){
			glm::vec3 currentPos = cubeToEdit->getPosition();
			std::string inputNameX = std::to_string(currentPos.x) + "##XPos" + cubeToEdit->getName();
			std::string inputNameY = std::to_string(currentPos.y) + "##YPos" + cubeToEdit->getName();
			std::string inputNameZ = std::to_string(currentPos.z) + "##ZPos" + cubeToEdit->getName();
			glm::vec2 range = glm::vec2(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
			float x = currentPos.x; float y = currentPos.y; float z = currentPos.z;
			if(ImGui::InputScalar(inputNameX.c_str(), ImGuiDataType_Float, &x, &range.x, &range.y, "%.4f", ImGuiInputTextFlags_EnterReturnsTrue)){
				ImGui::SameLine(); ImGui::Text("x");
			}
			if(ImGui::InputScalar(inputNameY.c_str(), ImGuiDataType_Float, &y, &range.x, &range.y, "%.4f", ImGuiInputTextFlags_EnterReturnsTrue)){
				ImGui::SameLine(); ImGui::Text("y");
			}
			if(ImGui::InputScalar(inputNameZ.c_str(), ImGuiDataType_Float, &z, &range.x, &range.y, "%.4f", ImGuiInputTextFlags_EnterReturnsTrue)){
				ImGui::Text("z");
			}

			currentPos.x = x; currentPos.y = y; currentPos.z = z;
			cubeToEdit->setPosition(currentPos);
		}*/

		static void IMGUI_ERROR(const char errorMsg[], bool* trigger, bool* hasClosed){
			if(*trigger){
				ImGui::OpenPopup("Vofog Error");
				if(ImGui::BeginPopupModal("Vofog Error"), 0, ImGuiWindowFlags_AlwaysAutoResize){
					ImGui::Text(errorMsg);

					if(ImGui::Button("Close")){
						ImGui::CloseCurrentPopup();
						*trigger = false;
						*hasClosed = true;
					}

					ImGui::EndPopup();
					
				}
			}

		}

		static void COLOR_TUNING(const char name[], glm::vec4 &data){
			COLOR_TUNING(name, glm::value_ptr(data));
		}
		static void COLOR_TUNING(const char name[], float* data){

			ImGui::Text(name);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 0.6, 0.6, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1, 0.3, 0.3, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(1.0f, 0.15, 0.15, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1, 0, 0, 1.25f));
			ImGui::SliderFloat("R", &data[0], 0, 1);
			ImGui::PopStyleColor(4);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.6, 1, 0.6, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3, 1, 0.3, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15, 1, 0.15, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0, 1, 0, 1.25f));
			ImGui::SliderFloat("G", &data[1], 0, 1);
			ImGui::PopStyleColor(4);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.6, 0.6, 1, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3, 0.3, 1, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15, 0.15, 1, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0, 0, 1, 1.25f));
			ImGui::SliderFloat("B", &data[2], 0, 1);
			ImGui::PopStyleColor(4);


			//ImGui::Text(name);
			ImGui::ColorEdit4("col", data, ImGuiColorEditFlags_PickerHueWheel);
		}
	};

}