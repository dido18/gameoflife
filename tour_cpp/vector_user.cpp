#include "vector.h"


using namespace std;

double sqrt_sum(Vector &v){
  double sum = 0;
   for(int i =0; i<v.size(); i++)
    sum+=v[i];
  return sum;
};


int main(){

  return(0);
}
