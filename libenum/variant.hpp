//@ {"targets":[{"name":"variaint.hpp", "type":"include"}]}

#ifndef LIBENUM_VARIANT_HPP
#define LIBENUM_VARIANT_HPP

#include <variant>
#include <type_traits>

namespace Enum
{
    template<class T>
    using Empty = std::type_identity<T>;

    namespace detail
    {
        template <class EnumType, template<EnumType> class EnumItemTraits, class T=std::make_integer_sequence<std::underlying_type_t<EnumType>, end(Empty<EnumType>{})> >
        struct make_variant;

        template<class EnumType, template<EnumType> class EnumItemTraits, std::underlying_type_t<EnumType> val>
        struct IntToType:public EnumItemTraits<static_cast<EnumType>(val)>{};

        template <class EnumType, template<EnumType> class EnumItemTraits, std::underlying_type_t<EnumType>... indices>
        struct make_variant<EnumType, EnumItemTraits, std::integer_sequence<std::underlying_type_t<EnumType>, indices...> >
        {
            using type = std::variant<typename IntToType<EnumType, EnumItemTraits, indices>::type...>;
        };
    }

    template<class EnumType, template<EnumType> class EnumItemTraits>
    using Variant = typename detail::make_variant<EnumType, EnumItemTraits>::type;
}

#endif