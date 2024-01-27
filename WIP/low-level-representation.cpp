

#include <iostream>
#include <vector>
#include <map>

using identifier_t = std::string;

/* disguise types for uint8_t constants */
struct _Byte   { uint8_t _; _Byte():   _(1){} };
struct _Bool   { uint8_t _; _Bool():   _(2){} };
struct _Char   { uint8_t _; _Char():   _(3){} };
struct _Int    { uint8_t _; _Int():    _(4){} };
struct _Float  { uint8_t _; _Float():  _(5){} };
struct _List   { uint8_t _; _List():   _(6){} };
struct _Struct { uint8_t _; _Struct(): _(7){} };
struct _Map    { uint8_t _; _Map():    _(8){} };

/* type enumerates */
_Byte   Byte;
_Bool   Bool;
_Char   Char;
_Int    Int;
_Float  Float;
_List   List;
_Struct Struct;
_Map    Map;

struct object_t {
    enum types {
        Byte   = 1,
        Bool   = 2,
        Char   = 3,
        Int    = 4,
        Float  = 5,
        List   = 6,
        Struct = 7,
        Map    = 8,
    };
    
    /* fields */
    uint8_t type;
    union _Value {
        uint8_t                            asByte;
        bool                               asBool;
        char      /* 0-127 ASCII */        asChar;
        int64_t                            asInt;
        double                             asFloat;
        std::vector<object_t*>*            asList;
        std::map<identifier_t, object_t*>* asStruct;
        std::map<object_t*, object_t*>*    asMap;
    } __attribute__ ((packed)) value; // packed attribute reduces struct size from 16 to 9 bytes
    
    /* constructors */
    explicit object_t(_Byte   type,                            uint8_t value) : type(type._), value(_Value{.asByte  =value}) {}
    explicit object_t(_Bool   type,                               bool value) : type(type._), value(_Value{.asBool  =value}) {}
    explicit object_t(_Char   type,                               char value) : type(type._), value(_Value{.asChar  =value}) {}
    explicit object_t(_Int    type,                            int64_t value) : type(type._), value(_Value{.asInt   =value}) {}
    explicit object_t(_Float  type,                             double value) : type(type._), value(_Value{.asFloat =value}) {}
    explicit object_t(_List   type,            std::vector<object_t*>* value) : type(type._), value(_Value{.asList  =value}) {}
    explicit object_t(_Struct type, std::map<identifier_t, object_t*>* value) : type(type._), value(_Value{.asStruct=value}) {}
    explicit object_t(_Map    type,    std::map<object_t*, object_t*>* value) : type(type._), value(_Value{.asMap   =value}) {}
};

template <typename Lambda>
void dispatchOnType(object_t obj, Lambda process) {
    auto val = obj.value;
    
    switch (obj.type) {
        case object_t::types::Byte:
        process(val.asByte);
        break;
        
        case object_t::types::Bool:
        process(val.asBool);
        break;
        
        case object_t::types::Char:
        process(val.asChar);
        break;
        
        case object_t::types::Int:
        process(val.asInt);
        break;
        
        case object_t::types::Float:
        process(val.asFloat);
        break;
        
        case object_t::types::List:
        process(val.asList);
        break;
        
        case object_t::types::Struct:
        process(val.asStruct);
        break;
        
        case object_t::types::Map:
        process(val.asMap);
        break;
        
        default:
        // cannot happen
        throw std::runtime_error("Invalid object_t type (id: `" + std::to_string(int(obj.type)) + "`)");
    }
}

template <typename T>
void print(T val) {
    std::cout << val;
}

template<>
void print<uint8_t>(uint8_t val) {
    std::cout << "0b";
    for (int i = 7; i >= 0; --i) {
        std::cout << ((val & 1 << i) == 0? 0 : 1);
    }
}

template<>
void print<bool>(bool val) {
    std::cout << (val? "true" : "false");
}

#ifdef MAIN1_CPP

int main()
{
    
    {
        auto obj = object_t(Byte, uint8_t(91));
        dispatchOnType(obj, [](auto n){println(n);}); // 0b01011011
    }
    
    {
        auto obj = object_t(Bool, bool(true));
        dispatchOnType(obj, [](auto n){println(n);}); // true
    }
    
    {
        auto obj = object_t(Char, char('A'));
        dispatchOnType(obj, [](auto n){println(n);}); // A
    }
    
    {
        auto obj = object_t(Int, int64_t(1997));
        dispatchOnType(obj, [](auto n){println(n);}); // 1997
    }
    
    {
        auto obj = object_t(Float, double(3.14));
        dispatchOnType(obj, [](auto n){println(n);}); // 3.14
    }
    
    {
        std::vector<object_t*> list = {};
        auto obj = object_t(List, (std::vector<object_t*>*)&list);
        dispatchOnType(obj, [](auto n){println(n);}); //~ 0x7fff7933c070
    }
    
    {
        std::map<identifier_t, object_t*> struct_ = {};
        auto obj = object_t(Struct, (std::map<identifier_t, object_t*>*)&struct_);
        dispatchOnType(obj, [](auto n){println(n);}); //~ 0x7fff7933c070
    }
    
    {
        std::map<object_t*, object_t*> map = {};
        auto obj = object_t(Map, (std::map<object_t*, object_t*>*)&map);
        dispatchOnType(obj, [](auto n){println(n);}); //~ 0x7fff7933c070
    }
}

#endif // MAIN1_CPP
