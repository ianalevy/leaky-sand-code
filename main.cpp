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

  // chips=65;
  dim=7;

  Matrix init(dim,dim); //initial chip config
  init=initializePile(chips,dim,dim);


  // std::cout << init << std::endl;

//change to resize grid
  stabilize(init,0);

  std::ofstream ex1("ex1.txt");
  writeSand(init, ex1);
  ex1.close();

return 0;
}
