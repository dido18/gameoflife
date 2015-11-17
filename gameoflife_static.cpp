#include <stdlib.h>//for rand
#include <iostream>

using namespace std;

const int ROW_SIZE = 6;
const int COLUMN_SIZE = 6;
const int MAX_ITER = 50 ;
const int ALIVE = 1;
const int DEAD = 0;
const int MAX_INIT_ALIVE = 4; // max live cells in a random pattern

int readBoard[ROW_SIZE][COLUMN_SIZE];
int outBoard[ROW_SIZE][COLUMN_SIZE];

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
int countAliveNSEO( int i, int j){
  // count cells alive in the North Suth East Ovest
  int num = 0;
  /* North*/
  if( readBoard[mod((i-1), ROW_SIZE)][ mod(j, COLUMN_SIZE)] == 1)
      num++;
  /* Ovest*/
  if(readBoard[mod(i,ROW_SIZE)][mod((j-1),COLUMN_SIZE)] == 1 )
      num++;
  /* Est*/
  if(readBoard[mod(i, ROW_SIZE)][mod((j+1), COLUMN_SIZE)] == 1)
      num++;
  /* South */
  if(readBoard[mod((i+1),ROW_SIZE)][mod(j,COLUMN_SIZE)] == 1)
      num++;
  return num;

}

int countAliveNeighbors( int i, int j){
  int diag = 0;
  /* check 4 extrems alive points in diagonal of the matrix */
  if( i == 0 && j == 0 ){ //first  point of the first row
      if(readBoard[i+1][j+1] == 1)
          diag ++;
  }else if( i == 0 && j == (COLUMN_SIZE -1)){ //last point of the first row
     if(readBoard[i+1][j-1] == 1)
        diag++;
  }else if( i == ROW_SIZE -1 && j==0 ){ //first point of the last row
      if(readBoard[i-1][j+1] == 1)
          diag ++;
  }
  else if( (i == ROW_SIZE-1)  && j == (COLUMN_SIZE -1)){ // last point of the last row
     if(readBoard[i-1][j-1] == 1)
        diag++;
  }else if( i == 0 || i == ROW_SIZE || j == 0 || j == COLUMN_SIZE){
      /* check diagonal points in critical ROWS and columns*/
      switch (i) {
          case 0: // first row
                if(readBoard[i+1][j-1] == 1)
                    diag++;
                if(readBoard[i+1][j+1] == 1)
                    diag++;
                break;
          case ROW_SIZE-1: // last row
              if(readBoard[i-1][j-1] == 1)
                  diag++;
              if(readBoard[i-1][j+1] == 1)
                  diag++;
              break;
      }
      /* Check diagonal points on critical COLUMNS of the matrix*/
      switch (j) {
        case 0: // first column
            if(readBoard[i-1][j+1] == 1)
                diag++;
            if(readBoard[i+1][j+1] == 1)
               diag++;
      case COLUMN_SIZE-1: // last column
          if(readBoard[i-1][j-1] == 1)
               diag++;
          if(readBoard[i+1][j-1] == 1)
                diag++;
      }

  }else { /* check alive diagonal points of center elements*/
      if(readBoard[i-1][j-1] == 1)
          diag++;
      if(readBoard[i-1][j+1] == 1)
          diag++;
      if(readBoard[i+1][j-1] == 1)
          diag++;
      if(readBoard[i+1][j+1] == 1)
               diag++;
  }
  /*counts alive points on North, South, Est, Ovest*/
  int nseo = countAliveNSEO(i,j);
  int num = nseo + diag;
  if( num > 0)
    std::cout  << i <<" " << j <<" diag: "<< diag <<" nseo: "<< nseo << " alive: " << readBoard[i][j]<< std::endl;


  return num;
}

void printBaseBoard(){
  std::cout << "Base readBoard " << std::endl;
  // print outBoard
  for(int i = 0; i < ROW_SIZE; i++){
    for(int j = 0; j < COLUMN_SIZE; j++ ){
      if(j == COLUMN_SIZE-1)
        std::cout << readBoard[i][j] << "\n";
      else
        std::cout << readBoard[i][j] << " ";
    }
  }
}

void printOutBoard(){
        std::cout << "Out Board " << std::endl;
        // print outBoard
        for(int i = 0; i < ROW_SIZE; i++){
          for(int j = 0; j < COLUMN_SIZE; j++ ){
            if(j == COLUMN_SIZE-1)
              std::cout << outBoard[i][j] << "\n";
            else
              std::cout << outBoard[i][j] << " ";
          }
        }
}

void copyOutToBaseBoard(){
  for(int i = 0; i < ROW_SIZE; i++){
    for(int j = 0; j < COLUMN_SIZE; j++ ){
       readBoard[i][j] = outBoard[i][j];
    }
  }
}

void setPatternBlinker(){
  int tot = 3;
  int middle = COLUMN_SIZE / 2;
  for( int i = middle; i < middle+tot; i++ ){
     readBoard[i][middle] = ALIVE;
     outBoard[i][middle] = ALIVE;
   }
}

void setPatternBlock(){
  readBoard[1][1] = ALIVE;
  readBoard[1][2] = ALIVE;
  readBoard[2][1] = ALIVE;
  readBoard[2][2] = ALIVE;
}

void setPatternToad(){
  readBoard[1][2] = ALIVE;
  readBoard[1][3] = ALIVE;
  readBoard[1][4] = ALIVE;
  readBoard[2][1] = ALIVE;
  readBoard[2][2] = ALIVE;
  readBoard[2][3] = ALIVE;
}

int main( int argc, char  *argv[]){

  // set the readBoard and outBoard all zeros
  for(int i = 0; i < ROW_SIZE; i++){
    for(int j = 0; j < COLUMN_SIZE; j++ ){
          readBoard[i][j] = DEAD;
          outBoard[i][j] = DEAD;
    }
  }

setPatternBlinker();
//setPatternBlock();
//setPatternToad();
//printOutBoard();

int flag = 1;
int numAlive = 0;
while( flag == 1){

  printBaseBoard();
  //read the readBoard for cecking alive or die
  for(int i = 0; i < ROW_SIZE; i++){
    for(int j = 0; j < COLUMN_SIZE; j++){
        numAlive = countAliveNeighbors(i,j);
        if(readBoard[i][j] == DEAD && numAlive == 3)
            outBoard[i][j] = ALIVE;
        if(readBoard[i][j] == ALIVE && numAlive < 2){
            std::cout << "alive-> dead "<< i <<" "<< j << " num Live "<< numAlive << std::endl;
            outBoard[i][j] = DEAD;
            }
        if(readBoard[i][j] == ALIVE && numAlive > 3){
            std::cout << "alive-> dead "<< i <<" "<< j << " num Live "<< numAlive << std::endl;
            outBoard[i][j] = DEAD;
          }
        if (readBoard[i][j] == ALIVE && (numAlive == 2  || numAlive == 3)) {
            outBoard[i][j] = ALIVE;
        }
      }
  }
  printOutBoard();
  //printOutBoard();
  copyOutToBaseBoard();
  std::cout << "0 : terminate \n 1: new generation" << std::endl;
  std::cin >> flag;
 }

}
