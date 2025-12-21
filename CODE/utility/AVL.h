#include "./definition.h"

typedef struct Node{
  Instance* address;
  struct Node* right;
  struct Node* left;
  int balanced;
} Node;

Node* makeNodeAVL(Instance* address);

int hasLeftChild(Node* node);
int hasRightChild(Node* node);
void deleteAllChilds(Node** node);
void AVLToList(Node* root, Instance** list, int* index);

Node* searchAVL(Node* node, char* id);

Node* rotateTreeLeft(Node** node);
Node* rotateTreeRight(Node** node);
Node* doubleRotateTreeLeft(Node** node);
Node* doubleRotateTreeRight(Node** node);

Node* balanceAVL(Node** node);

Node* addChildAVL(Node** node, Instance* address, int* h);
Node* deleteMin(Node** tree, char* nodeToDelete, int* h);
Node* deleteElementAVL(Node** node, Instance* address, int* h);

void printTree(Node *root, int space);
