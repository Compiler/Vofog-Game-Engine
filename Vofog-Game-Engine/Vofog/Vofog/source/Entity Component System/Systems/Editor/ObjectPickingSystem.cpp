#include <vpch.h>
#include "ObjectPickingSystem.h"

namespace Vofog{

	namespace ECS{

		void ObjectPickingSystem::update(EntityManager& manager){
			static bool mouseDragging = false;
			static bool mouseClicked = false;
			static bool entityGrabbed = false;
			static double xpos = 0;
			static double ypos = 0;
			static glm::vec3 grabSpot(0);
			static bool grab = false;
			if(EventManager::isMousePressed(VofogMouseKeys::VOFOG_MOUSE_BUTTON_LEFT)){
				auto _world = manager.getWorld();
				mouseDragging = true;

				glm::vec2 position = EventManager::getMouseMovedPosition();
				////VOFOG_CORE_DEBUG("ObjectPickingSystem.h:\t Left Mouse Clicked: {} - {}", position.x, position.y);

				float sWidth = (float)Vofog::Window::SCREEN_WIDTH;
				float sHeight = (float)Vofog::Window::SCREEN_HEIGHT;
				double xpos1 = position.x;
				double ypos1 = position.y;
				ypos1 = sHeight - ypos1;



				//TODO:: make this variable so that it can work no matter the resolutoin :D
				float frameStartX = 420; float frameWidth = 1706;
				float frameStartY = 226; float frameHeight = 1072;
				if((xpos1 >= frameStartX && xpos1 <= frameStartX + frameWidth)){


					float mousePosX = xpos1; float mousePosY = ypos1;
					float screenWidth = (float)Vofog::Window::SCREEN_WIDTH; float screenHeight = (float)Vofog::Window::SCREEN_HEIGHT;
					float frameMousePosX = (mousePosX - frameStartX) / ((frameWidth + frameStartX) - frameStartX) * (screenWidth - 0) + 0;
					float frameMousePosY = (mousePosY - frameStartY) / ((frameHeight + frameStartY) - frameStartY) * (screenHeight - 0) + 0;
					float newValueX = frameMousePosX;
					float newValueY = frameMousePosY;

					xpos1 = newValueX;
					ypos1 = newValueY;
					glm::vec4 lRayStart_NDC(
						((float)xpos1 / (float)sWidth - 0.5f) * 2.0f,
						((float)ypos1 / (float)sHeight - 0.5f) * 2.0f,
						-1.0,
						1.0f
					);
					glm::vec4 lRayEnd_NDC(
						((float)xpos1 / (float)sWidth - 0.5f) * 2.0f,
						((float)ypos1 / (float)sHeight - 0.5f) * 2.0f,
						0.0,
						1.0f
					);
					_camera->update();
					glm::mat4 InverseProjectionMatrix = glm::inverse(_camera->getProjectionMatrix());

					glm::mat4 InverseViewMatrix = glm::inverse(_camera->getViewMatrix());

					glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
					glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
					glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
					glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;

					glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
					lRayDir_world = glm::normalize(lRayDir_world);
					glm::vec3 out_origin = lRayStart_world;
					glm::vec3 out_end = out_origin + lRayDir_world * 1000.0f;

					//beg = btVector3(out_origin.x, out_origin.y, out_origin.z);
					//end = btVector3(out_end.x, out_end.y, out_end.z);
					btCollisionWorld::ClosestRayResultCallback RayCallback(
						btVector3(out_origin.x, out_origin.y, out_origin.z),
						btVector3(out_end.x, out_end.y, out_end.z)
					);
					//This line allows for ray to only hit static objects
					RayCallback.m_collisionFilterGroup = btBroadphaseProxy::CollisionFilterGroups::SensorTrigger;
					RayCallback.m_collisionFilterMask = btBroadphaseProxy::CollisionFilterGroups::KinematicFilter | btBroadphaseProxy::CollisionFilterGroups::StaticFilter/* | ~btBroadphaseProxy::CollisionFilterGroups::SensorTrigger*/;
					_world->rayTest(
						btVector3(out_origin.x, out_origin.y, out_origin.z),
						btVector3(out_end.x, out_end.y, out_end.z),
						RayCallback
					);
					bool hit = RayCallback.hasHit();

					if(mouseClicked == false){
						if(hit){
							auto current = manager.getCurrentEntity();
							//plane data
							static auto lastDir = _camera->getDirection();
							auto pb = manager.getComponent<ECS::PhysicsComponent>(current);
							auto dir = _camera->getDirection();
							if (lastDir != dir) {
								std::shared_ptr<btStaticPlaneShape> shape = std::make_shared<btStaticPlaneShape>(btVector3(dir.x, dir.y, dir.z), 0);
								pb->editor_planeBody->setCollisionShape(shape.get());
								pb->editor_planeShape = std::move(shape);
								//VOFOG_CORE_DEBUG("Changed\]n\nCHANGEDDDE");
							}
							//VOFOG_CORE_DEBUG("Grab spot updated :D");
							auto hit = RayCallback.m_hitPointWorld;
							grabSpot = glm::vec3(hit.getX(), hit.getY(), hit.getZ());
							grab = true;
							lastDir = dir;
						}
						mouseClicked = true;
					}

					if(hit){
						int id = (int)RayCallback.m_collisionObject->getUserPointer();
						manager.setCurrentEntity((ECS::EntityID)RayCallback.m_collisionObject->getUserPointer());
						//////VOFOG_CORE_FATAL("AHHHHHH\n\n\nAHHHH\n\t\tHit {}", manager.getEntity(id).getName());
						entityGrabbed = true;
					} else{
						/*color.setX(0);
						color.setY(1);*/
						entityGrabbed = false;
						////VOFOG_CORE_DEBUG("No Entity hit");
					}
				}
			}
			if(EventManager::isMouseReleased(VofogMouseKeys::VOFOG_MOUSE_BUTTON_LEFT)){
				auto _world = manager.getWorld();
				glm::vec2 position = EventManager::getMouseMovedPosition();
				//VOFOG_CORE_WARN("ObjectPickingSystem.h:\t Left Mouse Released: {} - {}", position.x, position.y);
				if(mouseClicked == true){
					mouseClicked = false;
					if(entityGrabbed){
						entityGrabbed = false;
						//VOFOG_CORE_LOG("Dragged body to spot");
						auto currentEntityTransform = manager.getComponent<ECS::TransformComponent>(manager.getCurrentEntity());
						Physics::moveBodyTransform(manager.getCurrentEntity(), std::make_shared<EntityManager>(manager), currentEntityTransform->position);
						auto physicsComp = manager.getComponent<ECS::PhysicsComponent>(manager.getCurrentEntity());
						_world->updateSingleAabb((btCollisionObject*)physicsComp->body.get());
						_world->updateSingleAabb((btCollisionObject*)physicsComp->editor_planeBody.get());
					}
				}
				mouseDragging = false;
			}
			if(mouseDragging){
				static double lastX, lastY;
				glm::vec2 position = EventManager::getMouseMovedPosition();
				//VOFOG_CORE_LOG("ObjectPickingSystem.h:\t Left Mouse Dragged: {} - {}", position.x, position.y);

				static bool firstRun = true;
				GLfloat sen = 0.5f;
				xpos = position.x;
				ypos = position.y;


				//double xx = xpos - (_viewportWidth / 2);
				double xx = xpos - lastX;
				//double yy = -ypos + (_viewportHeight / 2);
				double yy = lastY - ypos;
				if(firstRun){
					lastX = xx;
					lastY = yy;
					firstRun = false;
				}
				lastX = xpos;
				lastY = ypos;

				{
					float sWidth = (float)Vofog::Window::SCREEN_WIDTH;
					float sHeight = (float)Vofog::Window::SCREEN_HEIGHT;
					double xpos1 = xpos;
					double ypos1 = ypos;
					ypos1 = sHeight - ypos1;

					//TODO:: make this variable so that it can work no matter the resolutoin :D
					float frameStartX = 420; float frameWidth = 1706;
					float frameStartY = 226; float frameHeight = 1072;
					if((xpos1 >= frameStartX && xpos1 <= frameStartX + frameWidth)){

						if(mouseClicked){
							auto _world = manager.getWorld();
							float mousePosX = xpos1; float mousePosY = ypos1;
							float screenWidth = (float)Vofog::Window::SCREEN_WIDTH; float screenHeight = (float)Vofog::Window::SCREEN_HEIGHT;
							float frameMousePosX = (mousePosX - frameStartX) / ((frameWidth + frameStartX) - frameStartX) * (screenWidth - 0) + 0;
							float frameMousePosY = (mousePosY - frameStartY) / ((frameHeight + frameStartY) - frameStartY) * (screenHeight - 0) + 0;
							float newValueX = frameMousePosX;
							float newValueY = frameMousePosY;

							xpos1 = newValueX;
							ypos1 = newValueY;
							glm::vec4 lRayStart_NDC(
								((float)xpos1 / (float)sWidth - 0.5f) * 2.0f,
								((float)ypos1 / (float)sHeight - 0.5f) * 2.0f,
								-1.0,
								1.0f
							);
							glm::vec4 lRayEnd_NDC(
								((float)xpos1 / (float)sWidth - 0.5f) * 2.0f,
								((float)ypos1 / (float)sHeight - 0.5f) * 2.0f,
								0.0,
								1.0f
							);
							_camera->update();
							glm::mat4 InverseProjectionMatrix = glm::inverse(_camera->getProjectionMatrix());

							glm::mat4 InverseViewMatrix = glm::inverse(_camera->getViewMatrix());

							glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
							glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
							glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
							glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;

							glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
							lRayDir_world = glm::normalize(lRayDir_world);
							glm::vec3 out_origin = lRayStart_world;
							glm::vec3 out_end = out_origin + lRayDir_world * 1000.0f;

							btVector3 other_end = btVector3(out_end.x, out_end.y, out_end.z);
							glm::vec3 positionVector = out_end - out_origin;

							btVector3 begDrag = btVector3(out_origin.x, out_origin.y, out_origin.z);
							btVector3 endDrag = btVector3(out_end.x, out_end.y, out_end.z);
							btCollisionWorld::ClosestRayResultCallback RayCallback(
								btVector3(out_origin.x, out_origin.y, out_origin.z),
								btVector3(out_end.x, out_end.y, out_end.z)
							);
							//This line allows for ray to only hit static objects

							RayCallback.m_collisionFilterMask = btBroadphaseProxy::CollisionFilterGroups::SensorTrigger;/* 0b111111 | btBroadphaseProxy::CollisionFilterGroups::SensorTrigger;*/
							_world->rayTest(
								btVector3(out_origin.x, out_origin.y, out_origin.z),
								btVector3(out_end.x, out_end.y, out_end.z),
								RayCallback
							);
							bool hit = RayCallback.hasHit();
							if(hit){
								/*While dragging the entity...*/
								if(entityGrabbed){
									auto normal = _camera->getDirection();
									auto currentEntity = manager.getCurrentEntity();
									auto currentEntityTransform = manager.getComponent<ECS::TransformComponent>(currentEntity);
									auto hit = RayCallback.m_hitPointWorld;
									glm::vec3 hitPos = glm::vec3(hit.getX(), hit.getY(), hit.getZ()); //hitpos of plane to ray
									glm::vec3 ecurrentPos = currentEntityTransform->position;
									static glm::vec3 distFromCenter(0);
									static glm::vec3 dragSpotDistFromCenter(0);
									if (grab) {
										auto two_decimals = [](float f) -> float {return roundf(f * 100) / 100; };
										distFromCenter = currentEntityTransform->position - hitPos;
										dragSpotDistFromCenter = currentEntityTransform->position - hitPos;
										//VOFOG_CORE_DEBUG("RayHit pos: ({}, {}, {})", two_decimals(hitPos.x), two_decimals(hitPos.y), two_decimals(hitPos.z));
										//VOFOG_CORE_DEBUG("GrabSpot pos: ({}, {}, {})",two_decimals(grabSpot.x), two_decimals(grabSpot.y), two_decimals(grabSpot.z));
										//VOFOG_CORE_WARN("Current Pos: ({}, {}, {})", two_decimals(currentEntityTransform->position.x), two_decimals(currentEntityTransform->position.y), two_decimals(currentEntityTransform->position.z));
										//VOFOG_CORE_LOG("distance from center = currentPos - grabSpot =  ({}, {}, {})", two_decimals(distFromCenter.x), two_decimals(distFromCenter.y), two_decimals(distFromCenter.z));
										//VOFOG_CORE_LOG("distance for drag spot =  ({}, {}, {})", two_decimals(dragSpotDistFromCenter.x), two_decimals(dragSpotDistFromCenter.y), two_decimals(dragSpotDistFromCenter.z));
										//glm::vec3 tmp = glm::abs(hitPos) - glm::abs(distFromCenter);
										//VOFOG_CORE_TRACE("EndingPos = abs(hitPos) - abs(distFromCenter)... = \nendingPos.x = {} - {}\nendingPos.y = {} - {}\nendingPos.z = {} - {}",
											//two_decimals(glm::abs(hitPos.x)),
											//two_decimals(glm::abs(distFromCenter.x)),
											//two_decimals(glm::abs(hitPos.y)),
											//two_decimals(glm::abs(distFromCenter.y)),
											//two_decimals(glm::abs(hitPos.z)),
											//two_decimals(glm::abs(distFromCenter.z)));
										//VOFOG_CORE_ERROR("Ending Pos: ({}, {}, {})", two_decimals(tmp.x), two_decimals(tmp.y), two_decimals(tmp.z));
										//VOFOG_CORE_LOG("Offset: ({}, {}, {})", tmp.x, tmp.y, tmp.z);
										grab = false;
									}
									glm::vec3 offsetPosition = hitPos + distFromCenter;
									//if (distFromCenter.x <= ecurrentPos.x) offsetPosition.x -= glm::abs(distFromCenter.x);
									//if (distFromCenter.y <= ecurrentPos.y) offsetPosition.y -= glm::abs(distFromCenter.y);
									//if (hitPos.x <= ecurrentPos.x) offsetPosition.x = hitPos.x + glm::abs(distFromCenter.x);
									//if (hitPos.y <= ecurrentPos.y) offsetPosition.y = hitPos.y + glm::abs(distFromCenter.y);
									currentEntityTransform->position = offsetPosition;
								}
							}
						}
					}
				}
			}
		}

	}

}