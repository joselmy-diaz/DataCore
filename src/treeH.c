#include <stdio.h>
#include <stdlib.h>
#include "./stuctura.h"

// Función para obtener la altura de un nodo
int height(NodeEntry *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Función para obtener el valor máximo entre dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}


Obj* initTR() {
    ObjTree* objT = (ObjTree*)malloc(sizeof(ObjTree));
    objT->obj.type = OBJ_AVL_TREE;
    objT->obj.as.Num = 0;
    objT->root = NULL;
    return &objT->obj;
}

// Rotación derecha
NodeEntry *rightRotate(NodeEntry *y) {
    NodeEntry *x = y->left;
    NodeEntry *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotación izquierda
NodeEntry *leftRotate(NodeEntry *x) {
    NodeEntry *y = x->right;
    NodeEntry *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


// Función para insertar un nodo en el árbol AVL
NodeEntry* insertAVL(NodeEntry* node, Entry data) {
    if (node == NULL)  {
        NodeEntry* newNode = (NodeEntry*)malloc(sizeof(NodeEntry));
        newNode->key = strdup(data.key);
        assignData(&newNode->data, data.data);
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1; // nuevo nodo es inicialmente agregado al árbol
        return newNode;
    }

    int reHash = hash(data.key);
    int hashNode = hash(node->key);
    if (reHash < hashNode)
        node->left = insertAVL(node->left, data);
    else if (reHash > hashNode)
        node->right = insertAVL(node->right, data);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    // Obtener el factor de balance de un nodo
    int balance = height(node->left) - height(node->right);

    // Rotaciones necesarias para balancear el árbol
    // Caso Izquierda Izquierda
    int leftHash = node->left ? hash(node->left->key) : 0;
    int rightHash = node->right ? hash(node->right->key) : 0;
    if (balance > 1 && reHash < leftHash)
        return rightRotate(node);

    // Caso Derecha Derecha
    if (balance < -1 && reHash > rightHash)
        return leftRotate(node);

    // Caso Izquierda Derecha
    if (balance > 1 && node->left && reHash > leftHash) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso Derecha Izquierda
    if (balance < -1 && node->right && reHash < rightHash) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

NodeEntry* searchTreeN(NodeEntry* node, int Hkey){
    if (node == NULL)  return NULL;
    int hashNode = hash(node->key);
    if (Hkey < hashNode) return searchTreeN(node->left, Hkey);
    else if (Hkey > hashNode) return searchTreeN(node->right, Hkey);
    else if (hashNode == Hkey) return node;
    else return NULL;
}

Obj* searchTree(ObjTree* obj, const char *key){
    Obj* emptyData = NULL;
    if (obj == NULL) return emptyData;
    NodeEntry* node = obj->root;
    if (node == NULL) return emptyData;

    int reHash = hash(key);
    NodeEntry* resultNode = searchTreeN(node, reHash);
    if (resultNode == NULL) {
        return emptyData;
    }
    return resultNode->data;
}

// Función para imprimir el árbol en preorden
void preOrder(NodeEntry *root) {
    if (root != NULL) {
        printf("%s ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}


// Función para liberar memoria de los nodos del árbol AVL
void freeNode(NodeEntry* node) {
    if (node == NULL) return;
    freeNode(node->left);
    freeNode(node->right);
    free(node->key);
    freeObjs(node->data);
    free(node);
}

// Función para liberar memoria del árbol AVL
bool freeTR(ObjTree* objT) {
    if (objT ->root == NULL) return true;
    freeNode(objT->root);
    free(objT);
    return true;
}