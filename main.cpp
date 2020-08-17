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
  chips=15;

// stencil 
  Matrix A(3,3);
  A(0,1)=1; A(2,1)=1;
  A(1,0)=1; A(1,2)=1;

  MatrixPtr sten = new Matrix(A);

  SandpileData ex1(10000,sten,0); 
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

  // Matrix ian(3,3);
  // for(int i=0;i<3;i++){
  //   for(int j=0;j<3;j++){
  //     ian(i,j)=i*ian.Col()+j;
  //   }
  // }

  // cout << ian << endl;
  // MatrixPtr ianpt = new Matrix(ian);
  // resize(ianpt,7);
  // cout << *ianpt << endl;

  // cout << padDir(ian,3,1,2,0) << endl;


  // vector<double> ianm(4);
  // ianm=maxBdryVec(ian);

  // cout<< ian << endl;
  // cout<< ianm[1] << ""<< ianm[2]<< "" << ianm[3] << "" << ianm[4] << endl;

return 0;
}
