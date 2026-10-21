#ifndef AVL_INDEX_H
#define AVL_INDEX_H

typedef struct AVLNode {
    float key;
    int id;
    int height;
    struct AVLNode *left, *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    int count;
} AVLIndex;

AVLNode* avl_insert(AVLNode *root, float key, int id);
AVLNode* avl_delete(AVLNode *root, float key, int id);
void avl_range(AVLNode *root, float lo, float hi, int *out, int *cnt);
int avl_height(const AVLNode *root);
int avl_verify(const AVLNode *root);
void avl_destroy(AVLNode *root);

#endif