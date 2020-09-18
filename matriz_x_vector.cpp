// ayy.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h> 

void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n ,int local_n, MPI_Comm comm)
{
    double* x;
    int local_i, j;
    int local_ok = 1;

    x =new double[n];
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);

    for (local_i = 0; local_i < local_m; local_i++)
    {
        local_y[local_i] = 0.0;
        for ( j = 0; j < n; j++)
        {
            local_y[local_i] += local_A[local_i * n + j] * x[j];
        }
    }
}


int main(void)
{
    srand(time(NULL));
    int comm_sz;
    int my_rank;
    MPI_Comm comm;
	double start, finish, loc_elapsed, elapsed;
    
    double* local_A, * local_x, * local_y;
    double* A, * x, * y;
    int local_m, local_n, n=2048, m=2048;
    

    A = new double[m * n];
    x = new double[n];
    y = new double[m];

    
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    local_m = m / comm_sz;
    local_n = n / comm_sz;

    local_A = new double[local_m * n];
    local_x = new double[local_n];
    local_y = new double[local_m];

    
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                A[i * n + j] = rand() % 100 + 1.0;
            }
        }
        MPI_Scatter(A, local_m * n, MPI_DOUBLE, local_A, local_m * n, MPI_DOUBLE, 0, comm);

        for (int i = 0; i < n; i++)
            x[i] = rand() % 100 + 1.0;
        MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, comm);
    
	MPI_Barrier(comm);
	start = MPI_Wtime();
    Mat_vect_mult(local_A, local_x, local_y, local_m, n, local_n, comm);
    finish = MPI_Wtime();
	loc_elapsed = finish-start;
	MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
    MPI_Gather(local_y, local_n, MPI_DOUBLE, y, local_n, MPI_DOUBLE, 0, comm);
    if (my_rank == 0)
       std::cout<<"Elapsed time = "<< elapsed<<std::endl;
    /*
    if (my_rank == 0) {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << A[i * n + j] << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        for (int i = 0; i < n; i++)
            std::cout << x[i] << "  ";

        std::cout << std::endl;
    }
    for (int i = 0; i < m; i++)
        std::cout << y[i] << "  ";

    std::cout << std::endl;
    */
    delete A;
    delete x;
    delete y;

    delete local_A;
    delete local_x;
    delete local_y;
    MPI_Finalize();
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
/*
#include <iostream>
#include "mpi.h"
int main(int *argc,char *argv)
{
    int commsize;
    int rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    printf("hello wprld");

    MPI_Finalize();

    return 0;
}*/

/*
const int MAX_STRING = 200;

int main(void)
{
    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank != 0) {
        sprintf_s(greeting, "Greetings from process %d of %d", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else{
        printf("Greetings from process %d of %d\n", my_rank, comm_sz);
        for (int q = 1; q < comm_sz; q++)
        {
            MPI_Recv(greeting,MAX_STRING,MPI_CHAR,q,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf(" %s\n",greeting);
        }
    }



    MPI_Finalize();

    return 0;
}*/

/*
int cont = 0;

int main(void)
{

    int comm_sz;
    int my_rank;


    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    while (cont != 10) {
        if (my_rank == 0) {
            printf(" %d\n", cont);
            cont++;
            MPI_Send(&cont, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&cont, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else {
            MPI_Recv(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" %d\n", cont);
            cont++;
            MPI_Send(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    printf(" %d\n", cont);
    MPI_Finalize();

    return 0;
}*/