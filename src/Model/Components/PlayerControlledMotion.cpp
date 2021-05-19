//PlayerControlledMotion.cpp

#include <iostream>
#include "PlayerControlledMotion.hpp"

PlayerControlledMotion::PlayerControlledMotion(Transform& transform)
	:m_transformPtr(&transform)
{

}

void PlayerControlledMotion::setMovementSpeed(float movementSpeed)
{
	movementSpeed = movementSpeed;
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
	registerJump();
	registerMoveDown(upDirection);
	registerXYLook(frontDirection);
}

void PlayerControlledMotion::registerMoveForward(glm::vec3& frontDirection)
{
	auto moveForward = [&]()
	{
		m_transformPtr->translatePostMultiply(movementSpeed * frontDirection);
	};
	EMS::getInstance().add(NoReturnEvent::moveForward, moveForward);
}

void PlayerControlledMotion::registerMoveBackward(glm::vec3& frontDirection)
{
	auto moveBackward = [&]()
	{
		m_transformPtr->translatePostMultiply(-movementSpeed * frontDirection);
	};
	EMS::getInstance().add(NoReturnEvent::moveBackward, moveBackward);
}

void PlayerControlledMotion::registerMoveLeft(glm::vec3& frontDirection, glm::vec3& upDirection)
{
	auto moveLeft = [&]()
	{
		m_transformPtr->translatePostMultiply(glm::normalize(glm::cross(frontDirection, upDirection)) * -movementSpeed);
	};
	EMS::getInstance().add(NoReturnEvent::moveLeft, moveLeft);
}

void PlayerControlledMotion::registerMoveRight(glm::vec3& frontDirection, glm::vec3& upDirection)
{
	auto moveRight = [&]()
	{
		m_transformPtr->translatePostMultiply(glm::normalize(glm::cross(frontDirection, upDirection)) * movementSpeed);
	};
	EMS::getInstance().add(NoReturnEvent::moveRight, moveRight);
}

void PlayerControlledMotion::registerMoveDown(glm::vec3& upDirection)
{
	auto moveDown = [&]()
	{
		m_transformPtr->translatePostMultiply(-movementSpeed * upDirection);
	};
	EMS::getInstance().add(NoReturnEvent::moveDown, moveDown);
}


void PlayerControlledMotion::registerJump()
{
	auto jump = [&]()
	{
		if(canJump)
		{
			m_jumpTarget = m_transformPtr->getPosition().y + jumpHeight;
			jumping = true;
			canJump = false;
		}
	};
	EMS::getInstance().add(NoReturnEvent::jump, jump);
}

void PlayerControlledMotion::updateJump(glm::vec3 position, glm::vec3& upDirection)
{
	m_jumpDecay = (m_jumpTarget - position.y)*jumpSpeed;
	if(jumping)
	{
		if(position.y < m_jumpTarget)
			position += (jumpSpeed + m_jumpDecay/3) * upDirection;
		else
			jumping = false;
	}
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
	};

	EMS::getInstance().add(NoReturnEvent::xyLook, xyLook);
}