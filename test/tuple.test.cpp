//@{"targets":[{"name":"tuple.test", "type":"application", "autorun":1}]}

#include "libenum/tuple.hpp"

#include <cstdint>
#include <cassert>
#include <string>

namespace Client
{
	enum class PropertyId : uint8_t
	{
		Name = 1,
		YearOfBirth
	};

	constexpr auto end(Enum::Empty<PropertyId>) { return Enum::add(PropertyId::YearOfBirth, 1); }
	constexpr auto begin(Enum::Empty<PropertyId>) { return PropertyId::Name; }

	template<PropertyId n>
	struct PropertyInfo;

	template<>
	struct PropertyInfo<PropertyId::Name>
	{
		using type                        = std::string;
		static constexpr char const* name = "Name";
	};

	template<>
	struct PropertyInfo<PropertyId::YearOfBirth>
	{
		using type                        = long;
		static constexpr char const* name = "Year of birth";
	};

	using MyTuple = Enum::Tuple<PropertyId, PropertyInfo>;
}

namespace Testcases
{
	void getStuffFromMyTuple()
	{
		Client::MyTuple test{"Clive Staples Lewis", 1898};

		assert((test.template get<Client::PropertyId::Name>() == "Clive Staples Lewis"));
		assert((test.template get<Client::PropertyId::YearOfBirth>() == 1898));
	}
}

int main() { Testcases::getStuffFromMyTuple(); }