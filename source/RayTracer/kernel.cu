#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "Objects.h"

#define EPSILON 1e-21f

#define FWDTFORM true
#define BCKTFORM false

#define MAX_DEPTH 3

using namespace Duckvil::Graphics::RayTracer;

__device__ float clamp(float x, float a, float b)
{
    return max(a, min(b, x));
}

__device__ float3 normalize(float3 vec)
{
    float n = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    return make_float3(vec.x / n, vec.y / n, vec.z / n);
}

__device__ float norm(float3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

__device__ float3 cross(float3 A, float3 B)
{
    float3 _res = make_float3(0, 0, 0);

    _res.x = A.y * B.z - A.z * B.y;
    _res.y = A.z * B.x - A.x * B.z;
    _res.z = A.x * B.y - A.y * B.x;

    return _res;
}

__device__ float dot(float3 A, float3 B)
{
    float _res = 0;

    _res += A.x * B.x;
    _res += A.y * B.y;
    _res += A.z * B.z;

    return _res;
}

__device__ float3 operator-(float3 vec1, float3 vec2)
{
    return make_float3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

__device__ float3 operator+(float3 vec1, float3 vec2)
{
    return make_float3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

__device__ float3& operator+=(float3& vec1, const float3& vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;

    return vec1;
}

__device__ float3 operator*(float x, float3 vec)
{
    return make_float3(vec.x * x, vec.y * x, vec.z * x);
}

__device__ float3 operator*(float3 vec, float x)
{
    return make_float3(vec.x * x, vec.y * x, vec.z * x);
}

__device__ unsigned int rgbToInt(float _fR, float _fG, float _fB)
{
	return (int(255)<<24) | (int(_fB * 255.f)<<16) | (int(_fG * 255.f)<<8) | int(_fR * 255.f);
}

// __device__ glm::vec3 operator* (const glm::mat3& lhs, const glm::vec3& rhs)
// {
// 	glm::vec3 result;
	
// 	// Loop over rows and columns and perform the multiplication operation element-by-element.
// 	for (int row=0; row<3; ++row)
// 	{
// 		float cumulativeSum = 0.f;
// 		for (int col=0; col<3; ++col)
// 		{
// 			// cumulativeSum += (lhs.GetElement(row,col) * rhs.GetElement(col));
//             cumulativeSum += lhs[row][col] * rhs[col];
// 		}
// 		// result.SetElement(row, cumulativeSum);
//         result[row] = cumulativeSum;
// 	}
	
// 	return result;
// }

// __device__ float3 intToRgb(unsigned int _color)
// {
//     uint8_t red = (_color & 0x000000FF);
//     uint8_t green = (_color & 0x0000FF00) >> 8;
//     uint8_t blue = (_color & 0x000000FF) >> 16;

// 	return make_float3(red / 255.f, green / 255.f, blue / 255.f);
// }

struct Ray
{
    float3 m_f3Point1;
    float3 m_f3Point2;
    float3 m_f3Lab;

    __device__ Ray()
    {
        m_f3Point1 = make_float3(0, 0, 0);
        m_f3Point2 = make_float3(0, 0, 1);
        m_f3Lab = m_f3Point2 - m_f3Point1;
    }

    __device__ Ray(float3 _f3Point1, float3 _f3Point2) :
        m_f3Point1(_f3Point1),
        m_f3Point2(_f3Point2)
    {
        m_f3Lab = m_f3Point2 - m_f3Point1;
    }
};

struct Transform
{
    glm::mat4 m_mat4ForwardTransform;
    glm::mat4 m_mat4BackwardTransform;
    glm::mat3 m_mat3LinearTransform;

    // __device__ Transform() :
    //     m_mat4ForwardTransform(glm::mat4(1.f)),
    //     m_mat4BackwardTransform(glm::inverse(m_mat4ForwardTransform)),
    //     m_mat3LinearTransform(glm::transpose(m_mat4BackwardTransform))
    // {

    // }

    // __device__ Transform(const glm::mat4& _mat4Forward) :
    //     m_mat4ForwardTransform(_mat4Forward),
    //     m_mat4BackwardTransform(glm::inverse(m_mat4ForwardTransform)),
    //     m_mat3LinearTransform(glm::transpose(m_mat4BackwardTransform))
    // {

    // }

    // __device__ Transform(const glm::mat4& _mat4Forward, const glm::mat4& _mat4Backward) :
    //     m_mat4ForwardTransform(_mat4Forward),
    //     m_mat4BackwardTransform(_mat4Backward),
    //     m_mat3LinearTransform(glm::transpose(m_mat4BackwardTransform))
    // {

    // }
};

// __device__ void transform_extract_linear(Transform& _t)
// {
//     _t.m_mat4LinearTransform = glm::transpose(_t.m_mat4BackwardTransform);
// }

__device__ void transform_set(Transform& _t, const glm::mat4& _mat4Forward, const glm::mat4& _mat4Backward)
{
    _t.m_mat4ForwardTransform = _mat4Forward;
    _t.m_mat4BackwardTransform = _mat4Backward;
    _t.m_mat3LinearTransform = glm::transpose(_mat4Backward);
}

__device__ void transform_set(Transform& _t, const glm::mat4& _mat4Forward)
{
    _t.m_mat4ForwardTransform = _mat4Forward;
    _t.m_mat4BackwardTransform = glm::inverse(_t.m_mat4ForwardTransform);
    _t.m_mat3LinearTransform = glm::transpose(_t.m_mat4BackwardTransform);
}

// __device__ Transform operator*(const Transform& _lhs, const Transform& _rhs)
// {
//     return Transform(_lhs.m_mat4ForwardTransform * _rhs.m_mat4ForwardTransform);
// }

__device__ float3 transform_apply(Transform& _transform, const float3& _input, bool _dirFlag)
{
    glm::vec4 _res;
    glm::vec4 _tmp(
        _input.x,
        _input.y,
        _input.z,
        1.f
    );

    if(_dirFlag)
    {
        _res = _transform.m_mat4ForwardTransform * _tmp;
    }
    else
    {
        _res = _transform.m_mat4BackwardTransform * _tmp;
    }

    return make_float3(_res.x, _res.y, _res.z);
}

__device__ float3 transform_apply_norm(const Transform& _t, const float3& _i)
{
    glm::vec3 _r = _t.m_mat3LinearTransform * glm::vec3(_i.x, _i.y, _i.z);

    return make_float3(_r.x, _r.y, _r.z);
}

__device__ Ray transform_apply(Transform& _transform, const Ray& _inputRay, bool _dirFlag)
{
    Ray _outputRay;

    if(_dirFlag)
    {
        _outputRay.m_f3Point1 = transform_apply(_transform, _inputRay.m_f3Point1, FWDTFORM);
        _outputRay.m_f3Point2 = transform_apply(_transform, _inputRay.m_f3Point2, FWDTFORM);
        _outputRay.m_f3Lab = _outputRay.m_f3Point2 - _outputRay.m_f3Point1;
    }
    else
    {
        _outputRay.m_f3Point1 = transform_apply(_transform, _inputRay.m_f3Point1, BCKTFORM);
        _outputRay.m_f3Point2 = transform_apply(_transform, _inputRay.m_f3Point2, BCKTFORM);
        _outputRay.m_f3Lab = _outputRay.m_f3Point2 - _outputRay.m_f3Point1;
    }

    return _outputRay;
}

// struct Camera
// {
// 	float3 m_f3Position;
//     float3 m_f3LookAt;
//     float3 m_f3Up;
//     float m_fLength;
//     float m_fHorizontalSize;
//     float m_fAspectRatio;

//     float3 m_f3AlignmentVector;
//     float3 m_f3ProjectionScreenU;
//     float3 m_f3ProjectionScreenV;
//     float3 m_f3ProjectionScreenCentre;
// };

struct Hit
{
    float3 m_f3POI;
    float3 m_f3POI_N;
    float3 m_f3POI_C;
};

__device__ bool generate_ray(Camera& _camera, Ray& _ray, float x, float y)
{
    float3 _screenWorldPart1 = _camera.m_f3ProjectionScreenCentre + (_camera.m_f3ProjectionScreenU * x);
    float3 _screenWorldCoord = _screenWorldPart1 + (_camera.m_f3ProjectionScreenV * y);

    _ray.m_f3Point1 = _camera.m_f3Position;
    _ray.m_f3Point2 = _screenWorldCoord;
    _ray.m_f3Lab = _screenWorldCoord - _camera.m_f3Position;

    return true;
}

__device__ bool test(Sphere& _object, const Ray& _ray, float3& _f3Intersection, float3& _f3Normal, float3& _f3Color)
{
    float3 _vhat = normalize(_ray.m_f3Lab);
    float b = 2.f * dot(_ray.m_f3Point1, _vhat);
    float c = dot(_ray.m_f3Point1, _ray.m_f3Point1) - 1.f;
    float intTest = (b * b) - 4.f * c;

    if(intTest <= 0.f)
    {
        return false;
    }

    float numSqrt = sqrtf(intTest);
    float t1 = (-b + numSqrt) / 2.f;
    float t2 = (-b - numSqrt) / 2.f;

    if((t1 < 0) || (t2 < 0))
    {
        return false;
    }

    if(t1 < t2)
    {
        if(t1 > 0.f)
        {
            _f3Intersection = _ray.m_f3Point1 + (_vhat * t1);
        }
        else
        {
            if(t2 > 0.f)
            {
                _f3Intersection = _ray.m_f3Point1 + (_vhat * t2);
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        if(t2 > 0.f)
        {
            _f3Intersection = _ray.m_f3Point1 + (_vhat * t2);
        }
        else
        {
            if(t1 > 0.f)
            {
                _f3Intersection = _ray.m_f3Point1 + (_vhat * t1);
            }
            else
            {
                return false;
            }
        }
    }

    _f3Color = _object.m_f3Color;

    return true;
}

__device__ bool test(Plane& _object, const Ray& _ray, float3& _f3Intersection, float3& _f3Normal, float3& _f3Color)
{
    float3 _k = normalize(_ray.m_f3Lab);

    if(fabs(_k.z - 0.f) < EPSILON)
    {
        return false;
    }

    float _t = _ray.m_f3Point1.z / -_k.z;

    if(_t <= 0.f)
    {
        return false;
    }

    float _u = _ray.m_f3Point1.x + (_k.x * _t);
    float _v = _ray.m_f3Point1.y + (_k.y * _t);

    if((abs(_u) >= 1.f) || (abs(_v) >= 1.f))
    {
        return false;
    }

    _f3Intersection = _ray.m_f3Point1 + _t * _k;
    _f3Color = _object.m_f3Color;

    return true;
}

__device__ bool test(ObjectStream* _pObjectStream, unsigned int _uiStreamOffset, ObjectChunk** _ppCurrentObject, const Ray& _ray, float3& _f3Intersection, float3& _f3Normal, float3& _f3Color, ObjectChunk* _pCurrentObject = nullptr)
{
    unsigned char* _curr = reinterpret_cast<unsigned char*>(_pObjectStream);

    ObjectStream* _stream = reinterpret_cast<ObjectStream*>(_curr);
    _curr += sizeof(ObjectStream) + _uiStreamOffset;

    ObjectChunk* _chunk = reinterpret_cast<ObjectChunk*>(_curr);
    _curr += sizeof(ObjectChunk);

    *_ppCurrentObject = _chunk;

    if(_chunk == _pCurrentObject)
    {
        return false;
    }

    if(_chunk->m_material != MaterialType::none)
    {
        _curr += sizeof(SpecularMaterial);
    }

    Transform* _transform = reinterpret_cast<Transform*>(_curr);
    _curr += sizeof(Transform);

    Ray _bckRay = transform_apply(*_transform, _ray, BCKTFORM);

    if(_chunk->m_type == ObjectType::sphere)
    {
        Sphere* _object = reinterpret_cast<Sphere*>(_curr);
        _curr += sizeof(Sphere);

        float3 _poi;

        if(!test(*_object, _bckRay, _poi, _f3Normal, _f3Color))
        {
            return false;
        }

        _f3Intersection = transform_apply(*_transform, _poi, FWDTFORM);
        // _f3Normal = normalize(transform_apply_norm(*_transform, _poi));

        float3 _newOrigin = transform_apply(*_transform, make_float3(0, 0, 0), FWDTFORM);

        _f3Normal = normalize(_f3Intersection - _newOrigin);

        return true;
    }
    else if(_chunk->m_type == ObjectType::plane)
    {
        Plane* _object = reinterpret_cast<Plane*>(_curr);
        _curr += sizeof(Plane);

        float3 _poi;

        if(!test(*_object, _bckRay, _poi, _f3Normal, _f3Color))
        {
            return false;
        }

        _f3Intersection = transform_apply(*_transform, _poi, FWDTFORM);

        float3 _localOrigin = make_float3(0, 0, 0);
        float3 _normal = make_float3(0, 0, -1);
        float3 _globalOrigin = transform_apply(*_transform, _localOrigin, FWDTFORM);

        _f3Normal = normalize(transform_apply(*_transform, _normal, FWDTFORM) - _globalOrigin);

        // float3 _normal = make_float3(0, 0, -1);

        // _f3Normal = normalize(transform_apply_norm(*_transform, _normal));

        return true;
    }

    return false;
}

__device__ bool find_closest(ObjectStream* _pObjectStream, const Ray& _ray, ObjectChunk** _ppClosestObject, float3& _f3POI, float3& _f3POI_N, float3& _f3POI_C)
{
    unsigned int _currentObjectStreamOffset = 0;
    uint32_t _objectStreamSize = _pObjectStream->m_uiSize;

    float _minDist = 1e6;
    bool _intFound = false;

    float3 _intPoint = make_float3(0, 0, 0);
    float3 _localNormal = make_float3(0, 0, 0);
    float3 _localColor = make_float3(0, 0, 0);

    ObjectChunk* _currentObject;

    while(_currentObjectStreamOffset < _objectStreamSize)
    {
        if(test(_pObjectStream, _currentObjectStreamOffset, &_currentObject, _ray, _intPoint, _localNormal, _localColor))
        {
            _intFound = true;

            float _dist = norm(_intPoint - _ray.m_f3Point1);

            if(_dist >= _minDist)
            {
                continue;
            }

            _minDist = _dist;

            *_ppClosestObject = _currentObject;

            _f3POI = _intPoint;
            _f3POI_N = _localNormal;
            _f3POI_C = _localColor;
        }

        _currentObjectStreamOffset += sizeof(ObjectChunk) + _currentObject->m_uiSize;
    }

    return _intFound;
}

__device__ bool compute_light(PointLight& _light, const float3& _f3IntPoint, const float3& _f3LocalNormal, ObjectStream* _pObjectStream, ObjectChunk* _pCurrentObject, float3& _f3Color, float& _fIntensity)
{
    float3 _dir = normalize(_light.m_f3Location - _f3IntPoint);
    float _lightDist = norm(_light.m_f3Location - _f3IntPoint);
    float3 _startPoint = _f3IntPoint;

    float3 _poi;
    float3 _poiNormal;
    float3 _poiColor;

    unsigned int _currentObjectStreamOffset = 0;
    uint32_t _objectStreamSize = _pObjectStream->m_uiSize;

    Ray _lightRay(_startPoint, _startPoint + _dir);

    bool _validInt = false;

    ObjectChunk* _currentObject;

    while(_currentObjectStreamOffset < _objectStreamSize)
    {
        if(
            test(_pObjectStream, _currentObjectStreamOffset, &_currentObject, _lightRay, _poi, _poiNormal, _f3Color, _pCurrentObject) &&
            // _currentObject != _pCurrentObject &&
            !(norm(_poi - _startPoint) > _lightDist)
        )
        {
            _validInt = true;

            break;
        }

        _currentObjectStreamOffset += sizeof(ObjectChunk) + _currentObject->m_uiSize;
    }

    _f3Color = _light.m_f3Color;

    if(_validInt)
    {
        _fIntensity = 0.f;

        return false;
    }

    float _angle = acos(dot(_f3LocalNormal, _dir));

    if(_angle > 1.5708)
    {
        _fIntensity = 0.f;

        return false;
    }

    _fIntensity = _light.m_fIntensity * (1.f - (_angle / 1.5708));

    return true;
}

__device__ bool compute_light(LightStream* _pLightStream, unsigned int _uiLightChunkOffset, const float3& _f3IntPoint, const float3& _f3LocalNormal, ObjectStream* _pObjectStream, ObjectChunk* _pCurrentObject, float3& _f3Color, float& _fIntensity)
{
    unsigned char* _curr = reinterpret_cast<unsigned char*>(_pLightStream);

    LightStream* _lightStream = reinterpret_cast<LightStream*>(_curr);
    _curr += sizeof(LightStream) + _uiLightChunkOffset;

    LightChunk* _chunk = reinterpret_cast<LightChunk*>(_curr);
    _curr += sizeof(LightChunk);

    if(_chunk->m_type == LightType::point_light)
    {
        PointLight* _light = reinterpret_cast<PointLight*>(_curr);
        _curr += sizeof(PointLight);

        return compute_light(*_light, _f3IntPoint, _f3LocalNormal, _pObjectStream, _pCurrentObject, _f3Color, _fIntensity);
    }

    return false;
}

__device__ float3 g_f3AmbientColor = { 1.f, 1.f, 1.f };
__device__ float g_fAmbientIntensity = 0.2f;

__device__ float3 material_compute_diffuse_color(ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const float3& _f3BaseColor)
{
    float3 _diffuseColor = make_float3(0, 0, 0);

    float _intensity;
    float3 _color;
    bool _illumFound = false;

    unsigned int _currentLightStreamSize = 0;
    uint32_t _lightStreamSize = _pLightStream->m_uiSize;

    while(_currentLightStreamSize < _lightStreamSize)
    {
        if(compute_light(_pLightStream, _currentLightStreamSize, _f3IntPoint, _f3LocalNormal, _pObjectStream, _pCurrentObject, _color, _intensity))
        {
            _illumFound = true;

            _diffuseColor.x += _color.x * _intensity;
            _diffuseColor.y += _color.y * _intensity;
            _diffuseColor.z += _color.z * _intensity;
        }

        _currentLightStreamSize += sizeof(LightChunk) + sizeof(PointLight);
    }

    if(_illumFound)
    {
        _diffuseColor.x *= _f3BaseColor.x;
        _diffuseColor.y *= _f3BaseColor.y;
        _diffuseColor.z *= _f3BaseColor.z;
    }
    // else
    // {
    //     _diffuseColor.x = (g_f3AmbientColor.x * g_fAmbientIntensity) * _f3BaseColor.x;
    //     _diffuseColor.y = (g_f3AmbientColor.y * g_fAmbientIntensity) * _f3BaseColor.y;
    //     _diffuseColor.z = (g_f3AmbientColor.z * g_fAmbientIntensity) * _f3BaseColor.z;
    // }

    return _diffuseColor;
}

template <int depth>
__device__ float3 material_compute_specular(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    float3 _sColor = make_float3(0, 0, 0);

    unsigned int _currentLightStreamSize = 0;
    uint32_t _lightStreamSize = _pLightStream->m_uiSize;

    while(_currentLightStreamSize < _lightStreamSize)
    {
        unsigned char* _curr = reinterpret_cast<unsigned char*>(_pLightStream);

        // LightStream* _lightStream = reinterpret_cast<LightStream*>(_curr);
        _curr += sizeof(LightStream) + _currentLightStreamSize;

        LightChunk* _chunk = reinterpret_cast<LightChunk*>(_curr);
        _curr += sizeof(LightChunk);

        if(_chunk->m_type == LightType::point_light)
        {
            PointLight* _light = reinterpret_cast<PointLight*>(_curr);
            _curr += sizeof(PointLight);

            float _intensity = 0.f;
            float3 _lightDir = normalize(_light->m_f3Location - _f3IntPoint);
            float3 _startPoint = _f3IntPoint + (_lightDir * 0.001f);
            Ray _lightRay(_startPoint, _startPoint + _lightDir);

            unsigned int _currentObjectStreamOffset = 0;
            uint32_t _objectStreamSize = _pObjectStream->m_uiSize;

            float3 _poi;
            float3 _poiNormal;
            float3 _poiColor;

            bool _intFound = false;

            ObjectChunk* _currentObject;

            while(_currentObjectStreamOffset < _objectStreamSize)
            {
                _intFound = test(_pObjectStream, _currentObjectStreamOffset, &_currentObject, _lightRay, _poi, _poiNormal, _poiColor);

                if(_intFound)
                {
                    break;
                }

                _currentObjectStreamOffset += sizeof(ObjectChunk) + _currentObject->m_uiSize;
            }

            if(!_intFound)
            {
                float3 _d = _lightRay.m_f3Lab;
                float3 _r = normalize(_d - (2.f * dot(_d, _f3LocalNormal) * _f3LocalNormal));
                float3 _v = normalize(_ray.m_f3Lab);
                float _dotProduct = dot(_r, _v);

                if(_dotProduct > 0.f)
                {
                    _intensity = _m.m_fReflectivity * pow(_dotProduct, _m.m_fShininess);
                }
            }

            _sColor += _light->m_f3Color * _intensity;
        }

        _currentLightStreamSize += sizeof(LightChunk) + sizeof(PointLight);
    }

    return _sColor;
}

template <>
__device__ float3 material_compute_specular<MAX_DEPTH>(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    return make_float3(0, 0, 0);
}

template <int depth>
__device__ float3 material_compute_reflection(SpecularMaterial* _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray);

__device__ bool material_cast_ray(const Ray& _ray, ObjectStream* _pObjectStream, ObjectChunk* _pCurrentObject, ObjectChunk** _ppClosestObject, Hit& _closestHit);

template <int depth>
__device__ float3 material_compute_color(const MaterialType& _materialType, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray);

template <int depth>
__device__ float3 material_compute_translucency(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    float3 _p = normalize(_ray.m_f3Lab);
    float3 _tmpNormal = _f3LocalNormal;
    float _r = 1.f / _m.m_fIOR;
    float _c = -dot(_tmpNormal, _p);

    if(_c < 0.f)
    {
        _tmpNormal = _tmpNormal * -1.f;
        _c = -dot(_tmpNormal, _p);
    }

    float3 _rV = _r * _p + (_r * _c - sqrtf(1.f - pow(_r, 2.f) * (1.f - pow(_c, 2.f)))) * _tmpNormal;

    Ray _rR(_f3IntPoint + (_rV * 0.01f), _f3IntPoint + _rV);

    float3 _poi;
    float3 _poiNormal;
    float3 _poiColor;
    ObjectChunk* _currentObject;
    ObjectChunk* _closestObject;
    Hit _closestHit;

    unsigned int _currentObjectStreamOffset = 0;
    uint32_t _objectStreamSize = _pObjectStream->m_uiSize;

    bool _validInt = false;
    bool _intFound = false;

    while(_currentObjectStreamOffset < _objectStreamSize)
    {
        if(test(_pObjectStream, _currentObjectStreamOffset, &_currentObject, _rR, _poi, _poiNormal, _poiColor))
        {
            _validInt = true;

            break;
        }

        _currentObjectStreamOffset += sizeof(ObjectChunk) + _currentObject->m_uiSize;
    }

    Ray _finalRay;

    if(_validInt)
    {
        float3 _p2 = normalize(_rR.m_f3Lab);
        float3 _tmpNormal2 = _poiNormal;
        float _r2 = 1.f / _m.m_fIOR;
        float _c2 = -dot(_tmpNormal, _p2);

        if(_c2 < 0.f)
        {
            _tmpNormal2 = _tmpNormal2 * -1.f;
            _c2 = -dot(_tmpNormal2, _p2);
        }

        float3 _rV2 = _r2 * _p2 + (_r2 * _c2 - sqrtf(1.f - pow(_r2, 2.f) * (1.f - pow(_c2, 2.f)))) * _tmpNormal2;

        Ray _rR2(_poi + (_rV2 * 0.01f), _poi + _rV2);

        _intFound = material_cast_ray(_rR2, _pObjectStream, _currentObject, &_closestObject, _closestHit);
        _finalRay = _rR2;
    }
    else
    {
        _intFound = material_cast_ray(_rR, _pObjectStream, _currentObject, &_closestObject, _closestHit);
        _finalRay = _rR;
    }

    if(_intFound)
    {
        if(_closestObject->m_material == MaterialType::none)
        {
            return material_compute_diffuse_color(_pObjectStream, _pLightStream, _closestObject, _closestHit.m_f3POI, _closestHit.m_f3POI_N, _closestHit.m_f3POI_C);
        }
        else
        {
            return material_compute_color<depth>(_closestObject->m_material, _pObjectStream, _pLightStream, _closestObject, _closestHit.m_f3POI, _closestHit.m_f3POI_N, _finalRay);
        }
    }

    return make_float3(0, 0, 0);
}

template <>
__device__ float3 material_compute_translucency<MAX_DEPTH>(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    return make_float3(0, 0, 0);
}

template <int depth>
__device__ float3 material_compute_specular(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    float3 _mColor = make_float3(0, 0, 0);
    float3 _rColor = make_float3(0, 0, 0);
    float3 _dColor = make_float3(0, 0, 0);
    float3 _sColor = make_float3(0, 0, 0);
    float3 _tColor = make_float3(0, 0, 0);

    _dColor = material_compute_diffuse_color(_pObjectStream, _pLightStream, _pCurrentObject, _f3IntPoint, _f3LocalNormal, _m.m_f3Color);

    if(_m.m_fReflectivity > 0.f)
    {
        _rColor = material_compute_reflection<depth+1>(&_m, _pObjectStream, _pLightStream, _pCurrentObject, _f3IntPoint, _f3LocalNormal, _ray);
    }

    _mColor = (_rColor * _m.m_fReflectivity) + (_dColor * (1.f - _m.m_fReflectivity));

    if(_m.m_fTranslucency > 0.f)
    {
        _tColor = material_compute_translucency<depth+1>(_m, _pObjectStream, _pLightStream, _pCurrentObject, _f3IntPoint, _f3LocalNormal, _ray);
    }

    _mColor = (_tColor * _m.m_fTranslucency) + (_mColor * (1.f - _m.m_fTranslucency));

    if(_m.m_fShininess > 0.f)
    {
        _sColor = material_compute_specular<depth+1>(_m, _pObjectStream, _pLightStream, _f3IntPoint, _f3LocalNormal, _ray);
    }

    return _mColor + _sColor;
}

template <>
__device__ float3 material_compute_specular<MAX_DEPTH>(SpecularMaterial& _m, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    return make_float3(0, 0, 0);
}

__device__ bool material_cast_ray(const Ray& _ray, ObjectStream* _pObjectStream, ObjectChunk* _pCurrentObject, ObjectChunk** _ppClosestObject, Hit& _closestHit)
{
    unsigned int _currentObjectStreamOffset = 0;
    uint32_t _objectStreamSize = _pObjectStream->m_uiSize;

    float _minDist = 1e6;
    bool _intFound = false;

    Hit _hit;

    ObjectChunk* _currentObject;

    while(_currentObjectStreamOffset < _objectStreamSize)
    {
        if(test(_pObjectStream, _currentObjectStreamOffset, &_currentObject, _ray, _hit.m_f3POI, _hit.m_f3POI_N, _hit.m_f3POI_C, _pCurrentObject))
        {
            _intFound = true;

            float _dist = norm(_hit.m_f3POI - _ray.m_f3Point1);

            if(_dist >= _minDist)
            {
                continue;
            }

            _minDist = _dist;
            *_ppClosestObject = _currentObject;
            _closestHit = _hit;
        }

        _currentObjectStreamOffset += sizeof(ObjectChunk) + _currentObject->m_uiSize;
    }

    return _intFound;
}

template <int depth>
__device__ float3 material_compute_reflection(SpecularMaterial* _pSpecularMaterial, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    float3 _d = _ray.m_f3Lab;
    float3 _r = _d - (2.f * dot(_d, _f3LocalNormal) * _f3LocalNormal);

    Ray _reflectionRay(_f3IntPoint, _f3IntPoint + _r);

    ObjectChunk* _closestObject;

    Hit _closestHit;

    if(!material_cast_ray(_reflectionRay, _pObjectStream, _pCurrentObject, &_closestObject, _closestHit))
    {
        return make_float3(0, 0, 0);
    }

    if(_closestObject->m_material == MaterialType::none)
    {
        return material_compute_diffuse_color(_pObjectStream, _pLightStream, _closestObject, _closestHit.m_f3POI, _closestHit.m_f3POI_N, _closestHit.m_f3POI_C);
    }
    else
    {
        return material_compute_color<depth>(_closestObject->m_material, _pObjectStream, _pLightStream, _closestObject, _closestHit.m_f3POI, _closestHit.m_f3POI_N, _reflectionRay);
    }

    return make_float3(0, 0, 0);
}

template <>
__device__ float3 material_compute_reflection<MAX_DEPTH>(SpecularMaterial* _pSpecularMaterial, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    return make_float3(0, 0, 0);
}

template <int depth>
__device__ float3 material_compute_color(const MaterialType& _materialType, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    if(_materialType == MaterialType::specular)
    {
        SpecularMaterial& _m = *reinterpret_cast<SpecularMaterial*>(reinterpret_cast<uint8_t*>(_pCurrentObject) + sizeof(ObjectChunk));

        return material_compute_specular<depth>(_m, _pObjectStream, _pLightStream, _pCurrentObject, _f3IntPoint, _f3LocalNormal, _ray);
    }

    return make_float3(0, 0, 0);
}

template <>
__device__ float3 material_compute_color<MAX_DEPTH>(const MaterialType& _materialType, ObjectStream* _pObjectStream, LightStream* _pLightStream, ObjectChunk* _pCurrentObject, const float3& _f3IntPoint, const float3& _f3LocalNormal, const Ray& _ray)
{
    return make_float3(0, 0, 0);
}

__global__ void d_ray_trace(unsigned int* _aPixels, unsigned int _uiWidth, unsigned int _uiHeight, Camera& _camera, ObjectStream* _pObjectStream, LightStream* _pLightStream)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i >= _uiWidth || j >= _uiHeight)
    {
		return;
    }

    float x = (float)i;
	float y = (float)j;

    Ray _ray;

    float3 _intPoint;
    float3 _localNormal;
    float3 _localColor;

    float xFact = 1.f / ((float)_uiWidth / 2.f);
    float yFact = 1.f / ((float)_uiHeight / 2.f);

    float normX = ((float)x * xFact) - 1.f;
    float normY = ((float)y * yFact) - 1.f;

    generate_ray(_camera, _ray, normX, normY);

    float3 _closestIntPoint = make_float3(0, 0, 0);
    float3 _closestLocalNormal = make_float3(0, 0, 0);
    float3 _closestLocalColor = make_float3(0, 0, 0);

    ObjectChunk* _closestObject;

    float3 pixelColor = make_float3(0, 0, 0);

    if(find_closest(_pObjectStream, _ray, &_closestObject, _closestIntPoint, _closestLocalNormal, _closestLocalColor))
    {
        if(_closestObject->m_material == MaterialType::none)
        {
            pixelColor = material_compute_diffuse_color(_pObjectStream, _pLightStream, _closestObject, _closestIntPoint, _closestLocalNormal, _closestLocalColor);
        }
        else
        {
            pixelColor = material_compute_color<0>(_closestObject->m_material, _pObjectStream, _pLightStream, _closestObject, _closestIntPoint, _closestLocalNormal, _ray);
        }
    }

    _aPixels[j * _uiWidth + i] = rgbToInt(pixelColor.x, pixelColor.y, pixelColor.z);
}

extern "C" __declspec(dllexport) void ray_trace(unsigned int* _aPixels, unsigned int _uiWidth, unsigned int _uiHeight, Camera* _pCamera, ObjectStream* _pObjectStream, LightStream* _pLightStream)
{
    // int blockSize = 256;
    // int numBlocks = (n + blockSize - 1) / blockSize;

    dim3 block(16, 16, 1);
	dim3 grid(ceil((float)_uiWidth / block.x), ceil((float)_uiHeight / block.y), 1);

    d_ray_trace<<<grid, block>>>(_aPixels, _uiWidth, _uiHeight, *_pCamera, _pObjectStream, _pLightStream);

    auto _e = cudaGetLastError();

    if(_e == cudaSuccess)
    {
        return;
    }

    printf("%s\n", cudaGetErrorString(_e));
}