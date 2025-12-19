#include "./definition.h"

typedef struct Node{
  Facility* address;
  struct Node* right;
  struct Node* left;
  int balanced;
} Node;

Node* makeNodeAVL(Facility* address);

int hasLeftChild(Node* node);
int hasRightChild(Node* node);
void deleteAllChilds(Node** node);
void deleteLeftChild(Node** node);
void deleteRightChild(Node** node);

Node* rotateTreeLeft(Node** node);
Node* rotateTreeRight(Node** node);
Node* doubleRotateTreeLeft(Node** node);
Node* doubleRotateTreeRight(Node** node);

Node* balanceAVL(Node** node);

Node* addChildAVL(Node** node, Facility* address, int* h);
Node* deleteMin(Node** tree, char* nodeToDelete, int* h);
Node* deleteElementAVL(Node** node, Facility* address, int* h);

void printTree(Node *root, int space);
