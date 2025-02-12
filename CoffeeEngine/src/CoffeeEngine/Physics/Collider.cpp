#include "Collider.h"
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace Coffee {

    Collider::Collider()
        : m_ShapeConfig(CollisionShapeConfig())
        , m_IsTrigger(false)
    {
    }

    Collider::Collider(const CollisionShapeConfig& shapeConfig, bool isTrigger)
        : m_ShapeConfig(shapeConfig)
        , m_IsTrigger(isTrigger)
    {
        switch (shapeConfig.type)
        {
            case CollisionShapeType::Box:
            {
                JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(shapeConfig.size.x, shapeConfig.size.y, shapeConfig.size.z));
                boxShapeSettings.SetEmbedded(); // ????????

                JPH::ShapeSettings::ShapeResult result = boxShapeSettings.Create();
                m_Shape = result.Get();
            }
        }
    }

}