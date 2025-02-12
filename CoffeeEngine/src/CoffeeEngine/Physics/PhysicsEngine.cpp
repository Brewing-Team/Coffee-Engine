#include "PhysicsEngine.h"
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Math/Real.h>
#include <Jolt/RegisterTypes.h>

using namespace JPH::literals;

namespace Coffee {

    JPH::PhysicsSystem PhysicsEngine::m_PhysicsSystem;
    JPH::BodyInterface* PhysicsEngine::m_BodyInterface;
    JPH::TempAllocatorImpl* PhysicsEngine::temp_allocator;
	JPH::JobSystemThreadPool* PhysicsEngine::job_system;

    // Layer filters
    BPLayerInterfaceImpl PhysicsEngine::broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilterImpl PhysicsEngine::object_vs_broadphase_layer_filter;
    ObjectLayerPairFilterImpl PhysicsEngine::object_vs_object_layer_filter;

    // Listeners
    MyBodyActivationListener PhysicsEngine::body_activation_listener;
    MyContactListener PhysicsEngine::contact_listener;

    PhysicsEngine::PhysicsEngine()
    {
    }

    PhysicsEngine::~PhysicsEngine()
    {
    }

    void PhysicsEngine::Init()
    {
        JPH::RegisterDefaultAllocator();

        JPH::Trace = TraceImpl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

        JPH::Factory::sInstance = new JPH::Factory();

        JPH::RegisterTypes();

        temp_allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
        job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

        const uint cMaxBodies = 1024;
        const uint cNumBodyMutexes = 0;
        const uint cMaxBodyPairs = 1024;
        const uint cMaxContactConstraints = 1024;

        m_PhysicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	    m_PhysicsSystem.SetBodyActivationListener(&body_activation_listener);

	    m_PhysicsSystem.SetContactListener(&contact_listener);

        m_BodyInterface = &m_PhysicsSystem.GetBodyInterface();

        /* // Creation of physics objects (only for testing purposes)

        // Next we can create a rigid body to serve as the floor, we make a large box
        // Create the settings for the collision volume (the shape).
        // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
        JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));

        floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

        // Create the shape
        JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
        JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

        // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
        JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);

        // Create the actual rigid body
        JPH::Body *floor = m_BodyInterface->CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

        // Add it to the world
        m_BodyInterface->AddBody(floor->GetID(), JPH::EActivation::DontActivate);

        // Now create a dynamic body to bounce on the floor
        // Note that this uses the shorthand version of creating and adding a body to the world
        JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0_r, 2.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
        JPH::BodyID sphere_id = m_BodyInterface->CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

        // Now you can interact with the dynamic body, in this case we're going to give it a velocity.
        // (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
        m_BodyInterface->SetLinearVelocity(sphere_id, JPH::Vec3(0.0f, -5.0f, 0.0f)); */

    }

    void PhysicsEngine::Shutdown()
    {
    }

    void PhysicsEngine::Update(float dt)
    {
        const float cDeltaTime = 1.0f / 60.0f;

        // m_PhysicsSystem.OptimizeBroadPhase();

        // Update the physics system

        const int cCollisionSteps = 1;

        m_PhysicsSystem.Update(cDeltaTime, cCollisionSteps, temp_allocator, job_system);
    }

}