#include "avl_index.h"
#include <stdlib.h>
#include <math.h>

static int max(int a, int b) { return a > b ? a : b; }
static int height(AVLNode *n) { return n ? n->height : 0; }
static int get_balance(AVLNode *n) { return n ? height(n->left) - height(n->right) : 0; }

static int compare(float k1, int id1, float k2, int id2) {
    if (k1 < k2) return -1;
    if (k1 > k2) return 1;
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;
    return 0;
}

static AVLNode* new_node(float key, int id) {
    AVLNode *n = malloc(sizeof(AVLNode));
    n->key = key; n->id = id;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

static AVLNode* right_rotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

static AVLNode* left_rotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}


AVLNode* avl_insert(AVLNode *node, float key, int id) {
    if (!node) return new_node(key, id);
    
    int cmp = compare(key, id, node->key, node->id);
    if (cmp < 0) node->left = avl_insert(node->left, key, id);
    else if (cmp > 0) node->right = avl_insert(node->right, key, id);
    else return node; 

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);

    if (balance > 1 && compare(key, id, node->left->key, node->left->id) < 0) return right_rotate(node);
    if (balance < -1 && compare(key, id, node->right->key, node->right->id) > 0) return left_rotate(node);
    if (balance > 1 && compare(key, id, node->left->key, node->left->id) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1 && compare(key, id, node->right->key, node->right->id) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}

static AVLNode* min_value_node(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}


AVLNode* avl_delete(AVLNode *root, float key, int id) {
    if (!root) return root;
    int cmp = compare(key, id, root->key, root->id);
    
    if (cmp < 0) root->left = avl_delete(root->left, key, id);
    else if (cmp > 0) root->right = avl_delete(root->right, key, id);
    else {
        if (!root->left || !root->right) {
            AVLNode *temp = root->left ? root->left : root->right;
            if (!temp) { temp = root; root = NULL; }
            else *root = *temp;
            free(temp);
        } else {
            AVLNode *temp = min_value_node(root->right);
            root->key = temp->key; root->id = temp->id;
            root->right = avl_delete(root->right, temp->key, temp->id);
        }
    }
    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0) return right_rotate(root);
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0) return left_rotate(root);
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }
    return root;
}

void avl_range(AVLNode *root, float lo, float hi, int *out, int *cnt) {
    if (!root) return;
    if (root->key >= lo) avl_range(root->left, lo, hi, out, cnt);
    if (root->key >= lo && root->key <= hi) out[(*cnt)++] = root->id;
    if (root->key <= hi) avl_range(root->right, lo, hi, out, cnt);
}

int avl_height(const AVLNode *root) { return root ? root->height : 0; }

static int verify_helper(const AVLNode *n, const AVLNode **prev) {
    if (!n) return 0;
    int errs = verify_helper(n->left, prev);
    int hl = n->left ? n->left->height : 0;
    int hr = n->right ? n->right->height : 0;
    if (hl - hr > 1 || hr - hl > 1) errs++;
    if (n->height != 1 + (hl > hr ? hl : hr)) errs++;
    if (*prev && compare((*prev)->key, (*prev)->id, n->key, n->id) >= 0) errs++;
    *prev = n;
    return errs + verify_helper(n->right, prev);
}

int avl_verify(const AVLNode *root) {
    const AVLNode *prev = NULL;
    return verify_helper(root, &prev);
}

void avl_destroy(AVLNode *root) {
    if (!root) return;
    avl_destroy(root->left);
    avl_destroy(root->right);
    free(root);
}
