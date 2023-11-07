//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <shader.h>
//#include <camera.h>
//#include <Mesh/model.h>
//
//#include <iostream>
//#include <stb_image.h>
//#include <Light.h>
//#include "Materials.h"
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//glm::mat4* SetModelMatrices();
//void ConfigureInstanceArray(glm::mat4* modelMatrices, Model& rock);
//// settings
//const unsigned int SCR_WIDTH = 1000;
//const unsigned int SCR_HEIGHT = 1000;
//
//// camera
//Camera camera(glm::vec3(5.0f, 5.0f, 10.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//unsigned int amount = 100000;
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Render_Project", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//    stbi_set_flip_vertically_on_load(false);
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    // build and compile shaders
//    // -------------------------
//    Shader planetShader("shader/10.2.instancing.vs", "shader/10.2.instancing.fs");
//    Shader asteroidShader("shader/10.3.asteroids.vs", "shader/10.3.asteroids.fs");
//    Shader lightCubeShader("shader/8.light.vs", "shader/8.whiteLight.fs");
//    Light light;
//    glm::vec3 lightPos = glm::vec3(5.0f, 10.0f, -10.0f);
//    glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
//    light.setPosition(lightPos);
//    light.setColor(white);
//    // load models
//    // -----------
//    Model planet(("resources/objects/planet/planet.obj"));
//    Model rock(("resources/objects/rock/rock.obj"));
//
//    // generate a large list of semi-random model transformation matrices
//    glm::mat4* modelMatrices = SetModelMatrices();
//    std::cout << "model loaded" << std::endl;
//    planet.PrintNumbers();
//    rock.PrintNumbers();
//
//    ConfigureInstanceArray(modelMatrices, rock);
//    // draw in wireframe
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        camera.processInput(window, deltaTime);
//
//        // render
//        // ------
//        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // don't forget to enable shader before setting uniforms
//        planetShader.use();
//
//        // view/projection transformations
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        asteroidShader.use();
//        asteroidShader.setMat4("projection", projection);
//        asteroidShader.setMat4("view", view);
//        planetShader.use();
//        planetShader.setMat4("projection", projection);
//        planetShader.setMat4("view", view);
//
//        // draw planet
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
//        planetShader.setMat4("model", model);
//        planet.Draw(planetShader);
//
//        // draw meteorites
//        /*for (unsigned int i = 0; i < amount; i++)
//        {
//            planetShader.setMat4("model", modelMatrices[i]);
//            rock.Draw(planetShader);
//        }*/
//
//        asteroidShader.use();
//        asteroidShader.setInt("texture_diffuse1", 0);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
//        for (unsigned int i = 0; i < rock.meshes.size(); i++)
//        {
//            glBindVertexArray(rock.meshes[i].VAO);
//            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
//            glBindVertexArray(0);
//        }
//
//
//        //also draw the lamp object
//        glm::vec3 lightColor = glm::vec3(1.0f);
//        lightCubeShader.use();
//        lightCubeShader.setMat4("projection", projection);
//        lightCubeShader.setVec3("color", lightColor);
//        lightCubeShader.setMat4("view", view);
//        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
//        lightCubeShader.setMat4("model", model);
//
//        light.Draw(lightCubeShader);
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//
//
//void ConfigureInstanceArray(glm::mat4* modelMatrices, Model& rock)
//{
//    // configure instanced array
//    // -------------------------
//    unsigned int buffer;
//    glGenBuffers(1, &buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
//
//    // set transformation matrices as an instance vertex attribute (with divisor 1)
//    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
//    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
//    // -----------------------------------------------------------------------------------------------------------------------------------
//    for (unsigned int i = 0; i < rock.meshes.size(); i++)
//    {
//        unsigned int VAO = rock.meshes[i].VAO;
//        glBindVertexArray(VAO);
//        // set attribute pointers for matrix (4 times vec4)
//        glEnableVertexAttribArray(3);
//        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
//        glEnableVertexAttribArray(4);
//        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
//        glEnableVertexAttribArray(5);
//        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
//        glEnableVertexAttribArray(6);
//        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
//
//        glVertexAttribDivisor(3, 1);
//        glVertexAttribDivisor(4, 1);
//        glVertexAttribDivisor(5, 1);
//        glVertexAttribDivisor(6, 1);
//
//        glBindVertexArray(0);
//    }
//}
//
//glm::mat4 *SetModelMatrices()
//{
//    glm::mat4* modelMatrices;
//    modelMatrices = new glm::mat4[amount];
//    srand(glfwGetTime()); // 初始化随机种子    
//    float radius = 150.0;
//    float offset = 20.f;
//    for (unsigned int i = 0; i < amount; i++)
//    {
//        glm::mat4 model;
//        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
//        float angle = (float)i / (float)amount * 360.0f;
//        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
//        float x = sin(angle) * radius + displacement;
//        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
//        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
//        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
//        float z = cos(angle) * radius + displacement;
//        model = glm::translate(model, glm::vec3(x, y, z));
//
//        // 2. 缩放：在 0.05 和 0.25f 之间缩放
//        float scale = (rand() % 20) / 100.0f + 0.05;
//        model = glm::scale(model, glm::vec3(scale));
//
//        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
//        float rotAngle = (rand() % 360);
//        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
//
//        // 4. 添加到矩阵的数组中
//        modelMatrices[i] = model;
//    }
//    return modelMatrices;
//}
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
