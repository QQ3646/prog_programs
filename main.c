#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define IND 1


void swap(int first[3], int second[3]) {
    for (int i = 0; i < 3; ++i) {
        int temp = first[i];
        first[i] = second[i];
        second[i] = temp;
    }
}

void bubble_sort(int a[][3], int null, int size) {
    int b = 1;
    while (b == 1) {
        b = 0;
        for (int i = 0; i < size; ++i)
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                b = 1;
            }
    }
}

void quick_sort(int a[][3], int left, int right) {
    if (left >= right)
        return;
    int wall = left;
    int pivot = a[right][IND];
    for (int j = left; j <= right; ++j) {
        if (pivot > a[j][IND])
            swap(a[j], a[wall++]);
    }
    swap(a[right], a[wall]);
    quick_sort(a, left, wall - 1);
    quick_sort(a, wall + 1, right);
}

//O(n^3)
//Есть алгоритм лучше!!!!!!!!
int det(int size, int (*matrix)[64]) {
    if (size == 1)
        return matrix[0][0];
    else if (size == 2)
        return matrix[0][0] * matrix[1][1] - (matrix[0][1] * matrix[1][0]);
    else {
        int sum = 0;
        //Взятие минора 0-ого элемента i-ого столбца
        for (int i = 0; i < size; ++i) {
            int cofactor[64][64] = {};
            int counter = 0;
            //Вычисление алгебр. дополнения
            for (int j = 0; j < size; ++j) {
                if (i != j) {
                    for (int k = 1; k < size; ++k)
                        cofactor[k - 1][counter] = matrix[k][j];
                    counter++;
                }
            }
            sum += (i % 2 == 0 ? 1 : -1) *
                   matrix[0][i] * det(size - 1, cofactor);
        }
        return sum;
    }
}

void test(int matrix[100][64][64], int count_of_tests, void (*sort)(int[][3], int, int), char *name_of_sort,
          int count_of_matrix, int sorted_array[][3]) {
    unsigned long diff_time[1000] = {};
    for (int i = 0; i < count_of_tests; ++i) {
        clock_t start = clock(), diff;
        for (int j = 0; j < count_of_matrix; ++j) {
            sorted_array[j][IND] = det(sorted_array[j][2], matrix[j]);
        }
        sort(sorted_array, 0, count_of_matrix - 1);
        diff = clock() - start;
        diff_time[i] = diff * 1000 / CLOCKS_PER_SEC;
    }
    double srKvOt = 0;
    double sr = 0;
    for (int i = 0; i < count_of_tests; ++i) {
        sr += (double) diff_time[i];
    }
    sr /= count_of_tests;
    for (int i = 0; i < count_of_tests; ++i) {
        srKvOt += pow(diff_time[i] - sr, 2);
    }
    srKvOt = sqrt(srKvOt / count_of_matrix);
    printf("%s\n", name_of_sort);
    printf("Среднее: %lf миллисекунд\nСреднее квадратичное(стандартное) отклонение: %lf миллисекунд\n", sr, srKvOt);
    //Сюда бы сортировку, а не костыль
    unsigned long min = -1, max = 0;
    for (int i = 0; i < count_of_tests; ++i) {
        if (diff_time[i] > max)
            max = diff_time[i];
        if (diff_time[i] < min || min == -1)
            min = diff_time[i];
    }
    printf("Худшее время запуска: %lu\nЛучшее время запуска: %lu\n\n", max, min);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int matrix[100][64][64] = {};
    int n = 0;
    char mode[102] = {};
    gets(mode);
    unsigned int len = strlen(mode);
    //Режим "тестов" активируется + через пробел после количества матриц -> 'N +'
    int b = mode[len - 1] == '+';
    int step = 1;
    for (int i = len - 1; i >= 0; --i)
        if (isdigit(mode[i])) {
            n += (mode[i] - '0') * step;
            step *= 10;
        }
    int standard_arr[100][3] = {};  // 0 - изнач. номер
    // 1 - детерменант
    // 2 - размер
    for (int i = 0; i < n; ++i)
        standard_arr[i][0] = i;
    for (int i = 0; i < n; ++i) {
        int size;
        scanf("%d", &size);
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                scanf("%d", &(matrix[i][j][k]));
        standard_arr[i][1] = det(size, matrix[i]);
        standard_arr[i][2] = size;
    }
    quick_sort(standard_arr, 0, n - 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < standard_arr[i][2]; ++j) {
            for (int k = 0; k < standard_arr[i][2]; ++k)
                printf("%d ", matrix[standard_arr[i][0]][j][k]);
            printf("\n");
        }
    }
    //Доп. задание
    if (b) {
        int tests; //<=1000
        scanf("%d", &tests);
        freopen("stat.txt", "w", stdout);
        printf("Стандартные значения:\n\n");
        test(matrix, tests, &quick_sort, "Быстрая сортировка", n, standard_arr);
        test(matrix, tests, &bubble_sort, "Сортировка пузырьком", n, standard_arr);
        printf("\nСлучайные значения:\n\n");
        srand(time(NULL));
        n = rand() % 15 + 40; // 40 - 64
        for (int i = 0; i < n; ++i) {
            int size = rand() % 15 + 1;
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < size; ++k)
                    matrix[i][j][k] = rand() % 1000000;
        }
        test(matrix, tests, &quick_sort, "Быстрая сортировка", n, standard_arr);
        test(matrix, tests, &bubble_sort, "Сортировка пузырьком", n, standard_arr);
    }
}

