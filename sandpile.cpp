//sandpile.cpp

#include "matrix.h"

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

void stabilize(Matrix& sand, const int leak)
{
const int thresh=leak+4;
int max;

do{
    topple(sand,leak);
    max= maxEntry(sand);
} while (max>= thresh);

}

MatrixPtr stabilize(MatrixPtr sand, const int leak)
{
const int thresh=leak+4;
int max; int row; int col;
int size=1;
row= sand -> Row();
col= sand -> Col();
const int addRows=2;

MatrixPtr sandCur;
sandCur = new Matrix(row,col);
*sandCur = Matrix(*sand);

MatrixPtr sandUp;
sandUp = new Matrix(row,col);
*sandUp = Matrix(*sand);


do{
    if(maxBdry(*sandCur)>=thresh){
        delete sandUp;
        sandUp = new Matrix(row+2*size*addRows,col+2*size*addRows);
        *sandUp = pad(*sandCur, addRows);

        delete sandCur;
        sandCur = new Matrix(*sandUp);
        *sandCur = *sandUp;

        size +=1;
    }
    topple(*sandCur,leak);
    max= maxEntry(*sandCur);
} while (max>= thresh);

return(sandCur);
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
