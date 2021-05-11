#include <stdio.h>
#include <malloc.h>
#include <limits.h>

typedef struct Room_ {
    unsigned int number;
    int len;
    int beOrNotToBe;
    int treasure;
} Room;

void swap(Room *first, Room *second) {
    Room temp = *first;
    *first = *second;
    *second = temp;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    unsigned int P, N, M;
    scanf("%u %u %u", &P, &N, &M);

    int **rooms = (int **) malloc(sizeof(int *) * N);
    Room *roomsWeight = (Room *) malloc(sizeof(Room) * N);

    if (!rooms || !roomsWeight)
        return 1;
    for (int i = 0; i < N; ++i) {
        rooms[i] = (int *) malloc(sizeof(int *) * N);
        if (!rooms[i]) {
            return 1;
        }
        roomsWeight[i].number = i;
        roomsWeight[i].len = INT_MAX;
        roomsWeight[i].beOrNotToBe = 0;
        for (int j = 0; j < N; ++j) {
            rooms[i][j] = -1;
        }
    }
    roomsWeight[0].len = 0;
    for (int i = 0; i < M; ++i) {
        unsigned int from, to;
        int value;
        scanf("%u %u %d", &from, &to, &value);
        rooms[from][to] = value;
    }
    for (int i = 0; i < N; ++i) {
        scanf("%d", &roomsWeight[i].treasure);
    }
    int currentNum = 0;
    for (int j = 0; j < N; ++j) {
        int min = INT_MAX;
        for (int i = 0; i < N; ++i) {
            if (rooms[currentNum][i] > 0 && rooms[currentNum][i] + roomsWeight[currentNum].len < roomsWeight[i].len) {
                roomsWeight[i].len = rooms[currentNum][i] + roomsWeight[currentNum].len;
            }

        }
        roomsWeight[currentNum].beOrNotToBe = 1;
        int n = 0;
        while (2 * n + 2 < N) {
            if (roomsWeight[n].len < INT_MAX) {
                if (roomsWeight[n].len > roomsWeight[2 * n + 1].len) {
                    swap(&roomsWeight[n], &roomsWeight[2 * n + 1]);
                    n = 2 * n + 1;
                    continue;
                } else if (roomsWeight[n].len > roomsWeight[2 * n + 2].len) {
                    swap(&roomsWeight[n], &roomsWeight[2 * n + 2]);
                    n = 2 * n + 2;
                    continue;
                }
            }
            n = 2*n + 1;
        }
        for (int i = 0; i < N; ++i) {
            if (roomsWeight[i].len < min && roomsWeight[i].beOrNotToBe == 0) {
                min = roomsWeight[i].len;
                currentNum = i;
            }
        }
    }

    int maxT = 0;
    for (int i = 0; i < N; ++i) {
        if (maxT < roomsWeight[i].treasure && roomsWeight[i].len <= P)
            maxT = roomsWeight[i].treasure;
    }
    printf("\n%d", maxT);
}
