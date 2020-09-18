// ayy.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h> 
int compute_partner(int phase, int my_rank, int p);
int Compare(const void* a_p, const void* b_p) {
    int a = *((int*)a_p);
    int b = *((int*)b_p);

    if (a < b)
        return -1;
    else if (a == b)
        return 0;
    else 
        return 1;
}
void ordenamiento(int local_A[],int p,int my_rank,int local_n, MPI_Comm comm)
{
    int temp,partner;
    int* temp_A = new int[local_n];
    int* temp_C = new int[local_n];
    int iterator1, iterator2, iterator3;
    MPI_Status status;
    
    qsort(local_A, local_n, sizeof(int), Compare);
    
    for (int phase= 0; phase < p; phase++)
    {
        partner=compute_partner(phase, my_rank, p);

        if (partner != -1) {
            

            MPI_Sendrecv(local_A, local_n, MPI_INT, partner, 0,
                temp_A, local_n, MPI_INT, partner, 0, comm,
                &status);
            
            if (my_rank < partner) {

                iterator1 = iterator2 = iterator3 = 0;
                while (iterator3 < local_n) {
                    if (local_A[iterator1] <= temp_A[iterator2]) {
                        temp_C[iterator3] = local_A[iterator1];
                        iterator3++; iterator1++;
                    }
                    else {
                        temp_C[iterator3] = temp_A[iterator2];
                        iterator3++; iterator2++;
                    }
                }

                for (int i = 0; i < local_n; i++)
                {
                    local_A[i] = temp_C[i];

                }


            }
            else {

                iterator1 = local_n-1;
                iterator2 = local_n-1;
                iterator3 = local_n-1;
                while (iterator3 >= 0) {
                    if (local_A[iterator1] >= temp_A[iterator2]) {
                        temp_C[iterator3] = local_A[iterator1];
                        iterator3--; iterator1--;
                    } else {
                        temp_C[iterator3] = temp_A[iterator2];
                        iterator3--; iterator2--;
                    }
                }

                for (int i = 0; i < local_n; i++)
                {
                    local_A[i] = temp_C[i];
                    
                }


            }
        }
    }
    

}

int compute_partner(int phase, int my_rank, int p)
{
    int partner;
    if (phase % 2 == 0) {
        if (my_rank % 2 != 0) {
            partner = my_rank - 1;
        } 
        else {
            partner = my_rank + 1;
        }
    }
    else {
        if (my_rank % 2 != 0) {
            partner = my_rank + 1;
        }
        else {
            partner = my_rank - 1;
        }
    }
    if (partner == -1 || partner == p)
        partner = MPI_PROC_NULL;

    return partner;
}


int main(void)
{
    srand(time(NULL));
    int comm_sz;
    int my_rank;
    MPI_Comm comm;
	double start, finish, loc_elapsed, elapsed;
    
    int* local_A;
    int* A;
    int local_n, n=3200000;
    

    A = new int[n];
   
    
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    local_n = n / comm_sz;

    local_A = new int[local_n];

    for (int j = 0; j < n; j++)
    {
        A[j] = rand() % 100 + 1.0;
    }
        
    MPI_Scatter(A, local_n, MPI_INT, local_A, local_n, MPI_INT, 0, comm);

    /*for (int i = 0; i < local_n; i++)
        std::cout << local_A[i] << "  ";

    std::cout << std::endl;*/
    
	MPI_Barrier(comm);
	start = MPI_Wtime();
    ordenamiento(local_A, comm_sz, my_rank, local_n, comm);
    finish = MPI_Wtime();
	loc_elapsed = finish-start;
	MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
    MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0, comm);
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
    for (int i = 0; i < n; i++)
        std::cout << A[i] << "  ";

    std::cout << std::endl;*/
    
    delete A;
    
    delete local_A;

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