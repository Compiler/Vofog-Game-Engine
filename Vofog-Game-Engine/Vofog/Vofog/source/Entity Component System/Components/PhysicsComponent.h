#pragma once
#include <Vofog.h>
#include <Entity Component System/Components/Component.h>
#include <glm/vec3.hpp>
#include "btBulletDynamicsCommon.h"
#include <imgui/imgui.h>


namespace Vofog{
	
	namespace ECS{
		

		DEFINE_COMPONENT(PhysicsComponent)
			std::shared_ptr<btRigidBody> body;
			std::shared_ptr<btMotionState> motion;
			float mass = 0;

			std::shared_ptr<btRigidBody> editor_planeBody;
			std::shared_ptr<btStaticPlaneShape> editor_planeShape;


			std::shared_ptr<btRigidBody> editor_planeAxisX;
			std::shared_ptr<btRigidBody> editor_planeAxisY;
			std::shared_ptr<btRigidBody> editor_planeAxisZ;


			void _display_gui_information_()override{
				if(ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen)){
					ImGui::Text(std::to_string(body->getActivationState()).c_str());
					float currentMass = body->getMass();
					static float currentWidth = ImGui::GetContentRegionAvailWidth();
					static float currentHeight = ImGui::GetContentRegionAvail().y;
					ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(currentWidth * 0.025, currentHeight * 0.0025));
					static ImVec2 spacing(0, 10);
					float mag = 6.25f;
					ImGui::Dummy(spacing);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Mass");
					float progression = currentWidth / 5.0f;
					float subCompCount = 1.0f;
					ImGui::SameLine(progression * subCompCount++); ImGui::Text("kg."); ImGui::SameLine(0, 5); ImGui::SetNextItemWidth(currentWidth / mag);
					ImGui::AlignTextToFramePadding(); ImGui::InputScalar("##MassScalar", ImGuiDataType_::ImGuiDataType_Float, &mass, 0, 0, "%.2f");
					ImGui::PopStyleVar();


					static glm::vec3 vals = glm::vec3(body->getTotalForce().getX(), body->getTotalForce().getY(), body->getTotalForce().getZ());
					if (body->getTotalForce().getX() != 0) vals.x = body->getTotalForce().getX();
					std::string thingx = "Forcex: " + std::to_string(vals.x);
					ImGui::Text(thingx.c_str());
					ImGui::Text(std::string("Forcey: " + std::to_string(body->getTotalForce().getY())).c_str());
					ImGui::Text(std::string("Forcez: " + std::to_string(body->getTotalForce().getZ())).c_str());
					if (currentMass != mass) {
						currentMass = mass;
						body->setMassProps(mass, btVector3(0,0,0));
					}
				}
			}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<ECS::Component>(this), mass);
				VOFOG_CORE_WARN("Saved with mass: {}", mass);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<ECS::Component>(this), mass);
				VOFOG_CORE_WARN("Load with mass: {}", mass);

			}


		};

	
	}

}CEREAL_REGISTER_TYPE(Vofog::ECS::PhysicsComponent);