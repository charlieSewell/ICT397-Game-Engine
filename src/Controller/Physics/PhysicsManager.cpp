
#include "PhysicsManager.hpp"
PhysicsManager::PhysicsManager()
{
    physicsWorld = physicsCommon.createPhysicsWorld();

	timeStep = 1.0f / 60.0f;
}

PhysicsManager::~PhysicsManager()
{
	physicsWorld->destroyCollisionBody(playerCollision);
	physicsWorld->destroyCollisionBody(terrainCollision);
    physicsCommon.destroyPhysicsWorld(physicsWorld);
	delete[] heightFieldArray;
}

void PhysicsManager::update(Player player)
{
	physicsWorld->update(timeStep);

	rp3d::Vector3 position(player.getPosition().x, player.getPosition().y, player.getPosition().z);
	rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(position, orientation);
	playerCollision->setTransform(transform);

	//display the position of the body

	if(physicsWorld->testOverlap(playerCollision, terrainCollision))
		std::cout << "Hell yeah\n";
	//else
		//std::cout << "No collide motherfucker\n";

	//std::cout << "Body position: = " << terrainCollision->
	//std::cout << "Body position: = " << playerCollision->getTransform().getPosition().x << ", " << playerCollision->getTransform().getPosition().y << ", " << playerCollision->getTransform().getPosition().z << "\n";
	//std::cout << "Body position: = " << terrainCollision->getTransform().getPosition().x << ", " << terrainCollision->getTransform().getPosition().y << ", " << terrainCollision->getTransform().getPosition().z << "\n";
}

void PhysicsManager::test(Player player)
{
	addPlayer(player);
	addTerrain();
}

void PhysicsManager::addPlayer(Player player)
{
	rp3d::Vector3 position(player.getPosition().x, player.getPosition().y, player.getPosition().z);
	rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(position, orientation);
	playerCollision = physicsWorld->createCollisionBody(transform);

	rp3d::CapsuleShape *capsule = physicsCommon.createCapsuleShape(1.0, 2.0);
	transform = rp3d::Transform::identity();
	//rp3d::Collider *collider;
	playerCollision->addCollider(capsule, transform);

}


void PhysicsManager::addTerrain()
{
	rp3d::Vector3 position(256, 128, 256);
	//rp3d::Vector3 position(256, 10, 256);
	rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(position, orientation);
	terrainCollision = physicsWorld->createCollisionBody(transform);

	/*
	rp3d::BoxShape *box = physicsCommon.createBoxShape(rp3d::Vector3(250.0, 1.0, 250.0));
	transform = rp3d::Transform::identity();
	rp3d::Collider* collider;
	collider = terrainCollision->addCollider(box, transform);
	*/

	int i = 0;
	for(int x=0; x<512; ++x)
	{
		for(int z=0; z<512; ++z)
		{
			//heightFieldArray.push_back(TerrainFactory::getInstance().heightVals[z][x]);
			heightFieldArray[x * 512 + z] = TerrainFactory::getInstance().heightVals.data()[z].data()[x];
			//heightFieldArray[i] = TerrainFactory::getInstance().heightVals.data()[z].data()[x];
		}
	}

	heightField = physicsCommon.createHeightFieldShape(512, 512, 0, 256, heightFieldArray, rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);
	transform = rp3d::Transform::identity();
	//terrainCollider = terrainCollision->addCollider(heightField, transform);
	terrainCollision->addCollider(heightField, transform);
	
	std::cout << heightFieldArray[511*512+511] << ", " << /*TerrainFactory::getInstance().heightVals.data()->data()[513] <<*/ ", " << TerrainFactory::getInstance().heightVals[511][511] << "\n";

}