#include <iostream>
#include <vector>
#include <string>

#include "utility.h"
#include "matrix.h"
#include "sandpile.h"
#include <cmath>
#include <fstream>

using namespace std;


int main()
{

  int chips; //total chips
  int dim; //size of initial config

  cout << "chips=" << endl;
  cin >> chips;
  // cout << "initial size=" << endl;
  // cin >> dim;

  // chips=65;
  dim=7;

  Matrix init(dim,dim); //initial chip config
  init=initializePile(chips,dim,dim);

  // cout << init << endl;

//change to resize grid
  stabilize(init,0);
  
  // cout<< init << endl;

  std::ofstream ex1("ex1.txt");
  writeSand(init, ex1);
  ex1.close();

return 0;
}
