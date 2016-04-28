#ifndef  Board_h_
#define Board_h_

class Board{

  public:
     Board(int nrows, int ncols);
     int get(int row, int col);
     void set(int row, int col, int value);
     int *get_row(int i);
     void print();

  private:
    int **readBoard;
    int **writeBoard;
    int nrows;
    int  ncols;
};
  
#endif
