#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define CSharp_TestSharedScript_h_18_REFLECTION_MODULE_CSHARP \
public:\
MonoClassField* m_pCSharpField_a;\
void a(int _v);\
int a() const;\
DUCKVIL_CSHARP_VARIABLE_CHANGED(TestSharedScript, int, a);\
MonoMethod* m_pCSharpMethod_Say;\
void generated_csharp_Say(int a);\
void generated_csharp_Say(duckvil_cs_Say _args);\
void Say(int a);\
MonoMethod* m_pCSharpMethod_Initialize;\
void generated_csharp_Initialize();\
void generated_csharp_Initialize(duckvil_cs_Initialize _args);\
void Initialize();\
private: \
void* m_pCSharpObject;\
void* m_pCSharpClass;\
void SetCSharpClass(void* _pClass) override\
{\
m_pCSharpClass = _pClass;\
}\
void SetCSharpObject(void* _pObject) override\
{\
m_pCSharpObject = _pObject;\
}\
static void  csharp_internal_HelloFrom(size_t _ullSharedScriptID, int a);\
void InternalInit() override;
#define CSharp_TestSharedScript_h_18_REFLECTION_MODULE_DUMMY 
#define CSharp_TestSharedScript_h_18_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<TestSharedScript*>(_pObject); }
#define CSharp_TestSharedScript_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define CSharp_TestSharedScript_h_18_GENERATED_BODY \
CSharp_TestSharedScript_h_18_REFLECTION_MODULE_CSHARP \
CSharp_TestSharedScript_h_18_REFLECTION_MODULE_DUMMY \
CSharp_TestSharedScript_h_18_REFLECTION_MODULE_CASTER

#define CSharp_TestSharedScript_h_0_GENERATED_BODY \
CSharp_TestSharedScript_h_0_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID CSharp_TestSharedScript_h

