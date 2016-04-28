#include "vector.h"
#include <cmath>

using namespace std;

Vector::Vector(int s): elem{new double[s]}, sz{s}{} //initialize members


double *Vector::operator[](int i){
  return elem[i];
}

int Vector::size(){
  return sz;
}
