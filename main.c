#include<stdio.h>

int main() {
    int matrix[255][255];
    int a, b;
    int srInStolb[255];
    scanf("%i %i", &a, &b);
    int minInStr[255], maxInStolb[255];
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < a; j++) {
            scanf("%i", &matrix[i][j]);
            if (matrix[i][j] < minInStr[i] && matrix[i][j] != 0 || minInStr[i] == 0) {
                minInStr[i] = matrix[i][j];
            }
            if (matrix[i][j] > maxInStolb[j]) {
                maxInStolb[j] = matrix[i][j];
            }
            srInStolb[j] += matrix[i][j];
        }
    }
    for(int i = 0; i < a; i++){
        srInStolb[i] /= b;
    }
    int counter = 0;
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            //matrix[j][i] индексы поменялись местами
            if(matrix[j][i] == minInStr[j] && matrix[j][i] == maxInStolb[i]){
                counter+= 1;
                matrix[j][i] = srInStolb[i];
            }
        }
    }
    printf("%d\n", counter);
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < a; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}