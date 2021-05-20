#include "AutomatedBehaviours.hpp"

AutomatedBehaviours::AutomatedBehaviours(Transform& transform)
	: heading(glm::vec3(0)), angle(0), acceleration(0), topSpeed(0),		
	m_transformPtr(&transform), rotationSpeed(0), m_wanderAngle(0), state(0)
{

}

void AutomatedBehaviours::accelerate(float topSpeed)
{
	if (topSpeed > 0 && acceleration < topSpeed)
		acceleration += 0.01;

	if (topSpeed < 0 && acceleration > topSpeed)
		acceleration -= 0.01;

	m_transformPtr->translatePostMultiply(glm::normalize(heading) * acceleration);
}

void AutomatedBehaviours::decelerate()
{
	if (acceleration > 0)
		acceleration -= 0.01;

	if (acceleration < 0)
		acceleration += 0.01;

	m_transformPtr->translatePostMultiply(glm::normalize(heading) * acceleration);
}

void AutomatedBehaviours::seek(glm::vec3 targetPosition)
{
	glm::vec3 targetHeading = (targetPosition - m_transformPtr->getPosition());

	if (angle_XZ(heading) < angle_XZ(targetHeading))  
	{
		if(angle_XZ(targetHeading) - angle_XZ(heading) > rotationSpeed)		// greater than rotation speed as a buffer to stop unecesary rotation
		{
			if(angle_XZ(heading) - angle_XZ(targetHeading) > (glm::pi<float>()))		// if difference is greater than pi, it's quicker to turn the other way
			{
				angle -= rotationSpeed;
				m_transformPtr->rotate(rotationSpeed, glm::vec3(0, 1, 0));			// turn left
			}
			else
			{
				angle += rotationSpeed;
				m_transformPtr->rotate(-rotationSpeed, glm::vec3(0, 1, 0));			// turn right
			}
		}
	}
	else
	{
		if(angle_XZ(heading) - angle_XZ(targetHeading) > rotationSpeed)		// greater than rotation speed as a buffer to stop unecesary rotation
		{
			if (angle_XZ(heading) - angle_XZ(targetHeading) > (glm::pi<float>()))	// if difference is greater than pi, it's quicker to turn the other way
			{
				angle += rotationSpeed;
				m_transformPtr->rotate(-rotationSpeed, glm::vec3(0, 1, 0));			// turn right
			}
			else
			{
				angle -= rotationSpeed;
				//*m_transformPtr = glm::rotate(*m_transformPtr, m_rotationSpeed, glm::vec3(0, 1, 0));		// turn left
				m_transformPtr->rotate(rotationSpeed, glm::vec3(0, 1, 0));			// turn right
			}
		}
	}

	updateHeading();
}

#include <iostream>
void AutomatedBehaviours::wander()
{
	float ringDistance = 10;
	float ringRadius = 5;

	glm::vec3 ringLocation = m_transformPtr->getPosition() + glm::normalize(heading) * ringDistance;

	//rotate
	//theta = Point2D::randomNumber(0, 6.28);  //6.28 = 2pi
	//std::cout << glm::linearRand(m_wanderAngle - 1, m_wanderAngle + 1) << "\n";
	m_wanderAngle = glm::linearRand(m_wanderAngle - 1, m_wanderAngle + 1);  //defines the jitter
	if (m_wanderAngle > glm::pi<float>())
		--m_wanderAngle;
	if (m_wanderAngle < -glm::pi<float>())
		++m_wanderAngle;

	glm::vec3 seekTarget = ringLocation + glm::normalize(heading) * ringRadius;


	//test
	glm::vec3 temp1 = seekTarget - ringLocation;
	glm::vec3 temp2;
	temp2.x = temp1.x * cos(m_wanderAngle) - temp1.z * sin(m_wanderAngle);
	temp2.z = temp1.x * sin(m_wanderAngle) + temp1.z * cos(m_wanderAngle);

	seekTarget = temp2 + ringLocation;

	seek(seekTarget);
}

void AutomatedBehaviours::updateHeading()
{
	if (angle > glm::pi<float>())
		angle = -glm::pi<float>();
	if (angle < -glm::pi<float>())
		angle = glm::pi<float>();

	heading = glm::vec3(cos(angle), 0, sin(angle));
}


// UTILITY FUNCTIONS SHOULD GO IN SEPERATE CLASS //

float AutomatedBehaviours::angle_XZ(const glm::vec3 vector)
{
		return(std::atan2f(vector.z, vector.x));
}

void AutomatedBehaviours::registerClass()
{
	luabridge::getGlobalNamespace(LuaManager::getInstance().getState())
		.beginClass<AutomatedBehaviours>("behaviours")
		.addProperty("rotationSpeed", &AutomatedBehaviours::rotationSpeed, true)
		.addProperty("state", &AutomatedBehaviours::state, true)
		.addFunction("accelerate", &AutomatedBehaviours::accelerate)
		.addFunction("seek", &AutomatedBehaviours::seek)
		.addFunction("wander", &AutomatedBehaviours::wander)
		.endClass();
}