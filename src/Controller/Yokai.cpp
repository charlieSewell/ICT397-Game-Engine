//
// Created by charl on 2/03/2021.
//

#include "Yokai.hpp"

#include "Animator.hpp"
#include "DemoScene.hpp"
#include "Test/NewPhysicsSystem.hpp"
Yokai &Yokai::getInstance() 
{
    static Yokai instance;
    return instance;
}

void Yokai::Init() 
{
    registerClose();
    if(!window.Init())
    {
        return;
    }
    renderer.Init();
    if(!window.ImguiInit())
    {
        return;
    }
    //Add layers to layer stack
    layers.push_back(std::shared_ptr<Layer>(new DemoScene()));
    TerrainFactory::getInstance().Init();
    for(auto& layer: layers)
    {
        layer->Init();
    }
    GameObjectManager::init();
    endScreen = new SplashScreen("content/Textures/exit_screen.png");


}
void Yokai::Run()
{
    NewPhysicsSystem physicsTest;
    physicsTest.test(GameObjectManager::getInstance().getPlayer());
	const double frameRate = 1.0f / 300;	// 120 fps

	double lastFrame = 0;
    double lastTime = 0;

    while(isRunning)
	{
		double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;


		if((currentTime - lastFrame) >= frameRate)
		{
            renderer.Clear();
            window.startFrame();
            glm::mat4 model(1.0);
            model = glm::translate(model,glm::vec3(500,20,500));
            glm::mat4 model1(1.0);
            model1 = glm::translate(model1,glm::vec3(480,20,500));
            glm::mat4 model2(1.0);
            model2 = glm::translate(model2,glm::vec3(490,20,500));
            glm::mat4 model3(1.0);
            model3 = glm::translate(model3,glm::vec3(510,20,500));


			InputManagerGLFW::getInstance().processKeyboard(window.getWindow());
			InputManagerGLFW::getInstance().processMouse(window.getWindow());
            physicsTest.update(GameObjectManager::getInstance().getPlayer());
            for(auto& layer: layers)
            {
                layer->Update(frameTime);

            }
            for(auto& layer: layers)
            {
                layer->Draw();
            }

            if(endScreen->isActive())
            {
                endScreen->draw();
            }

			lastFrame = currentTime;

            ImGui::Begin("Menu");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Main Menu");               // Display some text (you can use a format strings too)
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if(ImGui::Button("Start Game",ImVec2(500,100)))
            {
                std::cout << "Starting Game" <<std::endl;
            }
            if(ImGui::Button("Settings",ImVec2(500,100)))
            {
                std::cout << "Entering Settings" <<std::endl;
            }
            if(ImGui::Button("Quit",ImVec2(500,100)))
            {
                std::cout << "Quitting" <<std::endl;
            }

            ImGui::End();
            renderer.DrawGui();
            window.endFrame();

		}

    }
    renderer.DeInit();
    window.DeInit();
}

void Yokai::registerClose()
{
    static bool isPressed = false;
    auto closeReleased = [&]()
    {
        isPressed = false;
    };
    EMS::getInstance().add(NoReturnEvent::closeReleased, closeReleased);

    auto closePressed = [&]()
    {
      if (!isPressed){
          if (endScreen->isActive())
          {
              endScreen->setInactive();
              isPressed = true;
          }
          else{
              endScreen->setActive();
              isPressed = true;
          }
      }
    };
    EMS::getInstance().add(NoReturnEvent::closePressed, closePressed);

    auto close = [&]() {
        if(endScreen->isActive())
        {
            isRunning = false;
        }

    };
    EMS::getInstance().add(NoReturnEvent::mouseClicked, close);
}