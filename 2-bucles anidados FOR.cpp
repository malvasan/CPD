#include <iostream>

using namespace std;

const int MAX=10000;
double A[MAX][MAX], x[MAX], y[MAX];

int main()
{
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            A[i][j]=(i+1)*j;
        }
        x[i]=i*2;
        y[i]=0;
    }

    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            y[i]+=A[i][j]*x[j];
        }
    }

    for(int j=0;j<MAX;j++){
        y[j]=0;
    }

    for(int j=0;j<MAX;j++){
        for(int i=0;i<MAX;i++){
            y[i]+=A[i][j]*x[j];
        }
    }

    return 0;
}
