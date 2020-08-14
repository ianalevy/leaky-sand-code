#include <iostream>
#include <vector>
#include <string>

#include "utility.h"
#include "matrix.h"
#include "sandpile.h"
#include <cmath>
#include <fstream>
#include <algorithm>

int main()
{

  int chips; //total chips
  int dim; //size of initial config

  std::cout << "chips=" << std::endl;
  std::cin >> chips;
  // std::cout << "initial size=" << std::endl;
  // std::cin >> dim;

  // chips=15;
  dim=3;

  Matrix init(dim,dim); //initial chip config

  MatrixPtr initPtr;
  MatrixPtr finalPtr;
  initPtr=new Matrix(init);
  initPtr=&init;

  init=initializePile(chips,dim,dim);
  finalPtr=new Matrix(init);

  finalPtr=stabilizeDyn(initPtr,0);

  std::cout << init << std::endl;
  std::cout << *finalPtr << std::endl;

  std::ofstream ex1("ex1.txt");
  writeSand(*finalPtr, ex1);
  ex1.close();

return 0;
}
