#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "Collider.h"

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Coffee {

    class RigidBody
    {
    public:
        enum class Type
        {
            Static,
            Dynamic,
            Kinematic
        };

        struct Properties
        {
            public:
                Type type = Type::Dynamic;
                float mass = 1.0f;
                bool useGravity = true;
                bool freezeY = false;
                bool isTrigger = false;
                glm::vec3 velocity = {0.0f, 0.0f, 0.0f};

            private:
                friend class cereal::access;

                template<class Archive>
                void save(Archive& archive) const {
                    archive(
                        CEREAL_NVP(type),
                        CEREAL_NVP(mass),
                        CEREAL_NVP(useGravity),
                        CEREAL_NVP(freezeY),
                        CEREAL_NVP(isTrigger),
                        CEREAL_NVP(velocity)
                    );
                }

                template<class Archive>
                void load(Archive& archive) {
                    archive(
                        CEREAL_NVP(type),
                        CEREAL_NVP(mass),
                        CEREAL_NVP(useGravity),
                        CEREAL_NVP(freezeY),
                        CEREAL_NVP(isTrigger),
                        CEREAL_NVP(velocity)
                    );
                }
        };

        static Ref<RigidBody> Create(const Properties& props, const Ref<Collider>& collider);
        RigidBody() = default;
        ~RigidBody();

        // Kinematics
        void SetPosition(const glm::vec3& position) const;
        void SetRotation(const glm::vec3& rotation) const;
        void SetVelocity(const glm::vec3& velocity) const;
        glm::vec3 GetPosition() const;
        glm::vec3 GetRotation() const;
        glm::vec3 GetVelocity() const;

        // Physics
        void ApplyForce(const glm::vec3& force) const;
        void ApplyImpulse(const glm::vec3& impulse) const;
        void SetTrigger(bool isTrigger);
        void ResetVelocity() const;
        void ClearForces() const;

        // Internal use
        btRigidBody* GetNativeBody() const { return m_Body; }

    private:
        void Initialize(const Properties& props, const Ref<Collider>& collider);

        btRigidBody* m_Body = nullptr;
        btMotionState* m_MotionState = nullptr;
        Ref<Collider> m_Collider;
        Properties m_Properties;


    private:
        friend class cereal::access;
        template<class Archive>
        void save(Archive& archive) const {
            archive(
                CEREAL_NVP(m_Properties),
                CEREAL_NVP(m_Collider)
            );
        }

        template<class Archive> 
        void load(Archive& archive) {
            archive(
                CEREAL_NVP(m_Properties),
                CEREAL_NVP(m_Collider)
            );
            Initialize(m_Properties, m_Collider);
        }
    };

} // namespace Coffee