#pragma once

#include "RuntimeReflectionStatic/local/variant.h"

#include "RuntimeReflectionStatic/global/Global.h"

namespace Duckvil { namespace RuntimeReflection {

    const __variant_t& get_variant_key(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle);

    const __variant_t& get_variant_value(DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(variant_t) _handle);

}}