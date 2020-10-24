//@{"targets":[{"name":"variant.test", "type":"application", "autorun":1}]}

#include "libenum/variant.hpp"

#include <cstdint>
#include <cassert>
#include <string>

namespace Client
{
    enum class TypeId:uint8_t{Int32 = 1, Float32};

    constexpr auto end(Enum::Empty<TypeId>){return Enum::add(TypeId::Float32, 1);}
    constexpr auto begin(Enum::Empty<TypeId>){return TypeId::Int32;}

    template<TypeId n>
    struct TypeInfo;

    template<>
    struct TypeInfo<TypeId::Int32>
    {
        using type = int32_t;
        static constexpr char const* name = "i32";
    };

    template<>
    struct TypeInfo<TypeId::Float32>
    {
        using type = float;
        static constexpr char const* name = "f32";
    };

    using MyVariant = Enum::Variant<TypeId, TypeInfo>;
}

namespace Testcases
{
	void visitMyVariantContainingFloat()
	{
		Client::MyVariant test{1.0f};

		auto res = Enum::visit([]<class Item>(Item const&){
			return std::string{Client::TypeInfo<Enum::WhichV<Item, Client::MyVariant>>::name};
		}, test);
		assert(res == "f32");
	}

	void visitMyVariantContainingInt()
	{
		Client::MyVariant test{1};

		auto res = Enum::visit([]<class Item>(Item const&){
			return std::string{Client::TypeInfo<Enum::WhichV<Item, Client::MyVariant>>::name};
		}, test);

		assert(res == "i32");
	}
}

int main()
{
	Testcases::visitMyVariantContainingFloat();
	Testcases::visitMyVariantContainingInt();
}