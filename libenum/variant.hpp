//@ {"targets":[{"name":"variant.hpp", "type":"include"}]}

#ifndef LIBENUM_VARIANT_HPP
#define LIBENUM_VARIANT_HPP

#include "./empty.hpp"
#include "./enum.hpp"

#include <type_traits>
#include <variant>

namespace Enum
{
    namespace detail
    {
        template<ContiguousEnum EnumType,
			template<EnumType> class EnumItemTraits,
			class T = std::make_integer_sequence<std::underlying_type_t<EnumType>,
				size(Empty<EnumType>{})
			>
		>
        struct make_variant;

        template<ContiguousEnum EnumType,
			template<EnumType> class EnumItemTraits,
			std::underlying_type_t<EnumType> val>
        struct int_to_type:public EnumItemTraits<add(begin(Empty<EnumType>{}), val)>{};

        template <ContiguousEnum EnumType, template<EnumType> class EnumItemTraits, std::underlying_type_t<EnumType>... indices>
        struct make_variant<EnumType, EnumItemTraits, std::integer_sequence<std::underlying_type_t<EnumType>, indices...> >
        {
            using type = std::variant<typename int_to_type<EnumType, EnumItemTraits, indices>::type...>;
        };
    }

    template<ContiguousEnum EnumType, template<EnumType> class EnumItemTraits>
    using Variant = typename detail::make_variant<EnumType, EnumItemTraits>::type;
}

#endif