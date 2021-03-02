#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_NONE
#include <iostream>
#include "Model/ObjectLoading/Model.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Controller/unnamedEngine.hpp"

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
int main() {
    auto &engine = unnamedEngine::getInstance();
    engine.renderer.Init();

    Shader testShader("content/Shaders/vertexShader.vert","content/Shaders/fragmentShader.frag");
    Model testModel("content/Models/CAR.fbx");
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




    //THIS IS ALL TEST CODE AND SUBJECT TO CHANGE DO NOT ADD RENDERING FUNCTIONS HERE




    while (!glfwWindowShouldClose(engine.renderer.window))
    {
        processInput(engine.renderer.window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        testShader.useShader();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(0,0,20),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
            );
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        testShader.setMat4("model", model);
        testModel.Draw(testShader);

        glfwSwapBuffers(engine.renderer.window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(engine.renderer.window);
    glfwTerminate();
    return 0;
}
