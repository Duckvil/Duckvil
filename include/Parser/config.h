#pragma once

#include "cppast/compile_config.hpp"
#include "cppast/libclang_parser.hpp"

namespace Duckvil { namespace Parser {

    namespace detail
    {
        struct compile_config_access;
    }

    class compile_config final : public cppast::compile_config
    {
    private:
        bool is_valid_binary(const std::string& binary);
        void add_default_include_dirs(compile_config& config);
        void do_set_flags(cppast::cpp_standard standard, cppast::compile_flags flags) override;
        bool do_enable_feature(std::string name) override;
        void do_add_include_dir(std::string path) override;
        void do_add_macro_definition(std::string name, std::string definition) override;
        void do_remove_macro_definition(std::string name) override;
        const char* do_get_name() const noexcept override;

        std::string clang_binary_;
        bool write_preprocessed_ : 1;
        bool fast_preprocessing_ : 1;
        bool remove_comments_in_macro_ : 1;

        friend detail::compile_config_access;

    public:
        compile_config();
        compile_config(std::string clang_binary);
        compile_config(const cppast::libclang_compilation_database& database, const std::string& file);
        compile_config(std::string clang_binary, const cppast::libclang_compilation_database& database, const std::string& file);

        // compile_config(const cppast::libclang_compile_config& other) = default;
        // compile_config& operator=(const cppast::libclang_compile_config& other) = default;
        
        bool set_clang_binary(std::string binary);
        void write_preprocessed(bool b) noexcept;
        void fast_preprocessing(bool b) noexcept;
        void remove_comments_in_macro(bool b) noexcept;
    };

}}