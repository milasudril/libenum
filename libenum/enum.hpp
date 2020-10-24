//@ {"targets":[{"name":"enum.hpp", "type":"include"}]}

#ifndef LIBENUM_ENUM_HPP
#define LIBENUM_ENUM_HPP

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
}

#endif