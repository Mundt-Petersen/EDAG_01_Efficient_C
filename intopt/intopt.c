#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv){ // same as St ring args [ ]

FILE *file;
int rows, cols;
int i;
int j;

file = fopen("intopt.in", "r");
if (file == NULL) {
    printf("Error opening file\n");
    return 1;
}



fscanf(file, "%d %d", &rows, &cols);

double *matrix = calloc(rows * cols, sizeof(double));
if(matrix == NULL){
    printf("Memory allocation failed\n");
    return 1;
}



//läs in värden
for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
        fscanf(file, "%lf", &matrix[i * cols + j]);
    }
}


for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
        printf("%10.3lf", matrix[i * cols + j]);
        printf("\n");
    }
}

return 0;
}
