#include <iostream>

using namespace std;

const int MAX=1000;
double A[MAX][MAX], x[MAX][MAX], y[MAX][MAX];

int main()
{
    int block=2;
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            A[i][j]=(i+1)*j;
            x[i][j]=(j+1)*i;
            y[i][j]=0;
        }
    }

    int i, j, k, kk, jj;
    int bsize=4;
    int n=MAX;
    double sum;
    int en = bsize * (n/bsize);
    for (kk = 0; kk < en; kk += bsize) {
        for (jj = 0; jj < en; jj += bsize) {
            for (i = 0; i < n; i++) {
                for (j = jj; j < jj + bsize; j++) {
                    sum = y[i][j];
                    for (k = kk; k < kk + bsize; k++) {
                        sum += A[i][k]*x[k][j];
                    }
                    y[i][j] = sum;
                }
            }
        }
    }

    /*
    for(int i=0 ; i<MAX ; i+=block){
        for(int j=0 ; j<MAX ; j+=block){
            for(int k=0 ; k<MAX ; k+=block){
                for(int i1=i ; i1<i+block ; i++){
                    for(int j1=j ; j1<j+block ; j++){
                        for(int k1=k ; k1<k+block ; k++){
                            y[i1][j1]+=A[i1][k1]*x[k1][j1];
                        }
                    }
                }
            }
        }
    }
    */

    return 0;
}
