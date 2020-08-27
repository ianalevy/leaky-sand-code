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
using std::to_string;
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

  cout << "Chips=10^n. Enter n=" << endl;
  cin >> chips;
  // chips = 6;

  cout << "Background height=" << endl;
  cin >> bht;
  cout << "Leak= 10^-n. Enter n=" << endl;
  cin >> logleak;
  cout << endl;

  string name="C10^"+to_string(chips)+"L10^-"+to_string(logleak)+"Bht"+ to_string(bht)+"N1E1S1W1.txt";
  string infile;
  infile = "data/"+ name;

  readSand(sand, infile);
  aveHtVec(sand, ht);

  //output file to data folder
  string outfile;
  outfile="data/analysis/"+ name;
  std::ofstream out1(outfile);
  writeSand(*ht, out1);
  out1.close();

return 0;
}
