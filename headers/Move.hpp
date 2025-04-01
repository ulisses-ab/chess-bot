#include <cstdint>

#ifndef MOVE
#define MOVE

struct Move {
    std::uint16_t start : 6;     
    std::uint16_t end : 6;       
    std::uint16_t captured : 3;
    std::uint16_t flag : 1;

    Move(int s, int e, int c, int f) {
        start = s;
        end = e;
        captured = c;
        flag = f;
    }
};

#endif