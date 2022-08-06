#include "Utils/ECS.h"

#include "flecs/flecs.h"

namespace Duckvil { namespace Utils {

    void init_ecs()
    {
        ecs_os_set_api_defaults();
    }

}}