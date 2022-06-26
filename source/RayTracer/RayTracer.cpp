#include "RayTracer/RayTracer.h"

#include "PlugNPlay/Module.h"

#include "imgui/imgui.h"

#include <chrono>

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "glm/gtx/rotate_vector.hpp"

#include "Window/Events/MouseMotionEvent.h"

// #define N 10000000

namespace Duckvil { namespace Graphics { namespace RayTracer {

    // Transform& Transform::operator=(const Transform& _rhs)
    // {
    //     if(&_rhs != this)
    //     {
    //         m_mat4ForwardTransform = _rhs.m_mat4ForwardTransform;
    //         m_mat4BackwardTransform = _rhs.m_mat4BackwardTransform;
    //     }

    //     return *this;
    // }

    // void transform_set(Transform& _t, const glm::mat4& _mat4Forward, const glm::mat4& _mat4Backward)
    // {
    //     _t.m_mat4ForwardTransform = _mat4Forward;
    //     _t.m_mat4BackwardTransform = _mat4Backward;
    //     _t.m_mat3LinearTransform = glm::transpose(_mat4Backward);
    // }

    void transform_set(Transform& _t, const glm::mat4& _mat4Forward)
    {
        _t.m_mat4ForwardTransform = _mat4Forward;
        _t.m_mat4BackwardTransform = glm::inverse(_t.m_mat4ForwardTransform);
        // _t.m_mat3LinearTransform = glm::transpose(_t.m_mat4BackwardTransform);

        glm::mat4 _y = glm::inverse(_mat4Forward);
        // _y = glm::transpose(_y);
        _t.m_mat3LinearTransform = glm::transpose(_t.m_mat4BackwardTransform);
    }

    void transform_set(Transform& _t, const glm::vec3& _p, const glm::vec3& _r, const glm::vec3& _s)
    {
        glm::mat4 _translation = glm::translate(_p);
        glm::mat4 _rotation = glm::eulerAngleYXZ(_r.x, _r.y, _r.z);
        glm::mat4 _scale = glm::scale(_s);

        // _t.m_mat4ForwardTransform = _translation * _rotation * _scale;
        // _t.m_mat4BackwardTransform = glm::inverse(_t.m_mat4ForwardTransform);
        // _t.m_mat3LinearTransform = glm::transpose(_t.m_mat4BackwardTransform);

        // _t = Transform(_translation * _rotation * _scale);

        transform_set(_t, _translation * _rotation * _scale);
    }

    float3 make_float3(float x, float y, float z)
    {
        return float3{ .x = x, .y = y, .z = z };
    }

    float3 operator-(float3 vec1, float3 vec2)
    {
        return make_float3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
    }

    float3 operator+(float3 vec1, float3 vec2)
    {
        return make_float3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
    }

    float3 operator*(float x, float3 vec)
    {
        return make_float3(vec.x * x, vec.y * x, vec.z * x);
    }

    float3 operator*(float3 vec, float x)
    {
        return make_float3(vec.x * x, vec.y * x, vec.z * x);
    }

    float3 normalize(float3 vec)
    {
        float n = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

        return make_float3(vec.x / n, vec.y / n, vec.z / n);
    }

    float3 cross(float3 A, float3 B)
    {
        float3 _res = make_float3(0, 0, 0);

        _res.x = A.y * B.z - A.z * B.y;
        _res.y = A.z * B.x - A.x * B.z;
        _res.z = A.x * B.y - A.y * B.x;

        return _res;
    }

    void update_camera(Camera& _camera, float _fX)
    {
        glm::vec3 _hAxis = glm::vec3(0, 1, 0);

        // _hAxis = glm::cross(_hAxis, glm::vec3(_camera.m_f3LookAt.x, _camera.m_f3LookAt.y, _camera.m_f3LookAt.z));
        // _hAxis = glm::normalize(_hAxis);

        // auto _c = glm::rotate(glm::vec3(_camera.m_f3LookAt.x, _camera.m_f3LookAt.y, _camera.m_f3LookAt.z), _fX, _hAxis);

        // _camera.m_f3LookAt.x = _c.x;
        // _camera.m_f3LookAt.y = _c.y;
        // _camera.m_f3LookAt.z = _c.z;

        // _camera.m_f3LookAt = normalize(_camera.m_f3LookAt);

        // _camera.m_f3Up = normalize(cross(_camera.m_f3LookAt, make_float3(_hAxis.x, _hAxis.y, _hAxis.z)));



        // _hAxis = glm::cross(glm::vec3(0, 1, 0), glm::vec3(_camera.m_f3LookAt.x, _camera.m_f3LookAt.y, _camera.m_f3LookAt.z));
        // _hAxis = glm::normalize(_hAxis);

        // glm::vec3 _r = glm::rotate(glm::vec3(_camera.m_f3LookAt.x, _camera.m_f3LookAt.y, _camera.m_f3LookAt.z), _fX, glm::vec3(0, 1, 0));
        // _r = glm::normalize(_r);

        // _camera.m_f3LookAt.x = _r.x;
        // _camera.m_f3LookAt.y = _r.y;
        // _camera.m_f3LookAt.z = _r.z;

        // _r = glm::cross(_r, _hAxis);
        // _r = glm::normalize(_r);

        // _camera.m_f3Up.x = _r.x;
        // _camera.m_f3Up.y = _r.y;
        // _camera.m_f3Up.z = _r.z;



        _camera.m_f3AlignmentVector = normalize(_camera.m_f3LookAt - _camera.m_f3Position);
        _camera.m_f3ProjectionScreenU = normalize(cross(_camera.m_f3AlignmentVector, _camera.m_f3Up));
        _camera.m_f3ProjectionScreenV = normalize(cross(_camera.m_f3ProjectionScreenU, _camera.m_f3AlignmentVector));
        _camera.m_f3ProjectionScreenCentre = _camera.m_f3Position + (_camera.m_fLength * _camera.m_f3AlignmentVector);
        _camera.m_f3ProjectionScreenU = _camera.m_f3ProjectionScreenU * _camera.m_fHorizontalSize;
        _camera.m_f3ProjectionScreenV = _camera.m_f3ProjectionScreenV * (_camera.m_fHorizontalSize / _camera.m_fAspectRatio);
    }

    // Camera::Camera()
    // {
    //     m_f3Position = make_float3(0, -10, 0);
    //     m_f3LookAt = make_float3(0, 0, 0);
    //     m_f3Up = make_float3(0, 0, 1);

    //     m_fLength = 1;
    //     m_fHorizontalSize = 1;
    //     m_fAspectRatio = 1;
    // }

    void RayTracer::RayTrace()
    {
        {
            auto t_start = std::chrono::high_resolution_clock::now();

            cudaMemcpy(m_dpLightStream, &m_vLightData[0], m_vLightData.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);
            cudaMemcpy(m_dpCamera, &m_camera, sizeof(Camera), cudaMemcpyKind::cudaMemcpyHostToDevice);
            cudaMemcpy(m_dpObjectStream, &m_vObjectData[0], m_vObjectData.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

            auto t_end = std::chrono::high_resolution_clock::now();

            elapsed_time_ms3 = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        }

        {
            auto t_start = std::chrono::high_resolution_clock::now();

            ray_trace(static_cast<uint32_t*>(m_pDeviceMemory), image_width, image_height, m_dpCamera, m_dpObjectStream, m_dpLightStream);

            cudaDeviceSynchronize();

            auto t_end = std::chrono::high_resolution_clock::now();

            elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        }

        {
            auto t_start = std::chrono::high_resolution_clock::now();

            cudaMemcpy(image_data, m_pDeviceMemory, image_width * image_height * sizeof(uint32_t), cudaMemcpyKind::cudaMemcpyDeviceToHost);
            glBindTexture(GL_TEXTURE_2D, image_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

            auto t_end = std::chrono::high_resolution_clock::now();

            elapsed_time_ms2 = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        }
    }

    uint32_t RayTracer::Add(uint8_t** _pCurr, const Transform& _t, const Sphere& _s)
    {
        m_vObjectData.resize(m_vObjectData.size() + sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform));
        *_pCurr = reinterpret_cast<uint8_t*>(m_vObjectData.data() + (m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform))));

        ObjectChunk* _c = reinterpret_cast<ObjectChunk*>(*_pCurr);
        *_pCurr += sizeof(ObjectChunk);

        _c->m_type = ObjectType::sphere;
        _c->m_material = MaterialType::none;

        {
            Transform* _x = reinterpret_cast<Transform*>(*_pCurr);
            *_pCurr += sizeof(Transform);

            *_x = _t;
        }

        {
            Sphere* _c = reinterpret_cast<Sphere*>(*_pCurr);
            *_pCurr += sizeof(Sphere);

            *_c = _s;
        }

        _c->m_uiSize = sizeof(Transform) + sizeof(Sphere);

        return m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform));
    }

    uint32_t RayTracer::Add(uint8_t** _pCurr, const Transform& _t, const Sphere& _s, const SpecularMaterial& _m)
    {
        m_vObjectData.resize(m_vObjectData.size() + sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform) + sizeof(SpecularMaterial));
        *_pCurr = reinterpret_cast<uint8_t*>(m_vObjectData.data() + (m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform) + sizeof(SpecularMaterial))));

        ObjectChunk* _c = reinterpret_cast<ObjectChunk*>(*_pCurr);
        *_pCurr += sizeof(ObjectChunk);

        _c->m_type = ObjectType::sphere;
        _c->m_material = MaterialType::specular;

        {
            SpecularMaterial* _c = reinterpret_cast<SpecularMaterial*>(*_pCurr);
            *_pCurr += sizeof(SpecularMaterial);

            *_c = _m;
        }

        {
            Transform* _x = reinterpret_cast<Transform*>(*_pCurr);
            *_pCurr += sizeof(Transform);

            *_x = _t;
        }

        {
            Sphere* _c = reinterpret_cast<Sphere*>(*_pCurr);
            *_pCurr += sizeof(Sphere);

            *_c = _s;
        }

        _c->m_uiSize = sizeof(Transform) + sizeof(Sphere) + sizeof(SpecularMaterial);

        return m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Sphere) + sizeof(Transform) + sizeof(SpecularMaterial));
    }

    uint32_t RayTracer::Add(uint8_t** _pCurr, const Transform& _t, const Plane& _o)
    {
        m_vObjectData.resize(m_vObjectData.size() + sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform));
        *_pCurr = reinterpret_cast<uint8_t*>(m_vObjectData.data() + (m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform))));

        ObjectChunk* _c = reinterpret_cast<ObjectChunk*>(*_pCurr);
        *_pCurr += sizeof(ObjectChunk);

        _c->m_type = ObjectType::plane;
        _c->m_material = MaterialType::none;

        {
            Transform* _x = reinterpret_cast<Transform*>(*_pCurr);
            *_pCurr += sizeof(Transform);

            *_x = _t;
        }

        {
            Plane* _c = reinterpret_cast<Plane*>(*_pCurr);
            *_pCurr += sizeof(Plane);

            *_c = _o;
        }

        _c->m_uiSize = sizeof(Transform) + sizeof(Plane);

        return m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform));
    }

    uint32_t RayTracer::Add(uint8_t** _pCurr, const Transform& _t, const Plane& _o, const SpecularMaterial& _m)
    {
        m_vObjectData.resize(m_vObjectData.size() + sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform) + sizeof(SpecularMaterial));
        *_pCurr = reinterpret_cast<uint8_t*>(m_vObjectData.data() + (m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform) + sizeof(SpecularMaterial))));

        ObjectChunk* _c = reinterpret_cast<ObjectChunk*>(*_pCurr);
        *_pCurr += sizeof(ObjectChunk);

        _c->m_type = ObjectType::plane;
        _c->m_material = MaterialType::specular;

        {
            SpecularMaterial* _c = reinterpret_cast<SpecularMaterial*>(*_pCurr);
            *_pCurr += sizeof(SpecularMaterial);

            *_c = _m;
        }

        {
            Transform* _x = reinterpret_cast<Transform*>(*_pCurr);
            *_pCurr += sizeof(Transform);

            *_x = _t;
        }

        {
            Plane* _c = reinterpret_cast<Plane*>(*_pCurr);
            *_pCurr += sizeof(Plane);

            *_c = _o;
        }

        _c->m_uiSize = sizeof(Transform) + sizeof(Plane) + sizeof(SpecularMaterial);

        return m_vObjectData.size() - (sizeof(ObjectChunk) + sizeof(Plane) + sizeof(Transform) + sizeof(SpecularMaterial));
    }

    void RayTracer::Add(uint8_t** _pCurr, const PointLight& _l)
    {
        m_vLightData.resize(m_vLightData.size() + sizeof(LightChunk) + sizeof(PointLight));
        *_pCurr = reinterpret_cast<uint8_t*>(m_vLightData.data() + (m_vLightData.size() - (sizeof(LightChunk) + sizeof(PointLight))));

        LightChunk* _c = reinterpret_cast<LightChunk*>(*_pCurr);
        *_pCurr += sizeof(LightChunk);

        _c->m_type = LightType::point_light;

        {
            PointLight* _c = reinterpret_cast<PointLight*>(*_pCurr);
            *_pCurr += sizeof(PointLight);

            *_c = _l;
        }
    }

    RayTracer::RayTracer() :
        m_pDeviceMemory(nullptr)
    {

    }

    RayTracer::RayTracer(const Memory::FreeList& _heap, Event::Pool<Event::mode::buffered>* _pEventPool) :
        m_pDeviceMemory(nullptr),
        m_pWindowEventPool(_pEventPool)
    {

    }

    RayTracer::~RayTracer()
    {

    }

    bool RayTracer::Init()
    {
        m_bRender = false;

        m_setMousePosition.m_iX = 1920 / 2;
        m_setMousePosition.m_iY = 1080 / 2;

        return true;
    }

    void RayTracer::Update(double _dDelta)
    {
        m_pWindowEventPool->Reset();

        while(m_pWindowEventPool->AnyEvents())
        {
            Window::MouseMotionEvent _mouseMotionEvent;

            if(m_pWindowEventPool->GetMessage(&_mouseMotionEvent))
            {
                // if(!m_bWrapCamera || !m_bIsWindowFocused)
                // {
                //     m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();

                //     continue;
                // }

                int _x = (1920 / 2) - _mouseMotionEvent.m_iX;
                int _y = (1080 / 2) - _mouseMotionEvent.m_iY;

                // m_viewport.m_rotation = glm::normalize(glm::angleAxis(_y * (float)_dDelta, m_viewport.m_rotation * glm::vec3(-1, 0, 0)) * m_viewport.m_rotation);
                // m_viewport.m_rotation = glm::normalize(glm::angleAxis(_x * (float)_dDelta, glm::vec3(0, -1, 0)) * m_viewport.m_rotation);
                m_f3CameraForward.z = _y;

                m_pWindowEventPool->EventHandled<Window::MouseMotionEvent>();
                m_pWindowEventPool->Broadcast(m_setMousePosition);
            }
            else
            {
                m_pWindowEventPool->Skip();
            }
        }
    }

    void RayTracer::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));

        image_data = new uint32_t[image_width * image_height];

        {
            _kernelModule = PlugNPlay::__module_information("kernel", "E:/Projects/C++/Duckvil/bin");

            PlugNPlay::module_init(&_module);

            _module.load(&_kernelModule);
            _module.get(_kernelModule, "ray_trace", reinterpret_cast<void**>(&ray_trace));
        }

        cudaMalloc(&m_pDeviceMemory, image_width * image_height * sizeof(uint32_t));

        {
            m_vObjectData.resize(sizeof(ObjectStream));

            uint8_t* _curr = nullptr;

            {
                Transform _t;
                Plane _o;
                SpecularMaterial _m;

                _m.m_f3Color = make_float3(.5f, .5f, .5f);
                _m.m_fReflectivity = 0.f;
                _m.m_fShininess = 10.f;
                _m.m_fTranslucency = 0.f;

                transform_set(_t, glm::vec3(0, 0, 0.75), glm::vec3(0, 0, 0), glm::vec3(4.f, 4.f, 1.f));

                _o.m_f3Color.x = .5f;
                _o.m_f3Color.y = .5f;
                _o.m_f3Color.z = .5f;

                Add(&_curr, _t, _o, _m);
            }

            // {
            //     Transform _t;
            //     Plane _o;

            //     transform_set(_t, glm::vec3(0, 0, 0.5), glm::vec3(0, 0, 0), glm::vec3(2.f, 2.f, 1.f));

            //     _o.m_f3Color.x = 0.5f;
            //     _o.m_f3Color.y = 0.5f;
            //     _o.m_f3Color.z = 0.5f;

            //     Add(&_curr, _t, _o);
            // }

            {
                Transform _t;
                Sphere _s;
                SpecularMaterial _m;

                _m.m_f3Color = make_float3(0.5f, 0.5f, 0.5f);
                _m.m_fReflectivity = 0.25f;
                _m.m_fShininess = 32.f;
                _m.m_fTranslucency = .75f;
                _m.m_fIOR = 1.333f;

                m_f3Object1Position = glm::vec3(-1, 1, 0);

                transform_set(_t, m_f3Object1Position, glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));

                _s.m_f3Color.x = 1.f;
                _s.m_f3Color.y = 1.f;
                _s.m_f3Color.z = 1.f;

                m_pObject1Offset = Add(&_curr, _t, _s, _m);
            }

            {
                Transform _t;
                Sphere _s;
                SpecularMaterial _m;

                _m.m_f3Color = make_float3(0.5f, 0.5f, 0.5f);
                _m.m_fReflectivity = 0.f;
                _m.m_fShininess = 2.f;
                _m.m_fTranslucency = 0.f;

                transform_set(_t, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));

                _s.m_f3Color.x = 1.f;
                _s.m_f3Color.y = 1.f;
                _s.m_f3Color.z = 1.f;

                Add(&_curr, _t, _s, _m);
            }

            // {
            //     Transform _t;
            //     Sphere _s;
            //     SpecularMaterial _m;

            //     _m.m_f3Color = make_float3(0.5f, 0.5f, 0.5f);
            //     _m.m_fReflectivity = 0.f;
            //     _m.m_fShininess = 10.f;
            //     _m.m_fTranslucency = 0.f;

            //     transform_set(_t, glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));

            //     _s.m_f3Color.x = 0.f;
            //     _s.m_f3Color.y = 0.f;
            //     _s.m_f3Color.z = 1.f;

            //     Add(&_curr, _t, _s, _m);
            // }

            // for(uint32_t _i = 0; _i < 50; ++_i)
            // {
            //     Transform _t;
            //     Sphere _s;
            //     SpecularMaterial _m;

            //     _m.m_f3Color = make_float3(0.5f, 0.5f, 0.5f);
            //     _m.m_fReflectivity = 1.f;
            //     _m.m_fShininess = 3.f;

            //     transform_set(_t, glm::vec3(_i, 2, 0), glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));

            //     _s.m_f3Color.x = 0.f;
            //     _s.m_f3Color.y = 0.f;
            //     _s.m_f3Color.z = 1.f;

            //     Add(&_curr, _t, _s, _m);
            // }

            ObjectStream* _objectStream = reinterpret_cast<ObjectStream*>(m_vObjectData.data());

            _objectStream->m_uiSize = m_vObjectData.size() - sizeof(ObjectStream);

            cudaMalloc(reinterpret_cast<void**>(&m_dpObjectStream), m_vObjectData.size());
            // cudaMemcpy(m_dpObjectStream, &m_vObjectData[0], m_vObjectData.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);
        }

        {
            m_vLightData.resize(sizeof(LightStream));

            uint8_t* _curr = nullptr;

            // {
            //     PointLight _c;

            //     _c.m_f3Color.x = 1.f;
            //     _c.m_f3Color.y = 0.f;
            //     _c.m_f3Color.z = 0.f;

            //     _c.m_f3Location.x = -5.f;
            //     _c.m_f3Location.y = 0.f;
            //     _c.m_f3Location.z = -5.f;

            //     _c.m_fIntensity = .9f;

            //     Add(&_curr, _c);
            // }

            {
                PointLight _c;

                _c.m_f3Color.x = 1.f;
                _c.m_f3Color.y = 1.f;
                _c.m_f3Color.z = 1.f;

                _c.m_f3Location.x = 0.f;
                _c.m_f3Location.y = 0.f;
                _c.m_f3Location.z = -5.f;

                _c.m_fIntensity = .9f;

                Add(&_curr, _c);
            }

            // {
            //     PointLight _c;

            //     _c.m_f3Color.x = 0.f;
            //     _c.m_f3Color.y = 0.f;
            //     _c.m_f3Color.z = 1.f;

            //     _c.m_f3Location.x = 5.f;
            //     _c.m_f3Location.y = 0.f;
            //     _c.m_f3Location.z = -5.f;

            //     _c.m_fIntensity = .9f;

            //     Add(&_curr, _c);
            // }

            LightStream* _lightStream = reinterpret_cast<LightStream*>(m_vLightData.data());

            _lightStream->m_uiSize = m_vLightData.size() - sizeof(LightStream);

            LightStream* ooo = (LightStream*)m_vLightData.data();

            cudaMalloc(reinterpret_cast<void**>(&m_dpLightStream), m_vLightData.size());
            // cudaMemcpy(m_dpLightStream, &m_vLightData[0], m_vLightData.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);
        }

        {
            m_f3CameraUp = glm::vec3(0, 1, 0);
            m_f3CameraForward = glm::vec3(0, 0, 1);
            m_f3CameraPosition = glm::vec3(0, 10, -1);

            m_camera.m_f3Position = make_float3(m_f3CameraPosition.x, m_f3CameraPosition.y, m_f3CameraPosition.z);
            m_camera.m_f3LookAt = make_float3(m_f3CameraForward.x, m_f3CameraForward.y, m_f3CameraForward.z);
            m_camera.m_f3Up = make_float3(m_f3CameraUp.x, m_f3CameraUp.y, m_f3CameraUp.z);
            m_camera.m_fHorizontalSize = 0.25f;
            m_camera.m_fAspectRatio = 16.f / 9.f;

            update_camera(m_camera, 0.f);

            cudaMalloc(reinterpret_cast<void**>(&m_dpCamera), sizeof(Camera));
            // cudaMemcpy(m_dpCamera, &m_camera, sizeof(Camera), cudaMemcpyKind::cudaMemcpyHostToDevice);
        }

        {
            glGenTextures(1, &image_texture);
            glBindTexture(GL_TEXTURE_2D, image_texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        // RayTrace();
    }

    void RayTracer::OnDraw()
    {
        ImGui::Begin("RayTracer");

        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image_width, image_height));

        ImGui::Checkbox("Render", &m_bRender);

        if(m_bRender || ImGui::Button("Render"))
        {
            RayTrace();
        }

        ImGui::Text("Copy to GPU: %f", elapsed_time_ms3);
        ImGui::Text("Last render time: %f", elapsed_time_ms);
        ImGui::Text("Copy from GPU: %f", elapsed_time_ms2);

        ImGui::End();

        ImGui::Begin("RayTracer Settings");

        // ImGui::InputFloat3("Camera position", (float*)&m_f3CameraPosition);
        // ImGui::InputFloat3("Camera forward", (float*)&m_f3CameraForward);
        // ImGui::InputFloat3("Camera up", (float*)&m_f3CameraUp);

        ImGui::SliderFloat("Camera up", (float*)&m_fCameraUp, -1, 1);
        // ImGui::SliderFloat3("Camera right", (float*)&m_f3CameraPosition, -45, 45);

        // m_f3CameraForward.x = m_fCameraRight;

        // m_camera.m_f3Position = make_float3(m_f3CameraPosition.x, m_f3CameraPosition.y, m_f3CameraPosition.z);
        // m_camera.m_f3LookAt = make_float3(m_f3CameraForward.x, m_f3CameraForward.y, m_f3CameraForward.z);
        // m_camera.m_f3Up = make_float3(m_f3CameraUp.x, m_f3CameraUp.y, m_f3CameraUp.z);
        // m_camera.m_fHorizontalSize = 0.25f;
        // m_camera.m_fAspectRatio = 16.f / 9.f;

        update_camera(m_camera, m_fCameraUp);

        // if(m_pObject1->m_type == ObjectType::sphere)
        // {
        //     uint8_t* _p = reinterpret_cast<uint8_t*>(m_pObject1);

        //     _p += sizeof(SpecularMaterial);

        //     Sphere* _s = reinterpret_cast<Sphere*>(_p);

        //     ImGui::InputFloat3("aaa", (float*)&_s->m_f3Color);
        // }

        /*if (ImGui::SliderFloat3("bbb", (float*)&m_f3Object1Position, -10, 10))
        {
            uint8_t* _p = reinterpret_cast<uint8_t*>(m_vObjectData.data()) + m_pObject1Offset;

            _p += sizeof(SpecularMaterial) + sizeof(ObjectChunk);

            Transform* _t = reinterpret_cast<Transform*>(_p);

            transform_set(*_t, m_f3Object1Position, glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));
        }

        if(ImGui::InputFloat3("Position", (float*)&m_f3Object1Position))
        {
            uint8_t* _p = reinterpret_cast<uint8_t*>(m_vObjectData.data()) + m_pObject1Offset;

            _p += sizeof(SpecularMaterial) + sizeof(ObjectChunk);

            Transform* _t = reinterpret_cast<Transform*>(_p);

            transform_set(*_t, m_f3Object1Position, glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, .5f));
        }*/

        ImGui::End();
    }

}}}