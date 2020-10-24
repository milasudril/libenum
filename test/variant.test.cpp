//@{"targets":[{"name":"variant.test", "type":"application", "autorun":1}]}

#include "libenum/variant.hpp"

#include <cstdint>

namespace Client
{
    enum class TypeId:uint8_t{Int32, Float32};
    constexpr auto end(Library::Empty<TypeId>){return static_cast<std::underlying_type_t<TypeId>>(TypeId::Float32) + 1;}

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

    using MyVariant = Library::Variant<TypeId, TypeInfo>;
}



int main()
{
}