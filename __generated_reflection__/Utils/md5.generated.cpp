#include "Utils/md5.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
_type = record_type<http>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "http");
_recordedTypes.push_back(_type);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
record_inheritance<//www.bzflag.org)basedon:md5.handmd5.creferenceimplementationofRFC1321Copyright(C)1991-2>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, record_inheritance<RSADataSecurity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, record_inheritance<Inc.Created1991.Allrightsreserved.Licensetocopyandusethissoftwareisgrantedprovidedthatitisidentifiedasthe"RSADataSecurity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, record_inheritance<Inc.MD5Message-DigestAlgorithm"inallmaterialmentioningorreferencingthissoftwareorthisfunction.Licenseisalsograntedtomakeandusederivativeworksprovidedthatsuchworksareidentifiedas"derivedfromtheRSADataSecurity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, record_inheritance<Inc.MD5Message-DigestAlgorithm"inallmaterialmentioningorreferencingthederivedwork.RSADataSecurity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, record_inheritance<Inc.makesnorepresentationsconcerningeitherthemerchantabilityofthissoftwareorthesuitabilityofthissoftwareforanyparticularpurpose.Itisprovided"asis"withoutexpressorimpliedwarrantyofanykind.Thesenoticesmustberetainedinanycopiesofanypartofthisdocumentationand/orsoftware.*/#ifndefBZF_MD5_H#defineBZF_MD5_H#include<cstring>#include<iostream>//asmallclassforcalculatingMD5hashesofstringsorbytearrays//itisnotmeanttobefastorsecure////usage:1)feeditblocksofucharswithupdate()//2)finalize()//3)gethexdigest()string//or//MD5(std::string).hexdigest()////assumesthatcharis8bitandintis32bitclassMD5>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, _function = record_function<http, *>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &http::MD5convertedtoC++MD5, "MD5convertedtoC++MD5");
_property = record_property<MD5(const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(http, text), "text");
_function = record_function<http, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &http::update, "update");
_function = record_function<http, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &http::update, "update");
_function = record_function<http, MD5&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &http::finalize, "finalize");
_function = record_function<http, std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &http::hexdigest, "hexdigest");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Utils/md5.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
