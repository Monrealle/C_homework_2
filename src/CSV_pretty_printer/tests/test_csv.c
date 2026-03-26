#include "csv.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Тест split
static void test_split()
{
    char* fields[10];

    int cnt = split("a,b,c", ',', fields, 10);
    assert(cnt == 3);
    assert(strcmp(fields[0], "a") == 0);
    assert(strcmp(fields[1], "b") == 0);
    assert(strcmp(fields[2], "c") == 0);
    for (int i = 0; i < cnt; i++)
        free(fields[i]);

    cnt = split("a,,c", ',', fields, 10);
    assert(cnt == 3);
    assert(strcmp(fields[0], "a") == 0);
    assert(strcmp(fields[1], "") == 0);
    assert(strcmp(fields[2], "c") == 0);
    for (int i = 0; i < cnt; i++)
        free(fields[i]);
}

// Тест is_number
static void test_is_number()
{
    assert(is_number("123") == 1);
    assert(is_number("-3.14") == 1);
    assert(is_number("abc") == 0);
    assert(is_number("") == 0);
}

// Тест reading_file
static void test_reading_file()
{
    char* name = strdup("/tmp/test.csv");
    FILE* f = fopen(name, "w");
    assert(f);
    fprintf(f, "A,B\n1,2\n3,4\n");
    fclose(f);

    char* argv[] = { "prog", name };
    CSVData data;
    int r = reading_file(argv, &data);
    assert(r == 0);
    assert(data.num_cols == 2);
    assert(strcmp(data.header[0], "A") == 0);
    assert(strcmp(data.header[1], "B") == 0);
    assert(data.num_rows == 2);
    assert(strcmp(data.rows[0].fields[0], "1") == 0);
    assert(strcmp(data.rows[0].fields[1], "2") == 0);
    free_csv_data(&data);
    remove(name);
    free(name);
}

// Тест print_table
static void test_print_table()
{
    CSVData data;
    data.num_cols = 1;
    data.header = malloc(sizeof(char*));
    data.header[0] = strdup("X");
    data.rows = malloc(sizeof(Row));
    data.rows[0].fields = malloc(sizeof(char*));
    data.rows[0].fields[0] = strdup("123");
    data.rows[0].num_fields = 1;
    data.num_rows = 1;

    char outname[] = "/tmp/out.txt";
    FILE* out = fopen(outname, "w");
    assert(out);
    print_table(&data, out);
    fclose(out);

    FILE* in = fopen(outname, "r");
    assert(in);
    char buf[256];
    char expected[] = "+===+\n|X  |\n+===+\n|123|\n+---+\n";
    char actual[256] = "";
    while (fgets(buf, sizeof(buf), in))
        strcat(actual, buf);
    fclose(in);
    remove(outname);

    assert(strcmp(actual, expected) == 0);
    free_csv_data(&data);
}

int main()
{
    test_split();
    test_is_number();
    test_reading_file();
    test_print_table();
    printf("Все тесты пройдены\n");
    return 0;
}
