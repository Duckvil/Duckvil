#pragma once

#include <type_traits>

#define DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(type, kind, trait) (Duckvil::RuntimeReflection::traits::kind::trait<type>::value ? static_cast<kind ## _traits>(_traits | kind ## _traits::trait) : _traits)

namespace Duckvil { namespace RuntimeReflection {

#ifdef DUCKVIL_PLATFORM_WINDOWS
    enum class property_traits : uint16_t
#elif DUCKVIL_PLATFORM_LINUX
    enum class property_traits
#endif
    {
        is_pointer          = 1 << 0,
        is_reference        = 1 << 1,
        is_array            = 1 << 2,
        is_void             = 1 << 3,
        is_integral         = 1 << 4,
        is_floating_point   = 1 << 5,
        is_enum             = 1 << 6,
        is_bool             = 1 << 7,
        is_const            = 1 << 8,
        is_static           = 1 << 9
    };

#ifdef DUCKVIL_PLATFORM_WINDOWS
    enum class function_traits : uint8_t
#elif DUCKVIL_PLATFORM_LINUX
    enum class function_traits
#endif
    {
        is_static   = 1 << 0,
        is_const    = 1 << 1
    };

#ifdef DUCKVIL_PLATFORM_LINUX
    inline property_traits operator|(property_traits a, property_traits b)
    {
        return static_cast<property_traits>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline function_traits operator|(function_traits a, function_traits b)
    {
        return static_cast<function_traits>(static_cast<int>(a) | static_cast<int>(b));
    }
#elif DUCKVIL_PLATFORM_WINDOWS
    inline property_traits operator|(property_traits a, property_traits b)
    {
        return static_cast<property_traits>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
    }

    inline bool operator&(property_traits a, property_traits b)
    {
        return static_cast<uint16_t>(a) & static_cast<uint16_t>(b);
    }

    inline function_traits operator|(function_traits a, function_traits b)
    {
        return static_cast<function_traits>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
#endif

    namespace traits {

        namespace function {

            template<class T> 
            struct is_const : std::false_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args...) const> : std::true_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args...) const &> : std::true_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args...) const &&> : std::true_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args..., ...) const> : std::true_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args..., ...) const &> : std::true_type {};

            template<class R, class T, class... Args> 
            struct is_const<R (T::*)(Args..., ...) const &&> : std::true_type {};

            template <typename T>
            using is_static = std::negation<std::is_member_function_pointer<T>>;

        }

        namespace property {

            template <typename Type>
            struct is_bool : std::is_same<bool, typename std::remove_cv<Type>::type> {};

            template <typename Type>
            using is_reference = std::is_reference<Type>;

            template <typename Type>
            using is_array = std::is_array<Type>;

            template <typename Type>
            using is_void = std::is_void<Type>;

            template <typename Type>
            using is_integral = std::is_integral<Type>;

            template <typename Type>
            using is_floating_point = std::is_floating_point<Type>;

            template <typename Type>
            using is_enum = std::is_enum<Type>;

            template <typename Type>
            using is_const = std::is_const<Type>;

            template <typename Type>
            using is_pointer = std::is_pointer<Type>;

            // template <typename Type>
            // using is_static = std::negation<std::is_member_pointer<Type>>;

        }

    }

    template <typename Type>
    static inline property_traits recorder_generate_property_traits()
    {
        property_traits _traits = {};

        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_pointer);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_reference);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_array);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_void);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_integral);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_floating_point);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_enum);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_bool);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_const);
        // _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, property, is_static);

        return _traits;
    }

    template <typename Type>
    static inline function_traits recorder_generate_function_traits()
    {
        function_traits _traits = {};

        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, function, is_const);
        _traits = DUCKVIL_RUNTIME_REFLECTION_RECORDER_TRAIT(Type, function, is_static);

        return _traits;
    }

    template <typename Type>
    static inline property_traits recorder_generate_property_traits(const Type& _type)
    {
        return recorder_generate_property_traits<Type>();
    }

    template <typename Type>
    static inline property_traits recorder_generate_function_traits(const Type& _type)
    {
        return recorder_generate_function_traits<Type>();
    }

}}