//@ {"targets":[{"name":"empty.hpp", "type":"include"}]}

#ifndef LIBENUM_EMPTY_HPP
#define LIBENUM_EMPTY_HPP

#include <type_traits>

namespace Enum
{
	/**
	 * \brief Helper type to make it possible to use ADL without an object.
	 *
	 * `Empty<T>` makes it possible to use ADL without an object of class `T`. Assume T is in
	 * namespace `foo`, then
	 *
	 *     do_stuff(Empty<T>{})
	 *
	 * will find a `do_stuff` implementation in namespace `foo`.
	 */
    template<class T>
    using Empty = std::type_identity<T>;
}

#endif