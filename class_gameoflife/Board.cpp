#include <iostream>
#include "Board.hpp"
#include <cmath>
  
Board::Board(int nrows, int ncols){
  srand(time(0));
  this->nrows = nrows;
  this->ncols = ncols;
  this-> readBoard = new int *[nrows];
  this-> writeBoard = new int *[nrows];
  for( int i=0; i<nrows; i++){
    this-> readBoard[i] = new int[ncols];
    this-> writeBoard[i] = new int[ncols];

    //random fill of the readBoard
    for(int j=0; j<ncols; j++){
      this->readBoard[i][j] = floor(rand()%2); 
    }
  }
}

  void Board::print(){

    for(int i=0;i< this->nrows;i++){
      for(int j=0; j <this->ncols;j++){
	std::cout << this->readBoard[i][j]<< " ";
	  }
      std::cout <<std::endl;
    }
    std::cout <<std::endl;
}
 
int Board::get(int row, int col){
  return this->readBoard[row][col];
}

 void Board::set(int row, int col, int value){
   this->writeBoard[row][col]=value;
 }
 
/*
int *get_row(int i){
   return &(this->readBoard[i]);
}
 
*/
