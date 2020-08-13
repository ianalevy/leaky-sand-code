//sandpile.h

#include <cmath>
#include <iostream>

#include "utility.h"
#include "matrix.h"
 
// initialize sandpile
Matrix initializePile(const int chips, const int dimx, const int dimy);

// topple each entry in matrix if allowed
void topple(Matrix& sand, const int leak);

// max of matrix
double maxEntry(const Matrix& config);

// stabilize sandpile
void stabilize(Matrix& sand, const int leak);

// Output Sandpile
void writeSand(const Matrix& sand, std::ostream & out);