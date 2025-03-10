#pragma once

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include <btBulletDynamicsCommon.h>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>

namespace Coffee {

    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;

    class Collider {
    protected:
        btCollisionShape* shape = nullptr;

    public:
        virtual ~Collider() {
            delete shape;
        }

        btCollisionShape* getShape() const {
            return shape;
        }

        template <class Archive> void save(Archive& archive) const;
        template <class Archive> void load(Archive& archive);
    };

    class BoxCollider final : public Collider {
    public:
        BoxCollider() = default;
        explicit BoxCollider(const glm::vec3& size) {
            shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
        }

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const {
            archive(cereal::base_class<Collider>(this));
        }

        template<class Archive>
        void load(Archive& archive) {
            archive(cereal::base_class<Collider>(this));
        }
    };

    class SphereCollider final : public Collider {
    public:
        SphereCollider() = default;
        explicit SphereCollider(const float radius) {
            shape = new btSphereShape(radius);
        }

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const {
            archive(cereal::base_class<Collider>(this));
        }

        template<class Archive>
        void load(Archive& archive) {
            archive(cereal::base_class<Collider>(this));
        }
    };

    class CapsuleCollider final : public Collider {
    public:
        CapsuleCollider() = default;
        explicit CapsuleCollider(const float radius, const float height) {
            shape = new btCapsuleShape(radius, height * 0.5f);
        }

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const {
            archive(cereal::base_class<Collider>(this));
        }

        template<class Archive>
        void load(Archive& archive) {
            archive(cereal::base_class<Collider>(this));
        }
    };

    template<class Archive>
    void Collider::save(Archive& archive) const {
        btCollisionShape* shape = getShape();
        int shapeType = shape ? shape->getShapeType() : -1;
        archive(cereal::make_nvp("ShapeType", shapeType));
    
        if (shape) {
            switch (shapeType) {
                case BOX_SHAPE_PROXYTYPE: {
                    auto* boxShape = static_cast<btBoxShape*>(shape);
                    btVector3 halfExtents = boxShape->getHalfExtentsWithoutMargin();
                    glm::vec3 size(halfExtents.x() * 2.0f, halfExtents.y() * 2.0f, halfExtents.z() * 2.0f);
                    archive(cereal::make_nvp("Size", size));
                    break;
                }
                case SPHERE_SHAPE_PROXYTYPE: {
                    auto* sphereShape = static_cast<btSphereShape*>(shape);
                    float radius = sphereShape->getRadius();
                    archive(cereal::make_nvp("Radius", radius));
                    break;
                }
                case CAPSULE_SHAPE_PROXYTYPE: {
                    auto* capsuleShape = static_cast<btCapsuleShape*>(shape);
                    float radius = capsuleShape->getRadius();
                    float height = capsuleShape->getHalfHeight() * 2.0f;
                    archive(cereal::make_nvp("Radius", radius));
                    archive(cereal::make_nvp("Height", height));
                    break;
                }
            }
        }
    }
    
    template<class Archive>
    void Collider::load(Archive& archive) {
        int shapeType;
        archive(cereal::make_nvp("ShapeType", shapeType));
    
        if (shape) {
            delete shape;
            shape = nullptr;
        }
    
        switch (shapeType) {
            case BOX_SHAPE_PROXYTYPE: {
                glm::vec3 size;
                archive(cereal::make_nvp("Size", size));
                shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
                break;
            }
            case SPHERE_SHAPE_PROXYTYPE: {
                float radius;
                archive(cereal::make_nvp("Radius", radius));
                shape = new btSphereShape(radius);
                break;
            }
            case CAPSULE_SHAPE_PROXYTYPE: {
                float radius, height;
                archive(cereal::make_nvp("Radius", radius));
                archive(cereal::make_nvp("Height", height));
                shape = new btCapsuleShape(radius, height * 0.5f);
                break;
            }
        }
    }

} // namespace Coffee

CEREAL_REGISTER_TYPE(Coffee::BoxCollider)
CEREAL_REGISTER_TYPE(Coffee::SphereCollider)
CEREAL_REGISTER_TYPE(Coffee::CapsuleCollider)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Collider, Coffee::BoxCollider)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Collider, Coffee::SphereCollider)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Collider, Coffee::CapsuleCollider)