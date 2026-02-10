#include <stdio.h>
#include <stdlib.h>
int main(void){ // same as St ring args [ ]

FILE *file;

int m;
int n;


file = fopen("intopt.in", "r");
if (file == NULL) {
    printf("Error opening file\n");
    return 1;
}

fscanf(file, "%d %d", &m, &n);


//ALLOCATION

//C-vector
double *C = calloc(n, sizeof(double));
if(C == NULL){
    printf("Memory allocation failed\n");
    return 1;
}

//A-matrix
double *A = calloc(m * n, sizeof(double));
if(A == NULL){
    printf("Memory allocation failed\n");
    return 1;
}

//B-vector
double *B = calloc(m, sizeof(double));
if(C == NULL){
    printf("Memory allocation failed\n");
    return 1;
}

//READING OF VALUES

//C-vector
for(int i = 0; i < n; i++){
        fscanf(file, "%lf", &C[i]);
    }

//A-matrix
for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
        fscanf(file, "%lf", &A[i * n + j]);
    }
}

//B-vector
for(int i = 0; i < m; i++){
        fscanf(file, "%lf", &B[i]);
    }


//PRINTS


printf("max z = ");
for(int i = 0; i < n; i++){
    printf("% +1.0lf x%1.0lf", C[i], i);
}
printf("\n");


for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
        printf("% + 2.1lfx%1.0lf", A[i * n + j], i);
    }
    printf(" <= %4.2lf", B[i]);
    printf("\n");
}

    return 0;
}
