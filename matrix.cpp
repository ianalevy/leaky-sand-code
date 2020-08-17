#include "matrix.h"

Matrix::Matrix(int r, int c){ // set up a matrix of zeros
   if (r<=0 || c<=0)error("bad matrix indeces \n");
   row=r;
   col=c;
   m=new double[r*c];
   for(int i=0; i<r*c; i++) m[i]=0.0;
 };

Matrix::Matrix( const Matrix& A){ // build a matrix from another one
   row = A.row;
   col = A.col;
   m = new double[row*col];
   for(int i=0; i< row*col; i++) m[i] =A.m[i];
 };

Matrix& Matrix::operator=( const Matrix& B){   // *this=B
   // note that this does not follow the standard form for
   // writing assignment operators. Instead it says it is
   // an error to write A = B unless A & B have the same dimensions.

   if( row!=B.row || col!=B.col) error("Matrix=: incompatible sizes");
   int i, rc = row*col;
   for( i=0; i<rc; i++ ) m[i] = B.m[i];
   return *this;
 }

Matrix& Matrix::operator+=( const Matrix& B){   // *this+=B
   if( row!=B.row || col!=B.col) error("Matrix+=: incompatible sizes");
   int i, rc = row*col;
   for( i=0; i<rc; i++ ) m[i] += B.m[i];
   return *this;
 }

Matrix& Matrix::operator-=( const Matrix& B){   // *this-=B
   if( row!=B.row || col!=B.col) error("Matrix-=: incompatible sizes");
   int i, rc = row*col;
   for( i=0; i<rc; i++ ) m[i] -= B.m[i];
   return *this;
 }

Matrix operator+(const Matrix& A, const Matrix& B){ // add two Matrices
 if (A.row!=B.row || A.col!=B.col)error("Matrix+: incomatable sizes \n");
 Matrix C(A.row, A.col);
 int i,rc=A.row*A.col;
 for(i=0; i<rc; i++)
    C.m[i] = A.m[i] + B.m[i];
 return C;
}

Matrix operator*(const Matrix& A, const Matrix& B){
 // Matrix multiplication
 if (A.col!=B.row)error("Matrix*: incomatable sizes \n");
 Matrix C(A.row, B.col);
 int i,j,k;
 for(i=0; i<A.row; i++)
    for(j=0; j<B.col; j++)
       for(k=0; k<A.col; k++)
          C.m[i*C.col+j]+=A.m[i*A.col + k] * B.m[k*B.col+j];
 return C;
}

Matrix operator*( double a, const Matrix& B){
 // a*B
 Matrix C( B);
 int i,s=C.row*C.col;
 for(i=0; i<s; i++)
   C.m[i] *=a;
 return C;
}

Matrix operator-(const Matrix& A, const Matrix& B){
 // Matrix subtraction
 if (A.row!=B.row || A.col!=B.col)error("Matrix-: incomatable sizes \n");
 Matrix C(A.row, A.col);
 int i,rc=A.row*A.col;
 for(i=0; i<rc; i++)
    C.m[i]=A.m[i] - B.m[i];
 return C;
}

std::ostream& operator<<(std::ostream& os, const Matrix& A){
 // output for small matrices
 int i, j, row=A.Row(), col=A.Col();
 for(i=0; i<row; i++){
    for(j=0; j<col; j++)
       os << A(i,j) << "\t";
    os << "\n";
   }
 return os;
}

Matrix transpose (const Matrix& A){
 Matrix C(A.col, A.row);
 int i,j;
 for(i=0; i<A.row; i++)
    for(j=0; j<A.col; j++)
       C.m[j*C.col + i]=A.m[i*A.col + j];
 return C;
}

Matrix abs (const Matrix& A){
  Matrix C(A);
 int i,k=A.col*A.row;
 for(i=0; i<k; i++)
   C.m[i] = abs(C.m[i]);
 return C;
}

Matrix solve(const Matrix& A, const Matrix& B){
 /*
   solve A x = B and return the answer.
   It is assumed that A can be factored without pivoting.
   The factors are not saved
 */
 if( A.row!=A.col || A.row!=B.row )
    error("Matrix solve: incompatible sizes\n");

 Matrix C(A),X(B); // local copies of A and B
 int i,j,k,cr=C.row;
 double a,b;

 // First preprocess C
 for(i=0; i<cr; i++){ // for each row of C
   if( C(i,i) == 0.0 ) error(" zero divide in solve ");
   a = (C(i,i) = 1./C(i,i)); 
    for( j=i+1; j<cr; j++ )  // normalize the current row
      C(i,j) *=a;
    for( k=i+1; k<cr; k++ ){// for each row after i modify the row
      if((b = C(k,i))!=0.0){ // but only if it is needed.
          for( j=i+1; j<cr; j++ ) 
            C(k,j) -= b*C(i,j);
       }
    }
 }

 // now solve the equations
 for(k=0; k<B.col; k++){ // for each column of X
    for(i=0; i<cr; i++ ){ // forward solution
       for( j=0; j<i; j++ )
         X(i,k) -= C(i,j)*X(j,k);
       X(i,k) *= C(i,i);
    }

    for(i=cr-2; i>=0; i--) // back solution
      for(j=i+1; j<cr;j++ )
         X(i,k) -= C(i,j)*X(j,k);
 }

 return X;

}

Matrix pad(const Matrix& A, const int n) /*
 pad A on all sides with n rows  on the top, n rows on the bottom,
n cols on the left, and n cols on the right */
{ Matrix C(A.row+2*n, A.col+2*n);
 int i,j;

 if( A.row!=A.col)
    error("Matrix pad: Matrix not square\n");

 for(i=0; i<A.row; i++)
    for(j=0; j<A.col; j++)
       C.m[(i+n)*C.col + (j+n)]=A.m[i*A.col + j];
 return C;
}

Matrix padDir(const Matrix& A,const int top, const int rt, const int bot, const int lt)
{// pad A on each side with top, rt, bot lt rows
 int ar = A.row; int ac = A.col;
 Matrix C(ar+top+bot,ac+rt+lt);
 int i,j;

  for(i=0; i<ar; i++){
    for(j=0; j<ac; j++){
       C.m[(i+top)*C.col + (j+lt)]=A.m[i*A.col + j];
      }
    }
 return C;

 }

TridiagonalMatrix::TridiagonalMatrix
( int r, double bv, double dv, double av )
{
  if( r<=0 ) error("tridiagonalMatrix: bad size");
  row = r;
  b = new double[3*row-2];
  d = b + row-1;
  a = d + row;
  int i;
  for( i=0; ; i++ ){
    d[i] = dv;
    if( i+1 == r ) break;
    a[i] = av;
    b[i] = bv;
  }
}

TridiagonalMatrix::TridiagonalMatrix( const tridiag & A )
{
  row = A.row;
  b = new double[3*row-2];
  d = b + row-1;
  a = d + row;
  int j;
  for(j=0; j<3*row-2; j++ )
    b[j] = A.b[j];
}

double& TridiagonalMatrix::operator()(int i, int j)
{
  if( i<0 || j<0 || i>=row || j>= row )
    error( "TridiagonalMatrix: index out of range");
  if( j==i-1 )return b[j];
  if( i==j ) return d[j];
  if( j==i+1 )return a[i];
  error( "TridiagonalMatrix: bad index");
  return d[j]; // get rid of a compiler warning
}

double TridiagonalMatrix::operator()(int i, int j)const
{
  if( i<0 || j<0 || i>=row || j>= row )
    error( "TridiagonalMatrix: index out of range");
  if( j==i-1 )return b[j];
  if( i==j ) return d[j];
  if( j==i+1 )return a[i];
  error( "TridiagonalMatrix: bad index");
  return d[j]; // get rid of a compiler warning
}

TridiagonalMatrix& 
TridiagonalMatrix::operator=( const TridiagonalMatrix& B)
{  // *this=B
  if( row != B.row  )
    error("TridiagonalMatrix operator=: size mismatch");
  int j;
  for( j=0; j<3*row-2; j++ )
    b[j] = B.b[j];
  return *this;
}

tridiag operator+(const tridiag& A, const tridiag& B)
{ // A+B
  if( A.row != B.row )
    error("Tridiagonal matrix operator+: size mismatch");
  tridiag C(A);
  int j;
  for( j=0; j<3*A.row-2; j++)
    C.b[j] += B.b[j];
  return C;
}

Matrix operator*(const tridiag& A, const Matrix& B)
{ // A*B
  if( A.row != B.Row() )
    error("tridiag*matrix: size mismatch");
  int j,k;
  Matrix C(B);
  for( k=0; k<B.Col(); k++ ){
    C(0,k) = A.d[0]*B(0,k);
      for(j=0; j+1<A.row; j++){
        C(j,k) += A.a[j]*B(j+1,k);
        C(j+1,k) = A.d[j+1]*B(j+1,k) + A.b[j]*B(j,k);
      }
  }
  return C;
}

tridiag operator*( double a, const tridiag& B)
{ // a*B
  tridiag C(B);
  int j;
  for( j=0; j<3*B.row-2; j++ )
    C.b[j] *= a;
  return C;
}

tridiag operator-(const tridiag& A, const tridiag& B)
{ // A-B
  if( A.row != B.row )
    error("Tridiagonal matrix operator+: size mismatch");
  tridiag C(A);
  int j;
  for( j=0; j<3*A.row-2; j++)
    C.b[j] -= B.b[j];
  return C;
}

Matrix solve(const tridiag& A, const Matrix& y)
{ // solve Ax=y
  tridiag C(A); // local copy
  Matrix X(y);
  int i,k;

  // modify A
  for( i=0; ; i++ ){
    if( C.d[i] == 0.0 )
      error("tridiag solve: divide by 0");
    C.d[i] = 1.0/C.d[i];
    if( i+1 == C.row )break;
    C.a[i] *= C.d[i];
    C.d[i+1] -= C.b[i]*C.a[i];
  }


  // solve equations
  for(k=0; k<X.Col(); k++ ){
    for( i=0; ; i++ ){
      X(i,k) *= C.d[i];
      if( i+1 == C.row ) break;
      X(i+1,k) -= C.b[i]*X(i,k);
    }
    if( C.row > 1){ // i = C.row -1 > 0
      for(i-- ; ; i-- ){
        X(i,k) -= C.a[i]*X(i+1,k);
        if( i== 0 ) break;
      }
    }
  }

  return X;
}

Matrix tri2full( const tridiag& A)
{ // A as a full matrix
  Matrix C( A.Row(),A.Row()); // C is zero

  int j;
  for( j=0; j+1 < A.Row(); j++ ){
    C(j,j) = A(j,j);
    C(j+1,j) = A(j+1,j);
    C(j,j+1) = A(j,j+1);
  }
  C(j,j) = A(j,j);

    return C;
  
}
