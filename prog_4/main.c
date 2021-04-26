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
        if(*root == NULL)
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

avl_node *simpleRightRotation(avl_node *node) {

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
    int sum = 1;
    if (root->left != NULL)
        sum += countNodes(root->left);
    if (root->right != NULL)
        sum += countNodes(root->right);
    return sum;
}

int main() {
    //Default BST
    node *mainRoot = NULL;
    char word[256];
    scanf("%s", word); //TEXT:
    scanf("%s", word); //scan first word
    while (strcmp(word, "DELETE:")) {
        if (word[strlen(word) - 1] == '.')
            word[strlen(word) - 1] = '\0';
        node *anotherNode = (node *) malloc(sizeof(node));
        strcpy(anotherNode->key, word);
        anotherNode->right = NULL;
        anotherNode->left = NULL;
        addToTree(&anotherNode, &mainRoot);
        scanf("%s", word);
    }
    scanf("%s", word); //scan first removing word
    while (strcmp(word, "LEVEL:")) {
        removeNode(word, mainRoot);
        scanf("%s", word);
    }
    int level;
    scanf("%d", &level); //scan lvl
    printf("%d\n", countNodes(mainRoot));
    printNodesFromLvl(mainRoot, level);
    removeFromMem(mainRoot);
    //Default BST
}
