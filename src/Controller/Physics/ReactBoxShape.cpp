//ReactBoxShape.cpp - Axis Aligned Bounding Box

#include "Controller/Physics/ReactBoxShape.hpp"
void ReactBoxShape::CreateBoxShape(glm::vec3 bounds, reactphysics3d::PhysicsCommon &physicsCommon)
{
    reactphysics3d::Vector3 extent(bounds.x,bounds.y,bounds.z);
    shape = physicsCommon.createBoxShape(extent);
    shapeID = shape->getId();

}
void ReactBoxShape::DeleteShape(reactphysics3d::PhysicsCommon &physicsCommon){
    physicsCommon.destroyBoxShape(static_cast<rp3d::BoxShape*>(shape));
}