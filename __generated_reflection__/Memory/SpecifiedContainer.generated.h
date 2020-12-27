#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
void* Cast(void* _pObject) { return static_cast<SpecifiedContainer*>(_pObject); }