//
// Created by charl on 2/03/2021.
//

#include "Yokai.hpp"
#include "DemoScene.hpp"
#include "Animator.hpp"
#include <glm/gtx/transform.hpp>
Yokai &Yokai::getInstance() 
{
    static Yokai instance;
    return instance;
}

void Yokai::Init() 
{
    registerClose();
    if(!window.Init())
        return;

    renderer.Init();
    //Add layers to layer stack
    layers.push_back(std::shared_ptr<Layer>(new DemoScene()));
    for(auto& layer: layers)
        layer->Init();

    GameObjectManager::init();
    endScreen = new SplashScreen("content/Textures/exit_screen.png");

}
void Yokai::Run()
{
	const double frameRate = 1.0f / 120;	// 120 fps

	double lastFrame = 0;
    double lastTime = 0;
	ModelLoader modelLoader;
	Model test = modelLoader.loadModel("content/Models/laySheary.gltf");
	Animator animator(std::make_shared<Model>(test));

	animator.setAnimation("idle");
	Shader testShader = Shader("content/Shaders/vertexShader.vert","content/Shaders/fragmentShader.frag");;

	auto& terrfac = TerrainFactory::getInstance();

    float x = 500, z = 500;

    while(isRunning)
	{
        if(glfwGetKey(window.getWindow(),GLFW_KEY_I) == GLFW_PRESS){
            animator.setAnimation("idle");
        }
        if(glfwGetKey(window.getWindow(),GLFW_KEY_J) == GLFW_PRESS){
            animator.setAnimation("attack");
        }
        if(glfwGetKey(window.getWindow(),GLFW_KEY_H) == GLFW_PRESS){
            animator.setAnimation("dying");
        }

		double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;


		if((currentTime - lastFrame) >= frameRate)
		{
            glm::mat4 transform(1.0);
            transform = glm::translate(transform,glm::vec3(x,terrfac.heightAt(x,z),z));
            transform = glm::scale(transform,glm::vec3(0.5,0.5,0.5));
            /*
            if(glfwGetKey(window.getWindow(),GLFW_KEY_UP) == GLFW_PRESS){
                animator.setAnimation("walking");
                z+=0.02;
            }
            else if(glfwGetKey(window.getWindow(),GLFW_KEY_DOWN) == GLFW_PRESS){
                animator.setAnimation("walking");
                z-=0.02;
            }
            else if(glfwGetKey(window.getWindow(),GLFW_KEY_UP) == GLFW_RELEASE){
                animator.setAnimation("idle");
            }

            else if(glfwGetKey(window.getWindow(),GLFW_KEY_DOWN) == GLFW_RELEASE){
                animator.setAnimation("idle");
            }
            */
			renderer.Clear();
			InputManagerGLFW::getInstance().processKeyboard(window.getWindow());
			InputManagerGLFW::getInstance().processMouse(window.getWindow());
            //z+= 0.03;


            for(auto& layer: layers)
				layer->Update(frameTime);

            for(auto& layer: layers)
				layer->Draw();

            animator.BoneTransform(frameRate);
            testShader.useShader();
            testShader.setBool("isAnimated", true);
            testShader.setVecMat4("boneTrans",animator.finalTransforms);
            test.Draw(testShader,transform);

            if(endScreen->isActive()){
                endScreen->draw();
            }




			window.endFrame();

			lastFrame = currentTime;
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
    EMS::getInstance().add(InputEvent::closeReleased, closeReleased);

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
    EMS::getInstance().add(InputEvent::closePressed, closePressed);

    auto close = [&]() {
        if(endScreen->isActive())
        {
            isRunning = false;
        }

    };
    EMS::getInstance().add(InputEvent::mouseClicked, close);
}; 