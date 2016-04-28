#include <iostream>
using namespace std;

class Vector{
  
public:
  Vector(int s): elem{new double[s]},sz{s}{}
  double &operator[](int i){return elem[i];}
  int size(){return sz;}
  void print(){
    for(int i=0; i< sz; i++)
      if(i==sz-1)
	cout<< elem[i] <<" "<<endl;
      else
	cout<<elem[i] << " "; 
  }
private:
  int sz;
  double *elem;
};


int main(int argc, char *argv[]){
  Vector v(4); //new vector of 4 elements;
  for(int i=0; i< v.size(); i++)
    cin >> v[i];
  v.print();
  
}

