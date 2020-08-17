//sandpile.cpp
#include "sandpile.h"

SandpileData::SandpileData(int c, MatrixPtr S, int l){ // set up sandpile
   chips=c;
   stencil =S;
   leak=l;
   Matrix A(3,3); A(1,1)=c;
   MatrixPtr Aptr = new Matrix(A);

   init=Aptr;
   stab=Aptr;
 };

 SandpileData::SandpileData(const SandpileData& A){ // build a sandpile from another one
   chips = A.chips;
   delete stencil;
   stencil = new Matrix(*A.stencil);
   leak = A.leak;
   delete init;
   init = new Matrix(*A.init);
   delete stab;
   stab = new Matrix(*A.stab);
 };

 SandpileData::~SandpileData( ){
     delete stencil;
     delete init;
     delete stab;
 }

SandpileData& SandpileData::operator=( const SandpileData& B){   // *this=B
   chips=B.chips;
   delete stencil;
   stencil = new Matrix(*B.stencil);
   leak=B.leak;
   delete init;
   init = new Matrix(*B.init);
   delete stab;
   stab = new Matrix(*B.stab);

   return *this;
 }

 void SandpileData::SetStab(MatrixPtr& A){
     delete stab;
     stab = new Matrix(*A);
 }

 string fileName(const SandpileData& A){ //write file name
    Matrix s(*A.stencil);
    int north = s(0,1);
    int east = s(1,2);
    int south = s(2,1);
    int west = s(1,0);

    string name = "./data/";
    name += "Chips"+ std::to_string(A.chips) 
     + "Leak" + std::to_string(A.leak)
     +"N" + std::to_string(north) +"E" + std::to_string(east) 
     +"S" + std::to_string(south) + "W" + std::to_string(west);
    name+= ".txt";
    return(name);
 }

Matrix initializePile(const int chips, const int dimx, const int dimy){
Matrix config(dimx,dimy);
int center;

if(dimx==dimy){
  center=floor(dimx/2);}
  else {center=0;}
config(center,center)+=chips;

  return (config);
}

double maxEntry(const Matrix& sand)
{
    double max;
    max=sand(0,0);
    int site;

    int rows; rows=sand.Row();
    int cols; cols=sand.Col();

    for (int i=0; i<rows; i++){
     for (int j=0; j<cols; j++){
        site=sand(i,j);
        if(max < site) {max=site;}
     }
    }
    return (max);
}

double maxBdry(const Matrix& sand)
{
    double max;
    max=sand(0,0);
    int site;

    int rows; rows=sand.Row();
    int cols; cols=sand.Col();

    for (int i=0; i<rows; i++){
        site=std::max(sand(i,0),sand(i,cols-1));
        if(max < site) {max=site;}
     }

    for (int j=0; j<cols; j++){
        site=std::max(sand(0,j),sand(rows-1,j));
        if(max < site) {max=site;}
     }


    return (max);
}

void topple(Matrix& sand, const int leak){
int rows; rows=sand.Row();
int cols; cols=sand.Col();
int site; int upSite;
int give; 
const int thresh=leak+4;

for (int i=1; i<rows-1; i++){
    for (int j=1; j<cols-1; j++){
        site=sand(i,j);
        if(site>= thresh) {
            give=floor(site/(thresh));
            sand(i,j)= site%thresh;
            sand(i+1,j)+=give;
            sand(i-1,j)+=give;
            sand(i,j+1)+=give;
            sand(i,j-1)+=give;
            }
    }
}
}

void stabilize(SandpileData& sand)
{
const int thresh=sand.Leak()+4;
int max; int row; int col;
int size=1;
row= sand.Init() -> Row();
col= sand.Init() -> Col();
const int addRows=2;

MatrixPtr sandCur = new Matrix(*sand.Init());
MatrixPtr sandUp = new Matrix(*sandCur);

do{
    if(maxBdry(*sandCur)>=thresh){ //resize grid
        delete sandUp;
        sandUp = new Matrix(row+2*size*addRows,col+2*size*addRows);
        *sandUp = pad(*sandCur, addRows);

        delete sandCur;
        sandCur = new Matrix(*sandUp);
        *sandCur = *sandUp;

        size +=1;
    }
    topple(*sandCur,sand.Leak());
    max= maxEntry(*sandCur);
} while (max>= thresh);

//update final config;
sand.SetStab(sandCur);
}

//Output sandpile
void writeSand(const Matrix& sand, std::ostream & out)
{ 
    for (int i=0; i<sand.Row(); i++){
     for (int j=0; j<sand.Col(); j++){
        out << sand(i,j) << " ";
     }
     out << "\n";
    }
}
