//@ {"targets":[{"name":"enum_concepts.hpp", "type":"include"}]}

#ifndef LIBENUM_ENUMCONCEPTS_HPP
#define LIBENUM_ENUMCONCEPTS_HPP

#include <type_traits>

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
	 * where `n` is item number in the enum definition.
	 *
	 *     enum class Contiguous:int{Foo = 1, Bar, Baz};
	 *     enum class NonContiguous:int{Foo = 1, Bar = 3, Baz = 7};
	 *
	*/
	template<class T>
	concept ContiguousEnum = std::is_enum_v<T> && detail::is_contiguous_enum<T>();
}

#endif