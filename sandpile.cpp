//sandpile.cpp
#include "sandpile.h"

int ipow(int x, int p)
{ //x^p for integers
    int i = 1;
    for (int j = 1; j <= p; j++)
        i *= x;
    return i;
}

SandpileData::SandpileData(int c, int ci, int b, MatrixPtr S, double d)
{ // set up sandpile
    chips = c;
    initChips = ci;
    bht = b;
    stencil = S;
    dloss = d;
    Matrix A(3, 3);
    A(1, 1) = ipow(10, ci);
    MatrixPtr Aptr = new Matrix(A);

    init = Aptr;
    stab = Aptr;
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
};

SandpileData::~SandpileData()
{
    delete stencil;
    delete init;
    delete stab;
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

    return *this;
}

void SandpileData::SetStab(MatrixPtr &A)
{
    delete stab;
    stab = new Matrix(*A);
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
        dstr += "+1e" + std::to_string(logdf);
    }

    string name = "./data/";
    name += "C10^" + std::to_string(A.chips) + dstr + "Bht" + std::to_string(A.bht) 
    + "N" + std::to_string(cn) + "NE" + std::to_string(cne) + "E" + std::to_string(ce) + "SE" + std::to_string(cse)
    + "S" + std::to_string(cs) + "SW" + std::to_string(csw) + "W" + std::to_string(cw) + "NW" + std::to_string(cnw);
    name += ".txt";
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

Matrix initializePile(const int chips, const int dimx, const int dimy)
{
    Matrix config(dimx, dimy);
    int center;

    if (dimx == dimy)
    {
        center = floor(dimx / 2);
    }
    else
    {
        center = 0;
    }
    config(center, center) += chips;

    return (config);
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
void topple(Matrix &sand, Matrix &sten, const double thresh, const int bht)
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
    double c = cn + cne + ce + cse + cs + csw + cw + cnw;

    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            site = sand(i, j) - bht; //account for background height
            if (site >= thresh)
            {
                give = floor(site / (thresh));
                // sand(i,j)= site%thresh+bht; //change for integer case
                // sand(i,j)=site-give*thresh+bht;
                sand(i, j) -= give * thresh;
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

void resize(MatrixPtr &sand, const double thresh, bool &r)
{ //resize sandpile
    int row;
    int col;
    r = false; // if don't resize
    row = sand->Row();
    col = sand->Col();
    MatrixPtr big;
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
        *big = padDir(*sand, nt, nr, nb, nl);

        delete sand;
        sand = new Matrix(*big);

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
    int orow = row;
    int ocol = col;
    int count = 100;
    int counti = 1000;
    int chips10i = sand.InitChips();
    int chips10f = sand.Chips();

    MatrixPtr sandCur = new Matrix(*sand.Init());

    for (int i = chips10i; i <= chips10f; i++)
    {
        max = maxEntry(*sandCur);

        while (max >= (thresh + sand.Bht()))
        { //account for background ht
            resize(sandCur, thresh, req);
            topple(*sandCur, *sand.Stencil(), thresh, sand.Bht());
            max = maxEntry(*sandCur);

            iter++;
            nrow = sandCur->Row();
            ncol = sandCur->Col();

            if (iter % counti == 0)
                cout << "Iters=" << iter << endl;

            if (((nrow % count == 0) || (ncol % count == 0)) && req)
                cout << "rowsxcols=" << nrow << "x" << ncol << endl;
        }
        if (i < chips10f)
        {
            *sandCur = 10 * (*sandCur);
        }
    }

    //subtract bht
    for (int i = 0; i < nrow; i++)
    {
        for (int j = 0; j < ncol; j++)
        {
            (*sandCur)(i, j) = (*sandCur)(i, j) - sand.Bht();
        }
    }

    //update final config;
    sand.SetStab(sandCur);
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
