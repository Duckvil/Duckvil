#include <cuda_runtime_api.h>
#include <cstdint>

namespace Duckvil { namespace Graphics { namespace RayTracer {

    struct Camera
    {
        float3 m_f3Position;
        float3 m_f3LookAt;
        float3 m_f3Up;
        float m_fLength = 1.f;
        float m_fHorizontalSize;
        float m_fAspectRatio;

        float3 m_f3AlignmentVector;
        float3 m_f3ProjectionScreenU;
        float3 m_f3ProjectionScreenV;
        float3 m_f3ProjectionScreenCentre;
    };

    enum ObjectType : uint8_t
    {
        sphere,
        plane,
        mesh
    };

    enum MaterialType : uint8_t
    {
        none,
        specular
    };

    struct ObjectStream
    {
        uint32_t m_uiSize;
    };

    struct ObjectChunk
    {
        ObjectType m_type;
        uint32_t m_uiSize;
        MaterialType m_material = MaterialType::none;
    };

    struct Sphere
    {
        float3 m_f3Color;
    };

    struct Plane
    {
        float3 m_f3Color;
    };

    // struct Mesh
    // {
    //     float3 m_f3Color;
    //     uint32_t m_uiTriangleCount;
    //     float3* m_pTriangles;
    //     uint32_t* m_pIndices;
    //     float3* m_pNormals;
    //     float2* m_pTexCoords;
    // };

    struct SpecularMaterial
    {
        float m_fReflectivity;
        float m_fShininess;
        float3 m_f3Color;
        float m_fTranslucency;
        float m_fIOR;
    };



    enum LightType
    {
        point_light
    };

    struct LightStream
    {
        uint32_t m_uiSize;
    };

    struct LightChunk
    {
        LightType m_type;
        // object data
    };

    struct PointLight
    {
        float3 m_f3Color;
        float3 m_f3Location;
        float m_fIntensity;
    };

}}}