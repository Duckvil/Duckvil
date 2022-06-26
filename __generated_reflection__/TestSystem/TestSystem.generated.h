#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_DUMMY 
#define TestSystem_TestSystem_h_58_REFLECTION_MODULE_DUMMY 
struct TestSystem_TestSystem_h_53_some_func2_args
{
int a;
int b = 2;
int c = 1;
};
#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_NAMED_ARGUMENTS \
public: \
inline int some_func2(const TestSystem_TestSystem_h_53_some_func2_args& _args)\
{\
return some_func2(_args.a, _args.b, _args.c);\
}
struct TestSystem_TestSystem_h_58_some_func_args
{
int a;
int b = 2;
int c = 1;
};
#define TestSystem_TestSystem_h_58_REFLECTION_MODULE_NAMED_ARGUMENTS \
public: \
inline void some_func(const TestSystem_TestSystem_h_58_some_func_args& _args)\
{\
some_func(_args.a, _args.b, _args.c);\
}
#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<Test*>(_pObject); }
#define TestSystem_TestSystem_h_58_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<test*>(_pObject); }
#define TestSystem_TestSystem_h_53_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_iA", m_iA); \
ISystem::Serialize(_pSerializer); \
}
#define TestSystem_TestSystem_h_58_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define TestSystem_TestSystem_h_53_GENERATED_BODY \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_DUMMY \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_NAMED_ARGUMENTS \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_CASTER \
TestSystem_TestSystem_h_53_REFLECTION_MODULE_RUNTIME_COMPILER

#define TestSystem_TestSystem_h_58_GENERATED_BODY \
TestSystem_TestSystem_h_58_REFLECTION_MODULE_DUMMY \
TestSystem_TestSystem_h_58_REFLECTION_MODULE_NAMED_ARGUMENTS \
TestSystem_TestSystem_h_58_REFLECTION_MODULE_CASTER \
TestSystem_TestSystem_h_58_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID TestSystem_TestSystem_h

