#pragma once

#include "DependencyInjection/IDependencyResolver.h"

#include "Memory/Vector.h"

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace DependencyInjection {

    class DependencyResolver : public IDependencyResolver
    {
    private:
        Memory::Vector<Dependency> m_aDependencies;

        Memory::FreeList m_heap;

        Event::Pool<Event::mode::immediate>* m_pEngineEventPool;

        const Dependency& FindDependency(size_t _ullTypeID, RuntimeReflection::__duckvil_resource_type_t* _pTypeHandle = nullptr);
    	Memory::Vector<Dependency> FindDependencies(size_t _ullTypeID, RuntimeReflection::__duckvil_resource_type_t* _pTypeHandle = nullptr);
        
    public:
        DependencyResolver(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool);
        ~DependencyResolver();

        bool Register(const Dependency& _dependency) override;
        bool Resolve(const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, void** _ppResolvedObject = nullptr, bool _bHot = false) override;
    };

}}