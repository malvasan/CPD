#include <iostream>

using namespace std;

const int MAX=800;
double A[MAX][MAX], x[MAX][MAX], y[MAX][MAX];

int main()
{
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            A[i][j]=(i+1)*j;
            x[i][j]=(j+1)*i;
            y[i][j]=0;
        }
    }


    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            for(int k=0;k<MAX;k++){
                y[i][j]+=A[i][k]*x[k][j];
            }
        }
    }


    return 0;
}
