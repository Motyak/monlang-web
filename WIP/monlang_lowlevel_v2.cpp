
/*
value_t
  -> either a :
    -> prim_value_t*
    -> type_value_t*
    -> struct_value_t*
    -> enum_value_t*
    nil value is represented as a nullptr (whatever variant)

prim_value_t
  -> Byte => uint8_t, Bool => bool,
     Int => int64_t, Float => double,
     Str => std::string,
     List => std::vector<value_t>,
     Map => std::map<value_t, value_t>,
     Lambda => std::function<value_t(const Environment&)>
       -> captures both the Environment AND the list of parameters name

type_value_t
  -> type_identifier (name)
  -> a value_t (value)
  
struct_value_t
  -> a type_identifier (name)
  -> a vector of pairs, associating a field_identifier (name)
     to a value_t (value)
  
enum_value_t
  -> a type_identifier (name)
  -> an enumerate identifier (name)
  -> an enumerate value (value_t)
*/

#include <variant>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <functional>

struct Environment;

struct prim_value_t;
struct type_value_t;
struct struct_value_t;
struct enum_value_t;

using value_t = std::variant<
    prim_value_t*, // primitive
    /* user-defined */
    type_value_t*,
    struct_value_t*,
    enum_value_t*
>;

using Byte = uint8_t;
using Bool = bool;
using Int = int64_t;
using Float = double;
using Str = std::string;
using List = std::vector<value_t>;
using Map = std::map<value_t, value_t>;
using Lambda = std::function<value_t(const Environment&)>;

struct prim_value_t {
    using Variant = std::variant<
        Byte,
        Bool,
        Int,
        Float,
        Str,
        List,
        Map,
        Lambda
    >;
    Variant variant;
};

Byte asByte(prim_value_t obj) { return std::get<Byte>(obj.variant); }
Bool asBool(prim_value_t obj) { return std::get<Bool>(obj.variant); }
Int asInt(prim_value_t obj) { return std::get<Int>(obj.variant); }
Float asFloat(prim_value_t obj) { return std::get<Float>(obj.variant); }
Str asStr(prim_value_t obj) { return std::get<Str>(obj.variant); }
List asList(prim_value_t obj) { return std::get<List>(obj.variant); }
Map asMap(prim_value_t obj) { return std::get<Map>(obj.variant); }
Lambda asLambda(prim_value_t obj) { return std::get<Lambda>(obj.variant); }

#ifdef PRIM_VALUE_T_TEST
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
void println<std::vector<value_t>>(std::vector<value_t> val) {
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
void println<std::map<value_t, value_t>>(std::map<value_t, value_t> val) {
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

template<>
void println<std::function<value_t()>>(std::function<value_t()> val) {
    std::cout << "<lambda>\n";
}

int main()
{
    {
        std::cout << "sizeof value_t : " << sizeof(value_t) << "\n";
        std::cout << "sizeof Byte : " << sizeof(Byte) << "\n";
        std::cout << "sizeof Bool : " << sizeof(Bool) << "\n";
        std::cout << "sizeof Int : " << sizeof(Int) << "\n";
        std::cout << "sizeof Float : " << sizeof(Float) << "\n";
        std::cout << "sizeof Str : " << sizeof(Str) << "\n";
        std::cout << "sizeof List : " << sizeof(List) << "\n";
        std::cout << "sizeof Map : " << sizeof(Map) << "\n";
        std::cout << "sizeof Lambda : " << sizeof(Lambda) << "\n";
        std::cout << "sizeof prim_value_t : " << sizeof(prim_value_t) << "\n";
        std::cout << "\n";
    }
    
    {
        auto obj = prim_value_t(Byte(91));
        println(asByte(obj));
    }
    
    {
        auto obj = prim_value_t(Bool(true));
        println(asBool(obj));
    }
    
    {
        auto obj = prim_value_t(Int(1997));
        println(asInt(obj));
    }
    
    {
        auto obj = prim_value_t(Float(3.14));
        println(asFloat(obj));
    }
    
    {
        auto obj = prim_value_t(Str("hello"));
        println(asStr(obj));
    }
    
    {
        auto obj = new prim_value_t(Byte(91));
        std::vector<value_t> list = {obj};
        auto list_obj = prim_value_t(List(list));
        println(asList(list_obj));
    }
    
    {
        auto obj_k = new prim_value_t(Str("hello"));
        auto obj_v = new prim_value_t(Str("world"));
        std::map<value_t, value_t> map = {{obj_k, obj_v}};
        auto obj = prim_value_t(Map(map));
        println(asMap(obj));
    }
    
    {
        auto some_obj = new prim_value_t(Str("test"));
        auto lambda = [&some_obj](){return some_obj;};
        auto obj = prim_value_t(Lambda(lambda));
        println(asLambda(obj));
    }
}

/*
sizeof value_t : 16
sizeof Byte : 1
sizeof Bool : 1
sizeof Int : 8
sizeof Float : 8
sizeof Str : 32
sizeof List : 24
sizeof Map : 48
sizeof Lambda : 32
sizeof prim_value_t : 56

0b01011011
true
1997
3.14
hello
{
    0x5f2e183b36c0
}
{
    0x5f2e183b3780 => 0x5f2e183b37c0
}
<lambda>
*/

#endif // PRIM_VALUE_T_TEST

struct type_value_t {
    std::string_view type;
    value_t value;
};

struct struct_value_t {
    std::string_view
    type;
    
    std::vector<std::pair<std::string_view, value_t>>
    fields;
};

struct enum_value_t {
    std::string_view type;
    std::string_view enumerate_name;
    value_t enumerate_value;
};

#ifdef MAIN
#include <iostream>
int main()
{
    {
        std::cout << "sizeof value_t : " << sizeof(value_t) << "\n";
        std::cout << "sizeof prim_value_t : " << sizeof(prim_value_t) << "\n";
        std::cout << "sizeof type_value_t : " << sizeof(type_value_t) << "\n";
        std::cout << "sizeof struct_value_t : " << sizeof(struct_value_t) << "\n";
        std::cout << "sizeof enum_value_t : " << sizeof(enum_value_t) << "\n";
        std::cout << "\n";
    }
    
    {
        auto john_str = prim_value_t(Str("John"));
        auto john_str_obj = value_t(&john_str);
        auto john_name = type_value_t("Name", john_str_obj);
        auto john_name_obj = value_t(&john_name);
        
        auto someage_int = prim_value_t(Int(28));
        auto someage_int_obj = value_t(&someage_int);
        auto someage_age = type_value_t("Age", someage_int_obj);
        auto someage_age_obj = value_t(&someage_age);
        
        auto blue_str = prim_value_t(Str("blue"));
        auto blue_str_obj = value_t(&blue_str);
        auto blue_color = type_value_t("Color", blue_str_obj);
        auto blue_color_obj = value_t(&blue_color);
        
        auto person = struct_value_t("Person", {
            {"name", john_name_obj}, 
            {"age", someage_age_obj},
            {"fav_color", blue_color_obj},
        });
        auto person_obj = value_t(&person);
        
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

/*
sizeof value_t : 16
sizeof prim_value_t : 56
sizeof type_value_t : 32
sizeof struct_value_t : 40
sizeof enum_value_t : 48

416
*/

#endif // MAIN
