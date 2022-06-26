#pragma once

#include "Engine/ISystem.h"

#include "Editor/Widget.h"

#include "glew/include/GL/glew.h"

#include "RayTracer/Objects.h"

#include "glm/glm.hpp"

#include "Event/BufferedPool.h"

#include "Window/Events/SetMousePositionEvent.h"

#include "RayTracer/RayTracer.generated.h"

namespace Duckvil { namespace Graphics { namespace RayTracer {

    struct Transform
    {
        glm::mat4 m_mat4ForwardTransform;
        glm::mat4 m_mat4BackwardTransform;
        glm::mat3 m_mat3LinearTransform;
    };

    // struct Camera
    // {
    //     float3 m_f3Position;
    //     float3 m_f3LookAt;
    //     float3 m_f3Up;
    //     float m_fLength;
    //     float m_fHorizontalSize;
    //     float m_fAspectRatio;

    //     float3 m_f3AlignmentVector;
    //     float3 m_f3ProjectionScreenU;
    //     float3 m_f3ProjectionScreenV;
    //     float3 m_f3ProjectionScreenCentre;

    //     Camera();
    // };

    class RayTracer : public ISystem, public Editor::Widget
    {
        DUCKVIL_GENERATED_BODY
    private:
        uint32_t image_width = 1920;
        uint32_t image_height = 1080;

        uint32_t* image_data;
        GLuint image_texture;

        PlugNPlay::__module _module;
        PlugNPlay::__module_information _kernelModule;

        void (*ray_trace)(unsigned int* _aPixels, unsigned int _uiWidth, unsigned int _uiHeight, Camera* _pCamera, ObjectStream* _pObjectStream, LightStream* _pLightStream);

        void* m_pDeviceMemory;

        double elapsed_time_ms;
        double elapsed_time_ms2;
        double elapsed_time_ms3;

        ObjectStream* m_dpObjectStream;
        std::vector<uint8_t> m_vObjectData;

        // LightStream* m_pLightStream;
        LightStream* m_dpLightStream;
        std::vector<uint8_t> m_vLightData;
        // uint32_t m_uiLightStreamSize;

        Camera m_camera;
        Camera* m_dpCamera;

        bool m_bRender;

        glm::vec3 m_f3Object1Position;
        uint32_t m_pObject1Offset;

        glm::vec3 m_f3CameraUp;
        glm::vec3 m_f3CameraForward;
        glm::vec3 m_f3CameraPosition;

        Event::Pool<Event::mode::buffered>* m_pWindowEventPool;

        Window::SetMousePositionEvent m_setMousePosition;

        float m_fCameraUp;
        float m_fCameraRight;

        void RayTrace();

        uint32_t Add(uint8_t** _pCurr, const Transform& _t, const Sphere& _s);
        uint32_t Add(uint8_t** _pCurr, const Transform& _t, const Sphere& _s, const SpecularMaterial& _m);
        uint32_t Add(uint8_t** _pCurr, const Transform& _t, const Plane& _o);
        uint32_t Add(uint8_t** _pCurr, const Transform& _t, const Plane& _o, const SpecularMaterial& _m);
        void Add(uint8_t** _pCurr, const PointLight& _l);

    public:
        RayTracer();
        RayTracer(const Memory::FreeList& _heap, DUCKVIL_ARGUMENT("Window") Event::Pool<Event::mode::buffered>* _pEventPool);
        ~RayTracer();

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}}