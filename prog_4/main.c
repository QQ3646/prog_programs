#include <stdio.h>
#include <malloc.h>
#include <string.h>

//Start of default BST struct and func

typedef struct node_ {
    char key[256];
    struct node_ *right;
    struct node_ *left;
} node;

//node *findGrandpaOfMostLeftNodeInRightSide(node *currentNode) {
////    currentNode = &(*currentNode)->right;
////    node *olderNode = NULL;
////    while ((*currentNode)->left != NULL) {
////        currentNode = &(*currentNode)->left;
////    }
////    return currentNode;
//    node *olderNode = currentNode;
//    while (currentNode->left != NULL) {
//        olderNode = currentNode;
//        currentNode = currentNode->left;
//    }
//    return olderNode;
//}

node **findMostLeftNodeInRightSide(node **currentNode) {
    currentNode = &(*currentNode)->right;
    while ((*currentNode)->left != NULL) {
        currentNode = &(*currentNode)->left;
    }
    return currentNode;
}

void addToTree(node **leaf1, node **root) {
    if (*root == NULL) {
        *root = (node *) malloc(sizeof(node));
        if (*root == NULL)
            exit(1);
        **root = **leaf1;
    } else {
        if (strcmp((*leaf1)->key, (*root)->key) > 0) {
            addToTree(leaf1, &(*root)->right);
        } else if (strcmp((*leaf1)->key, (*root)->key) < 0) {
            addToTree(leaf1, &(*root)->left);
        }
    }
}

int removeNode(char word[256], node *root) {
    if (root == NULL)
        return 0;
    if (strcmp(root->key, word) > 0) {
        if (removeNode(word, root->left)) {
            free(root->left);
            root->left = NULL;
        }
    } else if (strcmp(root->key, word) < 0) {
        if (removeNode(word, root->right)) {
            free(root->right);
            root->right = NULL;
        }
    } else {
        if (root->right == NULL && root->left != NULL) {
            node temp = *(root->left);
            free(root->left);
            *root = temp;
        } else if (root->right != NULL && root->left == NULL) {
            node temp = *(root->right);
            free(root->right);
            *root = temp;
        } else if (root->right == NULL && root->left == NULL) {
            return 1;
        } else {
            node **temp = findMostLeftNodeInRightSide(&root);
            strcpy(root->key, (*temp)->key);
            if ((*temp)->right == NULL) {
                free(*temp);
                *temp = NULL;
            } else {
                node tempp = *(*temp)->right;
                free((*temp)->right);
                **temp = tempp;
            }
        }
    }
    return 0;
}
//Вторая версия, которая не работает :c

//void deleteNode(node *grandpa, node *root, char value[256]) {
//    if (root == NULL)
//        return;
//    if (strcmp(root->key, value) > 0)
//        deleteNode(root, root->left, value);
//    else if (strcmp(root->key, value) < 0)
//        deleteNode(root, root->right, value);
//    else {
//        if (root->left == NULL && root->right == NULL) {
//            if (isLower(value, grandpa->key))
//                grandpa->left = NULL;
//            else
//                grandpa->right = NULL;
//            free(root);
//        } else if (root->left == NULL && root->right != NULL) {
//            if (isLower(value, grandpa->key))
//                grandpa->left = root->right;
//            else
//                grandpa->right = root->right;
//            free(root);
//        } else if (root->left != NULL && root->right == NULL) {
//            if (isLower(value, grandpa->key))
//                grandpa->left = root->left;
//            else
//                grandpa->right = root->left;
//            free(root);
//        } else {
//            node *mostLeft = root->right;
//            while (mostLeft->left != NULL && mostLeft->left->left != NULL)
//                mostLeft = mostLeft->left;
//            if (mostLeft->left == NULL) {
//                strcpy(root->key, mostLeft->key);
//                free(mostLeft);
//                root->left = NULL;
//            } else {
//                if (mostLeft->left->right != NULL) {
//                    strcpy(root->key, mostLeft->left->key);
//                    node *temp = mostLeft->left->right;
//                    free(mostLeft->left);
//                    mostLeft->left = temp;
//                } else {
//                    strcpy(root->key, mostLeft->left->key);
//                    free(mostLeft->left);
//                    mostLeft->left = NULL;
//                }
//            }
//        }
//    }
//}

//End of default BST struct and func

//Start of AVLTree struct and func

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

int addAVLNode(avl_node **root, char key[256]) {
    int temp = 0;
    int k = 0;
    if (*root == NULL) {
        avl_node *newNode = (avl_node *) malloc(sizeof(avl_node));
        newNode->left = NULL;
        newNode->right = NULL;
        strcpy(newNode->key, key);
        newNode->height = 0;
        *root = newNode;
        return 0;
    }
    if (strcmp(key, (*root)->key) > 0) {
        if(!(k = addAVLNode(&((*root)->right), key))) {
            temp = (*root)->height;
            if ((*root)->height == 1) {
                if ((*root)->right->height >= 0) {
                    simpleLeftRotation(root);
                } else {
                    simpleRightRotation(&((*root)->right));
                    simpleLeftRotation(root);
                }
                if (temp >= (*root)->height)
                    return 1;
            } else {
                (*root)->height++;
            }
        }
    } else if (strcmp(key, (*root)->key) < 0) {
        if(!(k = addAVLNode(&((*root)->left), key))) {
            temp = (*root)->height;
            if ((*root)->height == -1) {
                if ((*root)->left->height <= 0) {
                    simpleRightRotation(root);
                } else {
                    simpleLeftRotation(&((*root)->left));
                    simpleRightRotation(root);
                }
                if(temp <= (*root)->height)
                    return 1;
            } else {
                (*root)->height--;
            }
        }
    }
    return k;
}

//End of AVLTree struct and func

//
void removeFromMem(node *root) {
    if (root == NULL)
        return;
    else {
        if (root->left != NULL)
            removeFromMem(root->left);
        if (root->right != NULL)
            removeFromMem(root->right);
        free(root);
    }
}

void printNodesFromLvl(node *currentNode, int lvl) {
    if (currentNode == NULL)
        return;
    if (lvl == 0) {
        printf("%s ", currentNode->key);
    } else {
        if (currentNode->left != NULL)
            printNodesFromLvl(currentNode->left, lvl - 1);
        if (currentNode->right != NULL)
            printNodesFromLvl(currentNode->right, lvl - 1);
    }
}

int countNodes(node *root) {
    if (root == NULL)
        return 0;
    int sum = 1;
    if (root->left != NULL)
        sum += countNodes(root->left);
    if (root->right != NULL)
        sum += countNodes(root->right);
    return sum;
}

int main() {
    //Default BST
//    node *mainRoot = NULL;
    char word[256];
//    scanf("%s", word); //TEXT:
//    scanf("%s", word); //scan first word
//    while (strcmp(word, "DELETE:") != 0) {
//        if (word[strlen(word) - 1] == '.')
//            word[strlen(word) - 1] = '\0';
//        node *anotherNode = (node *) malloc(sizeof(node));
//        strcpy(anotherNode->key, word);
//        anotherNode->right = NULL;
//        anotherNode->left = NULL;
//        addToTree(&anotherNode, &mainRoot);
//        scanf("%s", word);
//    }
//    scanf("%s", word); //scan first removing word
//    while (strcmp(word, "LEVEL:") != 0) {
//        removeNode(word, mainRoot);
//        scanf("%s", word);
//    }
//    int level;
//    scanf("%d", &level); //scan lvl
//    printf("%d\n", countNodes(mainRoot));
//    printNodesFromLvl(mainRoot, level);
//    removeFromMem(mainRoot);
    //Default BST
    avl_node *mainAVLNode = NULL;
    int count;
    scanf("%d", &count);
    for (int i = 0; i < count; ++i) {
        scanf("%s", word);
        addAVLNode(&mainAVLNode, word);
        
    }
    }
