#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bptree/bptree.h"
#include "write_file.h"

int main()
{
    create_records_file();

    FILE* file = fopen("vinhos.txt", "r");

    if (file == NULL) {
        printf("Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    Record record1 = get_record_from_file(file, 0);
    record1.index = 0;

    printf("NAME %s\n", record1.name);
    printf("TYPE %s\n", record1.type);
    printf("YEAR %i\n", record1.year);

    FILE* index_file = fopen("bptree.txt", "a");
    bptree_insert(index_file, NULL, record1, 3);
    fclose(index_file);

    index_file = fopen("bptree.txt", "r");
    BPTreeNode* node = read_bptree_node_from_file(index_file, 0, 3);
    for (int i = 0; i < node->num_keys; i++) {
        printf("CHILD[%i]: %d\n", i, node->children[i]);
        printf("KEY[%i]: %d\n", i, node->keys[i]);
        printf("REF[%i]: %d\n", i, node->refs[i]);
    }
    fclose(index_file);

    fclose(file);

    return 0;
}