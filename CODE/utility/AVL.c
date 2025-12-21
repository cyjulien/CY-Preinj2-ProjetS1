#include "../main.h"
#include "./AVL.h"
#include "./utility.h"

Node* makeNodeAVL(Instance* address) {
  Node* node = NULL;
  node = malloc(sizeof(Node));
  if (node == NULL) exit(1);
  node->right = NULL;
  node->left = NULL;
  node->address = address;
  node->balanced = 0;
  return node;
}

int hasLeftChild(Node* node) {
  if (node == NULL) return 0;
  return node->left != NULL;
}
int hasRightChild(Node* node) {
  if (node == NULL) return 0;
  return node->right != NULL;
}
void deleteAllChilds(Node** node) {
  if (node == NULL || (*node) == NULL) return;
  deleteAllChilds(&(*node)->left);
  deleteAllChilds(&(*node)->right);
  if ((*node)->address){
    freeInstance((*node)->address);
    (*node)->address = NULL;
  }
  free(*node);
  *node = NULL;
}
void AVLToList(Node* root, Instance** list, int* index) {
  if (root == NULL) return;

  AVLToList(root->left, list, index);

  list[*index] = root->address;
  (*index)++;

  AVLToList(root->right, list, index);
}

Node* searchAVL(Node* node, char* id) {
  if (node == NULL) return NULL;
  else if (strcmp(id, node->address->id) < 0) {
    return searchAVL(node->left, id);
  }
  else if (strcmp(id, node->address->id) > 0) {
    return searchAVL(node->right, id);
  }
  return node;
}

Node* rotateTreeLeft(Node** node) {
  if (*node == NULL || (*node)->right == NULL) return *node;
  Node* pivot;
  int balanceNode, balancePivot;
  pivot = (*node)->right;
  (*node)->right = pivot->left;
  pivot->left = (*node);
  balanceNode = (*node)->balanced;
  balancePivot = pivot->balanced;
  (*node)->balanced = balanceNode - max2(balancePivot, 0) - 1;
  pivot->balanced = min3(balanceNode-2, balanceNode+balancePivot-2, balancePivot-1);
  (*node) = pivot;
  return *node;
}
Node* rotateTreeRight(Node** node) {
  if (*node == NULL || (*node)->left == NULL) return *node;
  Node* pivot;
  int balanceNode, balancePivot;
  pivot = (*node)->left;
  (*node)->left = pivot->right;
  pivot->right = (*node);
  balanceNode = (*node)->balanced;
  balancePivot = pivot->balanced;
  (*node)->balanced = balanceNode - min2(balancePivot, 0) + 1;
  pivot->balanced = max3(balanceNode+2, balanceNode+balancePivot+2, balancePivot+1);
  (*node) = pivot;
  return *node;
}
Node* doubleRotateTreeLeft(Node** node) {
  (*node)->right = rotateTreeRight(&(*node)->right);
  return rotateTreeLeft(node);
}
Node* doubleRotateTreeRight(Node** node) {
  (*node)->left = rotateTreeLeft(&(*node)->left);
  return rotateTreeRight(node);
}

Node* balanceAVL(Node** node) {
  if ((*node)->balanced >= 2 && (*node)->right) {
    if ((*node)->right->balanced >= 0) {
      return rotateTreeLeft(node);
    } else {
      return doubleRotateTreeLeft(node);
    }
  }
  else if ((*node)->balanced <= -2 && (*node)->left) {
    if ((*node)->left->balanced <= 0) {
      return rotateTreeRight(node);
    } else {
      return doubleRotateTreeRight(node);
    }
  }
  return *node;
}

Node* addChildAVL(Node** node, Instance* address, int* h) {
  if (*node == NULL) {
    *h = 1;
    *node = makeNodeAVL(address);
    return *node;
  }
  else if (strcmp(address->id, (*node)->address->id) < 0) {
    (*node)->left = addChildAVL(&(*node)->left, address, h);
    *h = -*h;
  }
  else if (strcmp(address->id, (*node)->address->id) > 0) {
    (*node)->right = addChildAVL(&(*node)->right, address, h);
  }
  else {
    *h = 0;
    return *node;
  }
  if (*h != 0) {
    (*node)->balanced += *h;
    *node = balanceAVL(node);
    if ((*node)->balanced == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return *node;
}
Node* deleteMin(Node** node, char* nodeToDelete, int* h) {
  if (!hasLeftChild(*node)) {
    strcpy(nodeToDelete, (*node)->address->id);
    Node* smallestNode = *node;
    *h = -1;
    smallestNode = *node;
    *node = (*node)->right;
    free(smallestNode);
    return *node;
  }
  else {
    (*node)->left = deleteMin(&(*node)->left, nodeToDelete, h);
    *h = -*h;
  }
  if (*h != 0) {
    (*node)->balanced = (*node)->balanced + *h;
    *node = balanceAVL(node);
    if ((*node)->balanced == 0) {
      *h = -1;
    } else {
      *h = 0;
    }
  }
  return *node;
}
Node* deleteElementAVL(Node** node, Instance* address, int* h) {
  if (*node == NULL) {
    *h = 0;
    return *node;
  }
  else if (strcmp(address->id, (*node)->address->id) > 0) {
    (*node)->right = deleteElementAVL(&(*node)->right, address, h);
  }
  else if (strcmp(address->id, (*node)->address->id) < 0) {
    (*node)->left = deleteElementAVL(&(*node)->left, address, h);
    *h = -*h;
  }
  else if (hasRightChild(*node)) {
    (*node)->right = deleteMin(&(*node)->right, (*node)->address->id, h);
  }
  else {
    Node* leftNode = *node;
    *node = (*node)->left;
    free(leftNode);
    *h = -1;
    return *node;
  }
  if (*h != 0) {
    (*node)->balanced = (*node)->balanced + *h;
    *node = balanceAVL(node);
    if ((*node)->balanced == 0) {
      *h = -1;
    } else {
      *h = 0;
    }
  }
  return *node;
}

void printTree(Node *root, int space) {//ChatGPT function for debugging
    if (root == NULL) return;
    // Increase distance between levels
    int indent = 5;
    space += indent;
    // Process right child first (will be printed on top)
    printTree(root->right, space);
    // Print current node after space count
    printf("\n");
    for (int i = indent; i < space; i++)
        printf(" ");
    printf("%s\n", root->address->id);
    // Process left child (will be printed below)
    printTree(root->left, space);
}
