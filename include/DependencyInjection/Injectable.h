#pragma once

#include "DependencyInjection/Scope.h"

namespace Duckvil { namespace DependencyInjection {

    struct Injectable
    {
        Scope m_scope;

        Injectable()
        {
	        
        }

        Injectable(Scope _scope) :
			m_scope(_scope)
        {
	        
        }
    };

}}