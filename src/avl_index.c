#include "avl_index.h"

AVLNode* avl_insert(AVLNode *root, float key, int id) {
    (void)key; (void)id;
    return root;
}

AVLNode* avl_delete(AVLNode *root, float key, int id) {
    (void)key; (void)id;
    return root;
}

void avl_range(AVLNode *root, float lo, float hi, int *out, int *cnt) {
    (void)root; (void)lo; (void)hi; (void)out; (void)cnt;
}

int avl_height(const AVLNode *root) {
    (void)root;
    return 0;
}

int avl_verify(const AVLNode *root) {
    (void)root;
    return -1;
}

void avl_destroy(AVLNode *root) {
    (void)root;
}