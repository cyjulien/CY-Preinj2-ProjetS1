#include "../main.h"
#include "./AVL.h"

Node* makeNode(int value) {
  Node* treeNode = NULL;
  treeNode = malloc(sizeof(Node));
  if (treeNode == NULL) exit(1);
  treeNode->right = NULL;
  treeNode->left = NULL;
  treeNode->value = value;
  treeNode->balanced = 0;
  return treeNode;
}

int hasLeftChild(Node* treeNode) {
  if (treeNode == NULL) return 0;
  return treeNode->left != NULL;
}
int hasRightChild(Node* treeNode) {
  if (treeNode == NULL) return 0;
  return treeNode->right != NULL;
}
void addLeftChild(Node** treeNode, int n) {
  if (*treeNode == NULL) return;
  (*treeNode)->left = makeNode(n);
  //return (*treeNode)->left;
}
void addRightChild(Node** treeNode, int n) {
  if (*treeNode == NULL) return;
  (*treeNode)->right = makeNode(n);
  //return (*treeNode)->right;
}


Node* rotateTreeLeft(Node** treeNode) {
  if (*treeNode == NULL || (*treeNode)->right == NULL) return *treeNode;
  Node* pivot;
  int balanceNode, balancePivot;
  pivot = (*treeNode)->right;
  (*treeNode)->right = pivot->left;
  pivot->left = (*treeNode);
  balanceNode = (*treeNode)->balanced;
  balancePivot = pivot->balanced;
  (*treeNode)->balanced = balanceNode - max2(balancePivot, 0) - 1;
  pivot->balanced = min3(balanceNode-2, balanceNode+balancePivot-2, balancePivot-1);
  (*treeNode) = pivot;
  return *treeNode;
}
Node* rotateTreeRight(Node** treeNode) {
  if (*treeNode == NULL || (*treeNode)->left == NULL) return *treeNode;
  Node* pivot;
  int balanceNode, balancePivot;
  pivot = (*treeNode)->left;
  (*treeNode)->left = pivot->right;
  pivot->right = (*treeNode);
  balanceNode = (*treeNode)->balanced;
  balancePivot = pivot->balanced;
  (*treeNode)->balanced = balanceNode - min2(balancePivot, 0) + 1;
  pivot->balanced = max3(balanceNode+2, balanceNode+balancePivot+2, balancePivot+1);
  (*treeNode) = pivot;
  return *treeNode;
}
Node* doubleRotateTreeLeft(Node** treeNode) {
  (*treeNode)->right = rotateTreeRight(&(*treeNode)->right);
  return rotateTreeLeft(treeNode);
}
Node* doubleRotateTreeRight(Node** treeNode) {
  (*treeNode)->left = rotateTreeLeft(&(*treeNode)->left);
  return rotateTreeRight(treeNode);
}

Node* balanceAVL(Node** treeNode) {
  if ((*treeNode)->balanced >= 2) {
    if ((*treeNode)->right->balanced >= 0) {
      return rotateTreeLeft(treeNode);
    } else {
      return doubleRotateTreeLeft(treeNode);
    }
  }
  else if ((*treeNode)->balanced <= -2) {
    if ((*treeNode)->left->balanced <= 0) {
      return rotateTreeRight(treeNode);
    } else {
      return doubleRotateTreeRight(treeNode);
    }
  }
  return *treeNode;
}

Node* addChildAVL(Node** treeNode, int n, int* h) {
  if (*treeNode == NULL) {
    *h = 1;
    *treeNode = makeNode(n);
  }
  else if (n < (*treeNode)->value) {
    (*treeNode)->left = addChildAVL(&(*treeNode)->left, n, h);
    *h = -*h;
  }
  else if (n > (*treeNode)->value) {
    (*treeNode)->right = addChildAVL(&(*treeNode)->right, n, h);
  }
  else {
    *h = 0;
    return *treeNode;
  }
  if (*h != 0) {
    (*treeNode)->balanced = (*treeNode)->balanced + *h;
    *treeNode = balanceAVL(treeNode);
    if ((*treeNode)->balanced == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return *treeNode;
}
Node* deleteMin(Node** tree, int* nodeToDelete, int* h) {
  if (!hasLeftChild(*tree)) {
    *nodeToDelete = (*tree)->value;
    Node* smallestNode = *tree;
    *h = -1;
    smallestNode = *tree;
    *tree = (*tree)->right;
    free(smallestNode);
    return *tree;
  }
  else {
    (*tree)->left = deleteMin(&(*tree)->left, nodeToDelete, h);
    *h = -*h;
  }
  if (*h != 0) {
    (*tree)->balanced = (*tree)->balanced + *h;
    *tree = balanceAVL(tree);
    if ((*tree)->balanced == 0) {
      *h = -1;
    } else {
      *h = 0;
    }
  }
  return *tree;
}
Node* deleteElementAVL(Node** treeNode, int n, int* h) {
  if (*treeNode == NULL) {
    *h = 0;
    return *treeNode;
  }
  else if (n > (*treeNode)->value) {
    (*treeNode)->right = deleteElementAVL(&(*treeNode)->right, n, h);
  }
  else if (n < (*treeNode)->value) {
    (*treeNode)->left = deleteElementAVL(&(*treeNode)->left, n, h);
    *h = -*h;
  }
  else if (hasRightChild(*treeNode)) {
    (*treeNode)->right = deleteMin(&(*treeNode)->right, &(*treeNode)->value, h);
  }
  else {
    Node* leftNode = *treeNode;
    *treeNode = (*treeNode)->left;
    free(leftNode);
    *h = -1;
    return *treeNode;
  }
  if (*h != 0) {
    (*treeNode)->balanced = (*treeNode)->balanced + *h;
    *treeNode = balanceAVL(treeNode);
    if ((*treeNode)->balanced == 0) {
      *h = -1;
    } else {
      *h = 0;
    }
  }
  return *treeNode;
}

void printTree(Node *root, int space) {//ChatGPT function
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
    printf("%d\n", root->value);
    // Process left child (will be printed below)
    printTree(root->left, space);
}
