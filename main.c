#include<stdio.h>

#define N 255

int main()  {
    int matrix[N][N];
    int a, b;
    int srInStolb[N];
    scanf("%i %i", &a, &b);
    int minInStr[N], maxInStolb[N];
    for (int i = 0; i < 255; i++)  {
        minInStr[i] = 0;
        maxInStolb[i] = 0;
    }
    for (int i = 0; i < b; i++)  {
        for (int j = 0; j < a; j++)  {
            scanf("%i", &matrix[i][j]);
            if (matrix[i][j] < minInStr[i] && matrix[i][j] != 0 || minInStr[i] == 0)  {
                minInStr[i] = matrix[i][j];
            }
            if (matrix[i][j] > maxInStolb[j])  {
                maxInStolb[j] = matrix[i][j];
            }
            srInStolb[j] += matrix[i][j];
        }
    }
    for (int i = 0; i < a; i++)  {
        srInStolb[i] /= b;
    }
    int counter = 0;
    for (int i = 0; i < b; i++)  {
        for (int j = 0; j < a; j++)  {
            if (matrix[i][j] == minInStr[i] && matrix[i][j] == maxInStolb[j])  {
                counter++;
                matrix[i][j] = srInStolb[j];
            }
        }
    }
    printf("%d\n", counter);
    for (int i = 0; i < b; i++)  {
        for (int j = 0; j < a; j++)  {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}