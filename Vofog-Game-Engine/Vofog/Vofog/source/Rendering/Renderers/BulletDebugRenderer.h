#pragma once

#include <Vofog.h>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include <Rendering/ShaderProgram.h>

namespace Vofog{
	
	class VOFOG_API BulletDebugRenderer : btIDebugDraw{

	public:
		ShaderProgram program;
        int debugMode;
		BulletDebugRenderer(){
			program.init("DEBUG/BulletDrawLine2.vert", "DEBUG/BulletDrawLine2.frag"/*, "DEBUG/BulletDrawLine.geom"*/);
			debugMode = DBG_MAX_DEBUG_DRAW_MODE;
		}

        ShaderProgram& getShader(){
            return program;
        }
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override{
			float vertices[20] = {
				from.getX(), from.getY(), from.getZ(),
				to.getX(), to.getY(), to.getZ(),
				color.getX(), color.getY(), color.getZ(), 1,

				to.getX(), to.getY(), to.getZ(),
				from.getX(), from.getY(), from.getZ(),
				color.getX(), color.getY(), color.getZ(), 1
			};

			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

			

			program.use();
			glBindVertexArray(VAO); 
			glDrawArrays(GL_LINES, 0, 2);
		}
        void reportErrorWarning(const char* warningString){}

       void draw3dText(const btVector3& location, const char* textString){}

       void setDebugMode(int debugMode1){ debugMode = debugMode1; }

       int getDebugMode() const{ return debugMode; }
       void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color){}

	};


}