#pragma once
#include <Vofog.h>
#include <Entity Component System/Components/Component.h>
#include <glm/vec3.hpp>
#include <stdint.h>
#include <stdio.h>


		

namespace Vofog{
	namespace ECS{
		

	

		DEFINE_COMPONENT(TransformComponent)
			glm::vec3 position = glm::vec3(0);
			glm::vec3 editorPosition = glm::vec3(0);
			
			glm::vec3 rotation = glm::vec3(0);
			glm::vec3 editorRotation = glm::vec3(0);
			
			glm::vec3 size = glm::vec3(1);
			TransformComponent() = default;
			TransformComponent(glm::vec3 pos, glm::vec3 sz, glm::vec3 rot):position(pos), size(sz), rotation(rot){}

			void _display_gui_information_()override{
				count++;
				if(ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen)){
					ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, 1);
					static float currentWidth = ImGui::GetContentRegionAvailWidth();
					static float currentHeight = ImGui::GetContentRegionAvail().y;
					ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(currentWidth * 0.025, currentHeight * 0.0025));
					static ImVec2 spacing(0, 10);
					static ImVec4 redCol(1, 0, 0, 1);
					static ImVec4 greCol(0, 1, 0, 1);
					static ImVec4 bluCol(0.5, 0.5, 1, 1);
					float mag = 6.25f;
					ImGui::Dummy(spacing);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Position");
					float progression = currentWidth / 5.0f;
					float subCompCount = 1.0f;
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.3,0.1,0.1,1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(redCol, "X"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::AlignTextToFramePadding(); ImGui::InputScalar("##XPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &position.x, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.3, 0.1, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(greCol, "Y"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::AlignTextToFramePadding(); ImGui::InputScalar("##YPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &position.y, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.3, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(bluCol, "Z"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::AlignTextToFramePadding(); ImGui::InputScalar("##ZPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &position.z, 0, 0, "%.2f");
					/*ImGui::InputFloat("Y", &position.y);
					ImGui::InputFloat("Z", &position.z);*/
					ImGui::Dummy(spacing);
					subCompCount = 1.0f;
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Size");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.3, 0.1, 0.1, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(redCol, "W"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##XSizeTransform", ImGuiDataType_::ImGuiDataType_Float, &size.x, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.3, 0.1, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(greCol, "H"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##YSizeTransform", ImGuiDataType_::ImGuiDataType_Float, &size.y, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.3, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(bluCol, "D"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##ZSizeTransform", ImGuiDataType_::ImGuiDataType_Float, &size.z, 0, 0, "%.2f");
					ImGui::Dummy(spacing);
					subCompCount = 1.0f;
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Rotation");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.3, 0.1, 0.1, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(redCol, "X"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##XRotationTransform", ImGuiDataType_::ImGuiDataType_Float, &rotation.x, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.3, 0.1, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(greCol, "Y"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##YRotationTransform", ImGuiDataType_::ImGuiDataType_Float, &rotation.y, 0, 0, "%.2f");
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.3, 1));
					ImGui::SameLine(progression * subCompCount++); ImGui::TextColored(bluCol, "Z"); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::InputScalar("##ZRotationTransform", ImGuiDataType_::ImGuiDataType_Float, &rotation.z, 0, 0, "%.2f");
					ImGui::Dummy(spacing);
					ImGui::PopStyleVar();
					ImGui::PopStyleVar();

					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
				}
			}


			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), position.x, editorPosition.x, rotation.x, editorRotation.x, size.x,
					position.y, editorPosition.y, rotation.y, editorRotation.y, size.y,
					position.z, editorPosition.z, rotation.z, editorRotation.z, size.z) ;
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), position.x, editorPosition.x, rotation.x, editorRotation.x, size.x,
					position.y, editorPosition.y, rotation.y, editorRotation.y, size.y,
					position.z, editorPosition.z, rotation.z, editorRotation.z, size.z);
			}
		}; 

		struct VOFOG_API MovementComponentData{
			int key;
			glm::vec3 movement;
			MovementComponentData(){}
			MovementComponentData(int keyy, glm::vec3 movementt):key(keyy), movement(movementt){}
			template<class Archive>
			void save(Archive& ar)const{
				ar(key, movement.x, movement.y, movement.z);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(key, movement.x, movement.y, movement.z);

			}
		};
		DEFINE_COMPONENT(MovementComponent)
			//key stroke -> direction/magnitude/force vector
			std::vector<MovementComponentData> movements;
			std::vector<bool> movementFlags;

			void _display_gui_information_()override{
				count++;
				static ImVec4 redCol(1, 0, 0, 1);
				static ImVec4 greCol(0, 1, 0, 1);
				static ImVec4 bluCol(0.5, 0.5, 1, 1);
				static ImVec2 spacing(0, 10);
				if(ImGui::CollapsingHeader("Movement Componenet", ImGuiTreeNodeFlags_DefaultOpen)){
					if(ImGui::BeginTabBar("##movecomponenttab", ImGuiTabBarFlags_None)){
						for(int i = 0; i < movements.size(); i++)
							if(ImGui::BeginTabItem(("Movement #" + std::to_string(i)).c_str())){
								static float currentWidth = ImGui::GetContentRegionAvailWidth();
								static float currentHeight = ImGui::GetContentRegionAvail().y;
								ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, 1);
								//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, 1);
								ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(currentWidth * 0.025, currentHeight * 0.0025));
								ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.4, 0.4, 0.4, 1));
								ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.3, 1));
								ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.1, 0.3, 0.1, 1));
								ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.3, 0.1, 0.1, 1));

								
								float progression = currentWidth / 5.0f;
								float mag = 6.25f;
								float subCompCount = 1.0f;
								ImGui::Dummy(spacing);

								ImGui::AlignTextToFramePadding();
								ImGui::Text("Direction"); ImGui::SameLine(progression * subCompCount++, mag);
								ImGui::TextColored(redCol, "X"); ImGui::SameLine(0, mag);
								ImGui::SetNextItemWidth(currentWidth / mag);
								ImGui::InputScalar("##XPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &movements[i].movement.x, 0, 0, "%.2f");
								ImGui::PopStyleColor();

								ImGui::SameLine(progression * subCompCount++, mag); ImGui::TextColored(greCol, "Y"); ImGui::SameLine(0, mag); ImGui::SetNextItemWidth(currentWidth / mag);
								ImGui::InputScalar("##YPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &movements[i].movement.y, 0, 0, "%.2f");
								ImGui::PopStyleColor();

								ImGui::SameLine(progression * subCompCount++, mag); ImGui::TextColored(bluCol, "Z"); ImGui::SameLine(0, mag); ImGui::SetNextItemWidth(currentWidth / mag);
								ImGui::InputScalar("##ZPositionTransform", ImGuiDataType_::ImGuiDataType_Float, &movements[i].movement.z, 0, 0, "%.2f");
								ImGui::PopStyleColor();

								ImGui::Dummy(spacing);
								subCompCount = 1;
							//	ImGui::Text(std::to_string(movements[i].first).c_str());
								ImGui::Text("Command");
								ImGui::SameLine(progression * subCompCount++); ImGui::Text("Key"); ImGui::SameLine(0, mag); ImGui::SetNextItemWidth(currentWidth / mag);
								static std::string keyData;
								keyData = movements[i].key + "##MovementKeyChar";
								static char buf[32] = {movements[i].key};
								ImGui::InputText(keyData.c_str(), buf, 32);
								ImGui::PopStyleColor();
								ImGui::Dummy(spacing);

								ImGui::EndTabItem();
								ImGui::PopStyleVar(); 
								ImGui::PopStyleVar(); 

							}
						ImGui::EndTabBar();
					}
					ImGui::Separator();
				}
			}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), movements, movementFlags);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), movements, movementFlags);
			}

			

		};
		
		
	}



}CEREAL_REGISTER_TYPE(Vofog::ECS::TransformComponent); CEREAL_REGISTER_TYPE(Vofog::ECS::MovementComponent);