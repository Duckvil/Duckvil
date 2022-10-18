#pragma once

#include "PlugNPlay/Module.h"

#include "RuntimeReflection/Resources.h"
#include "RuntimeReflection/Global.h"
#include "RuntimeReflection/util.h"

#include "RuntimeReflectionStatic/local/enum.h"
#include "RuntimeReflectionStatic/local/argument.h"
#include "RuntimeReflectionStatic/local/type.h"
#include "RuntimeReflectionStatic/local/constructor.h"
#include "RuntimeReflectionStatic/local/property.h"
#include "RuntimeReflectionStatic/local/variant.h"
#include "RuntimeReflectionStatic/local/object.h"

#include "RuntimeReflectionStatic/local/invoke_member_function.h"
#include "RuntimeReflectionStatic/local/invoke_static_function.h"
#include "RuntimeReflectionStatic/local/function.h"

typedef Duckvil::RuntimeReflection::__ftable* (*duckvil_runtime_reflection_init_callback)();

DUCKVIL_EXPORT Duckvil::RuntimeReflection::__ftable* duckvil_runtime_reflection_init();