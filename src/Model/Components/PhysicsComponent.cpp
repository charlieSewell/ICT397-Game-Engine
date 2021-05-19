//PhysicsComponent.cpp - manages physics

#include "PhysicsComponent.hpp"
PhysicsComponent::PhysicsComponent(Transform &transform)
	: m_transformPtr(&transform)
{

}

void PhysicsComponent::updatePhysics(float &movementSpeed, float jumpSpeed)
{
    auto& physManager = PhysicsSystem::getInstance();
    rp3d::Vector3 temp = physManager.getCollider(colliderID)->getTransform().getPosition();
    m_transformPtr->setPosition(ReactMath::r3pdVecToGlm(temp));

}

void PhysicsComponent::registerPhysicsToggle()
{
	/*auto togglePhysicsReleased = [&]()
	{	
		if(m_physicsTogglePressed)
			m_physicsTogglePressed = false;
	};
	EMS::getInstance().add(NoReturnEvent::togglePhysicsReleased, togglePhysicsReleased);

	auto togglePhysicsPressed = [&]()
	{
		if(!m_physicsTogglePressed)
		{
			m_physicsActive = !m_physicsActive;
			m_physicsTogglePressed = true;
		}
	};
	EMS::getInstance().add(NoReturnEvent::togglePhysicsPressed, togglePhysicsPressed);
	*/
}

//easy to implement if needed
/*void PhysicsComponent::registerBoundingSphere(glm::vec3 *position, double radius)
{
	m_colliderID = PhysicsSystem::getInstance().addBoundingSphere(position, radius);
}*/

void PhysicsComponent::registerAABB(float width, float length, float height)
{
	colliderID = PhysicsSystem::getInstance().addAABB(m_transformPtr, width, length, height);
}
void PhysicsComponent::registerSphere(float radius)
{
    colliderID = PhysicsSystem::getInstance().addSphere(m_transformPtr,radius);
}