//sandpile.cpp
#include "sandpile.h"

int ipow(int x, int p)
{ //x^p for integers
    int i = 1;
    for (int j = 1; j <= p; j++)
        i *= x;
    return i;
}

SandpileData::SandpileData(int c, int ci, double b, MatrixPtr S, double d)
{ // set up sandpile
    chips = c;
    initChips = ci;
    bht = b;
    stencil = S;
    dloss = d;
    Matrix A(3, 3);
    A(0,0)=b; A(0,1)=b; A(0,2)=b;
    A(1,0)=b; A(1,1)=ipow(10, ci); A(1,2)=b;
    A(2,0)=b; A(2,1)=b; A(2,2)=b;
    MatrixPtr Aptr = new Matrix(A);

    Matrix B(3, 3);
    MatrixPtr Bptr = new Matrix(B);

    init = Aptr;
    stab = Aptr;
    odom= Bptr;
};

SandpileData::SandpileData(const SandpileData &A)
{ // build a sandpile from another one
    chips = A.chips;
    initChips = A.initChips;
    bht = A.bht;
    delete stencil;
    stencil = new Matrix(*A.stencil);
    dloss = A.dloss;
    delete init;
    init = new Matrix(*A.init);
    delete stab;
    stab = new Matrix(*A.stab);
    delete odom;
    odom = new Matrix(*A.odom);
};

SandpileData::~SandpileData()
{
    delete stencil;
    delete init;
    delete stab;
    delete odom;
}

SandpileData &SandpileData::operator=(const SandpileData &B)
{ // *this=B
    chips = B.chips;
    initChips = B.initChips;
    bht = B.bht;
    delete stencil;
    stencil = new Matrix(*B.stencil);
    dloss = B.dloss;
    delete init;
    init = new Matrix(*B.init);
    delete stab;
    stab = new Matrix(*B.stab);
    delete odom;
    odom = new Matrix(*B.odom);

    return *this;
}

void SandpileData::SetStab(MatrixPtr& As, MatrixPtr& Ao)
{
    delete stab;
    stab = new Matrix(*As);
    delete odom;
    odom = new Matrix(*Ao);
}

string fileName(const SandpileData &A)
{ //write file name
    Matrix s(*A.stencil);
    int cn = s(0, 1);
    int cne = s(0, 2);
    int ce = s(1, 2);
    int cse = s(2, 2);
    int cs = s(2, 1);
    int csw = s(2, 0);
    int cw = s(1, 0);
    int cnw = s(0, 0);

    int digits=2; //set digits to right of decimal place in bht

    double logdf;
    double d;
    int di;
    double df;
    string dstr;

    d = A.dloss;
    di = floor(d);
    df = d - di;

    dstr = "d" + std::to_string(di);

    if (df > 1e-15)
    {
        logdf = log10(df);
        dstr += "p1e" + (std::to_string(logdf)).substr(0,2+digits);
    }

    double bht; int bhti; double bhtf;
    bht = A.bht;
    bhti = floor(bht);
    bhtf = bht - 1.0*bhti;

// c1e10d1p1e-0.6bht2.100n1ne0e1se0s1sw0w1w0.txt corresponds to c=10^10, d=1+1e-0.6, bht=2.100 then directions
    string name = "./data/";
    name += "c1e" + std::to_string(A.chips) + dstr + "bht" + (std::to_string(bht)).substr(0,3+digits)
    + "n" + std::to_string(cn) + "ne" + std::to_string(cne) + "e" + std::to_string(ce) + "se" + std::to_string(cse)
    + "s" + std::to_string(cs) + "sw" + std::to_string(csw) + "w" + std::to_string(cw) + "w" + std::to_string(cnw);
    // name += ".txt";
    return (name);
}

double sandThresh(const SandpileData &A) //threshold to fire
{
    double c;
    Matrix S(*A.stencil);
    c = S(0, 0) + S(0, 1) + S(0, 2) + S(1, 0) + S(1, 1) + S(1, 2) + S(2, 0) + S(2, 1) + S(2, 2);
    double thresh;
    thresh = c * A.dloss;

    return (thresh);
}

double maxEntry(const Matrix &sand)
{
    double max;
    max = sand(0, 0);
    double site;

    int rows;
    rows = sand.Row();
    int cols;
    cols = sand.Col();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            site = sand(i, j);
            if (max < site)
            {
                max = site;
            }
        }
    }
    return (max);
}

double maxBdry(const Matrix &sand)
{
    double max;
    max = sand(0, 0);
    double site;

    int rows;
    rows = sand.Row();
    int cols;
    cols = sand.Col();

    for (int i = 0; i < rows; i++)
    {
        site = std::max(sand(i, 0), sand(i, cols - 1));
        if (max < site)
        {
            max = site;
        }
    }

    for (int j = 0; j < cols; j++)
    {
        site = std::max(sand(0, j), sand(rows - 1, j));
        if (max < site)
        {
            max = site;
        }
    }

    return (max);
}

// max in each direction of matrix on bdry
void maxBdryVec(const Matrix &sand, double &top, double &rt, double &bot, double &lt)
{
    int rows;
    rows = sand.Row();
    int cols;
    cols = sand.Col();

    top = sand(0, 0);
    rt = sand(0, cols - 1);
    bot = sand(rows - 1, 0);
    lt = sand(0, 0);

    double s1;
    double s2;

    for (int i = 0; i < cols; i++)
    {
        s1 = sand(0, i);
        s2 = sand(rows - 1, i);
        if (top < s1)
        {
            top = s1;
        }
        if (bot < s2)
        {
            bot = s2;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        s1 = sand(i, 0);
        s2 = sand(i, cols - 1);
        if (lt < s1)
        {
            lt = s1;
        }
        if (rt < s2)
        {
            rt = s2;
        }
    }
}

// topple each entry in matrix if allowed
//input sandpile, firing stencil, threshold to fire, background height
void topple(Matrix &sand, Matrix& odom, Matrix &sten, const double thresh)
{
    int rows;
    rows = sand.Row();
    int cols;
    cols = sand.Col();
    double site;
    double give; //number of fires

    double cn = sten(0, 1);
    double cne = sten(0, 2);
    double ce = sten(1, 2);
    double cse = sten(2, 2);
    double cs = sten(2, 1);
    double csw = sten(2, 0);
    double cw = sten(1, 0);
    double cnw = sten(0, 0);

    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            site = sand(i, j); 
            if (site >= thresh)
            {
                give = floor(site / thresh);
                sand(i, j) -= give * thresh;
                odom(i,j) +=give*thresh;
                sand(i - 1, j) += cn * give;
                sand(i - 1, j + 1) += cne * give;
                sand(i, j + 1) += ce * give;
                sand(i + 1, j + 1) += cse * give;
                sand(i + 1, j) += cs * give;
                sand(i + 1, j - 1) += csw * give;
                sand(i, j - 1) += cw * give;
                sand(i - 1, j - 1) += cnw * give;
            }
        }
    }
}

void resize(MatrixPtr &sand, MatrixPtr& odom, const double thresh, bool &r, const double bht)
{ //resize sandpile
    int row;
    int col;
    r = false; // if don't resize
    row = sand->Row();
    col = sand->Col();
    MatrixPtr big;
    MatrixPtr bigo;
    int s = 1; // pad with s in each direction

    double topm, rtm, botm, ltm;
    maxBdryVec(*sand, topm, rtm, botm, ltm);

    int nt = 0;
    int nr = 0;
    int nb = 0;
    int nl = 0;
    if (topm >= thresh)
    {
        nt = s;
    }
    if (rtm >= thresh)
    {
        nr = s;
    }
    if (botm >= thresh)
    {
        nb = s;
    }
    if (ltm >= thresh)
    {
        nl = s;
    }

    if ((nt > 0) || (nr > 0) || (nb > 0) || (nl > 0))
    {
        big = new Matrix(row + nt + nb, col + nr + nl);
        *big = padDirVal(*sand, nt, nr, nb, nl, bht);
        bigo = new Matrix(row + nt + nb, col + nr + nl);
        *bigo = padDirVal(*odom, nt, nr, nb, nl, 0);

        delete sand;
        sand = new Matrix(*big);

        delete odom;
        odom = new Matrix(*bigo);

        r = true;
    }
}

void stabilize(SandpileData &sand)
{
    const double thresh = sandThresh(sand);
    double max = 0;
    int iter = 0;
    bool req = false;
    int row;
    int col;
    row = sand.Init()->Row();
    col = sand.Init()->Col();
    int nrow = row;
    int ncol = col;
    int count = 100;
    int counti = 1000;
    double bht = sand.Bht();
    double site = 0;
    double osite = 0;
    double guess = 0;

    MatrixPtr sandCur = new Matrix(*sand.Init());
    MatrixPtr odomCur = new Matrix(*sand.Odom());

    for (int ichips = sand.InitChips(); ichips <= sand.Chips(); ichips++)
    {
        max = maxEntry(*sandCur);

        while (max >= thresh)
        { //account for background ht
            resize(sandCur, odomCur, thresh, req, bht);
            topple(*sandCur, *odomCur, *sand.Stencil(), thresh);
            max = maxEntry(*sandCur);

            iter++;
            nrow = sandCur->Row();
            ncol = sandCur->Col();

            if (iter % counti == 0)
                cout << "Iters=" << iter << endl;

            if (((nrow % count == 0) || (ncol % count == 0)) && req)
                cout << "rowsxcols=" << nrow << "x" << ncol << endl;
        }
        if (ichips < sand.Chips())
        {   
            //every site which has already fired, fires 10 more times
            *odomCur = 10 * (*odomCur); //update odom
            for (int i = 0; i < nrow; i++)
            { //update sandpile
                for (int j = 0; j < ncol; j++)
                {
                    site = (*sandCur)(i, j);
                    osite = (*odomCur)(i, j);
                    if (bht <= 0)
                    { //negative bht is like a hole
                        (*sandCur)(i, j) = 10 * (site - bht) + bht;
                    }
                    else
                    { //positive bht use odom
                        if (osite > 0)
                        { // has fired
                            guess = odomSand(sand, *odomCur, i, j);
                            if (guess + bht >= 0)
                            { //not center
                                (*sandCur)(i, j) = guess + bht;
                            }
                            else
                            { // center
                                (*sandCur)(i, j) = 10 * site;
                            }
                        }
                        else
                        { //hasn't fired
                            (*sandCur)(i, j) = 10 * (site - bht) + bht;
                        }
                    }
                }
            }
        }
        //update final config;
        sand.SetStab(sandCur, odomCur);
    }
}

//Output sandpile
void writeSand(const Matrix &sand, std::ostream &out)
{
    for (int i = 0; i < sand.Row(); i++)
    {
        for (int j = 0; j < sand.Col(); j++)
        {
            out << sand(i, j) << " ";
        }
        out << "\n";
    }
}

//Odometer value
double odomSand(SandpileData &sand, Matrix& odom, int i, int j)
{
    const double thresh = sandThresh(sand);
    Matrix sten(*sand.Stencil());

    int cn = sten(0, 1);
    int cne = sten(0, 2);
    int ce = sten(1, 2);
    int cse = sten(2, 2);
    int cs = sten(2, 1);
    int csw = sten(2, 0);
    int cw = sten(1, 0);
    int cnw = sten(0, 0);
    double site = 0.0;

    int nrow;
    nrow = odom.Row();
    int ncol;
    ncol = odom.Col();

    site -= odom(i,j);
    if (i > 0)
    {
        site += (cs / thresh) * (odom)(i - 1, j);
    }
    if (i < nrow - 1)
    {
        site += (cn / thresh) * (odom)(i + 1, j);
    }
    if (j > 0)
    {
        site += (ce / thresh) * (odom)(i, j - 1);
    }
    if (j < ncol - 1)
    {
        site += (cw / thresh) * (odom)(i, j + 1);
    }
    if ((i > 0) && (j > 0))
    {
        site += (cse / thresh) * (odom)(i - 1, j - 1);
    }
    if ((i < nrow - 1) && (j > 0))
    {
        site += (cne / thresh) * (odom)(i + 1, j - 1);
    }
    if ((i > 0) && (j < ncol - 1))
    {
        site += (csw / thresh) * (odom)(i - 1, j + 1);
    }
    if ((i < nrow - 1) && (j < ncol - 1))
    {
        site += (cnw / thresh) * (odom)(i + 1, j + 1);
    }
    
    return(site);
}