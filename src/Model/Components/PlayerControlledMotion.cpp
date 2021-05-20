//PlayerControlledMotion.cpp

#include <iostream>
#include "PlayerControlledMotion.hpp"
#include "Controller/Physics/PhysicsSystem.hpp"
PlayerControlledMotion::PlayerControlledMotion(Transform& transform)
	:m_transformPtr(&transform)
{

}

void PlayerControlledMotion::setMovementSpeed(float newMovementSpeed)
{
	movementSpeed = newMovementSpeed;
}

float PlayerControlledMotion::getMovementSpeed() const
{
	return movementSpeed;
}

void PlayerControlledMotion::setJumpHeight(float jumpHeight)
{
	jumpHeight = jumpHeight;
}

void PlayerControlledMotion::registerAllMovement(glm::vec3& frontDirection, glm::vec3& upDirection)
{
	registerMoveForward(frontDirection);
	registerMoveBackward(frontDirection);
	registerMoveLeft(frontDirection, upDirection);
	registerMoveRight(frontDirection, upDirection);
	registerJump(upDirection);
	registerMoveDown(upDirection);
	registerXYLook(frontDirection);
}

void PlayerControlledMotion::registerMoveForward(glm::vec3& frontDirection)
{
	auto moveForward = [&]()
	{
		updateVector = (glm::vec3(frontDirection.x,0,frontDirection.z));
	};
	EMS::getInstance().add(NoReturnEvent::moveForward, moveForward);
}

void PlayerControlledMotion::registerMoveBackward(glm::vec3& frontDirection)
{
	auto moveBackward = [&]()
	{
        updateVector = (-glm::vec3(frontDirection.x,0,frontDirection.z));
		//m_transformPtr->translatePostMultiply(-movementSpeed * frontDirection);
	};
	EMS::getInstance().add(NoReturnEvent::moveBackward, moveBackward);
}

void PlayerControlledMotion::registerMoveLeft(glm::vec3& frontDirection, glm::vec3& upDirection)
{
	auto moveLeft = [&]()
	{
        updateVector = -glm::normalize(glm::cross(frontDirection, upDirection)) + updateVector;
        updateVector = glm::vec3(updateVector.x,0,updateVector.z);
		//m_transformPtr->translatePostMultiply(glm::normalize(glm::cross(frontDirection, upDirection)) * -movementSpeed);
	};
	EMS::getInstance().add(NoReturnEvent::moveLeft, moveLeft);
}

void PlayerControlledMotion::registerMoveRight(glm::vec3& frontDirection, glm::vec3& upDirection)
{
	auto moveRight = [&]()
	{
        updateVector = glm::normalize(glm::cross(frontDirection, upDirection)) + updateVector;
        updateVector = glm::vec3(updateVector.x,0,updateVector.z);
		//m_transformPtr->translatePostMultiply(glm::normalize(glm::cross(frontDirection, upDirection)) * movementSpeed);
	};
	EMS::getInstance().add(NoReturnEvent::moveRight, moveRight);
}

void PlayerControlledMotion::registerMoveDown(glm::vec3& upDirection)
{
	auto moveDown = [&]()
	{
        updateVector = (-upDirection)+ updateVector;
	};
	EMS::getInstance().add(NoReturnEvent::moveDown, moveDown);
}


void PlayerControlledMotion::registerJump(glm::vec3& upDirection)
{
	auto jump = [&]()
	{
        updateVector = (upDirection) + updateVector;
	};
	EMS::getInstance().add(NoReturnEvent::jump, jump);
}

void PlayerControlledMotion::registerXYLook(glm::vec3& frontDirection)
{
	static double yaw = -90.0f;
	static double pitch = 0.0f;

	auto xyLook = [&](double xoffset, double yoffset) 
	{
		xoffset *= lookSensitivity;
		yoffset *= lookSensitivity;

		yaw += xoffset;
		pitch -= yoffset;

		// stops bad weird camera movement
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontDirection = glm::normalize(direction);

		m_transformPtr->rotate(glm::radians(-xoffset), glm::vec3(0, 1, 0));
	};

	EMS::getInstance().add(NoReturnEvent::xyLook, xyLook);
}

void PlayerControlledMotion::registerClass()
{
	luabridge::getGlobalNamespace(LuaManager::getInstance().getState())
		.beginClass<PlayerControlledMotion>("PlayerControlledMotion")
		.addProperty("movementSpeed", &PlayerControlledMotion::getMovementSpeed, &PlayerControlledMotion::setMovementSpeed)
		.endClass();
}