#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 100
#define MAX_ROWS 1000

typedef struct {
    char** fields;
    int num_fields;
} Row;

typedef struct {
    char** header;
    int num_cols;
    Row* rows;
    int num_rows;
} CSVData;

int split(const char* str, char delim, char* fields[], int max_fields);
int is_number(const char* s);
int checking_arguments(int argc, char* argv[]);
int reading_file(char* argv[], CSVData* data);

void free_csv_data(CSVData* data);
void print_table(CSVData* data, FILE* out);

#endif
