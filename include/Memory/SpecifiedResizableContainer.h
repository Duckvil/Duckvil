#include "Memory/SpecifiedContainer.h"

namespace Duckvil { namespace Memory {

    template <typename ContainerType, typename Type>
    struct SpecifiedResizableContainer : public SpecifiedContainer<ContainerType, Type>
    {
        typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedResizableContainer* _pThis, std::size_t _ullNewSize);

        SpecifiedResizableContainer()
        {

        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator) :
            SpecifiedContainer(_pMemory, _pAllocator)
        {

        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pSpecifiedContainer) :
            SpecifiedContainer(_pMemory, _pAllocator, _pSpecifiedContainer)
        {

        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer, const copy_callback& _fnCopy, const destruct_callback& _fnDestruct, const resize_callback& _fnResize) :
            SpecifiedContainer(_pMemory, _pAllocator, _pContainer, _fnCopy, _fnDestruct),
            m_fnResize(_fnResize)
        {

        }

        resize_callback m_fnResize;
    };

}}