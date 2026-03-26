#include "../include/csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Разбиение строки на поля
int split(const char* str, char delim, char* fields[], int max_fields)
{
    int count = 0;
    const char* start = str;
    const char* end;
    while (1) {
        end = strchr(start, delim);
        if (end == NULL) {
            int len = strlen(start);
            fields[count] = malloc(len + 1);
            if (!fields[count])
                return count;
            strcpy(fields[count], start);
            count++;
            break;
        } else {
            int len = end - start;
            fields[count] = malloc(len + 1);
            if (!fields[count])
                return count;
            strncpy(fields[count], start, len);
            fields[count][len] = '\0';
            count++;
            start = end + 1;
        }
        if (count >= max_fields)
            break;
    }
    return count;
}

// Проверка, является ли строка числом
int is_number(const char* s)
{
    if (s == NULL || *s == '\0')
        return 0;
    char* endptr;
    strtod(s, &endptr);
    return *endptr == '\0';
}

// Функция проверки количества аргументов
int checking_arguments(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Использование: %s входной_файл.csv выходной_файл.txt\n", argv[0]);
        return 1;
    }
    return 0;
}

// Чтение CSV файла в структуру CSVData
int reading_file(char* argv[], CSVData* data)
{
    FILE* in = fopen(argv[1], "r");
    if (!in) {
        perror("Не удалось открыть входной файл");
        return 1;
    }

    char line[1024];

    if (!fgets(line, sizeof(line), in)) {
        fprintf(stderr, "Файл пуст\n");
        fclose(in);
        return 1;
    }

    line[strcspn(line, "\n")] = '\0';

    data->header = malloc(MAX_COLS * sizeof(char*));
    if (!data->header) {
        fclose(in);
        return 1;
    }

    data->num_cols = split(line, ',', data->header, MAX_COLS);
    data->rows = malloc(MAX_ROWS * sizeof(Row));

    if (!data->rows) {
        free(data->header);
        fclose(in);
        return 1;
    }

    data->num_rows = 0;

    while (fgets(line, sizeof(line), in) && data->num_rows < MAX_ROWS) {
        line[strcspn(line, "\n")] = '\0';
        char* fields[MAX_COLS];
        int cnt = split(line, ',', fields, MAX_COLS);

        char** row_fields = malloc(data->num_cols * sizeof(char*));
        if (!row_fields) {
            for (int i = 0; i < cnt; i++)
                free(fields[i]);
            break;
        }

        for (int i = 0; i < data->num_cols; i++) {
            if (i < cnt) {
                row_fields[i] = strdup(fields[i]);
                if (!row_fields[i]) {
                    for (int j = 0; j < i; j++)
                        free(row_fields[j]);
                    free(row_fields);
                    break;
                }
            } else {
                row_fields[i] = strdup("");
                if (!row_fields[i]) {
                    for (int j = 0; j < i; j++)
                        free(row_fields[j]);
                    free(row_fields);
                    break;
                }
            }
        }

        for (int i = 0; i < cnt; i++)
            free(fields[i]);

        data->rows[data->num_rows].fields = row_fields;
        data->rows[data->num_rows].num_fields = data->num_cols;
        data->num_rows++;
    }

    fclose(in);
    return 0;
}

// Освобождение памяти, выделенной для данных
void free_csv_data(CSVData* data)
{
    for (int i = 0; i < data->num_cols; i++) {
        free(data->header[i]);
    }
    free(data->header);

    for (int i = 0; i < data->num_rows; i++) {
        for (int j = 0; j < data->num_cols; j++) {
            free(data->rows[i].fields[j]);
        }
        free(data->rows[i].fields);
    }
    free(data->rows);
}

// Вывод таблицы в файл
void print_table(CSVData* data, FILE* out)
{
    int col_widths[MAX_COLS];
    for (int i = 0; i < data->num_cols; i++) {
        col_widths[i] = strlen(data->header[i]);
    }

    for (int r = 0; r < data->num_rows; r++) {
        for (int c = 0; c < data->num_cols; c++) {
            int len = strlen(data->rows[r].fields[c]);
            if (len > col_widths[c]) {
                col_widths[c] = len;
            }
        }
    }

    void print_sep(char ch)
    {
        fputc('+', out);
        for (int i = 0; i < data->num_cols; i++) {
            for (int j = 0; j < col_widths[i]; j++) {
                fputc(ch, out);
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
            }
            fputc('+', out);
        }
        fputc('\n', out);
    }

    print_sep('=');
    fputc('|', out);
    for (int i = 0; i < data->num_cols; i++) {
        fprintf(out, "%-*s|", col_widths[i], data->header[i]);
    }
    fputc('\n', out);
    print_sep('=');

    for (int r = 0; r < data->num_rows; r++) {
        fputc('|', out);
        for (int c = 0; c < data->num_cols; c++) {
            char* field = data->rows[r].fields[c];
            if (is_number(field)) {
                fprintf(out, "%*s|", col_widths[c], field);
            } else {
                fprintf(out, "%-*s|", col_widths[c], field);
            }
        }
        fputc('\n', out);
        print_sep('-');
    }
}
