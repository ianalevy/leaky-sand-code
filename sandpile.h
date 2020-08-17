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

class SandpileData{
  // This is a row x col matrix
 int chips;
 MatrixPtr stencil;
 int leak;
 MatrixPtr init;
 MatrixPtr stab;

public:
 SandpileData(int c , MatrixPtr S, int l);       // constructors and destructors
 SandpileData( const SandpileData& ); //copy constructor
 ~SandpileData( ); //Destructor

 SandpileData& operator=( const SandpileData& B);   // *this=B

 int Chips() const  {return chips;}
 int Leak() const {return leak;}
 MatrixPtr Stencil() const {return stencil;} 
 MatrixPtr Init() const {return init;}
 MatrixPtr Stab() const {return stab;}

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
void topple(Matrix& sand, const int leak);

//resize sandpile if max reached
void resize(MatrixPtr& sand, const int thresh);

// stabilize sandpile
void stabilize(SandpileData& sand);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);