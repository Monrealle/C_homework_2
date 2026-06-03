#include "../include/csv.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Разбиение строки на поля
int split(const char* str, char delim, char* fields[], int maxFields)
{
    int count = 0;
    const char* start = str;
    const char* end;
    while (1) {
        end = strchr(start, delim);
        if (end == NULL) {
            size_t len = strlen(start);
            fields[count] = malloc(len + 1);
            if (!fields[count])
                return count;
            memcpy(fields[count], start, len);
            fields[count][len] = '\0';
            count++;
            break;
        } else {
            ptrdiff_t len = end - start;
            fields[count] = malloc(len + 1);
            if (!fields[count])
                return count;
            strncpy(fields[count], start, len);
            fields[count][len] = '\0';
            count++;
            start = end + 1;
        }
        if (count >= maxFields)
            break;
    }
    return count;
}

// Проверка, является ли строка числом
int isNumber(const char* s)
{
    if (s == NULL || *s == '\0')
        return 0;
    char* endptr;
    strtod(s, &endptr);
    return *endptr == '\0';
}

// Функция проверки количества аргументов
int checkingArguments(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Использование: %s входной_файл.csv выходной_файл.txt\n", argv[0]);
        return 1;
    }
    return 0;
}

// Чтение CSV файла в структуру CSVData
int readingFile(char* argv[], CSVData* data)
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

    size_t newline = strcspn(line, "\n");
    if (newline < sizeof(line)) {
        line[newline] = '\0';
    }

    data->header = malloc(MAX_COLS * sizeof(char*));
    if (!data->header) {
        fclose(in);
        return 1;
    }

    data->num_cols = split(line, ',', data->header, MAX_COLS);

    // Проверка: если не удалось разобрать ни одного поля, освобождаем память и выходим
    if (data->num_cols == 0) {
        free(data->header);
        fclose(in);
        fprintf(stderr, "Ошибка: не удалось разобрать заголовок CSV (нет полей)\n");
        return 1;
    }

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

        // Выделение памяти под поля строки
        char** rowFields = malloc(data->num_cols * sizeof(char*));
        if (!rowFields) {
            for (int i = 0; i < cnt; i++)
                free(fields[i]);
            break;
        }

        for (int i = 0; i < data->num_cols; i++) {
            if (i < cnt) {
                rowFields[i] = strdup(fields[i]);
                if (!rowFields[i]) {
                    for (int j = 0; j < i; j++)
                        free(rowFields[j]);
                    free(rowFields);
                    break;
                }
            } else {
                rowFields[i] = strdup("");
                if (!rowFields[i]) {
                    for (int j = 0; j < i; j++)
                        free(rowFields[j]);
                    free(rowFields);
                    break;
                }
            }
        }

        for (int i = 0; i < cnt; i++)
            free(fields[i]);

        data->rows[data->num_rows].fields = rowFields;
        data->rows[data->num_rows].num_fields = data->num_cols;
        data->num_rows++;
    }

    fclose(in);
    return 0;
}

// Освобождение памяти, выделенной для данных
void freeCsvData(CSVData* data)
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

// Вспомогательная функция для печати разделителя
static void printSep(FILE* out, const int colWidths[], int numCols, char ch)
{
    fputc('+', out);
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) {
            fputc(ch, out);
        }
        fputc('+', out);
    }
    fputc('\n', out);
}

// Вывод таблицы в файл
void printTable(CSVData* data, FILE* out)
{
    int colWidths[MAX_COLS];
    for (int i = 0; i < data->num_cols; i++) {
        colWidths[i] = (int)strlen(data->header[i]);
    }

    for (int r = 0; r < data->num_rows; r++) {
        for (int c = 0; c < data->num_cols; c++) {
            size_t len = strlen(data->rows[r].fields[c]);
            if (len > (size_t)colWidths[c]) {
                colWidths[c] = (int)len;
            }
        }
    }

    printSep(out, colWidths, data->num_cols, '=');
    fputc('|', out);
    for (int i = 0; i < data->num_cols; i++) {
        fprintf(out, "%-*s|", colWidths[i], data->header[i]);
    }
    fputc('\n', out);
    printSep(out, colWidths, data->num_cols, '=');

    for (int r = 0; r < data->num_rows; r++) {
        fputc('|', out);
        for (int c = 0; c < data->num_cols; c++) {
            char* field = data->rows[r].fields[c];
            if (isNumber(field)) {
                fprintf(out, "%*s|", colWidths[c], field);
            } else {
                fprintf(out, "%-*s|", colWidths[c], field);
            }
        }
        fputc('\n', out);
        printSep(out, colWidths, data->num_cols, '-');
    }
}
