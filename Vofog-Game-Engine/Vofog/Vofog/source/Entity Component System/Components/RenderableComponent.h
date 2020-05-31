#pragma once
#include <Vofog.h>

#include <Entity Component System/Components/Component.h>
#include <Rendering/TextureHandler.h>
#include <Rendering/Renderables/Texture.h>
namespace Vofog{
	namespace ECS{

		namespace MeshType{
			unsigned short constexpr CUBE = 0;
			unsigned short constexpr PYRAMID = 1;
			unsigned short constexpr PLANE = 2;
			unsigned short constexpr SQUARE = 3;
			unsigned short constexpr TRIANGLE = 4;
			unsigned short constexpr CIRCLE = 5;
		}
		DEFINE_COMPONENT(MeshTypeComponent)
			unsigned short meshType = MeshType::CUBE;
			void _display_gui_information_()override{
				count++;
				if(ImGui::CollapsingHeader("MeshType Componenet", ImGuiTreeNodeFlags_DefaultOpen)){
					/*static ImVec2 spacing(0, 10);
					ImGui::Dummy(spacing);*/
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Cube", meshType == MeshType::CUBE))meshType = MeshType::CUBE;
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Pyramid", meshType == MeshType::PYRAMID))meshType = MeshType::PYRAMID;
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Plane", meshType == MeshType::PLANE))meshType = MeshType::PLANE;
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Square", meshType == MeshType::SQUARE))meshType = MeshType::SQUARE;
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Triangle", meshType == MeshType::TRIANGLE))meshType = MeshType::TRIANGLE;
					ImGui::AlignTextToFramePadding(); if(ImGui::Selectable("Cirlce", meshType == MeshType::CIRCLE))meshType = MeshType::CIRCLE;
				}
			}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), meshType);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), meshType);
			}
		};

		DEFINE_COMPONENT(MeshRendererComponent)
			bool castShadows;
			bool specularHighlight;

			void _display_gui_information_()override{
				count++;
				if(ImGui::CollapsingHeader("MeshRenderer Componenet", ImGuiTreeNodeFlags_DefaultOpen)){
					ImGui::Text("Component Information");
				}
			}


			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), castShadows, specularHighlight);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), castShadows, specularHighlight);
			}

		};



		DEFINE_COMPONENT(SpriteComponent)
			glm::vec4 textureIDs;

			//std::shared_ptr<Texture> textureID1;
			//std::shared_ptr<Texture> textureID2;
			//std::shared_ptr<Texture> textureID3;
			//std::shared_ptr<Texture> textureID4;
		void _display_gui_information_()override {
			count++;
			if (ImGui::CollapsingHeader("Sprite Componenet", ImGuiTreeNodeFlags_DefaultOpen)) {
				static float width;
				static ImVec2 dims;
				width = ImGui::GetContentRegionAvailWidth();
				dims = ImVec2(width / 5.0f, width / 5.0f);
				float spacing = (width * 0.2f) / 4.0;

				static std::vector<std::string>& imagePaths = TextureHandler::imagePaths;
				static std::vector<std::string>& imageReadableNames = TextureHandler::imageReadableNames;

				ImGui::Dummy(ImVec2(10, 10));
				//ImGui::Text("First"); ImGui::SameLine(dims.x / 2.0f); ImGui::Text("Second"); //ImGui::SameLine();
				ImGui::Image((void*)(intptr_t)textureIDs.x, dims); ImGui::SameLine(0, spacing);
				ImGui::Image((void*)(intptr_t)textureIDs.y, dims); ImGui::SameLine(0, spacing);
				ImGui::Image((void*)(intptr_t)textureIDs.z, dims); ImGui::SameLine(0, spacing);
				ImGui::Image((void*)(intptr_t)textureIDs.w, dims);
				ImGui::PushItemWidth(dims.x);
				
				if (ImGui::BeginCombo("##SpriteComponentImage1", imageReadableNames[textureIDs.x].c_str())) {
					for (int i = 0; i < imageReadableNames.size(); i++) {
						bool selected = (i == textureIDs.x);
						if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
							textureIDs.x = i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine(0, spacing);
				if (ImGui::BeginCombo("##SpriteComponentImage2", imageReadableNames[textureIDs.y].c_str())) {
					for (int i = 0; i < imageReadableNames.size(); i++) {
						bool selected = (i == textureIDs.y);
						if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
							textureIDs.y = i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine(0, spacing);
				if (ImGui::BeginCombo("##SpriteComponentImage3", imageReadableNames[textureIDs.z].c_str())) {
					for (int i = 0; i < imageReadableNames.size(); i++) {
						bool selected = (i == textureIDs.z);
						if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
							textureIDs.z = i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine(0, spacing);
				if (ImGui::BeginCombo("##SpriteComponentImage4", imageReadableNames[textureIDs.w].c_str())) {
					for (int i = 0; i < imageReadableNames.size(); i++) {
						bool selected = (i == textureIDs.w);
						if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
							textureIDs.w = i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
				ImGui::Dummy(ImVec2(10, 10));



				//ImGui::Text("First"); ImGui::SameLine(dims.x / 2.0f); ImGui::Text("Second"); //ImGui::SameLine();
				//ImGui::Image((void*)(intptr_t)textureIDs.x, dims); ImGui::SameLine(0, spacing);
				//ImGui::Image((void*)(intptr_t)textureIDs.y, dims); ImGui::SameLine(0, spacing);
				//ImGui::Image((void*)(intptr_t)textureIDs.z, dims); ImGui::SameLine(0, spacing);
				//ImGui::Image((void*)(intptr_t)textureIDs.w, dims);

				//static std::vector<std::string>& imagePaths = TextureHandler::imagePaths;
				//static std::vector<std::string>& imageReadableNames = TextureHandler::imageReadableNames;
				//for (int n = 0; n < imagePaths.size(); n++) {
				//	char buf[64];
				//	sprintf(buf, imagePaths[n].c_str(), n);
				//	ImGui::PushItemWidth(dims.x);
				//									if (ImGui::Selectable((imageReadableNames[n] + "##xImageSelection").c_str(), textureIDs.x == n + 1, 0, ImVec2(dims.x, 0)))textureIDs.x = n + 1;
				//	ImGui::SameLine(0, spacing);	if (ImGui::Selectable((imageReadableNames[n] + "##yImageSelection").c_str(), textureIDs.y == n + 1, 0, ImVec2(dims.x, 0)))textureIDs.y = n + 1;
				//	ImGui::SameLine(0, spacing);	if (ImGui::Selectable((imageReadableNames[n] + "##zImageSelection").c_str(), textureIDs.z == n + 1, 0, ImVec2(dims.x, 0)))textureIDs.z = n + 1;
				//	ImGui::SameLine(0, spacing);	if (ImGui::Selectable((imageReadableNames[n] + "##wImageSelection").c_str(), textureIDs.w == n + 1, 0, ImVec2(dims.x, 0)))textureIDs.w = n + 1;
				//	ImGui::PopItemWidth();
				//}



			}
		}


			template<class Archive>
			void save(Archive& ar)const{
				//ar(cereal::base_class<ECS::Component>(this), textureID1, textureID2, textureID3, textureID4);
				ar(cereal::base_class<ECS::Component>(this), textureIDs.x, textureIDs.y, textureIDs.z, textureIDs.w);
			}

			template<class Archive>
			void load(Archive& ar){
				//ar(cereal::base_class<ECS::Component>(this), textureID1, textureID2, textureID3, textureID4);
				ar(cereal::base_class<ECS::Component>(this), textureIDs.x, textureIDs.y, textureIDs.z, textureIDs.w);
			}

		};
		DEFINE_COMPONENT(RenderableComponent)
			glm::vec4 color;
			bool changed = true;
			
			void _display_gui_information_()override{
				count++;
				if(ImGui::CollapsingHeader("Renderable Componenet", ImGuiTreeNodeFlags_DefaultOpen)){
					static float width, height;
					width = ImGui::GetContentRegionAvail().x / 12.0f;
					height = ImGui::GetContentRegionAvail().y / 16.0f;
					ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float;
					
					flags |= ImGuiColorEditFlags_NoInputs;
					flags |= ImGuiColorEditFlags_NoLabel;
					//flags |= ImGuiColorEditFlags_PickerHueWheel;
					flags |= ImGuiColorEditFlags_NoSidePreview;
					flags |= ImGuiColorEditFlags_AlphaBar;
					 
					static float itemwidth = 5.9;
					ImGui::SliderFloat("", &itemwidth, 0.1f, 20);
					float pickerSize = width * itemwidth;
					float sliderHeight = pickerSize * 0.75f;
					ImGui::PushItemWidth(pickerSize);
					ImGui::ColorPicker4("Render Color", &color.x, flags); ImGui::SameLine();
					ImGui::PopItemWidth();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.9f, 0.9f));
					ImGui::VSliderFloat("##R",ImVec2(width, sliderHeight), &color.x, 0, 1); ImGui::SameLine();
					ImGui::PopStyleColor();ImGui::PopStyleColor();ImGui::PopStyleColor();ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(2 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(2 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.9f, 0.9f));
					ImGui::VSliderFloat("##G",ImVec2(width, sliderHeight), &color.y, 0, 1);   ImGui::SameLine();
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(4 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(4 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.9f, 0.9f));

					ImGui::VSliderFloat("##B",ImVec2(width, sliderHeight), &color.z, 0, 1);
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					
				}
			}
			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), color.x, color.y, color.z, color.w, changed);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), color.x, color.y, color.z, color.w, changed);
			}
		};


		DEFINE_COMPONENT(PointLightComponent)
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 lightProperties;


			void _display_gui_information_()override {
				count++;
				if (ImGui::CollapsingHeader("Point Light Componenet", ImGuiTreeNodeFlags_DefaultOpen)) {


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
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.3, 0.1, 0.1, 1));
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


					static float width, height;
					width = ImGui::GetContentRegionAvail().x / 12.0f;
					height = ImGui::GetContentRegionAvail().y / 16.0f;
					ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float;

					flags |= ImGuiColorEditFlags_NoInputs;
					flags |= ImGuiColorEditFlags_NoLabel;
					//flags |= ImGuiColorEditFlags_PickerHueWheel;
					flags |= ImGuiColorEditFlags_NoSidePreview;
					flags |= ImGuiColorEditFlags_AlphaBar;

					static float itemwidth = 5.9;
					ImGui::SliderFloat("", &itemwidth, 0.1f, 20);
					float pickerSize = width * itemwidth;
					float sliderHeight = pickerSize * 0.75f;
					ImGui::PushItemWidth(pickerSize);
					ImGui::ColorPicker4("Render Color", &color.x, flags); ImGui::SameLine();
					ImGui::PopItemWidth();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.9f, 0.9f));
					ImGui::VSliderFloat("##R", ImVec2(width, sliderHeight), &color.x, 0, 1); ImGui::SameLine();
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(2 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(2 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.9f, 0.9f));
					ImGui::VSliderFloat("##G", ImVec2(width, sliderHeight), &color.y, 0, 1);   ImGui::SameLine();
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(4 / 7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(4 / 7.0f, 0.9f, 0.9f));
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.9f, 0.9f));

					ImGui::VSliderFloat("##B", ImVec2(width, sliderHeight), &color.z, 0, 1);
					ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
				}
			}

			template<class Archive>
			void save(Archive& ar)const {
				ar(cereal::base_class<ECS::Component>(this), position.x, position.y, position.z, color.x, color.y, color.z, lightProperties.x, lightProperties.y, lightProperties.z);
			}

			template<class Archive>
			void load(Archive& ar) {
				ar(cereal::base_class<ECS::Component>(this), position.x, position.y, position.z, color.x, color.y, color.z, lightProperties.x, lightProperties.y, lightProperties.z);
			}
		};
	}

}
CEREAL_REGISTER_TYPE(Vofog::ECS::MeshRendererComponent); CEREAL_REGISTER_TYPE(Vofog::ECS::RenderableComponent);
CEREAL_REGISTER_TYPE(Vofog::ECS::SpriteComponent); CEREAL_REGISTER_TYPE(Vofog::ECS::MeshTypeComponent);
CEREAL_REGISTER_TYPE(Vofog::ECS::PointLightComponent);