typedef struct _treeNode{
  int value;
  struct _treeNode* right;
  struct _treeNode* left;
  int balanced;
} TreeNode;

TreeNode* makeTreeNode(int value);

int hasLeftChild(TreeNode* treeNode);
int hasRightChild(TreeNode* treeNode);
void addLeftChild(TreeNode** treeNode, int n);
void addRightChild(TreeNode** treeNode, int n);
