#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct node_ {
    char key[256];
    struct node_ *right;
    struct node_ *left;
} node;

node *findFOfMostLeftNode(node *root) {
    while (root->left) {
        root = root->left;
    }
    return root;
}

node *addNode(node *root, char word[]) {
    if (root == NULL) {
            node *leaf = (node*) malloc(sizeof(node));
            leaf->left = NULL;
            leaf->right = NULL;
            strcpy(leaf->key, word);
            return leaf;
    } else {
        if (strcmp(word, root->key) > 0) {
            root->right = addNode(root->right, word);
        } else if (strcmp(word, root->key) < 0) {
            root->left = addNode(root->left, word);
        }
        return root;
    }
}

node *removeNode(node *root, char word[]) {
    if (root == NULL)
        return NULL;
    if (strcmp(word, root->key) > 0) {
        root->right = removeNode(root->right, word);
    } else if (strcmp(word, root->key) < 0) {
        root->left = removeNode(root->left, word);
    } else {
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        } else if (!root->left && root->right) {
            node *right = root->right;
            free(root);
            return right;
        } else if (root->left && !root->right) {
            node *left = root->left;
            free(root);
            return left;
        } else {
            node *FLeftNode = findFOfMostLeftNode(root->right);
            char temp[256];
            strcpy(temp, root->key);
            strcpy(root->key, FLeftNode->key);
            strcpy(FLeftNode->key, temp);
            root->right = removeNode(root->right, word);
        }
    }
    return root;
}

void removeFromMem(node *root) {
    if (root == NULL)
        return;
    removeFromMem(root->left);
    removeFromMem(root->right);
    free(root);
}

void printNodesFromLvl(node *currentNode, int lvl) {
    if (currentNode == NULL)
        return;
    if (lvl == 0) {
        printf("%s ", currentNode->key);
    } else {
        printNodesFromLvl(currentNode->left, lvl - 1);
        printNodesFromLvl(currentNode->right, lvl - 1);
    }
}

int countNodes(node *root) {
    if (root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int main() {
    char word[256];
    node *mainRoot = NULL;
    scanf("%s", word); //TEXT:
    scanf("%s", word); //scan first word
    while (strcmp(word, "DELETE:") != 0) {
        if (word[strlen(word) - 1] == '.')
            word[strlen(word) - 1] = '\0';
        mainRoot = addNode(mainRoot, word);
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
    printNodesFromLvl(mainRoot, level);
    removeFromMem(mainRoot);
}