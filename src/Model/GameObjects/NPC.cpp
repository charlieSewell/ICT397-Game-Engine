#include "NPC.hpp"

NPC::NPC(std::string modelName)
	: GameObject(), m_behaviours(AutomatedBehaviours(m_transform))//, PhysicsComponent()
{
    modelID = ModelManager::getInstance().GetModelID(modelName);
    m_transform.setPosition(glm::vec3(0,0,0));
    m_transform.setScale(glm::vec3(0,0,0));
    animator.addModel(ModelManager::getInstance().GetModel(modelID));
	animator.setAnimation("ZombieWalk");
}

void NPC::draw() 
{
	// SUDO STATE MACHINE
	glm::vec3 targetPosition = EMS::getInstance().fire(ReturnVec3Event::getPlayerPosition);

	m_behaviours.rotationSpeed = 0.005;
	//m_behaviours.rotationSpeed = 
	//m_behaviours.seek(targetPosition);
	m_behaviours.wander();
	if(glm::distance(m_transform.getPosition(), targetPosition) > 10)
		m_behaviours.accelerate(0.01f);
	else
		m_behaviours.decelerate();

	m_transform.setPosition(glm::vec3(m_transform.getPosition().x, PhysicsManager::getInstance().checkTerrainHeight(m_transform.getPosition()), m_transform.getPosition().z));
	
	ModelManager::getInstance().DrawModel(modelID, m_transform.getMatrix(),animator.finalTransforms);
}

void NPC::setCollider(float width, float length, float height)
{
	//registerAABB(&m_position, width, length, height);
}

void NPC::test()
{
	std::cout << "Cool\n";
}

void NPC::registerClass()
{
	luabridge::getGlobalNamespace(LuaManager::getInstance().getState())
		.deriveClass<NPC, GameObject>("NPC")
		.addFunction("test", &NPC::test)
		.addFunction("sayHello", SayHello);
		.endClass();
	std::cout << "registered\n";
}

void NPC::update(float dt)
{
    animator.BoneTransform(dt);
}
