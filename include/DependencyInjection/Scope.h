#pragma once

namespace Duckvil { namespace DependencyInjection {

    enum class Scope
    {
        SINGLETON,
        SCOPED,
        TRANSIENT
    };

}}