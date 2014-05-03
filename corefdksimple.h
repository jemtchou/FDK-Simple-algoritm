#ifndef COREFDKSIMPLE_H
#define COREFDKSIMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "fileopen.h"
#include <stdlib.h>


class coreFDKsimple
{
public:
    coreFDKsimple();
    ~coreFDKsimple();
    void FDK();
    void Init();
    float **f; //output data
    int GetH() {return h1;}
    int GetW() {return w;}
    int getY() {return y0;}
    int getX() {return x0;}
    float output(int i,int j) {return f[i][j];}
    int x0, y0; //output picture sizes
    int w,h1; // input picture sizes

private:
    float ***F; //data after convolution
    float ***p; //input data
    double *g;
    double *k;
    double *kp;

    int ac,bc;
    int z;
    int h;
    int d;
    int B0, B;
    int a0, a1;
    int b1, b2;

    float x1, y1;
    float R;
};

#endif // COREFDKSIMPLE_H
