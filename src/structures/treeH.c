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
    if (objT == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    objT->obj.type = TYPE_AVL_TREE;
    objT->obj.reference = 0;
    objT->size = 0;
    objT->root = NULL;
    return (Obj*)objT;
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

NodeEntry* insertAVLH(NodeEntry* node, Entry* data, int reHash) {
    if (node == NULL) {
        NodeEntry* newNode = (NodeEntry*)malloc(sizeof(NodeEntry));
        newNode->data.obj.type = TYPE_ENTRY;
        assignData((Obj**)&newNode->data.data, data->data);
        newNode->data.key = strdup(data->key);
        if (newNode->data.key == NULL) fprintf(stderr, "Memory allocation for key failed\n");
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1; // nuevo nodo es inicialmente agregado al árbol
        return newNode;
    }

    int hashNode = hash(node->data.key);
    if (reHash < hashNode)
        node->left = insertAVLH(node->left, data, reHash);
    else if (reHash > hashNode)
        node->right = insertAVLH(node->right, data, reHash);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    // Obtener el factor de balance de un nodo
    int balance = height(node->left) - height(node->right);

    // Rotaciones necesarias para balancear el árbol
    // Caso Izquierda Izquierda
    int leftHash = node->left ? hash(node->left->data.key) : 0;
    int rightHash = node->right ? hash(node->right->data.key) : 0;
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

// Función para insertar un nodo en el árbol AVL
bool insertAVL(Obj* obj, Obj* entry) {
    Entry* data = (Entry*)entry;
    ObjTree* objT = (ObjTree*)obj;
    int reHash = hash(data->key);
    objT->root = insertAVLH(objT->root, data, reHash);
    objT->size++;
    return true;
}

NodeEntry* searchTreeN(NodeEntry* node, int Hkey){
    if (node == NULL)  return NULL;
    int hashNode = hash(node->data.key);
    if (Hkey < hashNode) return searchTreeN(node->left, Hkey);
    else if (Hkey > hashNode) return searchTreeN(node->right, Hkey);
    else if (hashNode == Hkey) return node;
    else return NULL;
}

Obj* searchTree(Obj* obj, const char *key){
    Obj* emptyData = NULL;
    if (obj == NULL) return emptyData;
    ObjTree* objT = (ObjTree*)obj;
    NodeEntry* node = objT->root;
    if (node == NULL) return emptyData;

    int reHash = hash(key);
    NodeEntry* resultNode = searchTreeN(node, reHash);
    if (resultNode == NULL) {
        return emptyData;
    }
    return resultNode->data.data;
}

int getTreeSize (Obj* obj) {
    if (obj == NULL) return 0;
    ObjTree* objT = (ObjTree*)obj;
    return objT->size;
}

// Función para imprimir el árbol en preorden
void preOrder(NodeEntry *root) {
    if (root != NULL) {
        printObjf((Obj*)&root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}


// Función para liberar memoria de los nodos del árbol AVL
void freeNode(NodeEntry* node) {
    if (node == NULL) return;
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

// Función para liberar memoria del árbol AVL
bool freeTR(Obj* obj) {
    ObjTree* objT = (ObjTree*)obj;
    if (objT->root != NULL) {
        freeNode(objT->root);
    }
    free(objT);
    objT = NULL;
    return true;
}