
#include "low-level-representation.cpp"

template<>
void print<std::vector<object_t*>*>(std::vector<object_t*>* val) {
    std::cout << "[";
    for (auto e: *val) {
        dispatchOnType(*e, [](auto n){print(n);});
        std::cout << ", ";
    }
    std::cout << "]";
}

template<>
void print<std::map<identifier_t, object_t*>*>(std::map<identifier_t, object_t*>* val) {
    std::cout << "[";
    for (auto [k, v]: *val) {
        std::cout << k << ":";
        dispatchOnType(*v, [](auto n){print(n);});
        std::cout << ", ";
    }
    std::cout << "]";
}

template<>
void print<std::map<object_t*, object_t*>*>(std::map<object_t*, object_t*>* val) {
    std::cout << "[";
    for (auto [k, v]: *val) {
        dispatchOnType(*k, [](auto n){print(n);});
        std::cout << ":";
        dispatchOnType(*v, [](auto n){print(n);});
        std::cout << ", ";
    }
    std::cout << "]";
}

int main()
{
    {
        auto obj = object_t(Int, int64_t(9'223'372'036'854'775'807));
        dispatchOnType(obj, [](auto n){print(n);});
        std::cout << std::endl;
        std::cout << sizeof(obj) << "\n";
        std::cout << std::endl;
    }

    {
        auto elem1 = object_t(Int, int64_t(1));
        auto elem2 = object_t(Int, int64_t(2));
        auto elem3 = object_t(Int, int64_t(3));
        std::vector<object_t*> list = {&elem1, &elem2, &elem3};
        auto obj = object_t(List, (std::vector<object_t*>*)&list);
        dispatchOnType(obj, [](auto n){print(n);});
        std::cout << std::endl;
        std::cout << sizeof(elem1) << " * 3 + " << sizeof(list) << " + " << sizeof(obj) 
                << " = " << (sizeof(elem1) * 3 + sizeof(list) + sizeof(obj)) << "\n"; /*
        9 * 3 + 24 + 9 = 60
        */
       std::cout << std::endl;
    }

    {
        auto elem1 = object_t(Char, char('f'));
        auto elem2 = object_t(Char, char('d'));
        auto elem3 = object_t(Char, char('s'));
        std::vector<object_t*> list = {&elem1, &elem2, &elem3};
        auto obj = object_t(List, (std::vector<object_t*>*)&list);
        dispatchOnType(obj, [](auto n){print(n);});
        std::cout << std::endl;
        std::cout << sizeof(elem1) << " * 3 + " << sizeof(list) << " + " << sizeof(obj) 
                << " = " << (sizeof(elem1) * 3 + sizeof(list) + sizeof(obj)) << "\n"; /*
        9 * 3 + 24 + 9 = 60
        */
       std::cout << std::endl;
    }

    {
        auto field1 = object_t(Bool, bool(true));
        auto field2 = object_t(Char, char('~'));
        auto field3 = object_t(Float, double(13.37));
        std::map<identifier_t, object_t*> struct_ = {
            {"field1", &field1},
            {"field2", &field2},
            {"field3", &field3}
        };
        auto obj = object_t(Struct, (std::map<identifier_t, object_t*>*)&struct_);
        dispatchOnType(obj, [](auto n){print(n);});
        std::cout << std::endl;
        std::cout << sizeof(field1) << " * 3 + " << sizeof(struct_) << " + " << sizeof(obj) 
                << " = " << (sizeof(field1) * 3 + sizeof(struct_) + sizeof(obj)) << "\n"; /*
        9 * 3 + 48 + 9 = 84
        */
       std::cout << std::endl;
    }

    {
        auto key1 = object_t(Int, int64_t(1));
        auto value1 = object_t(Char, char('f'));
        auto key2 = object_t(Char, char('d'));
        auto value2 = object_t(Int, int64_t(2));
        auto key3 = object_t(Float, double(3.14));
        auto value3 = object_t(Bool, bool(false));
        std::map<object_t*, object_t*> map = {
            {&key1, &value1},
            {&key2, &value2},
            {&key3, &value3}
        };
        auto obj = object_t(Map, (std::map<object_t*, object_t*>*)&map);
        dispatchOnType(obj, [](auto n){print(n);});
        std::cout << std::endl;
        std::cout << sizeof(key1) << " * 6"
                << " + " << sizeof(map) << " + " << sizeof(obj)
                << " = " << sizeof(key1) * 6 + sizeof(map) + sizeof(obj) << "\n";/*
        9 * 3 + 48 + 9 = 84
        */
       std::cout << std::endl;
    }
}
