#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>


//Из массива b копируем все в массив a
void get_copy(int a[][3], int b[][3], int size){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < 3; ++j) {
            a[i][j] = b[i][j];
        }
    }
}

void swap(int first[3], int second[3]) {
    for (int i = 0; i < 3; ++i) {
        int temp = first[i];
        first[i] = second[i];
        second[i] = temp;
    }
}

void quick_sort(int a[][3], int left, int right) {
    if (left >= right)
        return;
    int wall = left;
    int pivot = a[right][1];
    for (int j = left; j <= right; ++j) {
        if (pivot > a[j][1])
            swap(a[j], a[wall++]);
    }
    swap(a[right], a[wall]);
    quick_sort(a, left, wall - 1);
    quick_sort(a, wall + 1, right);
}

//O(n^3)
int det(int size, int (*matrix)[64]) {
    if (size == 1) {
        return matrix[0][0];
    } else if (size == 2) {
        return matrix[0][0] * matrix[1][1] - (matrix[0][1] * matrix[1][0]);
    } else {
        int sum = 0;
        //Взятие минора 0-ого элемента i-ого столбца
        for (int i = 0; i < size; ++i) {
            int cofactor[64][64] = {};
            int counter = 0;
            //Вычисление алгебр. дополнения
            for (int j = 0; j < size; ++j) {
                if (i != j) {
                    for (int k = 1; k < size; ++k) {
                        cofactor[k - 1][counter] = matrix[k][j];
                    }
                    counter++;
                }
            }
            sum += (i % 2 == 0 ? 1 : -1) *
                   matrix[0][i] * det(size - 1, cofactor);
        }
        return sum;
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int matrix[100][64][64] = {};
    int n = 0;
    char mode[102] = {};
    gets(mode);
    unsigned int len = strlen(mode);
    int b = mode[len - 1] == '+';
    int step = 1;
        for (int i = len - 1; i >= 0; --i) {
        if (isdigit(mode[i])) {
            n += (mode[i] - '0') * step;
            step *= 10;
        }
    }
    int standard_arr[100][3] = {};  // 0 - изнач. номер
                                    // 1 - детерменант
                                    // 2 - размер
    for (int i = 0; i < n; ++i) {
        standard_arr[i][0] = i;
    }
    for (int i = 0; i < n; ++i) {
        int size;
        scanf("%d", &size);
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                scanf("%d", &(matrix[i][j][k]));
            }
        }
        standard_arr[i][1] = det(size, matrix[i]);
        standard_arr[i][2] = size;
    }
    int copy_arr[100][3];
    get_copy(copy_arr, standard_arr, n);
    quick_sort(copy_arr, 0, n - 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < copy_arr[i][2]; ++j) {
            for (int k = 0; k < copy_arr[i][2]; ++k) {
                printf("%d ", matrix[copy_arr[i][0]][j][k]);
            }
            printf("\n");
        }
    }
    //Доп. задание
    if (b) {
        int tests; //<=1000
        scanf("%d", &tests);
        unsigned long diff_time[1000] = {};
        for (int i = 0; i < tests; ++i) {
            clock_t start = clock(), diff;
            for (int i = 0; i < n; ++i) {
                standard_arr[i][1] = det(standard_arr[i][2], matrix[i]);
            }
            quick_sort(copy_arr, 0, n - 1);
            diff = clock() - start;
            diff_time[i] = diff * 1000 / CLOCKS_PER_SEC;
//            printf("The program took %" PRIu64 " seconds to execute\n", msec);
        }
        double srKvOt = 0;
        double sr = 0;
        for (int i = 0; i < tests; ++i) {
            sr += (double) diff_time[i];
        }
        sr /= tests;
        for (int i = 0; i < tests; ++i) {
            srKvOt += pow(diff_time[i] - sr, 2);
        }
        srKvOt = sqrt(srKvOt / n);
        freopen("stat.txt", "w", stdout);
        printf("Среднее: %lf миллисекунд\nСреднее квадратичное(стандартное) отклонение: %lf", sr, srKvOt);
    }
    int a;
    scanf("%d", &a);
//    getchar();
}

