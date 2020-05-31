#include <vpch.h>
#include "BatchRenderer.h"

namespace Vofog{

	BatchRenderer::BatchRenderer(){
		//init();
	}

	void BatchRenderer::init(){
		glGenVertexArrays(1, &_vertexArrayID);
		glBindVertexArray(_vertexArrayID);

		glGenBuffers(1, &_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
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

		
		_VOFOG_DEBUG(static_assert(VOFOG_RENDERER_BUFFER_SIZE % 22== 0)); //assert buffer size is flush with vertex size





		vfg_indice_type* indices = new vfg_indice_type[VOFOG_RENDERER_MAX_INDICE_SIZE];
		//GLshort* indices = new GLshort[VRENDERER_MAX_INDICE_SIZE];
		GLushort offset = 0;
		for(int i = 0; i < VOFOG_RENDERER_MAX_INDICE_SIZE; i += 6){
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 1;
			indices[i + 5] = offset + 3;
			offset += 4;
		}
		

		_indexBuffer = std::make_unique<IndexBuffer>();
		_indexBuffer->init(&indices[0], VOFOG_RENDERER_MAX_INDICE_SIZE);
		_indexBuffer->unbind();

		


		//_buffer = (Vofog::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//_buffer = (Vofog::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		//glBindBuffer(GL_ARRAY_BUFFER, _bufferID);

	}

	void BatchRenderer::begin(){
		glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
		_buffer = (Vofog::VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}


	void BatchRenderer::submit(std::shared_ptr<Vofog::Cube> cube){
		//TODO: model loading by glyph
		if(!cube->submitted())
			m_models3D.push_back(cube);
		
	}
	void BatchRenderer::submit2(const std::shared_ptr<Vofog::ECS::EntityManager>& manager){
		std::vector<Vofog::ECS::EntityID> ids 
			= manager->getAssociatedEntities<Vofog::ECS::TransformComponent, Vofog::ECS::RenderableComponent, Vofog::ECS::SpriteComponent>();
		for(int i = 0; i < ids.size(); i++){
			static glm::vec2 texCoord1(0, 0);
			static glm::vec2 texCoord2(1, 0);
			static glm::vec2 texCoord3(0, 1);
			static glm::vec2 texCoord4(1, 1);

			const glm::vec3& size = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->size;
			const glm::vec3& position = manager->getComponent<Vofog::ECS::TransformComponent>(ids[i])->position;
			const glm::vec4& color = manager->getComponent<Vofog::ECS::RenderableComponent>(ids[i])->color;
			glm::vec4 textures(1);// = glm::vec4(manager->getComponent<Vofog::ECS::SpriteComponent>(ids[i])->textureIDs);

			glm::vec3 position2 = glm::vec3(position.x + size.x, position.y, position.z);
			glm::vec3 position3 = glm::vec3(position.x, position.y + size.y, position.z);

			_setBufferSlot(position, color, texCoord1, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
			_setBufferSlot(position2, color, texCoord2, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
			_setBufferSlot(position3, color, texCoord3, glm::vec3(0), glm::vec3(0), glm::vec3(0), textures);
		}
	}
	void BatchRenderer::_setBufferSlot(glm::vec3 pos, glm::vec4 col, glm::vec2 tex, glm::vec3 norm, glm::vec3 tangent, glm::vec3 biTangent, glm::vec4 textureIDs)
	{
		_buffer->position = pos;
		_buffer->color = col;
		_buffer->texCoords = tex;
		_buffer->normal = norm;
		_buffer->tangent = tangent;
		_buffer->biTangent = glm::vec3(0);
		_buffer->textureIDs = textureIDs;
		_buffer++;
		//_count++;
		

		
	}
	float BatchRenderer::_populateTextureIDs(int textureID) {
		float textureSlot = 0.0f;
		bool textureMapped = false;

		for (int i = 0; i < _textureSlots.size(); i++) {
			if (_textureSlots[i] == textureID) {
				textureMapped = true;
				textureSlot = (float)i;
				break;
			}
		}
		if (!textureMapped) {
			_textureSlots.push_back(textureID);
			textureSlot = _textureSlots.size();
		}
		return textureSlot;
	}

	void BatchRenderer::_sortAndBatch() {


		static glm::vec2 texCoord1(0, 0);
		static glm::vec2 texCoord2(1, 0);
		static glm::vec2 texCoord3(0, 1);
		static glm::vec2 texCoord4(1, 1);

		int bufferOffset = 0;
		int countTotal = 0;
		bool triggered = false;
		int drawIndices = 0;
		for (int i = 0; i < m_models3D.size(); i++) {
			std::shared_ptr < Vofog::Cube> cube = m_models3D[i];
			float textureID = cube->getMaterial().getDiffuseTexture()->getTextureID();
			glm::vec4 textureSlot = glm::vec4(VOFOG_MISSING_TEXTURE);
			textureSlot.x = _populateTextureIDs(cube->getMaterial().getDiffuseTextureID());
			textureSlot.y = _populateTextureIDs(cube->getMaterial().getSpecularTextureID());
			textureSlot.z = _populateTextureIDs(cube->getMaterial().getNormalTextureID());
			textureSlot.w = _populateTextureIDs(cube->getMaterial().getHeightTextureID());
			//VOFOG_CORE_LOG("{} mapped to {}", cube->getName(), textureSlot);


		/*	if (cube->submitted()) {
				if (!cube->hasChanged()) {
					bufferOffset += 6 * 4;
					continue;
				} else {
					_buffer += bufferOffset;
					triggered = true;
				}
			}*/
			const glm::vec3& size = cube->getSize();
			const glm::vec3& position = cube->getPosition();
			const glm::vec4& color = cube->getColor();
			const glm::mat4& modelMatrix = cube->getModelMatrix();// * translation * rotation;

			glm::mat4 modelInverse = glm::transpose(glm::inverse(modelMatrix));
			//GLfloat modelInverse = 1.0f;
			glm::vec3 vertexNormal = modelInverse * glm::vec4(glm::vec3(0, 0, 1), 1);
			//glm::vec3 vertexNormal(0,0,1);

			glm::vec3 modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z, 1));
			glm::vec3 modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z, 1));
			glm::vec3 modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z, 1));
			glm::vec3 modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z, 1));

			glm::vec3 tangent1;
			glm::vec3 edge1 = modelPos2 - modelPos1;
			glm::vec3 edge2 = modelPos3 - modelPos1;
			glm::vec2 deltaUV1 = texCoord2 - texCoord1;
			glm::vec2 deltaUV2 = texCoord3 - texCoord1;

			GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			glm::vec3 biTangent;
			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));



			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);


			//Left face
			vertexNormal = modelInverse * glm::vec4(glm::vec3(-1, 0, 0), 1);
			modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z - size.z, 1));
			modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z, 1));
			modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z - size.z, 1));
			modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z, 1));

			edge1 = modelPos2 - modelPos1;
			edge2 = modelPos3 - modelPos1;
			deltaUV1 = texCoord2 - texCoord1;
			deltaUV2 = texCoord3 - texCoord1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));

			glm::vec4 color1 = glm::vec4(1, 0.8, 0.8, 1);
			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);



			//Back face
			vertexNormal = modelInverse * glm::vec4(glm::vec3(0, 0, -1), 1);
			modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z - size.z, 1));
			modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z - size.z, 1));
			modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z - size.z, 1));
			modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z - size.z, 1));

			edge1 = modelPos2 - modelPos1;
			edge2 = modelPos3 - modelPos1;
			deltaUV1 = texCoord2 - texCoord1;
			deltaUV2 = texCoord3 - texCoord1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);



			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));


			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);



			//Right face
			vertexNormal = modelInverse * glm::vec4(glm::vec3(1, 0, 0), 1);
			//vertexNormal = glm::vec3(1, 0, 0);
			modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z, 1));
			modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z - size.z, 1));
			modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z, 1));
			modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z - size.z, 1));

			edge1 = modelPos2 - modelPos1;
			edge2 = modelPos3 - modelPos1;
			deltaUV1 = texCoord2 - texCoord1;
			deltaUV2 = texCoord3 - texCoord1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));




			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);



			//Bottom face
			vertexNormal = modelInverse * glm::vec4(glm::vec3(0, -1, 0), 1);
			//vertexNormal = glm::vec3(0, -1, 0);
			modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z - size.z, 1));
			modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z - size.z, 1));
			modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y, position.z, 1));
			modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y, position.z, 1));

			edge1 = modelPos2 - modelPos1;
			edge2 = modelPos3 - modelPos1;
			deltaUV1 = texCoord2 - texCoord1;
			deltaUV2 = texCoord3 - texCoord1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));


			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);

			//top face
			vertexNormal = modelInverse * glm::vec4(glm::vec3(0, 1, 0), 1);//vertexNormal = glm::vec3(0, 1, 0);
			modelPos1 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z, 1));
			modelPos2 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z, 1));
			modelPos3 = glm::vec3(modelMatrix * glm::vec4(position.x, position.y + size.y, position.z - size.z, 1));
			modelPos4 = glm::vec3(modelMatrix * glm::vec4(position.x + size.x, position.y + size.y, position.z - size.z, 1));

			edge1 = modelPos2 - modelPos1;
			edge2 = modelPos3 - modelPos1;
			deltaUV1 = texCoord2 - texCoord1;
			deltaUV2 = texCoord3 - texCoord1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);



			biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			biTangent = glm::normalize(modelInverse * glm::vec4(biTangent, 0.0));
			if (glm::dot(glm::cross(vertexNormal, tangent1), biTangent) < 0.0f)
				tangent1 = tangent1 * -1.0f;
			tangent1 = glm::normalize(modelInverse * glm::vec4(tangent1, 0.0));
			glm::vec3 cross = glm::cross(biTangent, tangent1);
			glm::vec3 err = glm::abs(cross) - glm::abs(vertexNormal);


			_setBufferSlot(modelPos1, color, texCoord1, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos2, color, texCoord2, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos3, color, texCoord3, vertexNormal, tangent1, biTangent, textureSlot);
			_setBufferSlot(modelPos4, color, texCoord4, vertexNormal, tangent1, biTangent, textureSlot);
			if (triggered) {
				bufferOffset = 0;
				triggered = false;
			}
			//bufferOffset += 6 * 4;


		//	if (!m_models3D[i]->submitted()) {
				//_indiceCount += 6 * 6;
				//drawIndices += 36;
			//	_spriteCounter += 6;
			//}
			cube->setHasChanged(false);
			cube->setSubmitted();


		}
		//_count = 0;



	}

	


	void BatchRenderer::flush(){
		//_spriteCounter = 0;
		glBindVertexArray(_vertexArrayID);
		_indexBuffer->bind();
		int indicesUsed = 0;
		
		for (int i = 0; i < _textureSlots.size(); i++) {//TODO: only rebind textures that are replaced or changed, if they are static dont rebind
			if (_textureSlots[i] == VOFOG_MISSING_TEXTURE) continue;
			glBindTextureUnit(i, _textureSlots[i]);
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
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
		{
			GLenum error = glGetError();
			if(error != GL_NO_ERROR){
				VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error); DebugBreak();
			}
		}
	
		//glBindVertexArray(0);
		
	}

	void BatchRenderer::end(){
		_sortAndBatch();
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	BatchRenderer::~BatchRenderer(){
		glDeleteBuffers(1, &_bufferID);
	}
}