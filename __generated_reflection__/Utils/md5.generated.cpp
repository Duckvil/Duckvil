#include "Utils/md5.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_namespaces.push_back("http");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<http>(_data, "http");
_recordedTypes.push_back(_type);
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_inheritance<//www.bzflag.org)basedon:md5.handmd5.creferenceimplementationofRFC1321Copyright(C)1991-2>(_data, _type, record_inheritance<RSADataSecurity>(_data, _type, record_inheritance<Inc.Created1991.Allrightsreserved.Licensetocopyandusethissoftwareisgrantedprovidedthatitisidentifiedasthe"RSADataSecurity>(_data, _type, record_inheritance<Inc.MD5Message-DigestAlgorithm"inallmaterialmentioningorreferencingthissoftwareorthisfunction.Licenseisalsograntedtomakeandusederivativeworksprovidedthatsuchworksareidentifiedas"derivedfromtheRSADataSecurity>(_data, _type, record_inheritance<Inc.MD5Message-DigestAlgorithm"inallmaterialmentioningorreferencingthederivedwork.RSADataSecurity>(_data, _type, record_inheritance<Inc.makesnorepresentationsconcerningeitherthemerchantabilityofthissoftwareorthesuitabilityofthissoftwareforanyparticularpurpose.Itisprovided"asis"withoutexpressorimpliedwarrantyofanykind.Thesenoticesmustberetainedinanycopiesofanypartofthisdocumentationand/orsoftware.*/#ifndefBZF_MD5_H#defineBZF_MD5_H#include<cstring>#include<iostream>//asmallclassforcalculatingMD5hashesofstringsorbytearrays//itisnotmeanttobefastorsecure////usage:1)feeditblocksofucharswithupdate()//2)finalize()//3)gethexdigest()string//or//MD5(std::string).hexdigest()////assumesthatcharis8bitandintis32bitclassMD5>(_data, _type, _function = record_function<http, *>(_data, _type, &http::MD5convertedtoC++MD5, "MD5convertedtoC++MD5");
_property = record_property<MD5(const std::string&>(_data, _type, offsetof(http, text), "text");
_function = record_function<http, void>(_data, _type, &http::update, "update");
_function = record_function<http, void>(_data, _type, &http::update, "update");
_function = record_function<http, MD5&>(_data, _type, &http::finalize, "finalize");
_function = record_function<http, std::string>(_data, _type, &http::hexdigest, "hexdigest");
_namespaces.pop_back();
_enum = record_enum<http::>(_data, _ntype, "");
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/md5.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
