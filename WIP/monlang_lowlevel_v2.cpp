
/*
object_t
  -> either a :
    -> prim_object_t*
    -> type_object_t*
    -> struct_object_t*
    -> enum_object_t*

prim_object_t
  -> Byte => uint8_t, Bool => bool,
     Int => int64_t, Float => double,
     Str => std::string,
     List => std::vector<object_t>,
     Map => std::map<object_t, object_t>

type_object_t
  -> type_identifier (name)
  -> an object_t (value)
  
struct_object_t
  -> a type_identifier (name)
  -> a vector of pairs, associating a field_identifier (name)
     to an object_t (value)
  
enum_object_t
  -> a type_identifier (name)
  -> an enumerate identifier (name)
  -> an enumerate value (object_t)
*/

#include <variant>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct prim_object_t;
struct type_object_t;
struct struct_object_t;
struct enum_object_t;

using object_t = std::variant<
    prim_object_t*, // primitive
    /* user-defined */
    type_object_t*,
    struct_object_t*,
    enum_object_t*
>;

using Byte = uint8_t;
using Bool = bool;
using Int = int64_t;
using Float = double;
using Str = std::string;
using List = std::vector<object_t>;
using Map = std::map<object_t, object_t>;

struct prim_object_t {
    using Variant = std::variant<
        Byte,
        Bool,
        Int,
        Float,
        Str,
        List,
        Map
    >;
    Variant variant;
};

Byte asByte(prim_object_t obj) { return std::get<Byte>(obj.variant); }
Bool asBool(prim_object_t obj) { return std::get<Bool>(obj.variant); }
Int asInt(prim_object_t obj) { return std::get<Int>(obj.variant); }
Float asFloat(prim_object_t obj) { return std::get<Float>(obj.variant); }
Str asStr(prim_object_t obj) { return std::get<Str>(obj.variant); }
List asList(prim_object_t obj) { return std::get<List>(obj.variant); }
Map asMap(prim_object_t obj) { return std::get<Map>(obj.variant); }

#ifdef PRIM_OBJECT_T_TEST
#include <iostream>

template <typename T>
void println(T val) {
    std::cout << val << "\n";
}

template<>
void println<uint8_t>(uint8_t val) {
    std::cout << "0b";
    for (int i = 7; i >= 0; --i) {
        std::cout << ((val & 1 << i) == 0? 0 : 1);
    }
    std::cout << "\n";
}

template<>
void println<bool>(bool val) {
    std::cout << (val? "true" : "false") << "\n";
}

template<>
void println<std::vector<object_t>>(std::vector<object_t> val) {
    std::cout << "{\n";
    for (auto e: val) {
        std::cout << "    ";
        std::visit(
            [](auto* ptr){std::cout << ptr << "\n";},
            e
        );
    }
    std::cout << "}\n";
}

template<>
void println<std::map<object_t, object_t>>(std::map<object_t, object_t> val) {
    std::cout << "{\n";
    for (auto [k, v]: val) {
        std::cout << "    ";
        std::visit(
            [](auto* ptr){std::cout << ptr;},
            k
        );
        std::cout << " => ";
        std::visit(
            [](auto* ptr){std::cout << ptr;},
            v
        );
    }
    std::cout << "\n}\n";
}

int main()
{
    {
        std::cout << "sizeof object_t : " << sizeof(object_t) << "\n";
        std::cout << "sizeof Byte : " << sizeof(Byte) << "\n";
        std::cout << "sizeof Bool : " << sizeof(Bool) << "\n";
        std::cout << "sizeof Int : " << sizeof(Int) << "\n";
        std::cout << "sizeof Float : " << sizeof(Float) << "\n";
        std::cout << "sizeof Str : " << sizeof(Str) << "\n";
        std::cout << "sizeof List : " << sizeof(List) << "\n";
        std::cout << "sizeof Map : " << sizeof(Map) << "\n";
        std::cout << "sizeof prim_object_t : " << sizeof(prim_object_t) << "\n";
        std::cout << "\n";
    }
    
    {
        auto obj = prim_object_t(Byte(91));
        println(asByte(obj));
    }
    
    {
        auto obj = prim_object_t(Bool(true));
        println(asBool(obj));
    }
    
    {
        auto obj = prim_object_t(Int(1997));
        println(asInt(obj));
    }
    
    {
        auto obj = prim_object_t(Float(3.14));
        println(asFloat(obj));
    }
    
    {
        auto obj = prim_object_t(Str("hello"));
        println(asStr(obj));
    }
    
    {
        auto obj = new prim_object_t(Byte(91));
        std::vector<object_t> list = {obj};
        auto list_obj = prim_object_t(List(list));
        println(asList(list_obj));
    }
    
    {
        auto obj_k = new prim_object_t(Str("hello"));
        auto obj_v = new prim_object_t(Str("world"));
        std::map<object_t, object_t> map = {{obj_k, obj_v}};
        auto obj = prim_object_t(Map(map));
        println(asMap(obj));
    }
}
#endif // PRIM_OBJECT_T_TEST

struct type_object_t {
    std::string_view type;
    object_t value;
};

struct struct_object_t {
    std::string_view
    type;
    
    std::vector<std::pair<std::string_view, object_t>>
    fields;
};

struct enum_object_t {
    std::string_view type;
    std::string_view enumerate_name;
    object_t enumerate_value;
};

#ifdef MAIN
#include <iostream>
int main()
{
    {
        std::cout << "sizeof object_t : " << sizeof(object_t) << "\n";
        std::cout << "sizeof prim_object_t : " << sizeof(prim_object_t) << "\n";
        std::cout << "sizeof type_object_t : " << sizeof(type_object_t) << "\n";
        std::cout << "sizeof struct_object_t : " << sizeof(struct_object_t) << "\n";
        std::cout << "sizeof enum_object_t : " << sizeof(enum_object_t) << "\n";
        std::cout << "\n";
    }
    
    {
        auto john_str = prim_object_t(Str("John"));
        auto john_str_obj = object_t(&john_str);
        auto john_name = type_object_t("Name", john_str_obj);
        auto john_name_obj = object_t(&john_name);
        
        auto someage_int = prim_object_t(Int(28));
        auto someage_int_obj = object_t(&someage_int);
        auto someage_age = type_object_t("Age", someage_int_obj);
        auto someage_age_obj = object_t(&someage_age);
        
        auto blue_str = prim_object_t(Str("blue"));
        auto blue_str_obj = object_t(&blue_str);
        auto blue_color = type_object_t("Color", blue_str_obj);
        auto blue_color_obj = object_t(&blue_color);
        
        auto person = struct_object_t("Person", {
            {"name", john_name_obj}, 
            {"age", someage_age_obj},
            {"fav_color", blue_color_obj},
        });
        auto person_obj = object_t(&person);
        
        std::cout << (
            sizeof(person_obj)
            + sizeof(person)
            + sizeof(blue_color_obj)
            + sizeof(blue_color)
            + sizeof(blue_str_obj)
            + sizeof(blue_str)
            
            + sizeof(someage_age_obj)
            + sizeof(someage_age)
            + sizeof(someage_int_obj)
            + sizeof(someage_int)
            
            + sizeof(john_name_obj)
            + sizeof(john_name)
            + sizeof(john_str_obj)
            + sizeof(john_str)
        ) << "\n";
    }
}
#endif // MAIN

/*
sizeof object_t : 16
sizeof prim_object_t : 56
sizeof type_object_t : 32
sizeof struct_object_t : 40
sizeof enum_object_t : 48

416
*/

