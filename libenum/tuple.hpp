//@ {"targets":[{"name":"tuple.hpp", "type":"include"}]}

#ifndef TEXPAINTER_LIBENUM_LIBENUM_TUPLE_HPP
#define TEXPAINTER_LIBENUM_LIBENUM_TUPLE_HPP

#include "./empty.hpp"
#include "./enum.hpp"

#include <type_traits>
#include <tuple>

namespace Enum
{
	namespace detail
	{
		template<ContiguousEnum EnumType,
		         template<EnumType>
		         class EnumItemTraits,
		         class T = std::make_integer_sequence<
		             std::underlying_type_t<EnumType>,
		             Size<EnumType>::value  // NOTE: calling distance here triggers ICE in gcc 10.2
		             >>
		struct make_tuple;

		template<ContiguousEnum EnumType,
		         template<EnumType>
		         class EnumItemTraits,
		         std::underlying_type_t<EnumType>... indices>
		struct make_tuple<EnumType,
		                  EnumItemTraits,
		                  std::integer_sequence<std::underlying_type_t<EnumType>, indices...>>
		{
			using type =
			    std::tuple<typename int_to_type<EnumType, EnumItemTraits, indices>::type...>;
		};
	}

	template<ContiguousEnum EnumType, template<EnumType> class EnumItemTraits>
	class Tuple: private detail::make_tuple<EnumType, EnumItemTraits>::type
	{
	public:
		using Base       = typename detail::make_tuple<EnumType, EnumItemTraits>::type;
		using index_type = EnumType;
		template<index_type index>
		using traits = EnumItemTraits<index>;

		using Base::Base;

		static constexpr auto size() { return std::tuple_size_v<Base>; }

		template<index_type i>
		using tuple_element = std::tuple_element_t<distance(begin(Empty<EnumType>{}), i), Base>;

		template<index_type i>
		constexpr auto const& get() const
		{
			return std::get<distance(begin(Empty<EnumType>{}), i)>(base());
		}

		template<index_type i>
		constexpr auto& get()
		{
			return std::get<distance(begin(Empty<EnumType>{}), i)>(base());
		}

	private:
		Base const& base() const { return *this; }

		Base& base() { return *this; }
	};

	namespace detail
	{
		template<auto types,
		         template<auto>
		         class EnumItemTraits,
		         class T = std::make_integer_sequence<size_t, types.size()>>
		struct make_tuple_from_array;

		template<auto types, template<auto> class EnumItemTraits, size_t index>
		struct int_to_type_array: public EnumItemTraits<types[index]>
		{
		};

		template<auto types, template<auto> class EnumItemTraits, size_t... indices>
		struct make_tuple_from_array<types,
		                             EnumItemTraits,
		                             std::integer_sequence<size_t, indices...>>
		{
			using type =
			    std::tuple<typename int_to_type_array<types, EnumItemTraits, indices>::type...>;
		};
	}

	template<auto types, template<auto> class EnumItemTraits>
	using TupleFromTypeArray = detail::make_tuple_from_array<types, EnumItemTraits>::type;
}
#endif