#include <iostream>

using namespace std;

//Simple function, does some arithmetic
int SimpleFunc(int a, int b) {
  return a / (a - b);
}

int main() {

  
  int x = 10, y = 7;
  cout << SimpleFunc(x, y) << '\n';
  
  y = x - y;
  x = 3;
  cout << SimpleFunc(x, y) << '\n';
  
  y = x - y;
  x = 4;
  cout << SimpleFunc(x, y) << '\n';


  return 0;
}