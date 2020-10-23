//sandpile.h

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

#include "utility.h"
#include "matrix.h"

int ipow(int x, int p); //power

class SandpileData
{                  //all info for sandpile with delta mass at origin
    int chips;     //total chips as power of 10
    int initChips; //chips to start with as power of 10
    double bht;       // background height
    MatrixPtr stencil;
    double dloss; // d parameter
    MatrixPtr init;
    MatrixPtr stab;
    MatrixPtr odom;

public:                                                        //total chips, init chips, background ht, stencil, leak
    SandpileData(int c, int ci, int b, MatrixPtr S, double d); // constructors and destructors
    SandpileData(const SandpileData &);                        //copy constructor
    ~SandpileData();                                           //Destructor

    SandpileData &operator=(const SandpileData &B); // *this=B

    int Chips() const { return chips; }
    int InitChips() const { return initChips; }
    double Bht() const { return bht; }
    double Dloss() const { return dloss; }
    MatrixPtr Stencil() const { return stencil; }
    MatrixPtr Init() const { return init; }
    MatrixPtr Stab() const { return stab; }
    MatrixPtr Odom() const { return odom; }

    void SetStab(MatrixPtr& As, MatrixPtr& Ao);

    friend string fileName(const SandpileData &A);   //write file name
    friend double sandThresh(const SandpileData &A); //threshold to fire
};

// max of matrix
double maxEntry(const Matrix &config);

// max of matrix on bdry
double maxBdry(const Matrix &config);

// max in each direction of matrix on bdry
void maxBdryVec(const Matrix &sand, double &top, double &rt, double &bot, double &lt);

// topple each entry in matrix if allowed
//input sandpile, firing stencil, threshold to fire
void topple(Matrix &sand, Matrix& odom, Matrix &sten, const double thresh);

//resize sandpile if max reached
void resize(MatrixPtr &sand, MatrixPtr& odom, const double thresh, bool &r, const double bht);

// stabilize sandpile
void stabilize(SandpileData &sand);

// Output Sandpile
void writeSand(const Matrix &sand, std::ostream &out);