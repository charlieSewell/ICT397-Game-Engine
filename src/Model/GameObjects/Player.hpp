//Player.hpp

#pragma once
#include "Model/Components/Camera.hpp"
#include "Model/Components/PhysicsComponent.hpp"
#include "Model/Components/PlayerControlledMotion.hpp"
#include "Model/GameObjects/GameObject.hpp"

/**
 * @class Player
 * @brief Child of GameObject for the player within the game. Inherits physics component, camera and player controlled motion
 */
class Player : public GameObject
{

public:
    /*!
     * @brief Constructor
     */
	Player();

	/*!
     * @brief Deconstructor
     */
	~Player();

	/*!
     * @brief Setter for the player collider
     * @param float - width
     * @param float - length
     * @param float - height
     */
	void setCollider(float width, float length, float height) override;

    /*!
     * @brief Draw call for the player model
     */
	void draw();

    /*!
     * @brief Update call for the player
     */
	void update() override;

private:
	Camera m_camera;
	PlayerControlledMotion m_movement;
	PhysicsComponent m_physics;
	void registerPosition();
};
