#include "RigidBody.h"
#include "PhysicsEngine.h"

namespace Coffee {
    
    JPH::ObjectLayer ConvertRigidBodyMotionTypeToObjectLayer(RigidBodyConfig::MotionType motionType)
    {
        switch (motionType)
        {
            case RigidBodyConfig::MotionType::Static: return Layers::NON_MOVING;
            case RigidBodyConfig::MotionType::Dynamic: return Layers::MOVING;
            case RigidBodyConfig::MotionType::Kinematic: return Layers::MOVING;
        }

        return Layers::NON_MOVING;
    }

    RigidBody::RigidBody(const RigidBodyConfig& config)
    {
        JPH::BodyCreationSettings settings;
        settings.mMotionType = static_cast<JPH::EMotionType>(config.motionType);
        settings.mPosition = JPH::Vec3(config.position.x, config.position.y, config.position.z);
        settings.mRotation = JPH::Quat(config.rotation.w, config.rotation.x, config.rotation.y, config.rotation.z);
        settings.mLinearVelocity = JPH::Vec3(config.linearVelocity.x, config.linearVelocity.y, config.linearVelocity.z);
        settings.mAngularVelocity = JPH::Vec3(config.angularVelocity.x, config.angularVelocity.y, config.angularVelocity.z);
        settings.SetShape(config.collider.GetShape());
        settings.mObjectLayer = ConvertRigidBodyMotionTypeToObjectLayer(config.motionType);
        settings.mGravityFactor = config.gravityFactor;
        settings.mLinearDamping = config.linearDamping;
        settings.mAngularDamping = config.angularDamping;
        //settings.mFreezeTranslation = JPH::EAxisBits(config.FreezePositionX, config.FreezePositionY, config.FreezePositionZ);
        //settings.mFreezeRotation = JPH::EAxisBits(config.FreezeRotationX, config.FreezeRotationY, config.FreezeRotationZ);

        m_Body = PhysicsEngine::m_BodyInterface->CreateBody(settings);
        m_BodyID = m_Body->GetID();

        PhysicsEngine::m_BodyInterface->AddBody(m_BodyID, JPH::EActivation::Activate);
    }

    RigidBody::~RigidBody()
    {
        PhysicsEngine::m_BodyInterface->DestroyBody(m_BodyID);
    }

    void RigidBody::SetPosition(const glm::vec3& position)
    {
        PhysicsEngine::m_BodyInterface->SetPosition(m_BodyID, JPH::Vec3(position.x, position.y, position.z), JPH::EActivation::Activate);
    }

    glm::vec3 RigidBody::GetPosition()
    {
        return glm::vec3(m_Body->GetPosition().GetX(), m_Body->GetPosition().GetY(), m_Body->GetPosition().GetZ());
    }

    void RigidBody::SetRotation(const glm::quat& rotation)
    {
        PhysicsEngine::m_BodyInterface->SetRotation(m_BodyID, JPH::Quat(rotation.w, rotation.x, rotation.y, rotation.z), JPH::EActivation::Activate);
    }

    void RigidBody::SetLinearVelocity(const glm::vec3& linearVelocity)
    {
        PhysicsEngine::m_BodyInterface->SetLinearVelocity(m_BodyID, JPH::Vec3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
    }

    void RigidBody::SetAngularVelocity(const glm::vec3& angularVelocity)
    {
        PhysicsEngine::m_BodyInterface->SetAngularVelocity(m_BodyID, JPH::Vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
    }

    void RigidBody::SetGravityFactor(float gravityFactor)
    {
        PhysicsEngine::m_BodyInterface->SetGravityFactor(m_BodyID, gravityFactor);
    }

    void RigidBody::SetFriction(float friction)
    {
        PhysicsEngine::m_BodyInterface->SetFriction(m_BodyID, friction);
    }
}