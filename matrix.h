#ifndef MATRIXH
#define MATRIXH
#include <cmath>
#include <iostream>

#include "utility.h"

class Matrix;
typedef Matrix* MatrixPtr;
class Matrix{
  // This is a row x col matrix
 int row, col;
 double *m;

public:
 Matrix(int r , int c=1);       // constructors & destructor
 Matrix( const Matrix& );
 ~Matrix(){delete [] m;};

 int Row() const  {return row;}
 int Col() const {return col;}
 inline double& operator()(int i, int j=0)
 { if( 0<=i && i<row && 0<=j && j<col)
     return (m[i*col+j]);
   else error("Matrix[]: index bust");
   return m[0]; //get rid of a compiler warning
 }

 inline double operator()(int i, int j=0)const
 { if( 0<=i && i<row && 0<=j && j<col)
     return (m[i*col+j]);
   else error("Matrix[]: index bust");
   return m[0]; //get rid of a compiler warning
 }

 Matrix& operator=( const Matrix& B);   // *this=B
 Matrix& operator+=( const Matrix& B);   // *this+=B
 Matrix& operator-=( const Matrix& B);   // *this-=B


 friend Matrix operator+(const Matrix& A, const Matrix& B); // A+B
 friend Matrix operator*(const Matrix& A, const Matrix& B); // A*B
 friend Matrix operator*( double a, const Matrix& B); // a*B
 friend Matrix operator-(const Matrix& A, const Matrix& B); // A-B
 friend Matrix transpose(const Matrix&); // return A^T
 friend Matrix abs(const Matrix&); // take abs of entries
 friend Matrix solve(const Matrix& A, const Matrix& y); // solve Ax=y
 friend Matrix pad(const Matrix& A, const int n); // pad A on all sides with n rows and n cols
};

std::ostream& operator<<(std::ostream& os, const Matrix& A);

// square tridiagonal matrices
class TridiagonalMatrix;
typedef TridiagonalMatrix tridiag;
class TridiagonalMatrix{
  int row;
  double *b,*d,*a;
 public:
  TridiagonalMatrix( int r, double bv=0.0, double dv=0.0, double av=0.0 );
  TridiagonalMatrix( const tridiag & A );
  ~TridiagonalMatrix(){ delete [] b;};

  int Row() const  {return row;}
  int Col() const {return row;} // it is square
  double& operator()(int i, int j);
  double operator()(int i, int j) const;
  TridiagonalMatrix& operator=( const TridiagonalMatrix& B);

  friend tridiag operator+(const tridiag& A, const tridiag& B);
  friend Matrix operator*(const tridiag& A, const Matrix& B);
  friend tridiag operator*( double a, const tridiag& B);
  friend tridiag operator-(const tridiag& A, const tridiag& B);
  friend Matrix solve(const tridiag& A, const Matrix& y);

};

Matrix tri2full( const tridiag& A); // A as a full matrix

#endif






