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

void topple(Matrix& sand, const int leak){
int rows; rows=sand.Row();
int cols; cols=sand.Col();
int site;
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

double maxEntry(const Matrix& sand)
{
    double max;
    max=0;
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

void stabilize(Matrix& sand, const int leak)
{
const int thresh=leak+4;
int max;
do
{
    topple(sand,leak);
    max= maxEntry(sand);

} while (max>= thresh);

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
