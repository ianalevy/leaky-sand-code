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
  int dim=3; //size of initial config

  std::cout << "chips=" << std::endl;
  std::cin >> chips;
  // chips=15;

  Matrix init(dim,dim); //initial chip config

  MatrixPtr initPtr;
  initPtr=new Matrix(init);
  initPtr=&init;

  init=initializePile(chips,dim,dim);
  initPtr=stabilize(initPtr,0);

  // std::cout << init << std::endl;
  // std::cout << *initPtr << std::endl;

  std::ofstream ex1("ex1.txt");
  writeSand(*initPtr, ex1);
  ex1.close();

return 0;
}
