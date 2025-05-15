#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bptree.h"

static BPTreeNode* create_node(int is_leaf, int max_keys) {
    BPTreeNode* node = malloc(sizeof(BPTreeNode));
    node->keys = malloc(sizeof(int) * max_keys);
    node->children = malloc(sizeof(int) * max_keys);
    node->refs = malloc(sizeof(int) * max_keys);
    node->degree = max_keys;

    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->next = -1;

    for (int i = 0; i < max_keys; i++) {
        node->children[i] = -1;
        node->refs[i] = -1;
    }
    return node;
}

BPTreeNode* read_bptree_node_from_file(FILE* file, int index, int degree) {
    char buffer[TREE_NODE_LENGTH + 1] = {0};
    long offset = (long)index * TREE_NODE_LENGTH;

    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    size_t read = fread(buffer, sizeof(char), TREE_NODE_LENGTH, file);
    if (read < TREE_NODE_LENGTH) {
        fprintf(stderr, "Could not read full node at index %d\n", index);
        exit(EXIT_FAILURE);
    }

    // Trim trailing spaces
    for (int i = TREE_NODE_LENGTH - 1; i >= 0; i--) {
        if (buffer[i] == ' ' || buffer[i] == '\n') {
            buffer[i] = '\0';
        } else {
            break;
        }
    }

    BPTreeNode* node = create_node(0, degree);

    char* token;

    // index (skip)
    token = strtok(buffer, ","); // index

    // is_leaf
    token = strtok(NULL, ",");
    node->is_leaf = atoi(token);

    // keys
    token = strtok(NULL, "]"); // e.g. "[13,17,24"
    char* key_section = token ? token + 1 : NULL;
    node->num_keys = 0;

    if (key_section) {
        char* key_token = strtok(key_section, ",");
        while (key_token && node->num_keys < degree) {
            node->keys[node->num_keys++] = atoi(key_token);
            key_token = strtok(NULL, ",");
        }
    }

    // parent (skip)
    token = strtok(NULL, ",");

    // next
    token = strtok(NULL, ",");
    node->next = (token && strcmp(token, "null") != 0) ? atoi(token) : -1;

    // children
    token = strtok(NULL, "]"); // e.g. "[1,2,3"
    char* children_section = token ? token + 1 : NULL;

    int child_index = 0;
    if (children_section) {
        char* child_token = strtok(children_section, ",");
        while (child_token && child_index < node->num_keys) {
            node->children[child_index++] = atoi(child_token);
            child_token = strtok(NULL, ",");
        }
    }

    // refs
    token = strtok(NULL, "]"); // e.g. "[9,10,11"
    char* refs_section = token ? token + 1 : NULL;

    int ref_index = 0;
    if (refs_section) {
        char* ref_token = strtok(refs_section, ",");
        while (ref_token && ref_index < node->num_keys) {
            node->refs[ref_index++] = atoi(ref_token);
            ref_token = strtok(NULL, ",");
        }
    }

    return node;
}


void write_bptree_node_to_file(FILE* file, int index, int parent, const BPTreeNode* node) {
    char buffer[TREE_NODE_LENGTH] = {0};
    char temp[400] = {0}; 

    // index, is_leaf
    sprintf(temp + strlen(temp), "%d,", index);
    sprintf(temp + strlen(temp), "%d,", node->is_leaf);

    // keys
    sprintf(temp + strlen(temp), "[");
    for (int i = 0; i < node->num_keys; i++) {
        sprintf(temp + strlen(temp), "%d", node->keys[i]);
        if (i < node->num_keys - 1)
            sprintf(temp + strlen(temp), ",");
    }
    sprintf(temp + strlen(temp), "],");

    // parent
    if (parent == -1)
        sprintf(temp + strlen(temp), "null,");
    else
        sprintf(temp + strlen(temp), "%d,", parent);

    // next
    if (node->next == -1)
        sprintf(temp + strlen(temp), "null,");
    else
        sprintf(temp + strlen(temp), "%d,", node->next);

    // children
    sprintf(temp + strlen(temp), "[");
    int written_children = 0;
    for (int i = 0; i < node->degree; i++) {
        if (node->children[i] != -1) {
            if (written_children)
                sprintf(temp + strlen(temp), ",");
            sprintf(temp + strlen(temp), "%d", node->children[i]);
            written_children = 1;
        }
    }
    sprintf(temp + strlen(temp), "],");

    // refs
    sprintf(temp + strlen(temp), "[");
    int written_refs = 0;
    for (int i = 0; i < node->degree; i++) {
        if (node->refs[i] != -1) {
            if (written_refs)
                sprintf(temp + strlen(temp), ",");
            sprintf(temp + strlen(temp), "%d", node->refs[i]);
            written_refs = 1;
        }
    }
    sprintf(temp + strlen(temp), "]");

    // Fill buffer with temp contents and pad with spaces
    size_t len = strlen(temp);
    memcpy(buffer, temp, len);
    for (size_t i = len; i < TREE_NODE_LENGTH; i++) {
        buffer[i] = ' ';
    }

    fwrite(buffer, sizeof(char), TREE_NODE_LENGTH, file);
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

// static int insert_non_full(BPTreeNode* node, Record record) {
//     int key = record.year;
//     int ref = record.index;
//     int i = node->num_keys - 1;

//     if (node->is_leaf) {
//         while (i >= 0 && key < node->keys[i]) {
//             node->keys[i + 1] = node->keys[i];
//             i--;
//         }
//         node->keys[i + 1] = key;
//         node->refs[i + 1] = ref;
//         node->num_keys++;
//         return node;
//     }

//     while (i >= 0 && key < node->keys[i]) i--;
//     i++;

//     // BPTreeNode* child = node->children[i];
//     // if (child->num_keys == MAX_KEYS) {
//     //     split_child(node, i);
//     //     if (key > node->keys[i]) i++;
//     // }

//     node->children[i] = insert_non_full(node->children[i], record);
//     return node;
// }

BPTreeNode* bptree_insert(FILE* file, BPTreeNode* root, Record record, int max_keys) {
    if (!root) {
        root = create_node(1, max_keys);
        root->keys[0] = record.year;
        root->num_keys = 1;
        root->refs[0] = record.index;

        write_bptree_node_to_file(file, 1, -1, root);

        return root;
    } 

    return create_node(0, max_keys);

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