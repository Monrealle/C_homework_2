#include "../include/dfa.h"
#include <stdio.h>
#include <string.h>

void test(const char* str, int expected_ok)
{
    int trans[8][256];
    memset(trans, -1, sizeof(trans));

    // Цифры
    for (char d = '0'; d <= '9'; ++d) {
        trans[0][d] = trans[1][d] = trans[2][d] = 2;
        trans[3][d] = trans[4][d] = 4;
        trans[5][d] = trans[6][d] = trans[7][d] = 7;
    }
    // Точка
    trans[0]['.'] = trans[1]['.'] = trans[2]['.'] = 3;
    // Минус
    trans[0]['-'] = 1;
    trans[5]['-'] = 6;
    // Плюс
    trans[5]['+'] = 6;
    // E/e
    trans[2]['E'] = trans[2]['e'] = 5;
    trans[4]['E'] = trans[4]['e'] = 5;

    int accepting[] = { 2, 4, 7 };
    DFA_Result res = dfa_run(trans, 8, 0, accepting, 3, str);

    int ok = (res == DFA_OK);
    printf("%-20s -> %s (%s)\n", str, ok ? "Число" : "Не число",
        (ok == expected_ok) ? "✓" : "✗ ошибка");
}

int main()
{
    printf("=== ТЕСТЫ ДЛЯ ДКА (числа) ===\n\n");

    // Корректные числа
    test("123", 1);
    test("-456", 1);
    test("0", 1);
    test(".591", 1);
    test("38.871", 1);
    test("38.871E5", 1);
    test("38.871e-5", 1);
    test("-3.14E+10", 1);
    test("123.456E789", 1);

    // Некорректные числа
    test("", 0);
    test("abc", 0);
    test("12a34", 0);
    test("823.16.10", 0);
    test("Это текст", 0);
    test("-", 0);
    test(".", 0);
    test("E5", 0);
    test("--5", 0);
    test("12E", 0);
    test("12E-", 0);
    test("12E3.4", 0);

    return 0;
}
