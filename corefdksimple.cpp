#include "corefdksimple.h"
#include <time.h>

coreFDKsimple::coreFDKsimple()
{
}

coreFDKsimple::~coreFDKsimple(){
    std::cout<<"~coreFDKSimple()"<<std::endl;

    for (int i = 0; i < B0; i++){
       for(int j = 0; j < w; j++){
         free(p[i][j]);
       }
       free(p[i]);
     }
     free(p);

    for (int i = 0; i < B0; i++){
         for(int j = 0; j < (2*a0+1); j++)
            free(F[i][j]);
         free(F[i]);
     }
     free(F);

     for (int i = 0; i < x0; i++)
         free (f[i]);
     free(f);

     free(k);
     free(g);
     free(kp);
     std::cout<<"End of ~coreFDK()"<<std::endl;
}

void coreFDKsimple::Init(){
    std::cout<<"FDK Init()"<<std::endl;

    x0 = 500;
    y0 = 500;
    z = -7;
    x1 = 0.8;
    y1 = 0.8;
    ac = 215;
    bc = 206;
    R = 4827.273;

    F = NULL;
    p = NULL;
    f = NULL;
    g = NULL;
    k = NULL;
    kp = NULL;
}

void coreFDKsimple::FDK(){

    std::cout<<"corefdk()"<<std::endl;
    FileOpen* file = new FileOpen();
    file->Init();
    file->OpenFile();
    w = file->GetW();
    h1 = file->GetH();
    B0 = file->getMAX();

    d = (w-1)/2;
    h = (h1-1)/2;

    p = (float ***)malloc(sizeof(float *) * B0);
    for (int i = 0; i < B0; i++) {
        p[i] = (float **)malloc(sizeof(float *) * (abs(w-2*ac) + w));
        for(int j = 0; j < (abs(w-2*ac) + w); j++) {
            p[i][j] = (float *)calloc((abs(h1-2*bc) + h1), sizeof(float));
        }
    }

    /*Rewriting raw data from buffer to p)*/
    for(int I = 0; I < B0; I++) {
        for (int i = (abs(w-2*ac)+(w-2*ac))/2; i < w+(abs(w-2*ac)+(w-2*ac))/2; i++) {
            for (int j = (abs(h1-2*bc)+(h1-2*bc))/2; j < h1+(abs(h1-2*bc)+(h1-2*bc))/2; j++) {
                p[I][i][j] = file->GetBuffer(I,j-(abs(h1-2*bc)+(h1-2*bc))/2,i-(abs(w-2*ac)+(w-2*ac))/2);
                p[I][i][j] =-log(p[I][i][j]);
            }
        }
    }
    //call ~FileOpen()
    delete file;

    h1 = abs(h1-2*bc) + h1;
    w = abs(w-2*ac) + w;
    d = (w-1)/2;
    h = (h1-1)/2;

    k=(double*)calloc((2*d+1), sizeof(double));
    g=(double*)calloc((4*d+3), sizeof(double));
    int l = -2*d+1;

    g[0] = 0;
    while (l < 0) {
        g[l+2*d] = -1.0/(l*l);
        g[l+2*d+1] = 0;
        l=l+2;
    }

    //M_PI_2 = M_PI/2
    g[2*d] = M_PI_2*M_PI_2;
    l = 1;

    while (l < 2*d) {
        g[l+2*d] = -1.0/(l*l);
        g[l+2*d+1] = 0;
        l=l+2;
    }

    a0 = d;//(int)(R*x0/(R-x0));
    //if (a0>d){a0=d;}

    f = (float **)malloc(sizeof(float *) * x0);
    for (int i = 0; i < x0; i++) {
        f[i] = (float *)calloc(y0, sizeof(float));
    }

    kp = (double*)calloc((2*d+1), sizeof(double));

    double a,b; //здесь a,b - функции x,y
    double s,c,k1,k2,k3,k4;

   /**************Convolution calculation*******************/

    if (z > 0){
    clock_t convtime; //convolution - свёртка
    convtime = clock();

    b1 = (int)((z*R/(R +(x1*x0/2)*sqrt(2))) + h);
    b2 = (int)((z*R/(R -(x1*x0/2)*sqrt(2))) + h)+1;
    F = (float ***)malloc(sizeof(float *) * B0);
    for (int i = 0; i < B0; i++) {
        F[i] = (float **)malloc(sizeof(float *) * (2*a0+1));
        for(int j = 0; j < (2*a0+1); j++) {
             F[i][j] = (float *)calloc(b2-b1, sizeof(float));
        }
    }
    if (b2 > h1) {b2 = h1;}

    for (int b = b1; b < b2; b++) {
        for (int i = -d; i < d+1; i++) {
            k[i+d] = 1/sqrt(R*R + i*i + (b-h)*(b-h)); //R - приведённый радиус (R/(единицу а))
        }
        for (B = 0; B < B0; B++) {
            for (a1 =-d; a1<d+1; a1++){
                kp[a1+d]= p[B][a1+d][b]*k[a1+d];
            }
        for (int a = -a0; a < a0+1; a++) {
                a1 = -d;
                while (a1<d) {
                    F[B][a+a0][b-b1]+= kp[a1+d]*g[a-a1+2*d];
                    a1++;
                }
            }
        }
    }
  convtime = clock() - convtime;
  std::cout << "Convolution time is " << ((float)convtime)/CLOCKS_PER_SEC << std::endl;
    }//end of if(z>0)

    if (z <= 0){
        clock_t convtime;
        convtime = clock();

        b2 = (int)((z*R/(R +(x1*x0/2)*sqrt(2))) + h)+1;
        b1 = (int)((z*R/(R -(x1*x0/2)*sqrt(2))) + h);

        F = (float ***)malloc(sizeof(float *) * B0);
        for (int i = 0; i < B0; i++) {
            F[i] = (float **)malloc(sizeof(float *) * (2*a0+1));
            for(int j = 0; j < (2*a0+1); j++) {
                 F[i][j] = (float *)calloc(b2-b1, sizeof(float));
            }
        }

        if (b1 <0) {b1 = 0;}
        for (int b = b1; b < b2; b++) {
            for (int i = -d; i < d+1; i++) {
                k[i+d] = 1/sqrt(R*R + i*i + (b-h)*(b-h)); //R - приведённый радиус (R/(единицу а))
            }
            for (B = 0; B<B0; B++) {
                for (a1 =-d; a1<d+1; a1++){
                    kp[a1+d]= p[B][a1+d][b]*k[a1+d];
                }
            for (int a = -a0; a < a0+1; a++) {
                    a1 = -d;
                    while (a1<d) {
                        F[B][a+a0][b-b1]+= kp[a1+d]*g[a-a1+2*d];
                        a1++;
                    }
             }
             }
        }

        convtime = clock() - convtime;
        std::cout << "Convolution time is " << ((float)convtime)/CLOCKS_PER_SEC << std::endl;
    }//end of if(z<=0)

    /***************************End of Convolution calculation*******************************/

        clock_t inttime;
        inttime = clock();
        std::cout<<"Starting Integration"<<std::endl;

        for (B = 0; B<B0; B++) {
            s = sin(B*2*M_PI/B0);
            c = cos(B*2*M_PI/B0);
            for (int x = (-x0/2); x<(x0/2); x++) {
                for (int y = (-y0/2); y<(y0/2); y++) {
                    a = R*((y*y1*c) - (x*x1*s))/(R + x*x1*c + y*y1*s)+a0;
                    b = z*R/(R + x*x1*c + y*y1*s)+h-b1;
                    //Петров, Лобанов - страница 154
                    k1 = (a - (int)a-1)*(b-(int)b-1);
                    k2 = (-1)*(a - (int)a)*(b-(int)b-1);
                    k3 = (-1)*(a - (int)a-1)*(b-(int)b);
                    k4 = (a - (int)a)*(b-(int)b);
                    f[x+(x0/2)][y+(y0/2)]+= (k1*F[B][(int)a][(int)b] +k2*F[B][(int)a+1][(int)b]+k3*F[B][(int)a][(int)b+1]+k4*F[B][(int)a+1][(int)b+1])/((R + x*x1*c + y*y1*s)*(R + x*x1*c + y*y1*s));
                }
            }
        }

        inttime = clock() - inttime;
        std::cout << "Integration time is" << ((float)inttime)/CLOCKS_PER_SEC<<std::endl;
        std::cout<<"End of core FDKSimple()"<<std::endl;

}//end of corefdk


