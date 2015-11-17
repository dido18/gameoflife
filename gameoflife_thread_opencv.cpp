#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

/* Parallel implementatatio of game of life.
 *  using thraeds and mutex.
 *  - Splitting the matrix into pieces and distributes to threds
 *  - memory managment: stick threads to cores.
 */
using namespace cv;
using namespace std;

/*
* g++  -std=c++11  gameoflife_thread_opencv.cpp -pthread -o app `pkg-config --cflags --libs opencv`
*
*   usage:
*         app nrow ncols miter nthreads
*/
Mat readBoard;
Mat writeBoard;
int COLUMN_SIZE = 0;
int ROW_SIZE = 0;
int MAX_ITER = 50;
const uchar ALIVE = 255;
const uchar DEAD = 0;


void setPatternBlinker(Mat img, int row, int col){
    //perdiodic pattern: three points vertical.
    for( int i = 0; i < 3; i++)
        img.at<uchar>(row + i,col) = ALIVE; //white
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void setPatternToad(Mat img){
  /* dynamic pattern
  *                         x
  *       x x x         x    x
  *     x x x      ->   x    x
  *                      x
  */
  img.at<uchar>(1, 2) = ALIVE;
  img.at<uchar>(1, 3) = ALIVE;
  img.at<uchar>(1, 4) = ALIVE;
  img.at<uchar>(2, 1) = ALIVE;
  img.at<uchar>(2, 2) = ALIVE;
  img.at<uchar>(2, 3) = ALIVE;
}

void setPatternGlider(Mat img){
  /* Spaceships pattern
  *    - - x
  *    x - x
  *    - x x
  *
  */
  img.at<uchar>(0, 2) = ALIVE;
  img.at<uchar>(1, 0) = ALIVE;
  img.at<uchar>(1, 2) = ALIVE;
  img.at<uchar>(2, 1) = ALIVE;
  img.at<uchar>(2, 2) = ALIVE;
}


void setPatternCritical(Mat img){
  /*Critical pattern
   *    x - x x
   *    x - - -
   *    - - - -
   *    x x - -
  */
  img.at<uchar>(1, img.cols-1) = ALIVE;
  img.at<uchar>(0, img.cols-1) = ALIVE;
  img.at<uchar>(0, 0) = ALIVE;
  img.at<uchar>(1, 0) = ALIVE;
  img.at<uchar>(img.rows - 1, 0) = ALIVE;
  img.at<uchar>(img.rows - 1, 1) = ALIVE;
}

void setPatternRandom(Mat img)
{    // Random pattern generator
     RNG rng (ALIVE);
     int tot = img.rows * img.cols;
     int randRow, randColumn;
     for( int i = 0; i < tot/2 ; i++){
        randRow = rng.uniform(0, img.rows);
        randColumn= rng.uniform(0, img.cols);
        img.at<uchar>(randRow, randColumn) = ALIVE;
   }
}



void nextGeneration( Mat& readBoard, Mat& writeBoard);
int eval_alive_regular_neighbours(uchar* previous, uchar* current, uchar* next, int col);
uchar get_status(uchar currentStatus, int numAlive);
void body( int thread, Mat& readBoard, Mat& writeBoard);


int main(int argc, char** argv )
{
    stringstream r, c, m;
    r << argv[1]; // number of rows
    r >> ROW_SIZE;
    c << argv[2];  // number of columns
    c >> COLUMN_SIZE;
    if(argv[3]){
      m << argv[3];
      m >> MAX_ITER;
    }

    int nw = atoi(argv[4]);

    std::vector<std::thread> lthreads;
    int howmuch = ROW_SIZE / nw;

    //set baseBoard image
    readBoard = Mat(ROW_SIZE,COLUMN_SIZE, CV_8UC1, Scalar(DEAD));
    //set OutBoard image
    writeBoard = Mat(ROW_SIZE,COLUMN_SIZE, CV_8UC1, Scalar(DEAD));

    //create windows for board
    //char board_window[] = "Game of life";

    //namedWindow("Game of life", WINDOW_NORMAL );

    // Set initial pattern
    //setPatternBlinker(readBoard,2, 2);
    //setPatternToad(readBoard);
    //setPatternCritical(readBoard);
    //setPatternGlider(readBoard);
    setPatternRandom(readBoard);


    //imshow("Game of life", readBoard);
  //  waitKey(0);

    Mat subReadBoard;
    Mat subWriteBoard;

    int start_row;
    //splits the work
    std::cout << "nthreads"<<nw << std::endl;
    for(int row = 0; row < nw; row++)
    {
      start_row = row * howmuch;
      if( row == (nw - 1)) //if is the last thread
        {//last worker

        subReadBoard = readBoard(Rect(0, start_row, readBoard.cols, ROW_SIZE-1));
        std::cout << "\n LAST sub board"<< subReadBoard << std::endl;
        //subWriteBoard = writeBoard(Rect(0, start_row, writeBoard.cols, readBoard.rows-1));

        }
      else{
        subReadBoard = readBoard(Rect(0, start_row, readBoard.cols, start_row + howmuch));
         std::cout << "\n intermeditate sub board::"<< subReadBoard << std::endl;
        //subWriteBoard = writeBoard(Rect(0, start_row, writeBoard.cols, start_row + howmuch));
      }

      //lthreads.push_back(std::thread(body,row,subReadBoard,subWriteBoard));
    }

    for(int i =0; i < nw ; i ++){
        lthreads[i].join();
      }

      std::cout<< "Job terminated" <<std::endl;
}

void body( int thread, Mat readBoard, Mat writeBoard)
{   int myMax = MAX_ITER;
    std::cout << "Thread "<< thread << " compute ["<< readBoard<< " and  " << writeBoard<< "]"<< std::endl;
    while(myMax > 0)
    {
        nextGeneration(readBoard, writeBoard);
        imshow("Game of life", writeBoard);
        waitKey(50);
        myMax--;
    }

}


void nextGeneration(  Mat& readBoard, Mat& writeBoard){
  /* Calculate the next generation of the game.
  *  Reads the baseBoard and calculate the alive/dead cells
  *  and implements the tule of the game.
  *  Writes the new generation in the writeBoard.
  *  Overwrite the baseBoard with the value of the writeBoard
  */



    CV_Assert(readBoard.depth() == CV_8U);  // accept only char type matrices

    const int nChannels = readBoard.channels();

    for (int row = 0; row < readBoard.rows ; ++row)
    {
        uchar * previous = readBoard.ptr<uchar>(mod(row -1 , readBoard.rows));
        uchar * current  = readBoard.ptr<uchar>(row );
        uchar * next     = readBoard.ptr<uchar>(mod(row + 1, readBoard.rows));

        uchar* output = writeBoard.ptr<uchar>(row);

        for (int col = 0; col < nChannels*readBoard.cols; col++)
          {
            //pass the starting pointer of the three vectors,
            int numAlive = eval_alive_regular_neighbours(previous, current, next, col);
            uchar status = get_status(current[col], numAlive);
          //  if( numAlive > 0)
          //    std::cout << "Alive [" << row << "," << col <<"] numALive: " << numAlive << "  "<< (int)current[col] << " ->"<< (int)status << std::endl;
            output[col] = status;
          }
    }
    writeBoard.copyTo(readBoard);

}

int eval_alive_regular_neighbours( uchar* previous, uchar* current, uchar* next, int col)
{ /*
  *  Takes the pointers to previous, current, next rows vectors.
  *  Return the status: ALIVE or DEAD according to the values of the neighbours
  *  Check the toroidal neighbours.
  */
  int numAlive = 0;
  for(int i = -1; i < 2; i++)
  {
    if(previous[mod(col + i, readBoard.cols)] == ALIVE)
      numAlive++;
    if(current[mod(col + i, readBoard.cols)] == ALIVE &&  i!= 0)
      numAlive++;
    if(next[mod(col + i,readBoard.cols)] == ALIVE)
      numAlive++;
  }
  return numAlive;
}

uchar get_status(uchar currentStatus, int numAlive){
    uchar nextStatus;
    if(currentStatus == DEAD && numAlive == 3)
      nextStatus = ALIVE;
    if(currentStatus == ALIVE && numAlive < 2)
      nextStatus = DEAD;
    if(currentStatus == ALIVE && numAlive > 3)
      nextStatus = DEAD;
    if(currentStatus == ALIVE && (numAlive == 2 || numAlive == 3))
      nextStatus = ALIVE;
    return nextStatus;
}
