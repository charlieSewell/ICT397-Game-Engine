//ReactBoxShape.hpp - Axis Alligned Bounding Box

#pragma once

#include "Controller/Physics/ReactShape.hpp"
/**
 * @class ReactBoxShape
 * @brief Class for createing Axis Aligned Bounding Boxes
 */
class ReactBoxShape : public ReactShape
{
    public:
        ReactBoxShape() =default;
        ~ReactBoxShape()=default;
        void CreateBoxShape(glm::vec3 bounds, reactphysics3d::PhysicsCommon &physicsCommon,
                            reactphysics3d::PhysicsWorld *physicsWorld);
        void DeleteShape(reactphysics3d::PhysicsCommon &physicsCommon) override;
    private:

};