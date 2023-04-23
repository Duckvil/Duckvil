#include "DependencyInjection/DependencyInjection.h"

#include "Utils/RuntimeDependencyInjector.h"

#include "Engine/Events/InjectConstructorArgumentEvent.h"

#include "DependencyInjection/Scope.h"

namespace Duckvil { namespace DependencyInjection {

    const DependencyResolver::Dependency& DependencyResolver::FindDependency(size_t _ullTypeID, RuntimeReflection::__duckvil_resource_type_t* _pTypeHandle)
    {
        for(const auto& _dependency : m_aDependencies)
        {
            if(_dependency.m_ullTypeID != _ullTypeID)
            {
                continue;
            }

            if(_pTypeHandle != nullptr)
            {
                *_pTypeHandle = RuntimeReflection::get_type(_ullTypeID);
            }

            return _dependency;
        }

        throw DependencyNotFound();
    }

    DependencyResolver::DependencyResolver(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool) :
		m_pEngineEventPool(_pEngineEventPool)
    {
        _heap.Allocate(m_heap, 1024 * 64);

        m_heap.Allocate(m_aDependencies, 1);
    }

    DependencyResolver::~DependencyResolver()
    {

    }

    bool DependencyResolver::Register(const Dependency& _dependency)
    {
        if(m_aDependencies.Full())
        {
            m_aDependencies.Resize(m_aDependencies.Size() * 2);
        }

        m_aDependencies.Allocate(_dependency);

        return true;
    }

    bool DependencyResolver::Resolve(const RuntimeReflection::__duckvil_resource_type_t& _clientTypeHandle, const RuntimeReflection::__duckvil_resource_constructor_t& _constructorHandle, void** _ppResolvedObject, bool _bHot)
    {
        RuntimeReflection::__constructor_t _constructor = RuntimeReflection::get_constructor(_clientTypeHandle, _constructorHandle);
        uint32_t _constructorArgumentsCount = DUCKVIL_SLOT_ARRAY_SIZE(_constructor.m_arguments);

        if(_constructorArgumentsCount == 0)
        {
            const auto& _constructors = RuntimeReflection::get_constructors(m_heap, _clientTypeHandle);

	        for(const auto& _consHandle : _constructors)
	        {
	            RuntimeReflection::__constructor_t _cons = RuntimeReflection::get_constructor(_clientTypeHandle, _consHandle);

	            if(DUCKVIL_SLOT_ARRAY_SIZE(_cons.m_arguments) > 0)
	            {
	                return false;
	            }
	        }

            RuntimeDependencyInjector _fap(5);

            _fap.Push(m_heap.GetMemoryInterface());
            _fap.Push(m_heap.GetAllocator());
            _fap.Push(RuntimeReflection::get_current().m_pReflection);
            _fap.Push(RuntimeReflection::get_current().m_pReflectionData);
            _fap.Push(_bHot);

            _fap.Call(_constructor.m_pData);

            void* _resolvedObject = _fap.Execute();

            if(_ppResolvedObject != nullptr)
            {
                *_ppResolvedObject = _resolvedObject;
            }

            return true;
        }
        
        RuntimeDependencyInjector _fap(5 + _constructorArgumentsCount);

        _fap.Push(m_heap.GetMemoryInterface());
        _fap.Push(m_heap.GetAllocator());
        _fap.Push(RuntimeReflection::get_current().m_pReflection);
        _fap.Push(RuntimeReflection::get_current().m_pReflectionData);
        _fap.Push(_bHot);

        for(uint32_t i = 0; i < _constructorArgumentsCount; ++i)
        {
            const RuntimeReflection::__argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, i);

            if(_argument.m_traits & RuntimeReflection::property_traits::is_pointer)
            {
	            // Pointer

                RuntimeReflection::__duckvil_resource_type_t _typeHandle;

                const Dependency& _dependency = FindDependency(_argument.m_ullTypeID, &_typeHandle);

                if(_dependency.m_scope == Scope::SINGLETON)
                {
	                if(_dependency.m_fnOnInject != nullptr && !_dependency.m_fnOnInject(_clientTypeHandle, _constructorHandle, { i }))
	                {
	                    // Call events

	                    InjectConstructorArgumentEvent::Info _info;

	                    _info.m_uiArgumentIndex = i;
	                    _info.m_constructorHandle = _constructorHandle;
	                    _info.m_typeHandle = _clientTypeHandle;

	                    InjectConstructorArgumentEvent _e(&_fap, _info, _argument);

	                    m_pEngineEventPool->Broadcast(_e);

	                    if(!_e.m_bSuccess)
	                    {
	                        auto _type = RuntimeReflection::get_type(_argument.m_ullTypeID);

	                        throw std::exception("Could not resolve dependency");
	                    }

		                continue;
	                }

	                /*if(_dependency.m_scope != Scope::SINGLETON)
	                {
	                    throw std::exception("Not implemented");
	                }*/

	                _fap.Push(_dependency.m_pData);
                }
                else if(_dependency.m_scope == Scope::TRANSIENT)
                {
	                if(_dependency.m_fnFactory != nullptr)
	                {
                        void* _data = _dependency.m_fnFactory(_clientTypeHandle, _constructorHandle, { i });

                        _fap.Push(_data);
	                }
                }
            }
            else
            {
	            // Reference

                RuntimeReflection::__duckvil_resource_type_t _typeHandle;

            	const Dependency& _dependency = FindDependency(_argument.m_ullTypeID, &_typeHandle);

                if(_dependency.m_fnOnInject != nullptr)
                {
                    throw std::exception("Not implemented");
                }

                const void* _data = nullptr;

                if(_dependency.m_scope == Scope::SINGLETON)
                {
                    _data = _dependency.m_pData;
                }
                else if(_dependency.m_scope == Scope::TRANSIENT && _dependency.m_fnFactory != nullptr)
                {
                    _data = _dependency.m_fnFactory(_clientTypeHandle, _constructorHandle, { i });
                }

                RuntimeReflection::__duckvil_resource_function_t _functionHandle = RuntimeReflection::get_function_handle<IDependencyInjector*, const void*>(_typeHandle, "Push");

                if(_functionHandle.m_ID == -1)
                {
                    /*throw std::exception("'Push' function not found!");*/

                    return false;
                }

                RuntimeReflection::invoke_static<IDependencyInjector*, const void*>(_typeHandle, _functionHandle, &_fap, _data);
            }
        }

        _fap.Call(_constructor.m_pData);

        void* _resolvedObject = _fap.Execute();

        if(_ppResolvedObject != nullptr)
        {
            *_ppResolvedObject = _resolvedObject;
        }

        return true;
    }

}}