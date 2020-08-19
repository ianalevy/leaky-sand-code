//sandpile.cpp
#include "sandpile.h"

int ipow (int x, int p) {//x^p for integers
  int i = 1;
  for (int j = 1; j <= p; j++)  i *= x;
  return i;
}

SandpileData::SandpileData(int c, int ci, int b, MatrixPtr S, double l){ // set up sandpile
   chips=c;
   initChips=ci;
   bht=b;
   stencil =S;
   leak=l;
   Matrix A(3,3); A(1,1)=ipow(10, ci);
   MatrixPtr Aptr = new Matrix(A);

   init=Aptr;
   stab=Aptr;
 };

 SandpileData::SandpileData(const SandpileData& A){ // build a sandpile from another one
   chips = A.chips;
   initChips=A.initChips;
   bht=A.bht;
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
   initChips=B.initChips;
   bht=B.bht;
   delete stencil;
   stencil = new Matrix(*B.stencil);
   leak=B.leak;
   delete init;
   init = new Matrix(*B.init);
   delete stab;
   stab = new Matrix(*B.stab);

   return *this;
 }

double SandpileData::Sent(){//chips sent out
     Matrix A(*stencil);
     double c;
     c=A(0,1)+A(1,2)+A(2,1)+A(1,0);
    
    return(c);
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
    int logleak;
    string logstring;
    if(A.leak< 1e-15){
        logleak=0;
        logstring = "L" + std::to_string(logleak);
    }
    else{
        logleak=round(log10(A.leak));
        logstring = "L10^" + std::to_string(logleak);
    }

    string name = "./data/";
    name += "C10^"+ std::to_string(A.chips) 
     + logstring
     + "Bht" + std::to_string(A.bht)
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
    double site;

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
    double site;

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

vector<double> maxBdryVec(const Matrix& sand)
{   int rows; rows=sand.Row();
    int cols; cols=sand.Col();

    vector<double> maxv(4);

    double top(sand(0,0));
    double rt(sand(0,cols-1));
    double bot(sand(rows-1,0));
    double lt(sand(0,0));

    double s1; double s2;

    for (int i=0;i<cols;i++){
        s1= sand(0,i);
        s2=sand(rows-1,i);
        if (top < s1) {top = s1;}
        if (bot < s2) {bot =s2;}
    }

    for (int i=0;i<rows;i++){
        s1= sand(i,0);
        s2=sand(i,cols-1);
        if (lt < s1) {lt = s1;}
        if (rt < s2) {rt =s2;}
    }


    maxv[1]=top; 
    maxv[2]=rt;
    maxv[3]=bot;
    maxv[4]=lt;

    return (maxv);
}

// topple each entry in matrix if allowed
//input sandpile, firing stencil, chips which leak out, background height
void topple(Matrix& sand, Matrix& sten, const double leak, const int bht){
int rows; rows=sand.Row();
int cols; cols=sand.Col();
double site;
double give; //number of fires

double cn=sten(0,1); double ce=sten(1,2); 
double cs=sten(2,1); double cw=sten(1,0);
double c=cn+ce+cs+cw;

const double thresh=leak+c;

for (int i=1; i<rows-1; i++){
    for (int j=1; j<cols-1; j++){
        site=sand(i,j)-bht; //account for background height
        if(site>= thresh) {
            give=floor(site/(thresh));
            // sand(i,j)= site%thresh+bht; //change for integer case
            sand(i,j)-=give*thresh+bht;
            sand(i+1,j)+=cn*give;
            sand(i-1,j)+=cs*give;
            sand(i,j+1)+=ce*give;
            sand(i,j-1)+=cw*give;
            }
    }
}
}

void resize(MatrixPtr& sand, const double thresh){//resize sandpile
int row; int col;
row = sand -> Row();
col = sand -> Col();
MatrixPtr big;
int s=1; // pad with s in each direction

// this speeds up code in uniform case. why?

if(maxBdry(*sand)>=thresh){
 big = new Matrix(row+2*s,col+2*s);
 *big = pad(*sand,s);

 delete sand;
 sand = new Matrix(*big);
}

/* //bug here
vector<double> maxv(4);
maxv = maxBdryVec(*sand);
double topm, rtm, botm, ltm;
topm=maxv[1]; rtm=maxv[2]; botm=maxv[3]; ltm=maxv[4];

int nt=0; int nr=0; int nb=0; int nl=0;
if(topm >= thresh){nt=s;}
if(rtm >= thresh){nr=s;}
if(botm >= thresh){nb=s;}
if(ltm >= thresh){nl=s;}

big = new Matrix(row+nt+nb,col+nr+nl);
*big = padDir(*sand,nt,nr,nb,nl);

delete sand;
sand = new Matrix(*big);
*/
}

void stabilize(SandpileData &sand){
 const double thresh = sand.Leak() + sand.Sent();
 double max = 0;
 int iter = 0;
 int row; int col;
 row = sand.Init()->Row(); col = sand.Init()->Col();
 int nrow = row; int ncol = col;
 int count = 1000;
 int chips10i = sand.InitChips();
 int chips10f = sand.Chips();

 MatrixPtr sandCur = new Matrix(*sand.Init());

 for (int i=chips10i; i<=chips10f; i++) {
     max = maxEntry(*sandCur); 

     while (max >= (thresh+sand.Bht())){ //account for background ht
         resize(sandCur, thresh);
         topple(*sandCur, *sand.Stencil(), sand.Leak(), sand.Bht());
         max = maxEntry(*sandCur);

         iter++;
         nrow = sandCur->Row();
         ncol = sandCur->Col();
         if (iter % count == 0){
             cout << "Iters=" << iter << endl;
             cout << "rowsxcols=" << nrow << "x" << ncol << endl;
            }
        }
     if (i<chips10f){ *sandCur = 10*(*sandCur);}
 }

 //subtract bht
 for (int i=0; i<nrow; i++){
     for(int j=0; j<ncol; j++){
         (*sandCur)(i,j) =(*sandCur)(i,j) - sand.Bht();
     }
 }

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
