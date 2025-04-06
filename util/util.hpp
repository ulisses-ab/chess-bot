#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include "../game_impl/Square.hpp"

template <typename T>
void pop(std::vector<T> &v, int index) {
    v[index] = v[v.size() - 1];
    v.pop_back();
}

template <typename T>
bool isIn(const std::vector<T> &v, T el) {
    return std::binary_search(v.begin(), v.end(), el);
}

template <typename T>
void intersect(std::vector<T> &v1, std::vector<T> &v2) {
    std::sort(v2.begin(), v2.end());

    for(int i = 0; i < v1.size(); i++) {
        if(!isIn(v2, v1[i])) {
            pop<T>(v1, i);
            i--;
        }
    }
}

template <typename T>
void intersect(std::vector<T> &v, T el) {
    for(int i = 0; i < v.size(); i++) {
        if(v[i] != el) {
            pop<T>(v, i);
            i--;
        }
    }
};

#define CONTINUE_LOOP 0
#define BREAK_LOOP 1

inline void loop(Square square, Offset offset, std::function<bool(Square)> f) {
    while(square.offsetIsSafe(offset)) {
        square += offset;

        if(f(square))
            break;
    }
}

template <size_t n>
inline void loop(Square square, const std::array<Offset, n> &offsets, std::function<bool(Square)> f) {
    for(auto offset : offsets) {
        if(square.offsetIsSafe(offset)) {
            Square end = square + offset;
            
            if(f(square))
                break;
        }
    }
};

