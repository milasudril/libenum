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
        template<ContiguousEnum EnumType>
        struct Size
        {
            static constexpr auto value = distance(begin(Empty<EnumType>{}), end(Empty<EnumType>{}));
        };

        template<ContiguousEnum EnumType,
			template<EnumType> class EnumItemTraits,
			class T = std::make_integer_sequence<std::underlying_type_t<EnumType>,
				Size<EnumType>::value  // NOTE: calling distance here triggers ICE in gcc 10.2
			>
		>
        struct make_variant;

        template<ContiguousEnum EnumType,
			template<EnumType> class EnumItemTraits,
			std::underlying_type_t<EnumType> val
		>
        struct int_to_type:public EnumItemTraits<add(begin(Empty<EnumType>{}), val)>{};

		template <ContiguousEnum EnumType,
			template<EnumType> class EnumItemTraits,
			std::underlying_type_t<EnumType>... indices
		>
		struct make_variant<EnumType, EnumItemTraits, std::integer_sequence<std::underlying_type_t<EnumType>, indices...> >
		{
			using type = std::variant<typename int_to_type<EnumType, EnumItemTraits, indices>::type...>;
		};
    }

    template<ContiguousEnum EnumType, template<EnumType> class EnumItemTraits>
    class Variant:private detail::make_variant<EnumType, EnumItemTraits>::type
    {
	public:
		using Base = typename detail::make_variant<EnumType, EnumItemTraits>::type;
		using index_type = EnumType;
		template<index_type index>
		using traits = EnumItemTraits<index>;

		using Base::Base;
		using Base::valueless_by_exception;

		constexpr auto index() const { return add(begin(Empty<EnumType>{}), Base::index()); }

		template<class Func>
 		decltype(auto) visit(Func&& f) { return std::visit(std::forward<Func>(f), base()); }

		template<class Func>
 		decltype(auto) visit(Func&& f) const { return std::visit(std::forward<Func>(f), base()); }

 		static constexpr auto size() { return std::variant_size_v<Base>;}

 		template<index_type i>
 		using variant_alternative = std::variant_alternative_t<distance(begin(Empty<EnumType>{}), i),
			typename detail::make_variant<EnumType, EnumItemTraits>::type>;

	private:
		Base const& base() const
		{
			return *this;
		}

		Base& base()
		{
			return *this;
		}
	};

	template<class Func, ContiguousEnum EnumType, template<EnumType> class EnumItemTraits>
	decltype(auto) visit(Func&& func, Variant<EnumType, EnumItemTraits> const& var)
	{
		return var.visit(std::forward<Func>(func));
	}

	template<class Func, ContiguousEnum EnumType, template<EnumType> class EnumItemTraits>
	decltype(auto) visit(Func&& func, Variant<EnumType, EnumItemTraits>& var)
	{
		return var.visit(std::forward<Func>(func));
	}

	namespace detail
	{
		template<class T, class VariantType, size_t I = VariantType::size()>
		class which
		{
			static constexpr auto Index = I - 1;
			static constexpr auto EnumVal = add(begin(Empty<typename VariantType::index_type>{}), Index);
			using CurrentAlternative = typename VariantType::variant_alternative<EnumVal>;
		public:
			static constexpr auto value = std::is_same_v<T, CurrentAlternative>?
			 EnumVal : which<T, VariantType, Index>::value;
		};

		template<class T, class VariantType>
		class which<T, VariantType, 0>
		{
		public:
			static constexpr typename VariantType::index_type value{0};
		};
	}

    template<class T, class VariantType>
    constexpr auto WhichV = detail::which<T, VariantType>::value;

}

#endif