//@ {"targets":[{"name":"tuple.hpp", "type":"include"}]}

#ifndef LIBENUM_TUPLE_HPP
#define LIBENUM_TUPLE_HPP

#include "./empty.hpp"
#include "./enum.hpp"

#include <type_traits>
#include <tuple>

namespace Enum
{
	namespace detail
	{
		template<ContiguousEnum EnumType>
		struct Size
		{
			static constexpr auto value =
			    distance(begin(Empty<EnumType>{}), end(Empty<EnumType>{}));
		};

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
		         std::underlying_type_t<EnumType> val>
		struct int_to_type: public EnumItemTraits<add(begin(Empty<EnumType>{}), val)>
		{
		};

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
}

#endif