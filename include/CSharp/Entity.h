#pragma once

#include "Engine/ScriptComponent.h"

#include "RuntimeReflection/Markers.h"

#include "CSharp/CSharp.h"

#include "Editor/Widget.h"

#include "CSharp/Entity.generated.h"

namespace Duckvil { namespace CSharp {

    struct ScriptField
    {
        Duckvil::CSharp::CSharp::ScriptFieldType m_type;
        void* m_monoField;
    };

    struct ScriptProperty
    {
        Duckvil::CSharp::CSharp::ScriptFieldType m_type;
        void* m_pMonoProperty;
        void* m_pGetMethod;
        void* m_pSetMethod;
    };

    class Script : public NativeScriptBase, public Editor::Widget
    {
        DUCKVIL_GENERATED_BODY
    	template <typename T>
        friend void imgui_variable(const Script* _pEntity, const ScriptProperty& _property, const std::string_view& _sName);
        template <typename T>
        friend void imgui_variable(const Script* _pEntity, const ScriptField& _field, const std::string_view& _sName);
    private:
        void* m_pObject;
        void* m_pClass;

        void* m_pInit;
        void* m_pUpdate;

        std::unordered_map<std::string, ScriptField> m_aFields;
        std::unordered_map<std::string, ScriptProperty> m_aProperties;

        void GetFieldValue(const ScriptField& _field, void* _pOutValue) const;
        void* GetPropertyValue(const ScriptProperty& _property) const;

        template <typename T>
        T GetFieldValue(const ScriptField& _field) const
        {
            T _value;

            GetFieldValue(_field, &_value);

            return _value;
        }

        template <typename T>
        T GetPropertyValue(const ScriptProperty& _propert) const
        {
            void* _object = GetPropertyValue(_propert);

            return *(T*)_object;
        }

        void SetFieldValue(const ScriptField& _field, const void* _pInValue) const;
        void SetPropertyValue(const ScriptProperty& _property, const void* _pInValue) const;

        template <typename T>
        void SetFieldValue(const ScriptField& _field, const T& _value) const
        {
            SetFieldValue(_field, (const void*)&_value);
        }

        template <typename T>
        void SetPropertyValue(const ScriptProperty& _property, const T& _value) const
        {
            SetPropertyValue(_property, (const void*)&_value);
        }

    public:
        Script();
        ~Script();

        void Update(double _dDelta) override;

        bool Init() override;

        // Populate C# classes/types which will be used as entity scripts
        static void GetScripts(const char*** _psScripts, uint32_t* _uiCount, void* _pEngine);

        void CreateScript(const char* _sScriptName, void* _pEngine);

        void DrawDetails() const;

        const std::unordered_map<std::string, ScriptField>& GetFields() const;

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}