#pragma once

#include <functional>
#include <vector>
#include "game_impl/Square.hpp"

template <typename T>
void pop(std::vector<T> &v, int index);

template <typename T>
bool isIn(const std::vector<T> &v, T el);

template <typename T>
void intersect(std::vector<Square> &v1, std::vector<Square> &v2);

template <typename T>
void intersect(std::vector<T> &v, T el);

#define CONTINUE_LOOP 0
#define BREAK_LOOP 1

void loop(Square square, Offset offset, std::function<bool(Square)> f);

template <size_t n>
void loop(Square square, const std::array<Offset, n> &offsets, std::function<bool(Square)> f);