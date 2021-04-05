//Entity.hpp

#pragma once
#include <glm/glm.hpp>

#include "View/Renderer/Shader.hpp"

enum class GameObjectType
{
	player, staticObject, npc
};

class GameObject
{
public:
    virtual glm::vec3 getPosition() const;
	virtual void setPosition(glm::vec3 position);

	virtual void setCollider(float width, float length, float height) = 0;

    virtual glm::mat4 getViewMatrix() = 0;
	virtual void update();
    virtual void draw(Shader &shader) = 0; 

protected:
	GameObjectType m_EntityType;
	glm::vec3 m_position;
};
