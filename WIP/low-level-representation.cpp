

#include <iostream>
#include <vector>
#include <map>

using identifier_t = std::string;

uint8_t Byte = 1;
uint8_t Bool = 2;
uint8_t Char = 3;
uint8_t Int = 4;
uint8_t Float = 5;
uint8_t List = 6;
uint8_t Struct = 7;
uint8_t Map = 8; 

struct object_t {
    enum types {
        Byte = 1,
        Bool = 2,
        Char = 3,
        Int = 4,
        Float = 5,
        List = 6,
        Struct = 7,
        Map = 8,
    };
    
    /* fields */
    uint8_t type;
    union _Value{
        uint8_t asByte;
        bool asBool;
        char asChar; // contract: make sure its 0-127 ASCII character exclusively
        int64_t asInt;
        double asFloat;
        std::vector<object_t*>* asList;
        std::map<identifier_t, object_t*>* asStruct;
        std::map<object_t*, object_t*>* asMap;
    } value;
    
    /* constructors */
    explicit object_t(uint8_t type, uint8_t value) : type(type), value(_Value{.asByte=value}){}
    explicit object_t(uint8_t type, bool value) : type(type), value(_Value{.asBool=value}){}
    explicit object_t(uint8_t type, char value) : type(type), value(_Value{.asChar=value}){}
    explicit object_t(uint8_t type, int64_t value) : type(type), value(_Value{.asInt=value}){}
    explicit object_t(uint8_t type, double value) : type(type), value(_Value{.asFloat=value}){}
    explicit object_t(uint8_t type, std::vector<object_t*>* value) : type(type), value(_Value{.asList=value}){}
    explicit object_t(uint8_t type, std::map<identifier_t, object_t*>* value) : type(type), value(_Value{.asStruct=value}){}
    explicit object_t(uint8_t type, std::map<object_t*, object_t*>* value) : type(type), value(_Value{.asMap=value}){}
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
        throw std::runtime_error("Invalid object_t type (id: `" + std::to_string(int(obj.type)) + "`)");
    }
}

template <typename T, typename std::enable_if_t<std::is_pointer<T>::value>* = 0>
void println(T val) { std::cout << (void*)val << std::endl; }

template <typename T>
void println(T val) {
    std::cout << val << std::endl;
}

template<>
void println<uint8_t>(uint8_t val) {
    for (int i = 7; i >= 0; --i) {
        std::cout << ((val & 1 << i) == 0? 0 : 1);
    }
    std::cout << std::endl;
}

template<>
void println<bool>(bool val) {
    std::cout << (val? "true" : "false") << std::endl;
}

int main()
{
    
    {
        auto obj = object_t(Byte, uint8_t(91));
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        auto obj = object_t(Bool, bool(true));
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        auto obj = object_t(Char, char('A'));
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        auto obj = object_t(Int, int64_t(1997));
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        auto obj = object_t(Float, double(3.14));
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        std::vector<object_t*> list = {};
        auto obj = object_t(List, (std::vector<object_t*>*)&list);
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        std::map<identifier_t, object_t*> struct_ = {};
        auto obj = object_t(List, (std::map<identifier_t, object_t*>*)&struct_);
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        std::map<object_t*, object_t*> map = {};
        auto obj = object_t(List, (std::map<object_t*, object_t*>*)&map);
        dispatchOnType(obj, [](auto n){println(n);});
    }
    
    {
        auto obj = object_t(0, 'A'); // invalid type id
        dispatchOnType(obj, [](auto n){println(n);});
    }
}
