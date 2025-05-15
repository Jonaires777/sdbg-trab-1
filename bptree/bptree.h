#ifndef BPTREE_H
#define BPTREE_H

#define MAX_KEYS 3  // Max keys per node (order = MAX_KEYS + 1)

typedef struct BPTreeNode {
    int keys[MAX_KEYS];
    int is_leaf;
    int num_keys;
    struct BPTreeNode* next; // Only for leaf nodes
    struct BPTreeNode* children[MAX_KEYS + 1]; // Child pointers or NULL for leaf
} BPTreeNode;

BPTreeNode* bptree_insert(BPTreeNode* root, int key);
int bptree_search(BPTreeNode* root, int key);
int bptree_height(BPTreeNode* root);
void bptree_free(BPTreeNode* root);

#endif
