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
 MatrixPtr stencil;
 double leak;
 MatrixPtr init;
 MatrixPtr stab;

public:
 SandpileData(int c , int ci, MatrixPtr S, double l);       // constructors and destructors
 SandpileData( const SandpileData& ); //copy constructor
 ~SandpileData( ); //Destructor

 SandpileData& operator=( const SandpileData& B);   // *this=B

 int Chips() const  {return chips;}
 int InitChips() const  {return initChips;}
 double Leak() const {return leak;}
 MatrixPtr Stencil() const {return stencil;} 
 MatrixPtr Init() const {return init;}
 MatrixPtr Stab() const {return stab;}
 double Sent(); //chips sent out

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
void topple(Matrix& sand, Matrix& sten, const double leak);

//resize sandpile if max reached
void resize(MatrixPtr& sand, const double thresh);

// stabilize sandpile
void stabilize(SandpileData& sand);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);