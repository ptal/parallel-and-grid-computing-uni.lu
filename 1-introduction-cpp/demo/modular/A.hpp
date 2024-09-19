#ifndef A_HPP
#define A_HPP

class A {
  int x;
public:
  A(int x);
  A(const A&) = default;
  void print();
};

#endif