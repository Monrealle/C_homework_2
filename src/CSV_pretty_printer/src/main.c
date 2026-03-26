#include "../include/csv.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (checkingArguments(argc, argv) != 0) {
        return 1;
    }

    CSVData data;
    if (readingFile(argv, &data) != 0) {
        fprintf(stderr, "Ошибка при чтении файла %s\n", argv[1]);
        return 1;
    }

    FILE* out = fopen(argv[2], "w");
    if (!out) {
        perror("Не удалось открыть выходной файл");
        freeCsvData(&data);
        return 1;
    }

    printTable(&data, out);
    fclose(out);
    freeCsvData(&data);

    return 0;
}
