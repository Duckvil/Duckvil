#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
void* Cast(void* _pObject) { return static_cast<__ast_ftable*>(_pObject); }