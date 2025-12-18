#include "./main.h"
#include "./AVL.h"

TreeNode* makeTreeNode(int value) {
  TreeNode* treeNode = NULL;
  treeNode = malloc(sizeof(TreeNode));
  if (treeNode == NULL) exit(1);
  treeNode->right = NULL;
  treeNode->left = NULL;
  treeNode->value = value;
  treeNode->balanced = 0;
  return treeNode;
}

int hasLeftChild(TreeNode* treeNode) {
  if (treeNode == NULL) return 0;
  return treeNode->left != NULL;
}
int hasRightChild(TreeNode* treeNode) {
  if (treeNode == NULL) return 0;
  return treeNode->right != NULL;
}
void addLeftChild(TreeNode** treeNode, int n) {
  if (*treeNode == NULL) return;
  (*treeNode)->left = makeTreeNode(n);
  //return (*treeNode)->left;
}
void addRightChild(TreeNode** treeNode, int n) {
  if (*treeNode == NULL) return;
  (*treeNode)->right = makeTreeNode(n);
  //return (*treeNode)->right;
}
