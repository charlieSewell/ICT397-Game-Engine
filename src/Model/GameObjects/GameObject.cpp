//GameObject.cpp

#include "GameObject.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 GameObject::getPosition() const
{
	return m_transform[3];
}

glm::mat4 GameObject::getTransform() const
{
    return m_transform;

}

void GameObject::setPosition(glm::vec3 position)
{
	m_transform[3][0] = position.x;
	m_transform[3][1] = position.y;
	m_transform[3][2] = position.z;
}

void GameObject::setLuaPosition(float x,float y,float z)
{
    m_transform = glm::mat4(1.0);
	setPosition(glm::vec3(x, y, z));
    m_transform = glm::scale(m_transform,m_scale);
}

void GameObject::setScale(glm::vec3 scale)
{
    m_scale = scale;
    m_transform = glm::mat4(1.0);
    m_transform = glm::translate(m_transform,m_position);
    m_transform = glm::scale(m_transform,m_scale);
}
void GameObject::setLuaScale(float x,float y,float z)
{
    setScale(glm::vec3(x,y,z));
    m_scale = glm::vec3(x,y,z);
    m_transform = glm::mat4(1.0);
    m_transform = glm::translate(m_transform,m_position);
    m_transform = glm::scale(m_transform,m_scale);
}
void GameObject::update()
{

}

void GameObject::registerClass() 
{
    luabridge::getGlobalNamespace(LuaManager::getInstance().getState())
        .beginClass<GameObject>("GameObject")
            .addFunction("update", &GameObject::update)
            .addFunction("getPosition", &GameObject::getPosition)
            .addFunction("setPosition", &GameObject::setLuaPosition)
            .addFunction("setScale", &GameObject::setLuaScale)
            .addFunction("setCollider", &GameObject::setCollider)
        .endClass();
}