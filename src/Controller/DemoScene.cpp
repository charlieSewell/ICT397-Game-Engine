#include "DemoScene.hpp"

void DemoScene::Init() 
{
    auto& terrainFactory = TerrainFactory::getInstance();
    terrainManager.setTerrainTexture(terrainFactory.getTextures());
    terrainManager.setGrassHeight(terrainFactory.getGrassHeight());
    terrainManager.setSandHeight(terrainFactory.getSandHeight());
    terrainManager.setSnowHeight(terrainFactory.getSnowHeight());
    terrainManager.Init();
    //controlsScreen = new SplashScreen("content/Textures/help_menu.png");
    registerMenuButtons();

    GameObjectManager::getInstance().init();
    UIManager::getInstance().init();

    //UIManager::getInstance().getObject("zero")->setupPanel(1700, 1750, 800, 900);
    //std::string name = UIManager::getInstance().add("zero", "content/Textures/0.png");
    //UIManager::getInstance().getObject(name)->setupPanel(1700, 1750, 800, 900);
    //UIManager::getInstance().getObject("zero")->setActive();
}

#include "Model/GameObjects/NPC.hpp"
void DemoScene::Update(double frameRate)
{
    GameObjectManager::getInstance().update(frameRate);
    UIManager::getInstance().update(frameRate);
}

void DemoScene::Draw()
{
    terrainManager.Draw(EMS::getInstance().fire(ReturnVec3Event::getPlayerPosition));
    //if(controlsScreen->isActive())
    //{
    //    controlsScreen->draw();
    //}
    GameObjectManager::getInstance().draw();

    if (Yokai::getInstance().getIsPaused()) 
    {
        int width = 1920, height = 1080;
        ImGui::SetNextWindowPos(ImVec2(width / 2, height / 2), ImGuiCond_Always, ImVec2(0.5, 0.5));
        ImGui::Begin("Pause Menu");

        ImGui::Text("Pause Menu");
        if (ImGui::Button("Resume", ImVec2(500, 100))) 
        {
            Yokai::getInstance().setIsPaused(false);
            glfwSetInputMode(Yokai::getInstance().window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (ImGui::Button("Save", ImVec2(500, 100))) 
        {
            std::cout << "Saving" << std::endl;
        }
        if (ImGui::Button("Quit", ImVec2(500, 100))) 
        {
            std::cout << "Quitting" << std::endl;
            Yokai::getInstance().setIsRunning(false);
        }
        ImGui::End();
    }
    UIManager::getInstance().draw();
}
void DemoScene::Enable()
{
    isEnabled = true;
}
void DemoScene::Disable()
{
    isEnabled = false;
}
void DemoScene::registerMenuButtons()
{
    static bool isPressed;
    auto menuButtonReleased = [&]()
    {
        isPressed = false;

    };
    EMS::getInstance().add(NoReturnEvent::toggleMenuReleased, menuButtonReleased);

    auto toggleMenuPressed = [&]()
    {
        if (!isPressed){
            /*
            if (controlsScreen->isActive())
            {
                glfwSetInputMode(Yokai::getInstance().window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                controlsScreen->setInactive();
                isPressed = true;
            }
            else
            {
                glfwSetInputMode(Yokai::getInstance().window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                controlsScreen->setActive();
                isPressed = true;
            }
            */
        }
    };
    EMS::getInstance().add(NoReturnEvent::toggleMenuPressed, toggleMenuPressed);
}