#include <vpch.h>
#include "MeshRenderer.h"

namespace Vofog{
	namespace Renderer{
	

		void MeshRenderer::init(){
			glGenVertexArrays(1, &m_vertexArrayID);
			glBindVertexArray(m_vertexArrayID);

			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, VOFOG_RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(SHADER_VERTEX_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_COLOR_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_TEXCOORD_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_NORMAL_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_TANGENT_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_BI_TANGENT_ATTRIB_LOCATION);
			glEnableVertexAttribArray(SHADER_TEXTURE_ID_ATTRIB_LOCATION);

			//x, y, z
			//x, y, z

			glVertexAttribPointer(SHADER_VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)0);///xyz
			glVertexAttribPointer(SHADER_COLOR_ATTRIB_LOCATION, 4, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(SHADER_TEXCOORD_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(7 * sizeof(GLfloat)));
			glVertexAttribPointer(SHADER_NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(9 * sizeof(GLfloat)));
			glVertexAttribPointer(SHADER_TANGENT_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(12 * sizeof(GLfloat)));
			glVertexAttribPointer(SHADER_BI_TANGENT_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(15 * sizeof(GLfloat)));
			glVertexAttribPointer(SHADER_TEXTURE_ID_ATTRIB_LOCATION, 4, GL_FLOAT, GL_FALSE, VOFOG_RENDERER_VERTEX_SIZE, (const GLvoid*)(18 * sizeof(GLfloat)));
			glBindVertexArray(0);


			VOFOG_ASSERT(VOFOG_RENDERER_BUFFER_SIZE % 22 == 0);




			vofog_indice_type indices[VOFOG_RENDERER_MAX_INDICE_SIZE];
			//GLshort* indices = new GLshort[VRENDERER_MAX_INDICE_SIZE];
			GLushort offset = 0;
			/*for(int i = 0; i < VOFOG_RENDERER_MAX_INDICE_SIZE; i += 6){
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 3;
				indices[i + 4] = offset + 1;
				indices[i + 5] = offset + 2;
				offset += 4;
				
			}*/
			for(int i = 0; i < VOFOG_RENDERER_MAX_INDICE_SIZE; i += 6){
				indices[i] =	 offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 1;
				indices[i + 5] = offset + 3;
				offset += 4;
			}


			m_indexBuffer = std::make_unique<IndexBuffer>();
			m_indexBuffer->init(&indices[0], VOFOG_RENDERER_MAX_INDICE_SIZE);
			m_indexBuffer->unbind();

			glGenBuffers(1, &m_indexID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, VOFOG_RENDERER_MAX_INDICE_SIZE, NULL, GL_STREAM_DRAW);
			
			//_buffer = (Vofog::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//_buffer = (Vofog::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

			//glBindBuffer(GL_ARRAY_BUFFER, _bufferID);

		}

		
		float MeshRenderer::_populateTextureIDs(int textureID){
			float textureSlot = 0.0f;
			bool textureMapped = false;

			for(int i = 0; i < _textureSlots.size(); i++){
				if(_textureSlots[i] == textureID){
					textureMapped = true;
					textureSlot = (float)(i+1);
					break;
				}
			}
			if(!textureMapped){
				_textureSlots.push_back(textureID);
				textureSlot = _textureSlots.size();
			}
			return textureSlot;
		}

		void MeshRenderer::submit_mesh_test(const std::shared_ptr<Vofog::ECS::EntityManager>& manager){
			std::vector<Vofog::ECS::EntityID> ids
				= manager->getAssociatedEntities<Vofog::ECS::TransformComponent, Vofog::ECS::MeshRendererComponent, Vofog::ECS::MeshTypeComponent>();
			for(int i = 0; i < ids.size(); i++){
				static const glm::vec2 texCoord1(0, 0);
				static const glm::vec2 texCoord2(1, 0);
				static const glm::vec2 texCoord3(0, 1);
				static const glm::vec2 texCoord4(1, 1);
				static glm::vec3 pos1;
				static glm::vec3 pos2;
				static glm::vec3 pos3;
				static glm::vec3 pos4;
				static glm::vec3 norm;
				static glm::mat4 model;
				static glm::vec3 centroi;
				const glm::vec3& size = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->size;
				glm::vec3& position = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->position;
				//VOFOG_CORE_WARN("Sending {}: {}, {}, {}", ids[i], position.x, position.y, position.z);
				const glm::vec3& rotation = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->rotation;
				glm::vec4 color = glm::vec4(1.0f); 
				glm::vec4 textures = glm::vec4(VOFOG_NULL_TEXTURE);
				auto rendComp = manager->getComponent<Vofog::ECS::RenderableComponent>(ids[i]);
				if(rendComp) 
					color = rendComp->color;

				auto comp = manager->getComponent<Vofog::ECS::SpriteComponent>(ids[i]);
				if (comp) {
					textures.x = _populateTextureIDs(comp->textureIDs.x);
					textures.y = _populateTextureIDs(comp->textureIDs.y);
					textures.z = _populateTextureIDs(comp->textureIDs.z);
					textures.w = _populateTextureIDs(comp->textureIDs.w);
				}
				//textures.y = _populateTextureIDs(cube->getMaterial().getSpecularTextureID());
				//textures.z = _populateTextureIDs(cube->getMaterial().getNormalTextureID());
				//textures.w = _populateTextureIDs(cube->getMaterial().getHeightTextureID());
				glm::vec3 position2 = glm::vec3(position.x + size.x, position.y, position.z);
				glm::vec3 position3 = glm::vec3(position.x, position.y + size.y, position.z);
				glm::vec3 position4 = glm::vec3(position.x+size.x, position.y + size.y, position.z);

				auto meshTypeComp = manager->getComponent<Vofog::ECS::MeshTypeComponent>(ids[i]);
				VOFOG_ASSERT(meshTypeComp);
				static auto thing = [](glm::vec3& pos1, glm::vec3& pos2, glm::vec3& pos3, glm::vec3& pos4, glm::vec3 size){
					pos1.x -= size.x / 2.0f; pos1.y -= size.y / 2.0f; pos1.z += size.z / 2.0f;
					pos2.x -= size.x / 2.0f; pos2.y -= size.y / 2.0f; pos2.z += size.z / 2.0f;
					pos3.x -= size.x / 2.0f; pos3.y -= size.y / 2.0f; pos3.z += size.z / 2.0f;
					pos4.x -= size.x / 2.0f; pos4.y -= size.y / 2.0f; pos4.z += size.z / 2.0f;
				};
				switch(meshTypeComp->meshType){
					case Vofog::ECS::MeshType::SQUARE:
						centroi = glm::vec3(position.x, position.y, position.z);
						model = glm::mat4(1);
						model = glm::translate(model, centroi);
						model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
						model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
						model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
						model = glm::translate(model, -centroi);
						glm::vec2 half = glm::vec2(size.x / 2.0f, size.y / 2.0f);
						glm::vec3 center = position;
						glm::vec3 p1 = center; p1.x -= half.x; p1.y -= half.y; 	
						glm::vec3 p2 = center; p2.x += half.x; p2.y -= half.y;	
						glm::vec3 p3 = center; p3.x -= half.x; p3.y += half.y;	
						glm::vec3 p4 = center; p4.x += half.x; p4.y += half.y;	
						_setBufferSlot(model * glm::vec4(p1, 1), color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(p2, 1), color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(p3, 1), color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(p4, 1), color, texCoord4, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 6;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 4;
						break;

						

					case Vofog::ECS::MeshType::TRIANGLE:
						_setBufferSlot(position, color, texCoord1, glm::vec3(0,0,1), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(position2, color, texCoord2, glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(position.x + (size.x / 2.0f), position.y + size.y, position.z), color, texCoord3,
							glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 3;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 3;
						break;

					
					case Vofog::ECS::MeshType::PLANE:
						glm::vec3 offsetPosition = glm::vec3(position.x -( size.x / 2.0f), position.y - size.y / 2.0f, position.z);
						_setBufferSlot(offsetPosition, color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x + size.x, offsetPosition.y, position.z), color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x, offsetPosition.y + size.y, position.z), color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x + size.x, offsetPosition.y + size.y, position.z), color, texCoord4, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 6;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 4;
						break;

					case Vofog::ECS::MeshType::CUBE:

						

						centroi = glm::vec3(position.x, position.y, position.z );
						model = glm::mat4(1);
						model = glm::translate(model, centroi);
						model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
						model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
						model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
						model = glm::translate(model, -centroi);
						//front
						pos1 = position;
						pos2 = position2;
						pos3 = position3;
						pos4 = position4;
						thing(pos1, pos2, pos3, pos4, size);
						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//bottom
						pos1 = position;
						pos2 = glm::vec3(position.x, position.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y, position.z);
						pos4 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);

						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//back
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos4 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);

						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);
						
						//left
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y, position.z);
						pos3 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos4 = glm::vec3(position.x, position.y + size.y, position.z);
						thing(pos1, pos2, pos3, pos4, size);
						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//right
						pos1 = glm::vec3(position.x + size.x, position.y + size.y, position.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						pos3 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						pos4 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);

						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//top
						pos1 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y + size.y, position.z);
						pos4 = glm::vec3(position.x, position.y + size.y, position.z);
						thing(pos1, pos2, pos3, pos4, size);

						pos1 = model * glm::vec4(pos1, 1);
						pos2 = model * glm::vec4(pos2, 1);
						pos3 = model * glm::vec4(pos3, 1);
						pos4 = model * glm::vec4(pos4, 1);
						norm = glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2));
						_setBufferSlot(pos1, color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos2, color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos3, color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(pos4, color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						for(int i = 0; i < 6; i++){
							m_indiceCount += 6;
							*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
							m_currentIndex += 4;
						}
						break;

					case Vofog::ECS::MeshType::PYRAMID:

						
						glm::vec3 positionOfTip = glm::vec3(position.x + (size.x / 2.0f), position.y + size.y, position.z - (size.z / 2.0f));
						
						centroi = glm::vec3(position.x + size.x / 2.0f, position.y + size.y / 2.0f, position.z - size.z / 2.0f);
						model = glm::mat4(1);
						model = glm::translate(model, centroi);
						model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1));
						model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
						model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
						model = glm::translate(model, -centroi);
						//bottom
						pos1 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2,1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(glm::vec3(position.x, position.y, position.z - size.z),1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(glm::vec3(position.x, position.y, position.z),1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);

						//left face
						pos1 = glm::vec3(position.x, position.y, position.z);
						pos2 = glm::vec3(position.x, position.y, position.z - size.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)),1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//right face
						pos1 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1,1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip,1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2,1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//front
						pos1 = glm::vec3(position.x + size.x, position.y, position.z);
						pos2 = glm::vec3(position.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//back
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						


						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_indiceCount += 6;
						m_currentIndex += 4;

						for(int i = 0; i < 4; i++){
							*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							m_indiceCount += 3;
							m_currentIndex += 3;
						}
						break;

				}
				
			}
		}

		void MeshRenderer::submit_debug(const std::shared_ptr<Vofog::ECS::EntityManager>& manager){
				ECS::EntityID debugEntity = manager->getCurrentEntity();
				auto transform = manager->getComponent<Vofog::ECS::TransformComponent>(debugEntity);
				auto meshTypeComp = manager->getComponent<Vofog::ECS::MeshTypeComponent>(debugEntity);
				if(!transform)return;
				if(!meshTypeComp) return;

				static const glm::vec2 texCoord1(0, 0);
				static const glm::vec2 texCoord2(1, 0);
				static const glm::vec2 texCoord3(0, 1);
				static const glm::vec2 texCoord4(1, 1);
				static glm::vec3 pos1;
				static glm::vec3 pos2;
				static glm::vec3 pos3;
				static glm::vec3 pos4;
				static glm::vec3 norm;
				static glm::mat4 model;
				static glm::vec3 centroi;
				const glm::vec3& size =		transform->size;
				const glm::vec3& position = transform->position;
				const glm::vec3& rotation = transform->rotation;
				glm::vec4 color = glm::vec4(1.0f);
				glm::vec4 textures = glm::vec4(VOFOG_NULL_TEXTURE);

				glm::vec3 position2 = glm::vec3(position.x + size.x, position.y, position.z);
				glm::vec3 position3 = glm::vec3(position.x, position.y + size.y, position.z);
				glm::vec3 position4 = glm::vec3(position.x + size.x, position.y + size.y, position.z);
				static auto thing = [](glm::vec3& pos1, glm::vec3& pos2, glm::vec3& pos3, glm::vec3& pos4, glm::vec3 size){
					pos1.x -= size.x / 2.0f; pos1.y -= size.y / 2.0f; pos1.z += size.z / 2.0f;
					pos2.x -= size.x / 2.0f; pos2.y -= size.y / 2.0f; pos2.z += size.z / 2.0f;
					pos3.x -= size.x / 2.0f; pos3.y -= size.y / 2.0f; pos3.z += size.z / 2.0f;
					pos4.x -= size.x / 2.0f; pos4.y -= size.y / 2.0f; pos4.z += size.z / 2.0f;
				};
				VOFOG_ASSERT(meshTypeComp);
				switch(meshTypeComp->meshType){
					case Vofog::ECS::MeshType::SQUARE:
						glm::vec2 half = glm::vec2(size.x / 2.0f, size.y / 2.0f);
						glm::vec3 center = position;
						glm::vec3 p1 = center; p1.x -= half.x; p1.y -= half.y;
						glm::vec3 p2 = center; p2.x += half.x; p2.y -= half.y;
						glm::vec3 p3 = center; p3.x -= half.x; p3.y += half.y;
						glm::vec3 p4 = center; p4.x += half.x; p4.y += half.y;
						_setBufferSlot(p1, color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(p2, color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(p3, color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(p4, color, texCoord4, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 6;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 4;
						break;

					case Vofog::ECS::MeshType::TRIANGLE:
						_setBufferSlot(position, color, texCoord1, glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(position2, color, texCoord2, glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(position.x + (size.x / 2.0f), position.y + size.y, position.z), color, texCoord3,
							glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 3;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 3;
						break;


					case Vofog::ECS::MeshType::PLANE:
						glm::vec3 offsetPosition = glm::vec3(position.x - (size.x / 2.0f), position.y - size.y / 2.0f, position.z);
						_setBufferSlot(offsetPosition, color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x + size.x, offsetPosition.y, position.z), color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x, offsetPosition.y + size.y, position.z), color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(glm::vec3(offsetPosition.x + size.x, offsetPosition.y + size.y, position.z), color, texCoord4, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
						m_indiceCount += 6;
						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_currentIndex += 4;
						break;

					case Vofog::ECS::MeshType::CUBE:

						centroi = glm::vec3(position.x, position.y, position.z);
						model = glm::mat4(1);
						model = glm::translate(model, centroi);
						model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
						model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
						model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
						model = glm::translate(model, -centroi);
						//front
						pos1 = position;
						pos2 = position2;
						pos3 = position3;
						pos4 = position4;
						thing(pos1, pos2, pos3, pos4, size);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//bottom
						pos1 = position;
						pos2 = glm::vec3(position.x, position.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y, position.z);
						pos4 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//back
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos4 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//left
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y, position.z);
						pos3 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos4 = glm::vec3(position.x, position.y + size.y, position.z);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);

						//right
						pos1 = glm::vec3(position.x + size.x, position.y + size.y, position.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						pos3 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						pos4 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						thing(pos1, pos2, pos3, pos4, size);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);

						//top
						pos1 = glm::vec3(position.x + size.x, position.y + size.y, position.z - size.z);
						pos2 = glm::vec3(position.x, position.y + size.y, position.z - size.z);
						pos3 = glm::vec3(position.x + size.x, position.y + size.y, position.z);
						pos4 = glm::vec3(position.x, position.y + size.y, position.z);
						thing(pos1, pos2, pos3, pos4, size);
						norm = model * glm::vec4(glm::cross(glm::vec3(pos3 - pos1), glm::vec3(pos3 - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos3, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos4, 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);

						for(int i = 0; i < 6; i++){
							m_indiceCount += 6;
							*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
							m_currentIndex += 4;
						}
						break;

					case Vofog::ECS::MeshType::PYRAMID:


						glm::vec3 positionOfTip = glm::vec3(position.x + (size.x / 2.0f), position.y + size.y, position.z - (size.z / 2.0f));

						centroi = glm::vec3(position.x + size.x / 2.0f, position.y + size.y / 2.0f, position.z - size.z / 2.0f);
						model = glm::mat4(1);
						model = glm::translate(model, centroi);
						model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
						model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
						model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
						model = glm::translate(model, -centroi);
						//bottom
						pos1 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, texCoord4, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, texCoord2, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(glm::vec3(position.x, position.y, position.z - size.z), 1), color, texCoord3, norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(glm::vec3(position.x, position.y, position.z), 1), color, texCoord1, norm, glm::vec3(0), glm::vec3(0), textures);

						//left face
						pos1 = glm::vec3(position.x, position.y, position.z);
						pos2 = glm::vec3(position.x, position.y, position.z - size.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//right face
						pos1 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//front
						pos1 = glm::vec3(position.x + size.x, position.y, position.z);
						pos2 = glm::vec3(position.x, position.y, position.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);

						//back
						pos1 = glm::vec3(position.x, position.y, position.z - size.z);
						pos2 = glm::vec3(position.x + size.x, position.y, position.z - size.z);
						norm = model * glm::vec4(glm::cross(-glm::vec3(positionOfTip - pos1), glm::vec3(positionOfTip - pos2)), 1);
						_setBufferSlot(model * glm::vec4(pos1, 1), color, glm::vec2(1, 0), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(positionOfTip, 1), color, glm::vec2(0.5, 1), norm, glm::vec3(0), glm::vec3(0), textures);
						_setBufferSlot(model * glm::vec4(pos2, 1), color, glm::vec2(0, 0), norm, glm::vec3(0), glm::vec3(0), textures);




						*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
						*m_dynamicIndexBuffer = 3 + m_currentIndex; m_dynamicIndexBuffer++;
						m_indiceCount += 6;
						m_currentIndex += 4;

						for(int i = 0; i < 4; i++){
							*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 1 + m_currentIndex; m_dynamicIndexBuffer++;
							*m_dynamicIndexBuffer = 2 + m_currentIndex; m_dynamicIndexBuffer++;
							m_indiceCount += 3;
							m_currentIndex += 3;
						}
						break;



				}
		}
		void MeshRenderer::submit(const std::shared_ptr<Vofog::ECS::EntityManager>& manager){
			std::vector<Vofog::ECS::EntityID> ids
				= manager->getAssociatedEntities<Vofog::ECS::TransformComponent, Vofog::ECS::RenderableComponent>();
			for(int i = 0; i < ids.size(); i++){
				static glm::vec2 texCoord1(0, 0);
				static glm::vec2 texCoord2(1, 0);
				static glm::vec2 texCoord3(0, 1);
				static glm::vec2 texCoord4(1, 1);

				const glm::vec3& size = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->size;
				const glm::vec3& position = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->position;
				const glm::vec4& color = manager->getComponent<Vofog::ECS::RenderableComponent>(ids[i])->color;
				glm::vec4 textures = glm::vec4(VOFOG_NULL_TEXTURE);
				auto comp = manager->getComponent<Vofog::ECS::SpriteComponent>(ids[i]);
				if (comp) {
					textures.x = _populateTextureIDs(comp->textureIDs.x);
					textures.y = _populateTextureIDs(comp->textureIDs.y);
					textures.z = _populateTextureIDs(comp->textureIDs.z);
					textures.w = _populateTextureIDs(comp->textureIDs.w);
				}
				glm::vec3 position2 = glm::vec3(position.x + size.x, position.y, position.z);
				glm::vec3 position3 = glm::vec3(position.x, position.y + size.y, position.z);

				_setBufferSlot(position, color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
				_setBufferSlot(position2, color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
				_setBufferSlot(position3, color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
			}
		}

		void MeshRenderer::begin(){
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
			m_buffer = (Vofog::Renderer::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			m_dynamicIndexBuffer = (vofog_indice_type*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

		}


		void MeshRenderer::_setBufferSlot(glm::vec3 pos, glm::vec4 col, glm::vec2 tex, glm::vec3 norm, glm::vec3 tangent, glm::vec3 biTangent, glm::vec4 textureIDs){
			m_buffer->position = pos;
			m_buffer->color = col;
			m_buffer->texCoords = tex;
			m_buffer->normal = norm;
			m_buffer->tangent = tangent;
			m_buffer->biTangent = glm::vec3(0);
			m_buffer->textureIDs = textureIDs;
			//VOFOG_CORE_DEBUG("ids = {} {} {} {}", textureIDs.x, textureIDs.y, textureIDs.z, textureIDs.w);
			m_buffer++;
			


		}

		void MeshRenderer::submit_raw_model(std::vector<float> packedData, glm::vec3 posOffset){
			VOFOG_ASSERT(packedData.size() % (3+2+3) == 0);
			glm::vec3 pos(0);
			glm::vec4 col(0.6);
			glm::vec2 tex(0);
			glm::vec3 norm(0);
			for(int index = 0; index < packedData.size(); index += 3+2+3){
				m_indiceCount += 1;
				*m_dynamicIndexBuffer = 0 + m_currentIndex; m_dynamicIndexBuffer++;
				m_currentIndex += 1;
				pos.x = packedData[index] + posOffset.x; pos.y = packedData[index+1] + posOffset.y; pos.z = packedData[index+2] + posOffset.z;
				tex.x = packedData[index+3]; tex.y = packedData[index+4];
				norm.x = packedData[index+5]; norm.y = packedData[index+6]; norm.z = packedData[index+7];
				_setBufferSlot(pos, col, tex, norm, glm::vec3(0), glm::vec3(0), glm::vec4(0));
				
			}
			//VOFOG_CORE_ERROR("!");
		}
	

		void MeshRenderer::flush(){
			//_spriteCounter = 0;
			glBindVertexArray(m_vertexArrayID);
			//m_indexBuffer->bind();
		
			
			for(int i = 0; i < _textureSlots.size(); i++){//TODO: only rebind textures that are replaced or changed, if they are static dont rebind
				if(_textureSlots[i] == VOFOG_NULL_TEXTURE) continue;
				//VOFOG_CORE_LOG("Binding slots {} to id {}", i+1, _textureSlots[i]);
				glBindTextureUnit(i+1, _textureSlots[i]);
				//glBindTextureUnit(_textureSlots[i], _textureSlots[i]);
			}
			{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error); DebugBreak();
				}
			}
			//_indiceCount = m_models3D.size() * 6 * 6;
			//0x0000
			//glDrawElements(GL_TRIANGLES, m_models3D.size() * 6 * 6, GL_UNSIGNED_SHORT, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
			glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_SHORT, NULL);
			m_indiceCount = 0;
			m_currentIndex = 0;
			{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error); DebugBreak();
				}
			}

			//glBindVertexArray(0);

		}

		void MeshRenderer::end(){
			
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}


		MeshRenderer::~MeshRenderer(){
			glDeleteBuffers(1, &m_bufferID);
			glDeleteBuffers(1, &m_indexID);
		}
	
	}

}