#include "Light.h"
#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Light::Light()
{
    configureLightVAO(VAO, VBO);
}

void Light::setLight(Shader& objectShader, Camera &camera)
{
    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); 
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); 

    /*
        Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
        the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
        by defining light types as classes and set their values in there, or by using a more efficient uniform approach
        by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
    */
    // directional light
    objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    objectShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    objectShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
    objectShader.setVec3("dirLight.specular", 0.6f, 0.6f, 0.6f);
    // point light 1
    objectShader.setVec3("pointLights[0].position", pointLightPosition);
    objectShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("pointLights[0].constant", 1.0f);
    objectShader.setFloat("pointLights[0].linear", 0.09f);
    objectShader.setFloat("pointLights[0].quadratic", 0.032f);
    // spotLight
    objectShader.setVec3("spotLight.position", camera.Position);
    objectShader.setVec3("spotLight.direction", camera.Front);
    objectShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    objectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("spotLight.constant", 1.0f);
    objectShader.setFloat("spotLight.linear", 0.09f);
    objectShader.setFloat("spotLight.quadratic", 0.032f);
    objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(3.5f)));
    objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(5.0f)));
}

void Light::configureLightVAO(unsigned int& VAO, unsigned int& VBO)
{
    float vertices[] = {
        // positions        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void Light::Draw(Shader& shader)
{

    glBindVertexArray(VAO);
    glm::mat4 model;
    model = glm::translate(model, pointLightPosition);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}

void Light::setPosition(glm::vec3& pointLightPosition)
{
    this->pointLightPosition = pointLightPosition;
}

void Light::setColor(glm::vec3 &color)
{
    Color = color;
}
