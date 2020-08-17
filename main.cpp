#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include "utility.h"
#include "matrix.h"
#include "sandpile.h"
#include <cmath>
#include <fstream>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std:: cin;

int main()
{

  int chips; //total chips
  int dim=3; //size of initial config

  // std::cout << "chips=" << std::endl;
  // std::cin >> chips;
  chips=100;

// stencil 
  Matrix A(3,3); //enter n,e,s,w
  A(0,1)=1; A(1,2)=1; A(2,1)=1; A(1,0)=1; 

  MatrixPtr sten = new Matrix(A);

  SandpileData ex1(chips,sten,0); 
  // cout << *ex1.Stab() << endl; //initial pile

  auto start = std::chrono::steady_clock::now();
  stabilize(ex1);
  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

  // cout << *ex1.Stab() << endl; //final pile

  std::ofstream out1("out1.txt");
  writeSand(*ex1.Stab(), out1);
  out1.close();

  // std::ofstream out1("./data/out1.txt");
  // writeSand(*ex1.Stab(), out1);
  // out1.close();

  // cout << fileName(ex1) << endl;
  // std::ofstream out1(fileName(ex1));
  // writeSand(*ex1.Stab(), out1);
  // out1.close();

return 0;
}
