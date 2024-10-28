#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"

namespace Coffee {

    /**
     * Enum class representing different types of primitive meshes.
     */
    enum class PrimitiveType
    {
        Quad,       ///< A quad mesh.
        Cube,       ///< A cube mesh.
        Sphere,     ///< A sphere mesh.
        Plane,      ///< A plane mesh.
        Cylinder,   ///< A cylinder mesh.
        Cone,       ///< A cone mesh.
        Torus,      ///< A torus mesh.
        Capsule,    ///< A capsule mesh.
        None        ///< No mesh.
    };

    /**
     * @defgroup scene Scene
     * @{
     */

    /**
     * @brief Class representing different types of primitive meshes.
     * @ingroup scene
     */
    class PrimitiveMesh
    {
    public:

        /*
         *
         * FIXME : This is currently not implemented.
         *
        template <PrimitiveType T, typename... Args>
        static Ref<Mesh> Create(Args&&... args);
        */

    // private:
    /**
     * Creates a quad mesh.
     * @return A reference to the created quad mesh.
     */
        static Ref<Mesh> CreateQuad();

        /**
         * Creates a cube mesh.
         * @param size The size of the cube.
         * @param subdivideW Number of subdivisions along the width.
         * @param subdivideH Number of subdivisions along the height.
         * @param subdivideD Number of subdivisions along the depth.
         * @return A reference to the created cube mesh.
         */
        static Ref<Mesh> CreateCube(const glm::vec3& size = { 1.0f, 1.0f, 1.0f }, int subdivideW = 0, int subdidiveH = 0, int subdivideD = 0);

        /**
         * Creates a sphere mesh.
         * @param radius The radius of the sphere.
         * @param height The height of the sphere.
         * @param radialSegments Number of radial segments.
         * @param rings Number of rings.
         * @param isHemiSphere Whether the sphere is a hemisphere.
         * @return A reference to the created sphere mesh.
         */
        static Ref<Mesh> CreateSphere(float radius = 0.5f, float height = 1.0f, int radialSegments = 64, int rings = 32, bool isHemiSphere = false);

        /**
         * Creates a plane mesh.
         * @param size The size of the plane.
         * @param normal The normal vector of the plane.
         * @return A reference to the created plane mesh.
         */
        static Ref<Mesh> CreatePlane(const glm::vec2& size = { 1.0f, 1.0f }, const glm::vec3& normal = { 0.0f, 1.0f, 0.0f });

        /**
         * Creates a cylinder mesh.
         * @param topRadius The radius of the top of the cylinder.
         * @param bottomRadius The radius of the bottom of the cylinder.
         * @param height The height of the cylinder.
         * @param radialSegments Number of radial segments.
         * @param rings Number of rings.
         * @param capTop Whether to cap the top of the cylinder.
         * @param capBottom Whether to cap the bottom of the cylinder.
         * @return A reference to the created cylinder mesh.
         */
        static Ref<Mesh> CreateCylinder(float topRadius = 0.5f, float bottomRadius = 0.5f, float height = 1.0f, int radialSegments = 64, int rings = 1, bool capTop = true, bool capBottom = true);

        /**
         * Creates a cone mesh.
         * @param radius The radius of the base of the cone.
         * @param height The height of the cone.
         * @param radialSegments Number of radial segments.
         * @param rings Number of rings.
         * @param cap Whether to cap the base of the cone.
         * @return A reference to the created cone mesh.
         */
        static Ref<Mesh> CreateCone(float radius = 0.5f, float height = 1.0f, int radialSegments = 64, int rings = 1, bool cap = true);

        /**
         * Creates a torus mesh.
         * @param innerRadius The inner radius of the torus.
         * @param outerRadius The outer radius of the torus.
         * @param rings Number of rings.
         * @param ringSegments Number of ring segments.
         * @return A reference to the created torus mesh.
         */
        static Ref<Mesh> CreateTorus(float innerRadius = 0.5f, float outerRadius = 1.0f, int rings = 64, int ringSegments = 32);

        /**
         * Creates a capsule mesh.
         * @param radius The radius of the capsule.
         * @param height The height of the capsule.
         * @param radialSegments Number of radial segments.
         * @param rings Number of rings.
         * @return A reference to the created capsule mesh.
         */
        static Ref<Mesh> CreateCapsule(float radius = 1.0f, float height = 4.0f, int radialSegments = 64, int rings = 8);

    };

    // Explicit specialization declaration
    /*
     *
     *  FIXME : This is currently not implemented.
     *
    template <>
    Ref<Mesh> PrimitiveMesh::Create<PrimitiveType::Plane>(const glm::vec2& size, const glm::vec3& normal);
    */

    /** @} */ // end of scene group

}