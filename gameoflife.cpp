#include "Board.cpp"

int main( int argc, char *argv[]){

    stringstream r, c, m;
    r << argv[1]; // number of rows
    c << argv[2];  // number of columns
    if(argv[3]){
      m << argv[3];
    }
    Board game_board = Board(r,c);
    


}
