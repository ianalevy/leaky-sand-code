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
  double leak; //chips which leak each fire 
  int bht; //background height

  MatrixPtr sand;
  sand= new Matrix(4,4);

  MatrixPtr ht;
  ht = new Matrix(1);

  string name="C10^6L10^-5Bht0N1E1S1W1.txt";
  string infile;
  infile = "data/"+ name;
  // // file ="data/test.txt";
  // file="data/C10^6L10^-4Bht0N1E1S1W1.txt";

  readSand(sand, infile);
  aveHtVec(sand, ht);

  // cout << *sand << endl;
  // cout << *ht << endl;

  //output file to data folder
  string outfile;
  outfile="data/analysis/"+ name;
  std::ofstream out1(outfile);
  writeSand(*ht, out1);
  out1.close();

  // cout << "start chips=10^n. For n<=9 enter n=" << endl;
  // cin >> ichips;
  // ichips =3;
  // cout << "Total chips=10^m. For m<=3 enter m=" << endl;
  // cin >> chips;
  // cout << "Background height=" << endl;
  // cin >> bht;
  // cout << "Leak=" << endl;
  // cin >> leak;
  // cout << endl;

// stencil 
  // Matrix A(3,3); //enter n,e,s,w
  // A(0,1)=1; A(1,2)=1; A(2,1)=1; A(1,0)=1; 
  // MatrixPtr sten = new Matrix(A);

//   SandpileData ex1(chips,ichips,bht,sten,leak); 

//   auto start = std::chrono::steady_clock::now();
//   stabilize(ex1);
//   auto end = std::chrono::steady_clock::now();

//   std::chrono::duration<double> elapsed_seconds = end-start;
//   cout << "elapsed time: " << elapsed_seconds.count() << endl;

// //output file to data folder
//   string fn = fileName(ex1);
//   cout << fn << endl;
//   std::ofstream out1(fn);
//   writeSand(*ex1.Stab(), out1);
//   out1.close();

return 0;
}
