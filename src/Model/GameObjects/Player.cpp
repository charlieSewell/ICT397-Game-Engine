
#include "Player.hpp"
#include "Controller/GameObjectManager.hpp"
Player::Player()
	: m_camera(Camera()), m_movement(PlayerControlledMotion(m_transform)), m_physics(m_transform)
{

	m_movement.movementSpeed = 2000.0f;

	m_movement.lookSensitivity = 0.05f;
	m_movement.jumpSpeed = 6.0f;
	//m_mass = 0.025f;
	registerPosition();
	registerColliderID();
	m_movement.registerAllMovement(m_camera.m_frontDirection, m_camera.m_upDirection);
	m_physics.registerPhysicsToggle();

    health = 100;
    shields = 100;
}

Player::~Player() {}

void Player::draw() 
{
    gun.draw();
}

void Player::update(float dt)
{
	if(m_physics.m_physicsActive)
	{
        if(onBox)
        {
            m_movement.canJump = true;
        }
        else{
            m_movement.canJump = false;
        }
        if (m_physics.getCollider()->GetLinearVelocity().y > m_movement.jumpSpeed) 
        {
            m_physics.getCollider()->SetLinearVelocity(
                glm::vec3(m_physics.getCollider()->GetLinearVelocity().x, m_movement.jumpSpeed,
                          m_physics.getCollider()->GetLinearVelocity().z));
        }   
        if(m_movement.canJump)
        {
            m_physics.getCollider()->SetLinearVelocity(glm::vec3(0.0));
            m_physics.getCollider()->SetAngularVelocity(glm::vec3(0.0));

            if (m_movement.updateVector != glm::vec3{})
            { 
                
                m_physics.getCollider()->ApplyForceToCentre(glm::normalize(glm::vec3(m_movement.updateVector)) * m_movement.movementSpeed * dt);

            }
        }
    }
	else
	{
        if (m_movement.updateVector != glm::vec3{})
        {
            m_movement.canJump = true;
            m_physics.getCollider()->SetPosition(m_physics.getCollider()->GetPosition()+m_movement.updateVector);

        }
	}


    m_physics.updatePhysics(m_movement.movementSpeed, m_movement.jumpSpeed);
    m_movement.updateVector = glm::vec3{};

	gun.update(m_transform, m_camera.m_frontDirection);
	m_camera.m_position = glm::vec3(m_transform.getPosition().x, m_transform.getPosition().y + 3, m_transform.getPosition().z);		//TODO: make this better

	if(gun.getIsFiring() && gun.canFire)
	{
		int targetID = rayCaster.CastRay(m_camera.m_position, m_camera.m_frontDirection, 200);
		if(targetID != -1 && GameObjectManager::getInstance().getNPC(targetID))
		{
			//GameObjectManager::getInstance().getNPC(targetID)->hit = true;
			if(GameObjectManager::getInstance().getNPC(targetID)->health < 0)		//dead
			{
				GameObjectManager::getInstance().DeleteGameObject(targetID);
				gun.setReserveAmmo(gun.getReserveAmmo() + 15);
			}
			else
				GameObjectManager::getInstance().getNPC(targetID)->hit = true;
		}
	}
	
	gun.getWeaponAnimation()->setCurrentFrame(dt);
    gun.update(m_transform, m_camera.m_frontDirection);
    LuaManager::getInstance().runScript("content/Scripts/playerLogic.lua");
    LuaManager::getInstance().runScript("content/Scripts/gunLogic.lua");
    onBox = false;

	if(hit)
		takeDamage(dt);
}

void Player::setCollider(float width, float length, float height)
{
    m_physics.registerSphere(ID,1);
    m_physics.getCollider()->SetMass(0.1);
    m_physics.getCollider()->SetIsAllowedToSleep(false);
    m_physics.getCollider()->SetFrictionCoefficient(0.6);
    m_physics.getCollider()->SetAngularDamping(0.6);
    m_physics.getCollider()->SetLinearDamping(0.6);
    m_physics.getCollider()->SetRollingResistance(0.8);
    m_physics.getCollider()->SetBounciness(0);
    rayCaster.setOwnColliderID( m_physics.getCollider()->getColliderID());
}

void Player::setHealth(int h) 
{
    health = h;
}

int Player::getHealth() 
{
    return health;
}

void Player::setShields(int s) 
{
    shields = s;
}

int Player::getShields() 
{
    return shields;
}

void Player::registerPosition()
{
	auto getPlayerPosition = [&]()
	{
		return m_transform.getPosition();
	};

	EMS::getInstance().add(ReturnVec3Event::getPlayerPosition, getPlayerPosition);
}

void Player::registerColliderID()
{
	auto getPlayerColliderID = [&]()
	{
		return m_physics.getCollider()->getColliderID();
	};

	EMS::getInstance().add(ReturnIntEvent::getPlayerColliderID, getPlayerColliderID);
}

void Player::registerClass()
{
	PlayerControlledMotion::registerClass();
	Weapon::registerClass();
	PhysicsComponent::registerPhysicsComponent();
	luabridge::getGlobalNamespace(LuaManager::getInstance().getState())
		.deriveClass<Player, GameObject>("Player")
		.addProperty("movement", &Player::m_movement)
		.addProperty("health", &Player::health, true)
		.addProperty("shields", &Player::shields, true)
		.addProperty("gun", &Player::gun, true)
		.addProperty("physics",&Player::m_physics,true)
		.addProperty("onGround",&Player::onBox,true)
		.addProperty("hit", &Player::hit, true)
		.endClass();
}

void Player::takeDamage(float dt)
{
	if(takingDamage > dt * 60)
	{
		takingDamage = 0;
		if(shields > 0)
			shields -= 5;
		else
			health -= 5;
		hit = false;
	}
	else
		takingDamage += dt;

	//if(health <= 0);
}