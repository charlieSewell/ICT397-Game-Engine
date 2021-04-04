//Player.hpp

#pragma once

#include "../View/Camera.hpp"
#include "GameObject.hpp"
#include "PlayerControlledMotion.hpp"
#include "Controller/Physics/PhysicsComponent.hpp"

class Player : public GameObject, public Camera, public PlayerControlledMotion, public PhysicsComponent
{
	using Camera::m_position;

public:
	Player();
	~Player();

	void update() override;

	glm::vec3 getPosition() const override;
	void setPosition(glm::vec3 position);
};