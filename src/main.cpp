#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_NONE
#include <iostream>

#include "Controller/EventManager.h"

#include "Controller/Yokai.hpp"
#include "View/Camera.hpp"
#include "Controller/InputManagerGLFW.hpp"
#include "Model/Player.hpp"
#include "Controller/Factory/TerrainFactory.hpp"
#include "Controller/ModelManager.hpp"
#include "Controller/Factory/GameAssetFactory.hpp"

void error_callback(int error, const char* description)
{
	std::cout << "Error:" << error << " " << description << std::endl;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	//Player player;
	ModelManager modelManager;
	ModelLoader modelLoader;
    TerrainManager terrainManager;
	auto& engine = Yokai::getInstance();
	GLFWwindow* window;
	if (!glfwInit()) {
		return 0;
	}
	window = glfwCreateWindow(800, 600, "ICT397 Game Engine", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetErrorCallback(error_callback);
	if (!window) {
		return 0;
	}
	glfwMakeContextCurrent(window);
	engine.Init();

	//Shader testShader("content/Shaders/vertexShader.vert","content/Shaders/testShader.frag");
	Shader testShader("content/Shaders/terrainVertex.vert", "content/Shaders/terrainFragment.frag");
	Shader modelShader("content/Shaders/vertexShader.vert", "content/Shaders/fragmentShader.frag");
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, InputManagerGLFW::processMouse);     // move to input engine
	//int modelID = modelManager.GetModelID("content/Models/pine.fbx");
    terrainManager.Init();
	// TESTING FOR ASSET FACTORY
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	GameAssetFactory GF;
	std::vector<std::shared_ptr<GameObject>> npcs;

	std::shared_ptr<GameObject> player = GF.Create(GameObjectType::player, "");
	std::shared_ptr<GameObject> pineTree = GF.Create(GameObjectType::staticObject, "content/Models/pine.fbx");
	std::shared_ptr<GameObject> zombie1 = GF.Create(GameObjectType::npc, "content/Models/zombie.fbx");
	std::shared_ptr<GameObject> zombie2 = GF.Create(GameObjectType::npc, "content/Models/zombie.fbx");
	std::shared_ptr<GameObject> zombie3 = GF.Create(GameObjectType::npc, "content/Models/zombie.fbx");
	std::shared_ptr<GameObject> rock = GF.Create(GameObjectType::staticObject, "content/Models/rock.fbx");

	pineTree->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	zombie1->setPosition(glm::vec3(255.0f, 5.0f, 255.0f));
	zombie2->setPosition(glm::vec3(270.0f, 5.0f, 240.0f));
	zombie3->setPosition(glm::vec3(240.0f, 5.0f, 270.0f));
	rock->setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
	player->setPosition(glm::vec3(225.0f, 10.0f, 225.0f));

	npcs.push_back(zombie1);
	npcs.push_back(zombie2);
	npcs.push_back(zombie3);
	//npcs.push_back(zombie2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//THIS IS ALL TEST CODE AND SUBJECT TO CHANGE DO NOT ADD RENDERING FUNCTIONS HERE
	while (!glfwWindowShouldClose(window))
	{
		//will be moved to input engine later
		InputManagerGLFW::getInstance().processKeyboard(window);
		InputManagerGLFW::getInstance().processMouse(window);
		Renderer::Clear();
		player->update();

		pineTree->draw(modelShader);
		rock->draw(modelShader);


		for (int i = 0; i < npcs.size(); i++)
		{
			npcs[i]->draw(modelShader);
		}
		terrainManager.Draw(player->getPosition());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}