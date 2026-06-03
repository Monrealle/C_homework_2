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

int split(const char* str, char delim, char* fields[], int maxFields);
int isNumber(const char* s);
int checkingArguments(int argc, char* argv[]);
int readingFile(char* argv[], CSVData* data);

void freeCsvData(CSVData* data);
void printTable(CSVData* data, FILE* out);

#endif
