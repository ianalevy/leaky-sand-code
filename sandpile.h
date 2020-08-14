//sandpile.h

#include <cmath>
#include <iostream>

#include "utility.h"
#include "matrix.h"
 
// initialize sandpile
Matrix initializePile(const int chips, const int dimx, const int dimy);

// max of matrix
double maxEntry(const Matrix& config);

// max of matrix on bdry
double maxBdry(const Matrix& config);

// topple each entry in matrix if allowed
void topple(Matrix& sand, const int leak);

// stabilize sandpile
void stabilize(Matrix& sand, const int leak);

// stabilize sandpile
MatrixPtr stabilizeDyn(MatrixPtr sand, const int leak);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);