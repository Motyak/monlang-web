
#include <functional>
#include <iostream>
#include <optional>

struct object_t {
    int val;
};

const object_t END_OF_STREAM = object_t(-1);

using Stream = std::function<std::optional<object_t>()>;

#define OUT
std::optional<object_t> next(OUT Stream*);

int main()
{
    Stream stream;
    {
        auto initial = object_t(91);
        auto& var = initial; // could have captured `initial` variable directly
        stream = [var]() mutable -> std::optional<object_t> {
            if (var.val >= 93) {
                return {};
            }
            var.val = var.val + 1;
            return var;
        };
    }

    {
        auto val1 = next(&stream).value_or(END_OF_STREAM);
        auto val2 = next(&stream).value_or(END_OF_STREAM);
        auto val3 = next(&stream).value_or(END_OF_STREAM);
        
        std::cout << val1.val << "\n"; // 92
        std::cout << val2.val << "\n"; // 93
        std::cout << val3.val << std::endl; // -1
    }
}

std::optional<object_t> next(OUT Stream* stream) {
    return (*stream)();
}
