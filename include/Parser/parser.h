#pragma once

#include "Parser/config.h"

namespace Duckvil { namespace Parser {

    template <class Parser>
    class custom
    {
        static_assert(std::is_base_of<cppast::parser, Parser>::value,
            "Parser must be derived from cppast::parser");

    private:
        Parser parser_;
        cppast::detail::intrusive_list<cppast::cpp_file> files_;
        type_safe::object_ref<const cppast::cpp_entity_index> idx_;

    public:
        using parser = Parser;
        using config = typename Parser::config;

        template <typename... Args>
        explicit custom(type_safe::object_ref<const cppast::cpp_entity_index> idx, Args&&... args) :
            parser_(std::forward<Args>(args)...),
            idx_(idx)
        {

        }

        type_safe::optional_ref<const cppast::cpp_file> parse(std::string path, const compile_config& c)
        {
            parser_.logger().log("simple file parser", cppast::diagnostic{ "parsing file '" + path + "'", cppast::source_location(), cppast::severity::info });

            auto file = parser_.parse(*idx_, std::move(path), c);
            auto ptr = file.get();

            if (file)
                files_.push_back(std::move(file));

            return type_safe::opt_ref(ptr);
        }

        bool error() const noexcept
        {
            return parser_.error();
        }

        void reset_error() noexcept
        {
            parser_.reset_error();
        }

        const cppast::cpp_entity_index& index() const noexcept
        {
            return *idx_;
        }

        cppast::detail::iteratable_intrusive_list<cppast::cpp_file> files() const noexcept
        {
            return type_safe::ref(files_);
        }
    };

}}