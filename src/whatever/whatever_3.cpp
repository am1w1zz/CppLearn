 #include <iostream>
#include <string>
#include <type_traits>

#define self(Member, ...) std::decay_t<decltype(__VA_ARGS__)> Member = __VA_ARGS__
using namespace std::literals;

struct bitch final {
    self(x, 0);
};

struct fuck final {
    self(x, ""s);
};

auto cunt(auto &&obj) {
    auto y = obj.x + obj.x;
    std::cout << y << std::endl;
}

auto main()->int {
    auto bitch_obj = bitch{};
    auto fuck_obj = fuck{};
    bitch_obj.x = 1;
    fuck_obj.x = "aa";
    cunt(bitch_obj);
    cunt(fuck_obj);
}