#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RECORD_LENGTH 50

typedef struct {
    char name[31];
    char type[11];
    int year;
} Record;

void create_records_file();
Record get_record_from_file(FILE* file, int index);
Record read_record_buffer(char* buffer);

int main()
{
    create_records_file();

    FILE* file = fopen("vinhos.txt", "r");

    if (file == NULL) {
        printf("Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    Record record1 = get_record_from_file(file, 0);

    printf("NAME %s\n", record1.name);
    printf("TYPE %s\n", record1.type);
    printf("YEAR %i\n", record1.year);

    fclose(file);

    return 0;
}

Record read_record_buffer(char* buffer) {
    Record record = {0};
    char year[4] = {0};
    strncpy(record.name, buffer, 30);
    strncpy(record.type, buffer + 30, 10);
    strncpy(year, buffer + 40, 4);

    record.name[30] = '\0';
    record.type[10] = '\0';
    record.year = atoi(year);

    return record;
}

void create_records_file() {
    FILE* csv_file = fopen("vinhos.csv", "r");
    if (csv_file == NULL) {
        perror("ERROR: Could not open vinhos.csv");
        exit(EXIT_FAILURE);
    }

    FILE* txt_file = fopen("vinhos.txt", "w");
    if (txt_file == NULL) {
        perror("ERROR: Could not create vinhos.txt");
        fclose(csv_file);
        exit(EXIT_FAILURE);
    }

    char line[256];
    fgets(line, sizeof(line), csv_file); // Skip header

    while (fgets(line, sizeof(line), csv_file)) {
        int id;
        char name[30], type[10];
        int year;

        if (sscanf(line, "%d,%[^,],%d,%s", &id, name, &year, type) == 4) {
            char buffer[RECORD_LENGTH + 1] = {0};

            // Format exactly 50 characters (no newline)
            snprintf(buffer, sizeof(buffer),
                "%-30s%-10s%4d      ", // 30 + 10 + 4 + 6 = 50
                name, type, year);

            // Insert Record in File
            fwrite(buffer, sizeof(char), RECORD_LENGTH, txt_file); // Write raw 50 chars
            // TODO: Insert Record in Tree file;
            // insert_in_tree();
        }
    }

    fclose(csv_file);
    fclose(txt_file);
}

Record get_record_from_file(FILE* file, int index) {
    // Calculate byte offset
    long offset = (long)index * RECORD_LENGTH;

    // Move to the correct position
    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("ERROR: Could not seek to position");
        exit(EXIT_FAILURE);
    }

    char buffer[RECORD_LENGTH + 1] = {0}; 

    size_t read = fread(buffer, sizeof(char), RECORD_LENGTH, file);
    if (read < RECORD_LENGTH) {
        printf("ERROR: Could not read record at index %d (possibly out of range)\n", index);
        exit(EXIT_FAILURE);
    } 

    return read_record_buffer(buffer);
}