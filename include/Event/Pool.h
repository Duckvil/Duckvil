#pragma once

namespace Duckvil { namespace Event {

    // TODO: Maybe specific channel type pool? Then it could be used to grouping event channels

    enum pool_specification
    {
        pool_specification_any,
        pool_specification_immediate,
        pool_specification_buffered
    };

    template <pool_specification Specification>
    class Pool
    {

    };

}}