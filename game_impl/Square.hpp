#pragma once

struct Offset {
    int row : 4;
    int col : 4;

    int number() const {
        return row*8 + col;
    }

    constexpr Offset(int row, int col) : row(row), col(col) {}
};

union Square {
    int number;
    struct 
    {
        unsigned col : 3;
        unsigned row : 3;
    };

    constexpr Square(int number) : number(number) {}
    constexpr Square(int r, int c) : number(0) {
        row = r;
        col = c;
    };

    bool isSafe() {
        return number >= 0 && number < 64;
    }

    Square operator+(int offset) const {
        return Square{number + offset};
    }

    Square operator-(int offset) const {
        return Square{number - offset};
    }

    Square operator+=(int offset) {
        number += offset;
        return Square{number};
    }

    Square operator-=(int offset) {
        number -= offset;
        return Square{number};
    }

    Square operator+(Offset o) const {
        return Square{row + o.row, col + o.col};
    }

    Square operator-(Offset o) const {
        return Square{row - o.row, col - o.col};
    }

    Square operator+=(Offset o) {
        row += o.row;
        col += o.col;
        return Square{number};
    }

    Square operator-=(Offset o) {
        row -= o.row;
        col -= o.col;
        return Square{number};
    }

    Offset operator-(Square s) const {
        return Offset{row - s.row, col - s.col};
    }

    bool operator==(Square s) const {
        return number == s.number;
    }

    bool operator==(int n) const {
        return number == n;
    }

    bool operator!=(Square s) const {
        return number != s.number;
    }

    bool operator!=(int n) const {
        return number != n;
    }

    bool operator<(Square s) const {
        return number < s.number;
    }

    bool operator>(Square s) const {
        return number > s.number;
    }

    bool offsetIsSafe(Offset o) const {
        return row + o.row >= 0 && row + o.row < 8 && col + o.col >= 0 && col + o.col < 8;
    }
};