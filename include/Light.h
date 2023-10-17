#pragma once
#include <shader.h>
#include "camera.h"
#include <Mesh/Mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	void setLight(Shader& objectShader, Camera& camera);
	void configureLightVAO(unsigned int& VAO, unsigned int& VBO);
	void Draw(Shader& shader);
	void setPosition(glm::vec3& pointLightPosition);
	void setColor(glm::vec3 &color);
private:
	unsigned int VAO, VBO;
	glm::vec3 pointLightPosition;
	glm::vec3 Color;
};