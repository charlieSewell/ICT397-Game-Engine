#pragma once

#include <map>
#include "Controller/LuaManager.hpp"
//#include "Controller/LuaTypes.hpp"
#include "Model/SplashScreen.hpp"

/**
 * @class GameObjectManager
 * @brief Responsible for the management of all game objects created
 */
class UIManager 
{
  public:
    /*!
     * @brief Singleton Pattern for a single instance of the class
     * @return instance
     */
    static UIManager &getInstance();

    /*!
     * @brief Initialises the manager
     */
    void init();

    std::shared_ptr<SplashScreen> create(std::string texturePath);
    /*!
     * @brief Adds a GameObject to the map of GameObjects
     * @param GameObject smart pointer - &gameObject
     * @return objectCount-1
     */
    std::string add(std::string name, std::string texturePath);

    /*!
     * @brief Getter for a GameObject with the specified id
     * @param int - id
     * @return GameObject
     */
    std::shared_ptr<SplashScreen> getObject(std::string name);

    /*!
     * @brief Calls the update function for all GameObjects
     */
    void update(float dt);

    /*!
     * @brief Calls the draw function for all GameObjects
     */
    void draw();

    static void registerClass();

    void luaSetUpPanel(std::string name, float left, float right, float top, float bottom);

    void luaSetTexture(std::string name, std::string texturePath);

    void luaSetActive(std::string name, bool a);

  private:
    /*!
     * @brief Constructor
     */
    UIManager();

    /// Map of GameObjects, with the key of the GameObject id, and value of the GameObject smart pointer
    std::map<std::string, std::shared_ptr<SplashScreen>> uiObjects;

};
