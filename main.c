#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SIZE 64

unsigned long long level;

typedef struct MatrixInfo_ {
    int number;
    long long determinant;
    int size;
} MatrixInfo;

void swap(MatrixInfo *first, MatrixInfo *second) {
    MatrixInfo temp = *first;
    *first = *second;
    *second = temp;
}

void print_matrix(MatrixInfo *array, int matrix[][MAX_SIZE][MAX_SIZE], int n, int size_b) {
    for (int i = 0; i < n; ++i) {
        if (size_b)
            printf("%d\n", array[i].size);
        for (int j = 0; j < array[i].size; ++j) {
            for (int k = 0; k < array[i].size; ++k) {
                printf("%d ", matrix[array[i].number][j][k]);
            }
            printf("\n");
        }
    }
}

void bubble_sort(MatrixInfo *a, int null, int size) {
    int b = 1;
    while (b) {
        b = 0;
        for (int i = 0; i <= size; ++i) {
            if (a[i].determinant > a[i + 1].determinant) {
                swap(&a[i], &a[i + 1]);
                b = 1;
            }
        }
    }
}

void quick_sort(MatrixInfo *a, int left, int right) {
    if (left >= right)
        return;
    int wall = left;
    int pivot_ind = rand() % (right + 1 - left) + left;
    swap(&a[pivot_ind], &a[right]);
    long long pivot = a[right].determinant;
    for (int j = left; j <= right; ++j) {
        if (pivot > a[j].determinant)
            swap(&a[j], &a[wall++]);
    }
    swap(&a[right], &a[wall]);
    quick_sort(a, left, wall - 1);
    quick_sort(a, wall + 1, right);
}

long long determinant(int matrix[][MAX_SIZE], int lvl, int size) {
    long long result = 0;
    int temp = 0;
    for (int i = 0; i < size; ++i) {
        if (!(level & (0x1 << i))) {
            if (lvl < size - 1) {
                level |= 0x1 << i;
                result += ((i + temp) % 2 ? -1 : 1) * matrix[lvl][i] * determinant(matrix, lvl + 1, size);
                level &= ~(0x1 << i);
            } else {
                return matrix[lvl][i];
            }
        } else {
            temp++;
        }
    }
    return result;
}

long time_of_algorithm(int matrix[][MAX_SIZE][MAX_SIZE], int count_of_matrix, void (*sort)(MatrixInfo *, int, int),
                       MatrixInfo *matrixInfo) {
    clock_t start = clock(), diff;
    for (int j = 0; j < count_of_matrix; ++j) {
        matrixInfo[j].determinant = determinant(matrix[j], 0, matrixInfo[j].size);
    }
    sort(matrixInfo, 0, count_of_matrix - 1);
    diff = clock() - start;
    return diff * 1000 / CLOCKS_PER_SEC;
}

void test(int matrix[][MAX_SIZE][MAX_SIZE], int count_of_tests, void (*sort)(MatrixInfo *, int, int),
          char *name_of_sort, int count_of_matrix, MatrixInfo *matrixInfo) {
    long *diff_time = (long *) malloc(sizeof(long) * count_of_tests);
    if (diff_time == NULL)
        exit(1);
    freopen("stat.txt", "a", stdout);
    double sr = 0;
    double srKvOt = 0;
    long best_time = -1, worst_time = 0;
    for (int i = 0; i < count_of_tests; ++i) {
        diff_time[i] = time_of_algorithm(matrix, count_of_matrix, sort, matrixInfo);
        if (diff_time[i] > worst_time)
            worst_time = diff_time[i];
        if (diff_time[i] < best_time || best_time == -1)
            best_time = diff_time[i];
        sr += (double) diff_time[i];
    }
    sr /= count_of_tests;
    for (int i = 0; i < count_of_tests; ++i)
        srKvOt += pow(diff_time[i] - sr, 2);
    srKvOt = sqrt(srKvOt / count_of_matrix);
    printf("%s\n", name_of_sort);
    printf("Среднее: %lf миллисекунд\nСреднее квадратичное(стандартное) отклонение: %lf миллисекунд\n", sr, srKvOt);
    printf("Худшее время запуска: %ld\nЛучшее время запуска: %ld\n\n", worst_time, best_time);
    free(diff_time);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    srand(time(NULL));
    int matrix[100][MAX_SIZE][MAX_SIZE] = {}, n = 0, len, step = 1;
    char mode[5] = {};
    gets(mode);
    len = (int) strlen(mode);
    //Режим "тестов" активируется + через пробел после количества матриц -> 'N +'
    int b = mode[len - 1] == '+';
    for (int i = len - 1; i >= 0; --i) {
        if (isdigit(mode[i])) {
            n += (mode[i] - '0') * step;
            step *= 10;
        }
    }
    MatrixInfo *array = (MatrixInfo *) malloc(n * sizeof(MatrixInfo));
    //проверка на NULL
    if (array == NULL)
        return 1;
    for (int i = 0; i < n; ++i)
        array[i].number = i;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &array[i].size);
        for (int j = 0; j < array[i].size; ++j) {
            for (int k = 0; k < array[i].size; ++k)
                scanf("%d", &(matrix[i][j][k]));
        }
        array[i].determinant = determinant(matrix[i], 0, array[i].size);
    }
    quick_sort(array, 0, n - 1);
    print_matrix(array, matrix, n, 0);
    //Доп. задание
    if (b) {
        int tests; //<=1000
        long worst = 0, best = -1;
        scanf("%d", &tests);
        freopen("stat.txt", "w", stdout);
        printf("Стандартные значения:\n\n");
        test(matrix, tests, &quick_sort, "Быстрая сортировка\0", n, array);
        printf("\nСлучайные значения:\n\nХудший запуск записан в worst.txt\nЛучший запуск записан в best.txt");

        for (int c = 0; c < 5; ++c) {
            short size_of_matrix = rand() % 24 + 40; // 40 - 64
            if (size_of_matrix > n) {
                //Вроде бы есть realloc, но вроде бы здесь он будет дольше исполнятся, с учетом того, что старые данные нам уже не нужны
                free(array);
                array = (MatrixInfo*) malloc(size_of_matrix * sizeof(MatrixInfo));
                if (array == NULL)
                    exit(1);
            }
            n = size_of_matrix;
            for (int i = 0; i < n; ++i) {
                array[i].size = rand() % 8 + 1;
                array[i].number = i;
                for (int j = 0; j < array[i].size; ++j) {
                    for (int k = 0; k < array[i].size; ++k)
                        matrix[i][j][k] = rand() % 1000;
                }
            }
            long temp = time_of_algorithm(matrix, n, &quick_sort, array);
            if (temp > worst) {
                worst = temp;
                freopen("worst.txt", "w", stdout);
                printf("Время запуска: %ld\n%d\n", temp, n);
                print_matrix(array, matrix, n, 1);
            }
            if (temp < best || best == -1) {
                best = temp;
                freopen("best.txt", "w", stdout);
                printf("Время запуска: %ld\n%d\n", temp, n);
                print_matrix(array, matrix, n, 1);
            }
        }
    }
    free(array);
}

