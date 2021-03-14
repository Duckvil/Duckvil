#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__lexer_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_tokenType", m_tokenType); \
_pSerializer->SerializeProperty("m_aLines", m_aLines); \
_pSerializer->SerializeProperty("m_sCurrentLine", m_sCurrentLine); \
_pSerializer->SerializeProperty("m_uiCurrentLine", m_uiCurrentLine); \
_pSerializer->SerializeProperty("m_uiCurrentCharacterIndex", m_uiCurrentCharacterIndex); \
_pSerializer->SerializeProperty("m_sToken", m_sToken); \
_pSerializer->SerializeProperty("m_cCurrentCharacter", m_cCurrentCharacter); \
_pSerializer->SerializeProperty("m_bWasSpace", m_bWasSpace); \
_pSerializer->SerializeProperty("m_bSpace", m_bSpace); \
_pSerializer->SerializeProperty("m_bNewLine", m_bNewLine); \
_pSerializer->SerializeProperty("m_bEnd", m_bEnd); \
}
