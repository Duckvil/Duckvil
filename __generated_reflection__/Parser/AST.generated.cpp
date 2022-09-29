#include "Parser/AST.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Parser");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_enum = record_enum<Duckvil::Parser::__behavior>(_data, _ntype, "__behavior");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__behavior::__behavior_Nothing, "__behavior_Nothing");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__behavior::__behavior_Skip, "__behavior_Skip");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__behavior::__behavior_Pause, "__behavior_Pause");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__behavior::__behavior_Resume, "__behavior_Resume");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__behavior::__behavior_Proceed, "__behavior_Proceed");
_enum = record_enum<Duckvil::Parser::__ast_entity_type>(_data, _ntype, "__ast_entity_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_function, "__ast_entity_type_function");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_constructor, "__ast_entity_type_constructor");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_destructor, "__ast_entity_type_destructor");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_callback, "__ast_entity_type_callback");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_namespace, "__ast_entity_type_namespace");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_structure, "__ast_entity_type_structure");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_variable, "__ast_entity_type_variable");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_argument, "__ast_entity_type_argument");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_enum, "__ast_entity_type_enum");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_main, "__ast_entity_type_main");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_typedef, "__ast_entity_type_typedef");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_callback_typedef, "__ast_entity_type_callback_typedef");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_define, "__ast_entity_type_define");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_entity_type::__ast_entity_type_undefined, "__ast_entity_type_undefined");
_enum = record_enum<Duckvil::Parser::__ast_structure_type>(_data, _ntype, "__ast_structure_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_structure_type::__ast_structure_type_class, "__ast_structure_type_class");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_structure_type::__ast_structure_type_struct, "__ast_structure_type_struct");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_structure_type::__ast_structure_type_undefined, "__ast_structure_type_undefined");
_enum = record_enum<Duckvil::Parser::__ast_access>(_data, _ntype, "__ast_access");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_access::__ast_access_public, "__ast_access_public");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_access::__ast_access_protected, "__ast_access_protected");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_access::__ast_access_private, "__ast_access_private");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_access::__ast_access_not_specified, "__ast_access_not_specified");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_access::__ast_access_undefined, "__ast_access_undefined");
_enum = record_enum<Duckvil::Parser::__ast_flags>(_data, _ntype, "__ast_flags");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_undefined, "__ast_flags_undefined");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_static, "__ast_flags_static");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_const, "__ast_flags_const");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_inline, "__ast_flags_inline");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_virtual, "__ast_flags_virtual");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_override, "__ast_flags_override");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_flags::__ast_flags_operator, "__ast_flags_operator");
_namespaces.push_back("__ast_inheritance");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Parser;
_type = record_type<Duckvil::Parser::__ast_inheritance>(_data, "__ast_inheritance");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<__ast_access>(_data, _type, offsetof(Duckvil::Parser::__ast_inheritance, m_protection), "m_protection");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_inheritance, m_sName), "m_sName");
_namespaces.pop_back();
_namespaces.push_back("__ast_meta");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_meta>(_data, "__ast_meta");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_meta, m_sKey), "m_sKey");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_meta, m_sValue), "m_sValue");
_namespaces.pop_back();
_namespaces.push_back("__ast_template_element");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_template_element>(_data, "__ast_template_element");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_template_element, m_sType), "m_sType");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_template_element, m_sName), "m_sName");
_namespaces.pop_back();
_namespaces.push_back("__ast_template");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_template>(_data, "__ast_template");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::vector<__ast_template_element>>(_data, _type, offsetof(Duckvil::Parser::__ast_template, m_aElements), "m_aElements");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Parser::__ast_preprocessor_type>(_data, _ntype, "__ast_preprocessor_type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_undef, "__ast_preprocessor_type_undef");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_if, "__ast_preprocessor_type_if");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_ifdef, "__ast_preprocessor_type_ifdef");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_ifndef, "__ast_preprocessor_type_ifndef");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_else, "__ast_preprocessor_type_else");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_elif, "__ast_preprocessor_type_elif");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_endif, "__ast_preprocessor_type_endif");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Parser::__ast_preprocessor_type::__ast_preprocessor_type_undefined, "__ast_preprocessor_type_undefined");
_namespaces.push_back("__ast_preprocessor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_preprocessor>(_data, "__ast_preprocessor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<__ast_preprocessor_type>(_data, _type, offsetof(Duckvil::Parser::__ast_preprocessor, m_type), "m_type");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_preprocessor, m_token), "m_token");
_property = record_property<std::vector<__ast_preprocessor>>(_data, _type, offsetof(Duckvil::Parser::__ast_preprocessor, m_aChildren), "m_aChildren");
_property = record_property<__ast_preprocessor*>(_data, _type, offsetof(Duckvil::Parser::__ast_preprocessor, m_pParent), "m_pParent");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity>(_data, "__ast_entity");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_constructor = record_constructor<Duckvil::Parser::__ast_entity, __ast_entity_type>(_data, _type);
_property = record_property<__ast_entity_type>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_scopeType), "m_scopeType");
_property = record_property<std::vector<__ast_entity*>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_aScopes), "m_aScopes");
_property = record_property<__ast_entity*>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_pParentScope), "m_pParentScope");
_property = record_property<std::vector<__ast_meta>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_aMeta), "m_aMeta");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_aNeededDefines), "m_aNeededDefines");
_property = record_property<__ast_preprocessor>(_data, _type, offsetof(Duckvil::Parser::__ast_entity, m_aNeededDefines2), "m_aNeededDefines2");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_define");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_define>(_data, "__ast_entity_define");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_define, __ast_entity_type>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_define, m_sName), "m_sName");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_argument");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_argument>(_data, "__ast_entity_argument");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_argument, __ast_entity_type>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_argument, m_sType), "m_sType");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_argument, m_sName), "m_sName");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_argument, m_sValue), "m_sValue");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_variable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_variable>(_data, "__ast_entity_variable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_argument>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_variable, __ast_entity_type>(_data, _type);
_property = record_property<__ast_access>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_variable, m_accessLevel), "m_accessLevel");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_structure");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_structure>(_data, "__ast_entity_structure");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_structure, __ast_structure_type>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_sName), "m_sName");
_property = record_property<__ast_structure_type>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_structureType), "m_structureType");
_property = record_property<std::vector<__ast_inheritance>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_aInheritance), "m_aInheritance");
_property = record_property<std::vector<__ast_template>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_aTemplates), "m_aTemplates");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_uiGeneratedBodyMacroLine), "m_uiGeneratedBodyMacroLine");
_property = record_property<__ast_access>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_structure, m_accessLevel), "m_accessLevel");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_enum");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_enum>(_data, "__ast_entity_enum");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_enum>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_enum, m_sName), "m_sName");
_property = record_property<std::vector<std::string>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_enum, m_aElements), "m_aElements");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_namespace");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_namespace>(_data, "__ast_entity_namespace");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_namespace>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_namespace, m_sName), "m_sName");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_callable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_callable>(_data, "__ast_entity_callable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_callable, __ast_entity_type>(_data, _type);
_property = record_property<std::vector<__ast_entity_argument>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callable, m_aArguments), "m_aArguments");
_property = record_property<__ast_access>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callable, m_accessLevel), "m_accessLevel");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_typedef");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_typedef>(_data, "__ast_entity_typedef");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_typedef, __ast_entity_type>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_typedef, m_sType), "m_sType");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_typedef, m_sName), "m_sName");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_callback_typedef");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_callback_typedef>(_data, "__ast_entity_callback_typedef");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_typedef>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_callback_typedef>(_data, _type);
_property = record_property<std::vector<__ast_entity_argument>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callback_typedef, m_aArguments), "m_aArguments");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_callback");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_callback>(_data, "__ast_entity_callback");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_callable>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_callback>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callback, m_sName), "m_sName");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callback, m_sReturnType), "m_sReturnType");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_callback, m_sMemberType), "m_sMemberType");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_constructor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_constructor>(_data, "__ast_entity_constructor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_callable>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_constructor>(_data, _type);
_property = record_property<std::vector<__ast_template>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_constructor, m_aTemplates), "m_aTemplates");
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_destructor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_destructor>(_data, "__ast_entity_destructor");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_callable>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_destructor>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("__ast_entity_function");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_entity_function>(_data, "__ast_entity_function");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<__ast_entity_callable>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Parser::__ast_entity_function>(_data, _type);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_function, m_sName), "m_sName");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_function, m_sReturnType), "m_sReturnType");
_property = record_property<std::vector<__ast_template>>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_function, m_aTemplates), "m_aTemplates");
_property = record_property<__ast_flags>(_data, _type, offsetof(Duckvil::Parser::__ast_entity_function, m_flags), "m_flags");
_namespaces.pop_back();
_namespaces.push_back("__ast");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast>(_data, "__ast");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_namespaces.pop_back();
_namespaces.push_back("user_define");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::user_define>(_data, "user_define");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::user_define, m_sUserDefine), "m_sUserDefine");
_property = record_property<__behavior(*)(__ast*, __lexer_ftable*, __lexer_data*, const std::string&, std::string*)>(_data, _type, offsetof(Duckvil::Parser::user_define, m_fnBehavior), "m_fnBehavior");
_namespaces.pop_back();
_namespaces.push_back("__ast");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast>(_data, "__ast");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<__ast_entity>(_data, _type, offsetof(Duckvil::Parser::__ast, m_main), "m_main");
_property = record_property<__ast_entity*>(_data, _type, offsetof(Duckvil::Parser::__ast, m_pCurrentScope), "m_pCurrentScope");
_property = record_property<__ast_entity*>(_data, _type, offsetof(Duckvil::Parser::__ast, m_pPendingScope), "m_pPendingScope");
_property = record_property<__ast_access>(_data, _type, offsetof(Duckvil::Parser::__ast, m_currentAccess), "m_currentAccess");
_property = record_property<std::string>(_data, _type, offsetof(Duckvil::Parser::__ast, m_sCurrentDefineNeeded), "m_sCurrentDefineNeeded");
_property = record_property<__ast_preprocessor*>(_data, _type, offsetof(Duckvil::Parser::__ast, m_sCurrentDefineNeeded2), "m_sCurrentDefineNeeded2");
_property = record_property<std::vector<user_define>>(_data, _type, offsetof(Duckvil::Parser::__ast, m_aUserDefines), "m_aUserDefines");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Parser::__ast, m_bPendingIfdef), "m_bPendingIfdef");
_property = record_property<std::filesystem::path>(_data, _type, offsetof(Duckvil::Parser::__ast, m_sFile), "m_sFile");
_property = record_property<std::vector<__ast_template>>(_data, _type, offsetof(Duckvil::Parser::__ast, m_aTemplates), "m_aTemplates");
_namespaces.pop_back();
_namespaces.push_back("__ast_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Parser::__ast_ftable>(_data, "__ast_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Parser");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<void(*)(__ast*, __lexer_ftable*, __lexer_data&)>(_data, _type, offsetof(Duckvil::Parser::__ast_ftable, ast_generate), "ast_generate");
_property = record_property<void(*)(const __ast&)>(_data, _type, offsetof(Duckvil::Parser::__ast_ftable, ast_print), "ast_print");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Parser/AST.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
