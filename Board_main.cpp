#include <iostream>

#include "Board.hpp"

int main(int arc, char *argv[]){

    int r, c, m;
    r << std::atoi(argv[1]); // number of rows
    c << std::atoi(argv[2]);  // number of columns
    if(argv[3]){
      m << std::atoi(argv[3]);
    }
    Board * game_board;
    game_board = new Board(r,c);
    


}
