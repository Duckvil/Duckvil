#include "Parser/config.h"

namespace Duckvil { namespace Parser {

    bool compile_config::is_valid_binary(const std::string& binary)
    {
        return true;
    }

    void compile_config::add_default_include_dirs(compile_config& config)
    {
        config.add_include_dir("E:/Projects/C++/Duckvil/include");
        config.add_include_dir("E:/Projects/C++/Duckvil/external");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/glm");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/json/single_include");
        config.add_include_dir("E:/Projects/C++/Duckvil/__generated_reflection__");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/mono/msvc/include");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/xbyak");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/hex_editor");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/assimp/include");
        config.add_include_dir("F:/Programs/CUDA Toolkit/v11.6/include");
        config.add_include_dir("E:/Projects/C++/Duckvil/external/asio/asio/include");
        
        do_add_macro_definition("DUCKVIL_AST", "");
        do_add_macro_definition("WIN32", "");
        do_add_macro_definition("_WINDOWS", "");
        do_add_macro_definition("DUCKVIL_PLATFORM_WINDOWS", "");
        do_add_macro_definition("DUCKVIL_OUTPUT", "\"E:/Projects/C++/Duckvil/bin\"");
        do_add_macro_definition("_WINDLL", "");
    }

    void compile_config::do_set_flags(cppast::cpp_standard standard, cppast::compile_flags flags)
    {
        switch (standard)
        {
        case cppast::cpp_standard::cpp_98:
            if (flags & cppast::compile_flag::gnu_extensions)
                add_flag("-std=gnu++98");
            else
                add_flag("-std=c++98");
            break;
        case cppast::cpp_standard::cpp_03:
            if (flags & cppast::compile_flag::gnu_extensions)
                add_flag("-std=gnu++03");
            else
                add_flag("-std=c++03");
            break;
        case cppast::cpp_standard::cpp_11:
            if (flags & cppast::compile_flag::gnu_extensions)
                add_flag("-std=gnu++11");
            else
                add_flag("-std=c++11");
            break;
        case cppast::cpp_standard::cpp_14:
            if (flags & cppast::compile_flag::gnu_extensions)
                add_flag("-std=gnu++14");
            else
                add_flag("-std=c++14");
            break;
        case cppast::cpp_standard::cpp_1z:
            if (flags & cppast::compile_flag::gnu_extensions)
                add_flag("-std=gnu++1z");
            else
                add_flag("-std=c++1z");
            break;
        case cppast::cpp_standard::cpp_17:
            if (cppast::libclang_parser::libclang_minor_version() >= 43)
            { // Corresponds to Clang version 5
                if (flags & cppast::compile_flag::gnu_extensions)
                    add_flag("-std=gnu++17");
                else
                    add_flag("-std=c++17");
                break;
            }
            else
                throw std::invalid_argument("c++17 is not yet supported for current version of clang");
        case cppast::cpp_standard::cpp_2a:
            if (cppast::libclang_parser::libclang_minor_version() >= 59)
            { // Corresponds to Clang version 9
                if (flags & cppast::compile_flag::gnu_extensions)
                    add_flag("-std=gnu++2a");
                else
                    add_flag("-std=c++2a");
                break;
            }
            else
                throw std::invalid_argument("c++2a is not yet supported for current version of clang");
        case cppast::cpp_standard::cpp_20:
            if (cppast::libclang_parser::libclang_minor_version() >= 60)
            { // Corresponds to Clang version 10
                if (flags & cppast::compile_flag::gnu_extensions)
                    add_flag("-std=gnu++20");
                else
                    add_flag("-std=c++20");
                break;
            }
            else
                throw std::invalid_argument("c++20 is not yet supported for current version of clang");
        }

        if (flags & cppast::compile_flag::ms_compatibility)
        {
            add_flag("-fms-compatibility");
            // see https://github.com/foonathan/cppast/issues/46
            define_macro("_DEBUG_FUNCTIONAL_MACHINERY", "");
        }

        if (flags & cppast::compile_flag::ms_extensions)
            add_flag("-fms-extensions");
    }

    bool compile_config::do_enable_feature(std::string name)
    {
        add_flag("-f" + std::move(name));
        return true;
    }

    void compile_config::do_add_include_dir(std::string path)
    {
        add_flag("-I" + std::move(path));
    }

    void compile_config::do_add_macro_definition(std::string name, std::string definition)
    {
        auto str = "-D" + std::move(name);
        if (!definition.empty())
            str += "=" + definition;
        add_flag(std::move(str));
    }

    void compile_config::do_remove_macro_definition(std::string name)
    {
        add_flag("-U" + std::move(name));
    }

    const char* compile_config::do_get_name() const noexcept
    {
        return "libclang";
    }

    compile_config::compile_config() :
        compile_config(CPPAST_CLANG_BINARY)
    {
        
    }
    
    compile_config::compile_config(std::string clang_binary) :
        cppast::compile_config({}),
        write_preprocessed_(false),
        fast_preprocessing_(false),
        remove_comments_in_macro_(false)
    {
        // set given clang binary
        set_clang_binary(clang_binary);

        // set macros to detect cppast
        define_macro("__cppast__", "libclang");
        define_macro("__cppast_version_major__", CPPAST_VERSION_MAJOR);
        define_macro("__cppast_version_minor__", CPPAST_VERSION_MINOR);
        add_flag("-Wno-nonportable-include-path");
    }
    
    compile_config::compile_config(const cppast::libclang_compilation_database& database, const std::string& file) :
        compile_config(CPPAST_CLANG_BINARY, database, file)
    {
        
    }
    
    compile_config::compile_config(std::string clang_binary, const cppast::libclang_compilation_database& database, const std::string& file) :
        compile_config(clang_binary)
    {
        
    }

    bool compile_config::set_clang_binary(std::string binary)
    {
        if(is_valid_binary(binary))
        {
            clang_binary_ = binary;

            add_default_include_dirs(*this);

            return true;
        }
        //else
        //{
        //    // first search in current directory, then in PATH
        //    static const char* paths[]
        //        = { "./clang++",   "clang++",       "./clang++-4.0", "clang++-4.0", "./clang++-5.0",
        //           "clang++-5.0", "./clang++-6.0", "clang++-6.0",   "./clang-7",   "clang-7",
        //           "./clang-8",   "clang-8",       "./clang-9",     "clang-9",     "./clang-10",
        //           "clang-10",    "./clang-11",    "clang-11" };
        //    for (auto& p : paths)
        //        if (is_valid_binary(p))
        //        {
        //            clang_binary_ = p;
        //            add_default_include_dirs(*this);
        //            return false;
        //        }

        //    throw std::invalid_argument("unable to find clang binary '" + binary + "'");
        //}
    }

    void compile_config::write_preprocessed(bool b) noexcept
    {
        write_preprocessed_ = b;
    }

    void compile_config::fast_preprocessing(bool b) noexcept
    {
        fast_preprocessing_ = b;
    }

    void compile_config::remove_comments_in_macro(bool b) noexcept
    {
        remove_comments_in_macro_ = b;
    }

    void compile_config::AddInclude(const std::string& _sInclude)
    {
        add_include_dir(_sInclude);
    }

    void compile_config::AddMacroDefinition(const std::string& _sName, const std::string& _sDefinition)
    {
        do_add_macro_definition(_sName, _sDefinition);
    }

}}