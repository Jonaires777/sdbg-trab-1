#ifndef BPTREE_H
#define BPTREE_H

#define MAX_KEYS 3  // Max keys per node (order = MAX_KEYS + 1)
#define TREE_NODE_LENGTH 512 // 1kb

typedef struct BPTreeNode {
    int* keys;
    int is_leaf;
    int num_keys;
    int next;
    int* children;
    int* refs;
    int degree;
} BPTreeNode;

typedef struct Record {
    int index;
    char name[31];
    char type[11];
    int year;
} Record;

BPTreeNode* bptree_insert(FILE* file, BPTreeNode* root, Record record, int max_keys);
// int bptree_search(BPTreeNode* root, int key);
// int bptree_height(BPTreeNode* root);
// void bptree_free(BPTreeNode* root);

#endif
