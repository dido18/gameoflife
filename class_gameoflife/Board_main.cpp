#include <iostream>
#include "Board.hpp"

int main(int arc, char *argv[]){

    int r, c, m;
    r << strtol(argv[1],NULL,2); // number of rows
    c << strtol(argv[2],NULL,2);  // number of columns
    if(argv[3]){
      m << std::atoi(argv[3]);
    }
    Board * game_board;
    std::cout << "r" << r << " c " << c << std::endl;
    game_board = new Board(4,4);
    game_board->print();
   
}
