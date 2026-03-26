#include "../include/csv.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (checking_arguments(argc, argv) != 0) {
        return 1;
    }

    CSVData data;
    if (reading_file(argv, &data) != 0) {
        fprintf(stderr, "Ошибка при чтении файла %s\n", argv[1]);
        return 1;
    }

    FILE* out = fopen(argv[2], "w");
    if (!out) {
        perror("Не удалось открыть выходной файл");
        free_csv_data(&data);
        return 1;
    }

    print_table(&data, out);
    fclose(out);
    free_csv_data(&data);

    return 0;
}
