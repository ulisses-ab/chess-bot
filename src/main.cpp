#include "./../headers/Minimax.hpp"
#include <iostream>
#include <typeinfo>
#include <stdexcept>

using namespace std;

int main() {
    Board b;
    b.print(true);

    int t = 6;
    while(t--) {
        b = Minimax::findBestMove(b, 6);
        b.print(true);
    }
 
    return 0;
}