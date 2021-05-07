#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct AVLNode_ {
    char key[256];
    short balanceF;
    struct AVLNode_ *left;
    struct AVLNode_ *right;
} AVLNode;

AVLNode *simpleRightRotate(AVLNode *MainNode) {
    AVLNode *leftNode = MainNode->left;
    leftNode->balanceF++;
    MainNode->balanceF++;
    MainNode->left = leftNode->right;
    leftNode->right = MainNode;
    return leftNode;
}

AVLNode *simpleLeftRotate(AVLNode *MainNode) {
    AVLNode *rightNode = MainNode->right;
    rightNode->balanceF--;
    MainNode->balanceF--;
    MainNode->right = rightNode->left;
    rightNode->left = MainNode;
    return rightNode;
}

short balance(AVLNode **root, short diff) {
    if ((*root)->balanceF == 1 && diff == 1) {
        if ((*root)->right->balanceF >= 0) {
            (*root) = simpleLeftRotate(*root);
        } else {
            (*root)->right = simpleRightRotate((*root)->right);
            (*root) = simpleLeftRotate(*root);
        }
        return 0;
    } else if ((*root)->balanceF == -1 && diff == -1) {
        if ((*root)->left->balanceF <= 0) {
            (*root) = simpleRightRotate(*root);
        } else {
            (*root)->left = simpleLeftRotate((*root)->left);
            (*root) = simpleRightRotate(*root);
        }
        return 0;
    } else {
        (*root)->balanceF += diff;
        if ((*root)->balanceF == 0)
            return 0;
        return diff;
    }
}

short addNode(AVLNode **root, char word[]) {
    if (!*root) {
        *root = (AVLNode *) malloc(sizeof(AVLNode));
        (*root)->balanceF = 0;
        strcpy((*root)->key, word);
        (*root)->left = NULL;
        (*root)->right = NULL;
        return 2; // 2 - код создания новой ноды, баллансим на дефолтное значение 1 или -1
    } else if (strcmp(word, (*root)->key) > 0) {
        short bf = addNode(&(*root)->right, word);
        if (bf == 2 || bf == -1) {
            return balance(root, 1);
        } else {
            return balance(root, bf);
        }
    } else if (strcmp(word, (*root)->key) < 0) {
        short bf = addNode(&(*root)->left, word);
        if (bf == 2 || bf == 1) {
            return balance(root, -1);
        } else {
            return balance(root, bf);
        }
    }
    return 0;
}

AVLNode *removeNode(AVLNode *root, char word[]) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(word, root->key) > 0) {
        short temp = root->right ? root->right->balanceF : 2;
        root->right = removeNode(root->right, word);
        if (temp == 2)
            return root;
        if (root->right == NULL || (temp == 1 || temp == -1) && root->right->balanceF == 0)
            balance(&root, -1);
        return root;
    } else if (strcmp(word, root->key) < 0) {
        short temp = root->left ? root->left->balanceF : 2;
        root->left = removeNode(root->left, word);
        if (temp == 2)
            return root;
        if (root->left == NULL || ((temp == 1 || temp == -1) && root->left->balanceF == 0))
            balance(&root, 1);
        return root;
    } else {
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        } else if (!root->left && root->right) {
            AVLNode *rightNode = root->right;
            free(root);
            return rightNode;
        } else if (root->left && !root->right) {
            AVLNode *leftNode = root->left;
            free(root);
            return leftNode;
        } else {
            short temp = root->right->balanceF;
            AVLNode *mostLeftInRight = root->right;
            while (mostLeftInRight->left != NULL) {
                mostLeftInRight = mostLeftInRight->left;
            }
            char tempW[256];
            strcpy(tempW, root->key);
            strcpy(root->key, mostLeftInRight->key);
            strcpy(mostLeftInRight->key, tempW);
            root->right = removeNode(root->right, word);
            if (root->right == NULL || ((temp == 1 || temp == -1) && root->right->balanceF == 0))
                balance(&root, -1);
            return root;
        }
    }
}

int countNodes(AVLNode *root) {
    if (!root)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void printFromLvl(AVLNode *root, int lvl) {
    if (!root)
        return;
    if (lvl == 0)
        printf("%s ", root->key);
    else {
        printFromLvl(root->left, lvl - 1);
        printFromLvl(root->right, lvl - 1);
    }
}

void removeFromMem(AVLNode *root) {
    if (root == NULL)
        return;
    removeFromMem(root->left);
    removeFromMem(root->right);
    free(root);
}

int main() {
    char word[256];
    AVLNode *mainRoot = NULL;
    scanf("%s", word); //TEXT:
    scanf("%s", word); //scan first word
    while (strcmp(word, "DELETE:") != 0) {
        if (word[strlen(word) - 1] == '.')
            word[strlen(word) - 1] = '\0';
        addNode(&mainRoot, word);
        scanf("%s", word);
    }
    scanf("%s", word); //scan first removing word
    while (strcmp(word, "LEVEL:") != 0) {
        mainRoot = removeNode(mainRoot, word);
        scanf("%s", word);
    }
    int level;
    scanf("%d", &level); //scan lvl
    printf("%d\n", countNodes(mainRoot));
    printFromLvl(mainRoot, level);
    removeFromMem(mainRoot);
}