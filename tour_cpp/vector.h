class Vector{
 public:
  Vector(int size);
  double &operator[](int i);
  int size();

 private:
  double * elem;
  int sz;
};
