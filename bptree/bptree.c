#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

void write_bptree_node_to_file(FILE* file, int index, int parent, const BPTreeNode* node) {
    fprintf(file, "%d,", index);
    fprintf(file, "%d,", node->is_leaf);

    // keys
    fprintf(file, "[");
    for (int i = 0; i < node->num_keys; i++) {
        fprintf(file, "%d", node->keys[i]);
        if (i < node->num_keys - 1) fprintf(file, ",");
    }
    fprintf(file, "],");

    // parent
    if (parent == -1)
        fprintf(file, "null,");
    else
        fprintf(file, "%d,", parent);

    // next
    if (node->next == -1)
        fprintf(file, "null,");
    else
        fprintf(file, "%d,", node->next);

    // children
    fprintf(file, "[");
    int has_written = 0;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        if (node->children[i] != -1) {
            if (has_written) fprintf(file, ",");
            fprintf(file, "%d", node->children[i]);
            has_written = 1;
        }
    }
    fprintf(file, "],");

    // refs
    fprintf(file, "[");
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        fprintf(file, "[");
        int has_written = 0;
        for (int j = 0; j < MAX_KEYS + 1; j++) {
            if (node->refs[i][j] != -1) {
                if (has_written) fprintf(file, ",");
                fprintf(file, "%d", node->refs[i][j]);
                has_written = 1;
            }
        }
        fprintf(file, "]");
        if (i != MAX_KEYS) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "]\n");

    fclose(file);
}

static BPTreeNode* create_node(int is_leaf) {
    BPTreeNode* node = malloc(sizeof(BPTreeNode));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->next = -1;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        node->children[i] = -1;
    }
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        for (int j = 0; j < MAX_KEYS + 1; j++) {
            node->refs[i][j] = -1;
        }
    }
    return node;
}

// static void split_child(BPTreeNode* parent, int index) {
//     BPTreeNode* old_child = parent->children[index];
//     BPTreeNode* new_child = create_node(old_child->is_leaf);
//     new_child->num_keys = MAX_KEYS / 2;

//     // Copy the second half to new_child
//     for (int i = 0; i < MAX_KEYS / 2; i++) {
//         new_child->keys[i] = old_child->keys[i + (MAX_KEYS + 1) / 2];
//     }
//     if (!old_child->is_leaf) {
//         for (int i = 0; i <= MAX_KEYS / 2; i++) {
//             new_child->children[i] = old_child->children[i + (MAX_KEYS + 1) / 2];
//         }
//     }
//     old_child->num_keys = (MAX_KEYS + 1) / 2;

//     // Insert the new child into parent
//     for (int i = parent->num_keys; i > index; i--) {
//         parent->children[i + 1] = parent->children[i];
//         parent->keys[i] = parent->keys[i - 1];
//     }

//     parent->children[index + 1] = new_child;
//     parent->keys[index] = old_child->keys[old_child->num_keys - 1];
//     parent->num_keys++;

//     // Handle leaf next pointers
//     if (old_child->is_leaf) {
//         new_child->next = old_child->next;
//         old_child->next = new_child;
//     }
// }

// static BPTreeNode* insert_non_full(BPTreeNode* node, int key) {
//     int i = node->num_keys - 1;

//     if (node->is_leaf) {
//         while (i >= 0 && key < node->keys[i]) {
//             node->keys[i + 1] = node->keys[i];
//             i--;
//         }
//         node->keys[i + 1] = key;
//         node->num_keys++;
//         return node;
//     }

//     while (i >= 0 && key < node->keys[i]) i--;
//     i++;

//     BPTreeNode* child = node->children[i];
//     if (child->num_keys == MAX_KEYS) {
//         split_child(node, i);
//         if (key > node->keys[i]) i++;
//     }

//     node->children[i] = insert_non_full(node->children[i], key);
//     return node;
// }

BPTreeNode* bptree_insert(FILE* file, BPTreeNode* root, Record record) {
    if (!root) {
        root = create_node(1);
        root->keys[0] = record.year;
        root->num_keys = 1;
        root->refs[0][0] = record.index;

        write_bptree_node_to_file(file, 1, -1, root);

        return root;
    } 

    return create_node(0);

    // if (root->num_keys == MAX_KEYS) {
    //     BPTreeNode* new_root = create_node(0);
    //     new_root->children[0] = root;
    //     split_child(new_root, 0);
    //     return insert_non_full(new_root, record.year);
    // }

    // return insert_non_full(root, record.year);
}

// int bptree_search(BPTreeNode* node, int key) {
//     if (!node) return 0;

//     int i = 0;
//     while (i < node->num_keys && key > node->keys[i]) i++;

//     if (i < node->num_keys && key == node->keys[i]) return 1;

//     if (node->is_leaf) return 0;

//     return bptree_search((BPTreeNode*)node->children[i], key);
// }

// int bptree_height(BPTreeNode* root) {
//     int height = 0;
//     BPTreeNode* node = root;
//     while (node && !node->is_leaf) {
//         height++;
//         node = node->children[0];
//     }
//     return node ? height + 1 : 0;
// }