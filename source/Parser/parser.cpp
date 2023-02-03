#include "Parser/parser.h"

namespace Duckvil { namespace Parser {

    namespace detail
    {
        struct compile_config_access
        {
            static const std::string& clang_binary(const compile_config& config)
            {
                return config.clang_binary_;
            }

            static const std::vector<std::string>& flags(const compile_config& config)
            {
                return config.get_flags();
            }

            static bool write_preprocessed(const compile_config& config)
            {
                return config.write_preprocessed_;
            }

            static bool fast_preprocessing(const compile_config& config)
            {
                return config.fast_preprocessing_;
            }

            static bool remove_comments_in_macro(const compile_config& config)
            {
                return config.remove_comments_in_macro_;
            }
        };

        void for_each_file(const compile_config& database, void* user_data, void (*callback)(void*, std::string));
    }

    // template <class Parser>
    // template <typename... Args>
    // custom<Parser>::custom(type_safe::object_ref<const cppast::cpp_entity_index> idx, Args&&... args) :
    //     parser_(std::forward<Args>(args)...),
    //     idx_(idx)
    // {

    // }

    // template <class Parser>
    // type_safe::optional_ref<const cppast::cpp_file> custom<Parser>::parse(std::string path, const compile_config& c)
    // {
    //     parser_.logger().log("simple file parser", cppast::diagnostic{ "parsing file '" + path + "'", cppast::source_location(), cppast::severity::info });

    //     auto file = parser_.parse(*idx_, std::move(path), c);
    //     auto ptr = file.get();

    //     if (file)
    //         files_.push_back(std::move(file));

    //     return type_safe::opt_ref(ptr);
    // }

    // template <class Parser>
    // bool custom<Parser>::error() const noexcept
    // {
    //     return parser_.error();
    // }

    // template <class Parser>
    // void custom<Parser>::reset_error() noexcept
    // {
    //     parser_.reset_error();
    // }

    // template <class Parser>
    // const cppast::cpp_entity_index& custom<Parser>::index() const noexcept
    // {
    //     return *idx_;
    // }

    // template <class Parser>
    // cppast::detail::iteratable_intrusive_list<cppast::cpp_file> custom<Parser>::files() const noexcept
    // {
    //     return type_safe::ref(files_);
    // }

}}