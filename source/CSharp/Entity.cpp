#include "CSharp/Entity.h"

#include "mono/mono/metadata/tabledefs.h"

namespace Duckvil { namespace CSharp {

    void Script::GetFieldValue(const ScriptField& _field, void* _pOutValue) const
    {
        mono_field_get_value(m_pObject, _field.m_monoField, _pOutValue);
    }

    void Script::SetFieldValue(const ScriptField& _field, const void* _pInValue) const
    {
        mono_field_set_value(m_pObject, _field.m_monoField, (void*)_pInValue);
    }

    void* Script::GetPropertyValue(const ScriptProperty& _property) const
    {
        MonoObject* _object = mono_property_get_value(_property.m_pMonoProperty, m_pObject, 0, 0);
		
        return mono_object_unbox(_object);
    }

    void Script::SetPropertyValue(const ScriptProperty& _property, const void* _pInValue) const
    {
        void* _params[1];

        _params[0] = { (void*)_pInValue };

        mono_property_set_value(_property.m_pMonoProperty, m_pObject, _params, 0);
    }

    Script::Script()
    {

    }

    Script::~Script()
    {

    }

    void Script::Update(double _dDelta)
    {
        void* _params[1];

        _params[0] = &_dDelta;

        mono_runtime_invoke(m_pUpdate, m_pObject, _params, nullptr);
    }

    bool Script::Init()
    {
        mono_runtime_invoke(m_pInit, m_pObject, nullptr, nullptr);

        return true;
    }

    void Script::GetScripts(const char*** _psScripts, uint32_t* _uiCount, void* _pEngine)
    {
        auto _csEngine = static_cast<Duckvil::CSharp::CSharp*>(_pEngine);

        static std::vector<std::string> _classes;

        MonoImage* _coreImage = mono_assembly_get_image(static_cast<MonoAssembly*>(_csEngine->m_pCoreAssembly));
        MonoImage* _appImage = mono_assembly_get_image(static_cast<MonoAssembly*>(_csEngine->m_pAppAssembly));
        MonoClass* _entityScriptClass = mono_class_from_name(_coreImage, "Duckvil", "Entity");

        const MonoTableInfo* _tableInfo = mono_image_get_table_info(_appImage, MONO_TABLE_TYPEDEF);
        int _rows = mono_image_get_table_rows(_appImage, MONO_TABLE_TYPEDEF);

        for(int _i = 0; _i < _rows; ++_i)
        {
            uint32_t _cols[MONO_TYPEDEF_SIZE];

            mono_metadata_decode_row(_tableInfo, _i, _cols, MONO_TYPEDEF_SIZE);

            const char* _name = mono_metadata_string_heap(_appImage, _cols[MONO_TYPEDEF_NAME]);
            const char* _namespace = mono_metadata_string_heap(_appImage, _cols[MONO_TYPEDEF_NAMESPACE]);

            MonoClass* _class = mono_class_from_name(_appImage, _namespace, _name);

            if(_entityScriptClass == _class || !mono_class_implements_interface(_class, _entityScriptClass))
            {
                continue;
            }

            _classes.push_back(std::string(_namespace) + "." + _name);
        }

        *_uiCount = _classes.size();

        *_psScripts = new const char*[*_uiCount];

        for(uint32_t _i = 0; _i < _classes.size(); ++_i)
        {
            const auto& _className = _classes[_i];

            *_psScripts[_i] = _className.c_str();
        }
    }

    void Script::CreateScript(const char* _sScriptName, void* _pEngine)
    {
        auto _csEngine = static_cast<Duckvil::CSharp::CSharp*>(_pEngine);

        std::string _name(_sScriptName);

        size_t _pos = _name.find_last_of(".");

        MonoImage* _coreImage = mono_assembly_get_image(static_cast<MonoAssembly*>(_csEngine->m_pCoreAssembly));
        MonoImage* _appImage = mono_assembly_get_image(static_cast<MonoAssembly*>(_csEngine->m_pAppAssembly));

        m_pClass = mono_class_from_name(_appImage, _name.substr(0, _pos).c_str(), _name.substr(_pos + 1).c_str());
        m_pInit = mono_class_get_method_from_name(m_pClass, "Initialize", 0);
        m_pUpdate = mono_class_get_method_from_name(m_pClass, "Update", 1);

        m_pObject = _csEngine->Create(m_pClass);

        mono_runtime_object_init(m_pObject);

        MonoClass* _entityScriptClass = mono_class_from_name(_coreImage, "Duckvil", "Entity");
        const auto _constructor = mono_class_get_method_from_name(_entityScriptClass, ".ctor", 1);

        void* _params[1];
        size_t _id = GetEntity().ID();

        _params[0] = &_id;

        mono_runtime_invoke(_constructor, m_pObject, _params, nullptr);

        // Process fields

        void* _fIt = nullptr;

        while(MonoClassField* _f = mono_class_get_fields(m_pClass, &_fIt))
        {
            uint32_t _flags = mono_field_get_flags(_f);

            if(!(_flags & FIELD_ATTRIBUTE_PUBLIC))
            {
                continue;
            }

            std::string _fName = mono_field_get_name(_f);
            MonoType* _type = mono_field_get_type(_f);

            _fName = Utils::replace_all(_fName, "_an_", "");

            m_aFields[_fName] =
            {
                .m_type = CSharp::MonoTypeToScriptFieldType(_type),
                .m_monoField =  _f
            };
        }

        // Process properties

        void* _pIt = nullptr;

        while(MonoProperty* _p = mono_class_get_properties(m_pClass, &_pIt))
        {
            MonoClass* _attrClass = mono_class_from_name(_coreImage, "Duckvil", "CppSharedAttribute");
            MonoCustomAttrInfo* _attrsInfo = mono_custom_attrs_from_property(m_pClass, _p);

            if(!mono_custom_attrs_has_attr(_attrsInfo, _attrClass))
            {
                continue;
            }

            MonoMethod* _get = mono_property_get_get_method(_p);
            MonoMethod* _set = mono_property_get_set_method(_p);

            uint32_t flags = mono_method_get_flags(_get, nullptr);

            if(!(flags & METHOD_ATTRIBUTE_PUBLIC))
            {
                continue;
            }

            std::string _pName = mono_property_get_name(_p);

            auto _sig = mono_method_get_signature(_get, _appImage, 0);
            auto _rType = mono_signature_get_return_type(_sig);

            m_aProperties[_pName] =
            {
                .m_type = CSharp::MonoTypeToScriptFieldType(_rType),
                .m_pMonoProperty = _p,
                .m_pGetMethod = _get,
                .m_pSetMethod = _set
            };
        }
    }

    template <typename T>
	void imgui_variable(const Script* _pEntity, const ScriptProperty& _property, const std::string_view& _sName)
    {
        T _value = _pEntity->GetPropertyValue<T>(_property);

        ImGui::InputInt(_sName.data(), &_value);

        _pEntity->SetPropertyValue(_property, _value);
    }

    template <typename T>
    void imgui_variable(const Script* _pEntity, const ScriptField& _field, const std::string_view& _sName)
    {
        T _value = _pEntity->GetFieldValue<T>(_field);

        ImGui::InputInt(_sName.data(), &_value);

        _pEntity->SetFieldValue(_field, _value);
    }

    void Script::DrawDetails() const
    {
        for(const auto& [_name, _field] : m_aFields)
        {
            switch(_field.m_type)
            {
	            case CSharp::ScriptFieldType::Int32:
	            {
	                imgui_variable<int>(this, _field, _name);
	            }
            	break;
            }
        }

        /*for(const auto& [_name, _property] : m_aProperties)
        {
            switch(_property.m_type)
            {
	            case CSharp::ScriptFieldType::Int32:
	            {
	                imgui_variable<int>(this, _property, _name);
	            }
				break;
            }
        }*/
    }

    const std::unordered_map<std::string, ScriptField>& Script::GetFields() const
    {
        return m_aFields;
    }

    void Script::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void Script::OnDraw()
    {

    }

}}