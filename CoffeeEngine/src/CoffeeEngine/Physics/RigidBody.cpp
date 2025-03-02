#include "RigidBody.h"

namespace Coffee {

    Ref<RigidBody> RigidBody::Create(const Properties& props, const Ref<Collider>& collider) {
        auto rb = CreateRef<RigidBody>();
        rb->Initialize(props, collider);
        return rb;
    }

    void RigidBody::Initialize(const Properties& props, const Ref<Collider>& collider) {
        m_Properties = props;
        m_Collider = collider;

        btTransform transform;
        transform.setIdentity();
        m_MotionState = new btDefaultMotionState(transform);

        btVector3 localInertia(0, 0, 0);
        if (props.type != Type::Static)
        {
            m_Collider->getShape()->calculateLocalInertia(props.mass, localInertia);
        }

        const btRigidBody::btRigidBodyConstructionInfo rbInfo(
            props.mass,
            m_MotionState,
            m_Collider->getShape(),
            localInertia
        );

        m_Body = new btRigidBody(rbInfo);
        m_Body->setActivationState(DISABLE_DEACTIVATION);
        m_Body->setUserPointer(nullptr);

        if (props.type == Type::Static) {
            m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
            m_Body->setMassProps(0, btVector3(0, 0, 0)); // Zero mass for static objects
        }

        if (props.freezeY) {
            m_Body->setLinearFactor(btVector3(1, 0, 1));
        }
    }

    RigidBody::~RigidBody() {
        if (m_Body) {
            delete m_Body->getMotionState();
            delete m_Body;
        }

        if (m_Collider) {
            m_Collider.reset();
        }
    }

    void RigidBody::SetPosition(const glm::vec3& position) const
    {
        btTransform transform = m_Body->getWorldTransform();
        transform.setOrigin(btVector3(position.x, position.y, position.z));
        m_Body->setWorldTransform(transform);
    }

    void RigidBody::SetVelocity(const glm::vec3& velocity) const
    {
        m_Body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    }

    glm::vec3 RigidBody::GetPosition() const {
        btVector3 pos = m_Body->getWorldTransform().getOrigin();
        return {pos.x(), pos.y(), pos.z()};
    }

    void RigidBody::ApplyForce(const glm::vec3& force) const
    {
        m_Body->activate(true);
        m_Body->applyCentralForce(btVector3(force.x, force.y, force.z));
    }

    void RigidBody::SetRotation(const glm::vec3& rotation) const
    {
        btTransform transform = m_Body->getWorldTransform();
        btQuaternion quat;
        quat.setEulerZYX(rotation.z, rotation.y, rotation.x);
        transform.setRotation(quat);
        m_Body->setWorldTransform(transform);
    }

    glm::vec3 RigidBody::GetRotation() const
    {
        const btTransform transform = m_Body->getWorldTransform();
        const btQuaternion quat = transform.getRotation();
        btScalar x, y, z;
        quat.getEulerZYX(z, y, x);
        return {x, y, z};
    }

    glm::vec3 RigidBody::GetVelocity() const
    {
        btVector3 vel = m_Body->getLinearVelocity();
        return {vel.x(), vel.y(), vel.z()};
    }

    void RigidBody::ApplyImpulse(const glm::vec3& impulse) const
    {
        m_Body->activate(true);
        m_Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
    }

    void RigidBody::SetTrigger(const bool isTrigger)
    {
        m_Properties.isTrigger = isTrigger;
        if (isTrigger)
        {
            m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }
        else
        {
            m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }
    }

    void RigidBody::ResetVelocity() const
    {
        // Check this function (ResetVelocity(); bullet?)
        m_Body->setLinearVelocity(btVector3(0, 0, 0));
        m_Body->setAngularVelocity(btVector3(0, 0, 0));
    }

    void RigidBody::ClearForces() const
    {
        m_Body->clearForces();
    }
} // namespace Coffee