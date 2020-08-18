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

int ipow (int x, int p); //x^p for integers

class SandpileData{//all info for sandpile with delta mass at origin
 int chips; //total chips as power of 10
 int initChips; //chips to start with as power of 10
 int bht; // background height is negative bht
 MatrixPtr stencil;
 int leak;
 MatrixPtr init;
 MatrixPtr stab;

public:
 SandpileData(int c , int ci, int b, MatrixPtr S, int l);       // constructors and destructors
 SandpileData( const SandpileData& ); //copy constructor
 ~SandpileData( ); //Destructor

 SandpileData& operator=( const SandpileData& B);   // *this=B

 int Chips() const  {return chips;}
 int InitChips() const  {return initChips;}
 int Bht() const {return bht;}
 int Leak() const {return leak;}
 MatrixPtr Stencil() const {return stencil;} 
 MatrixPtr Init() const {return init;}
 MatrixPtr Stab() const {return stab;}
 int Sent(); //chips sent out

 void SetStab(MatrixPtr& A); 

 friend string fileName(const SandpileData& A); //write file name
 
};

// initialize sandpile
Matrix initializePile(const int chips, const int dimx, const int dimy);

// max of matrix
double maxEntry(const Matrix& config);

// max of matrix on bdry
double maxBdry(const Matrix& config);

// max in each direction of matrix on bdry
vector<double> maxBdryVec(const Matrix& config);

// topple each entry in matrix if allowed
//input sandpile, firing stencil, chips which leak out, background height
void topple(Matrix& sand, Matrix& sten, const int leak, const int bht);

//resize sandpile if max reached
void resize(MatrixPtr& sand, const int thresh);

// stabilize sandpile
void stabilize(SandpileData& sand);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);