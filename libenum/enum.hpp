//@ {"targets":[{"name":"enum.hpp", "type":"include"}]}

#ifndef TEXPAINTER_LIBENUM_LIBENUM_ENUM_HPP
#define TEXPAINTER_LIBENUM_LIBENUM_ENUM_HPP

#include "./empty.hpp"

#include <type_traits>
#include <cstddef>

namespace Enum
{
	namespace detail
	{
		template<class T>
		constexpr bool is_contiguous_enum()
		{
			// Unimplementable as of C++20. Always return true for now.
			return true;
		}
	}

	template<auto Id>
	struct Tag
	{
		static constexpr auto value = Id;
		using value_type            = decltype(Id);
	};


	/**
	 * \brief Satisfied if and only if all members of the enum `T` equals `begin(Empty<T>{}) + n`
	 * where `n` is item number in the enum definition
	 *
	 *     enum class Contiguous:int{Foo = 1, Bar, Baz};
	 *     enum class NonContiguous:int{Foo = 1, Bar = 3, Baz = 7};
	 *
	*/
	template<class T>
	concept ContiguousEnum = std::is_enum_v<T>&& detail::is_contiguous_enum<T>();

	/**
	 * \brief Steps `offset` elements and retunrs the result
	*/
	template<ContiguousEnum T>
	constexpr auto add(T base, std::underlying_type_t<T> offset = 1)
	{
		return static_cast<T>(std::underlying_type_t<T>(base) + offset);
	}

	/**
	 * \brief Difference between two enum values
	*/
	template<ContiguousEnum T>
	constexpr auto distance(T begin, T end)
	{
		return static_cast<std::ptrdiff_t>(end) - static_cast<std::ptrdiff_t>(begin);
	}

	namespace detail
	{
		template<ContiguousEnum EnumType>
		struct Size
		{
			static constexpr auto value =
			    distance(begin(Empty<EnumType>{}), end(Empty<EnumType>{}));
		};


		/**
		 * \brief Helper to pull in all members from `EnumItemTraits<>`
		 */
		template<ContiguousEnum EnumType,
		         template<EnumType>
		         class EnumItemTraits,
		         std::underlying_type_t<EnumType> val>
		struct int_to_type: public EnumItemTraits<add(begin(Empty<EnumType>{}), val)>
		{
		};
	}

	namespace detail
	{
		template<auto enum_item>
		struct VisitEnumItem
		{
			using EnumType = decltype(enum_item);

			template<class Function>
			constexpr static void process(Function&& f)
			{
				constexpr auto current_id = add(enum_item, -1);
				if constexpr(current_id != begin(Empty<EnumType>{}))
				{ VisitEnumItem<current_id>::process(f); }
				f(Tag<current_id>{});
			}
		};

		template<auto enum_item>
		struct FindIfEnumItem
		{
			using EnumType = decltype(enum_item);

			template<class Function>
			constexpr static auto process(Function&& f)
			{
				if constexpr(enum_item == end(Empty<EnumType>{}))
				{ return enum_item; }
				else
				{
					if(f(Tag<enum_item>{}))
					{ return enum_item; }

					return FindIfEnumItem<add(enum_item, 1)>::process(f);
				}
			}
		};
	}

	/** Calls `f` on each item in EnumType
	*/
	template<class EnumType, class Function>
	constexpr void forEachEnumItem(Function&& f)
	{
		detail::VisitEnumItem<end(Empty<EnumType>{})>::process(f);
	}

	template<class EnumType, class Function>
	constexpr auto findIfEnumItem(Function&& f)
	{
		return detail::FindIfEnumItem<begin(Empty<EnumType>{})>::process(f);
	}
}

#endif