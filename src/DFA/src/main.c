#include "../include/dfa.h"
#include <stdio.h>
#include <string.h>

int main()
{
    // Таблица переходов: 8 состояний, 256 символов, инициализируем -1
    int trans[8][256];
    memset(trans, -1, sizeof(trans));

    // Состояния:
    // 0 – начало
    // 1 – после минуса
    // 2 – целая часть (или цифры после минуса)
    // 3 – после точки
    // 4 – дробная часть
    // 5 – после E/e
    // 6 – после знака экспоненты
    // 7 – экспонента (цифры)

    // Цифры: из {0,1,2} -> 2; из {3,4} -> 4; из {5,6,7} -> 7
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

    // Плюс (только после E)
    trans[5]['+'] = 6;

    // E и e
    trans[2]['E'] = trans[2]['e'] = 5;
    trans[4]['E'] = trans[4]['e'] = 5;

    // Допускающие состояния
    int accepting[] = { 2, 4, 7 };
    int num_accepting = 3;
    int start = 0;

    char input[256];
    printf("Введите строку: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    DFA_Result res = dfa_run(trans, 8, start, accepting, num_accepting, input);

    if (res == DFA_OK)
        printf("Это число!\n");
    else
        printf("Это не число :(\n");

    return 0;
}
