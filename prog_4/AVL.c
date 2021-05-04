#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct avl_node_ {
    char key[256];
    short height;
    struct avl_node_ *right;
    struct avl_node_ *left;
} avl_node;

void simpleRightRotation(avl_node **node) {
    (*node)->height++;
    (*node)->left->height++;
    avl_node *temp = *node;
    avl_node *leftNode = (*node)->left;
    *node = leftNode;
    temp->left = leftNode->right;
    leftNode->right = temp;
}

void simpleLeftRotation(avl_node **node) {
    (*node)->height--;
    (*node)->right->height--;
    avl_node *temp = *node;
    avl_node *rightNode = (*node)->right;
    *node = rightNode;
    temp->right = rightNode->left;
    rightNode->left = temp;
}

int softMoreThan(int first, int second) {
    return first >= second;
}

int softLowThan(int first, int second){
    return first <= second;
}
// НЕ РАБОТАЕТ
int balance(avl_node ***root, short diff, int (*comparison)(int, int),
                  void (*firstRotation)(avl_node**), void (*secondRotation)(avl_node**)) {
    int temp;
    if ((**root)->height == diff) {
        temp = ((**root))->height;
        if (comparison((**root)->right->height, 0) == 1) {
            firstRotation(*root);
        } else {
            secondRotation(&((**root)->right));
            firstRotation(*root);
        }
        if (temp >= (**root)->height)
            return 1;
    } else
        (**root)->height += diff;
    return 0;
}

int addAVLNode(avl_node **root, char key[256]) {
    if (*root == NULL) {
        avl_node *newNode = (avl_node *) malloc(sizeof(avl_node));
        newNode->left = NULL;
        newNode->right = NULL;
        strcpy(newNode->key, key);
        newNode->height = 0;
        *root = newNode;
        return 0;
    }
    int k = 0;
    int temp;
    if (strcmp(key, (*root)->key) > 0) {
        if (!(k = addAVLNode(&((*root)->right), key))) {
            if ((*root)->height == 1) {
                temp = (*root)->height;
                if ((*root)->right->height >= 0) {
                    simpleLeftRotation(root);
                } else {
                    simpleRightRotation(&((*root)->right));
                    simpleLeftRotation(root);
                }
                if (temp >= (*root)->height)
                    return 1;
            } else
                (*root)->height++;
        }
    } else if (strcmp(key, (*root)->key) < 0) {
        if (!(k = addAVLNode(&((*root)->left), key))) {
            if ((*root)->height == -1) {
                temp = (*root)->height;
                if ((*root)->left->height <= 0) {
                    simpleRightRotation(root);
                } else {
                    simpleLeftRotation(&((*root)->left));
                    simpleRightRotation(root);
                }
                if (temp <= (*root)->height)
                    return 1;
            } else
                (*root)->height--;
        }
    }
    return k;
}

int removeAVLNode(avl_node *root, char word[256]) {
    if (root == NULL)
        return 0;
    if (strcmp(word, root->key) > 0) {
        root->height--;
        removeAVLNode(root->right, word);
    } else if (strcmp(word, root->key) < 0) {
        root->height++;
        removeAVLNode(root->left, word);
    } else {
        if (root->left == NULL && root->right == NULL) {

        }
    }
}

void removeAVLFromMem(avl_node *root) {
    if (root == NULL)
        return;
    removeAVLFromMem(root->left);
    removeAVLFromMem(root->right);
    free(root);
}

void printAVLNodesFromLvl(avl_node *root, int lvl) {
    if (root == NULL)
        return;
    if (lvl == 0) {
        printf("%s ", root->key);
    } else {
        printAVLNodesFromLvl(root->left, lvl - 1);
        printAVLNodesFromLvl(root->right, lvl - 1);
    }
}

int countAVLNodes(avl_node *root) {
    if (root == NULL)
        return 0;
    return 1 + countAVLNodes(root->left) + countAVLNodes(root->right);
}

void printG(avl_node *root) {
    if (root == NULL) {
        printf("x");
        return;
    } else {
        printf("(%s", root->key);
        printG(root->left);
        printG(root->right);
        printf(")");
    }
}


int main() {
    char word[256];
    avl_node *mainAVLNode = NULL;
    int count;
    scanf("%d", &count);
    for (int i = 0; i < count; ++i) {
        scanf("%s", word);
        addAVLNode(&mainAVLNode, word);
        printG(mainAVLNode);
        printf("\n");
    }

    removeAVLFromMem(mainAVLNode);
}
