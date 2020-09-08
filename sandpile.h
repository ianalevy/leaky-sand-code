//sandpile.h

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;
using std::vector;

#include "utility.h"
#include "matrix.h"

int ipow (int x, int p); //power

class SandpileData{//all info for sandpile with delta mass at origin
 int chips; //total chips as power of 10
 int initChips; //chips to start with as power of 10
 int bht; // background height is negative bht
 MatrixPtr stencil;
 double dloss; // d parameter
 MatrixPtr init;
 MatrixPtr stab;

public: //total chips, init chips, background ht, stencil, leak
 SandpileData(int c , int ci, int b, MatrixPtr S, double d);       // constructors and destructors
 SandpileData( const SandpileData& ); //copy constructor
 ~SandpileData( ); //Destructor

 SandpileData& operator=( const SandpileData& B);   // *this=B

 int Chips() const  {return chips;}
 int InitChips() const  {return initChips;}
 int Bht() const {return bht;}
 double Dloss() const {return dloss;}
 MatrixPtr Stencil() const {return stencil;} 
 MatrixPtr Init() const {return init;}
 MatrixPtr Stab() const {return stab;}

 void SetStab(MatrixPtr& A); 

 friend string fileName(const SandpileData& A); //write file name
 friend double sandThresh(const SandpileData& A); //threshold to fire
 
};

// initialize sandpile
Matrix initializePile(const int chips, const int dimx, const int dimy);

// max of matrix
double maxEntry(const Matrix& config);

// max of matrix on bdry
double maxBdry(const Matrix& config);

// max in each direction of matrix on bdry
void maxBdryVec(const Matrix& sand, double& top, double& rt, double& bot, double& lt);

// topple each entry in matrix if allowed
//input sandpile, firing stencil, threshold to fire, background height
void topple(Matrix& sand, Matrix& sten, const double thresh, const int bht);

//resize sandpile if max reached
void resize(MatrixPtr& sand, const double thresh);

// stabilize sandpile
void stabilize(SandpileData& sand);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);