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

  int chips; //total chips as pow 10
  int ichips; //inital chips as pow 10
  int logleak;
  double dloss; //chips which leak each fire 
  int bht; //background height

  // cout << "start chips=10^n. For n<=9 enter n=" << endl;
  // cin >> ichips;
  ichips =3;
  cout << "Total chips=10^m. For m>=3 enter m=" << endl;
  cin >> chips;
  cout << "Background height=" << endl;
  cin >> bht;
  cout << "d=" << endl;
  cin >> dloss;
  cout << endl;

// stencil 
  Matrix A(3,3); //enter n,e,s,w
  A(0,1)=1; A(1,2)=1; A(2,1)=1; A(1,0)=1; 
  MatrixPtr sten = new Matrix(A);

  SandpileData ex1(chips,ichips,bht,sten,dloss); 

  auto start = std::chrono::steady_clock::now();
  stabilize(ex1);
  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  cout << "elapsed time: " << elapsed_seconds.count() << endl;

  // cout << *ex1.Stab() << endl; //final pile

  // std::ofstream out1("out1.txt");
  // writeSand(*ex1.Stab(), out1);
  // out1.close();

  // std::ofstream out1("./data/out1.txt");
  // writeSand(*ex1.Stab(), out1);
  // out1.close();

//output file to data folder
  string fn = fileName(ex1);
  cout << fn << endl;
  std::ofstream out1(fn);
  writeSand(*ex1.Stab(), out1);
  out1.close();

return 0;
}
