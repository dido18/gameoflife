#include <cstdlib>
#include <iostream>

using namespace std;

struct Vector
{
  int sz;       // size of the vector
  double * elem; // pointer to the first element 
};

void vector_init(Vector &v, int size)
{
  v.sz = size;
  v.elem = new double[size]; //allocate an array of number of size elements in the heap
}

double read_and_sum(Vector& v)
{
  //read s integers from cin and return their sum, s is assumed positive
  int i;
  for(i=0; i<v.sz; i++)
    {
     cout << "Inserisci un intero positivo" << endl;
     cin >> v.elem[i];
    }
  double sum = 0;
  for(i=0; i<v.sz;i++)
    {
      //std::cout << v.elem[i] << "  "<<std::endl;
      sum += v.elem[i];
    }
  return sum;
}

void print_size(Vector v, Vector &rv, Vector *pv)
{
  cout<<v.sz <<"vector name" <<endl;
  cout<<rv.sz <<"vector references" <<endl;
  cout<<pv->sz <<"vector pointer" <<endl;
}


int main(int argc, char* argv[]){
  int x;
  if(argc>1)
    x = atoi(argv[1]);  

  Vector v;
  vector_init(v,x);
  int  sum = read_and_sum(v);
  cout << "somma " << sum << endl;
  
  print_size(v, v, &v);
  return(0);
}
