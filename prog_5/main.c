#include <stdio.h>
#include <malloc.h>
#include <limits.h>

typedef struct ListNode_ {
    unsigned int jumpTo;
    unsigned int jumpValue;
    struct ListNode_ *next;
} ListNode;

typedef struct Room_ {
    unsigned int number; //num elem on heap
    unsigned int lenToThatRoom;
    char beOrNotToBe;
    unsigned int treasure;
    ListNode *head;
    ListNode *end;
} Room;

typedef struct MinHeapNode_ {
    unsigned int ind;
    unsigned int path;
} MinHeapNode;

Room *addNewRoom() {
    Room *newRoom = (Room *) malloc(sizeof(Room));
    if (!newRoom)
        exit(1);
    newRoom->number = -1;
    newRoom->lenToThatRoom = INT_MAX;
    newRoom->beOrNotToBe = 0;
    newRoom->head = NULL;
    newRoom->end = NULL;
    return newRoom;
}


//Быстрее и меньше по памяти, чем свап структур
void swap(MinHeapNode **first, MinHeapNode **second) {
    MinHeapNode *temp = *first;
    *first = *second;
    *second = temp;
}

void addNewJump(Room *roomFrom, unsigned int to, unsigned int value) {
    ListNode *newNode = (ListNode *) malloc(sizeof(ListNode));
    newNode->jumpTo = to;
    newNode->jumpValue = value;
    newNode->next = NULL;
    if (!roomFrom->end && !roomFrom->head) {
        roomFrom->end = newNode;
        roomFrom->head = newNode;
    } else {
        roomFrom->end->next = newNode;
        roomFrom->end = newNode;
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    unsigned int charges, roomsCount, jumpsCount;
    scanf("%u %u %u", &charges, &roomsCount, &jumpsCount);

    MinHeapNode **minHeapOfRooms = (MinHeapNode **) malloc(sizeof(MinHeapNode *) * roomsCount - 1);
//    int *minHeap = (int *) malloc(sizeof(int) * roomsCount - 1);
    Room **rooms = (Room **) malloc(sizeof(Room *) * roomsCount);
    if (!rooms || !minHeapOfRooms)
        exit(1);
    for (int i = 0; i < roomsCount; i++) {
        rooms[i] = addNewRoom();
    }
    rooms[0]->lenToThatRoom = 0;

    for (int i = 0; i < jumpsCount; i++) {
        unsigned int from, to, value;
        scanf("%u %u %u", &from, &to, &value);
        addNewJump(rooms[from], to, value);
    }

    for (int i = 0; i < roomsCount; i++)
        scanf("%u", &rooms[i]->treasure);

    int heapSize = 0;
    unsigned int currentRoomNum = 0;
    for (int i = 0; i < roomsCount; i++) {
        ListNode *currentNode = rooms[currentRoomNum]->head;
        while (currentNode) {
            if (rooms[currentRoomNum]->lenToThatRoom + currentNode->jumpValue <
                rooms[currentNode->jumpTo]->lenToThatRoom) {
                rooms[currentNode->jumpTo]->lenToThatRoom =
                        rooms[currentRoomNum]->lenToThatRoom + currentNode->jumpValue;
                //create elem on heap
                if (rooms[currentNode->jumpTo]->number == -1) {
                    minHeapOfRooms[heapSize] = (MinHeapNode*) malloc(sizeof(MinHeapNode));
                    minHeapOfRooms[heapSize]->ind = currentNode->jumpTo;
                    minHeapOfRooms[heapSize]->path = rooms[currentNode->jumpTo]->lenToThatRoom;
                    rooms[currentNode->jumpTo]->number = heapSize++;
                } else {
                    minHeapOfRooms[rooms[currentNode->jumpTo]->number]->path = rooms[currentNode->jumpTo]->lenToThatRoom;
                }
                //save property of heap
                unsigned int n = rooms[currentNode->jumpTo]->number;
                while (n != 0) {
                    if (minHeapOfRooms[n]->path < minHeapOfRooms[(n - 1) / 2]->path) {
                        unsigned int temp = rooms[minHeapOfRooms[n]->ind]->number;
                        rooms[minHeapOfRooms[n]->ind]->number = rooms[minHeapOfRooms[(n - 1) / 2]->ind]->number;
                        rooms[minHeapOfRooms[(n - 1) / 2]->ind]->number = temp;

                        swap(&minHeapOfRooms[n], &minHeapOfRooms[(n - 1) / 2]);
                        n = (n - 1) / 2;
                    } else
                        break;
                }
            }
            currentNode = currentNode->next;
        }
        if(heapSize == 0)
            break;
        currentRoomNum = minHeapOfRooms[0]->ind;
        free(minHeapOfRooms[0]);
        minHeapOfRooms[0] = minHeapOfRooms[--heapSize];
        int n = 0;
        while (1) {
            int minLeft = 2*n + 1;
            int minRight = 2*n + 2;
            int current = n;

            if (minLeft < heapSize && minHeapOfRooms[minLeft]->path < minHeapOfRooms[current]->path)
                current = minLeft;
            if (minRight < heapSize && minHeapOfRooms[minRight]->path < minHeapOfRooms[current]->path)
                current = minRight;
            if (n == current)
                break;
            else {
                unsigned int temp = minHeapOfRooms[current]->ind;
                minHeapOfRooms[current]->ind = minHeapOfRooms[n]->ind;
                minHeapOfRooms[n]->ind = temp;
                swap(&minHeapOfRooms[current], &minHeapOfRooms[n]);
                n = current;
            }
        }
    }

    unsigned int maxT = 0;
    for (int i = 0; i < roomsCount; ++i) {
        if (maxT < rooms[i]->treasure && rooms[i]->lenToThatRoom <= charges)
            maxT = rooms[i]->treasure;
    }
    printf("%u", maxT);
}
