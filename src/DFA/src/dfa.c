#include "../include/dfa.h"

DFA_Result dfa_run(const int transitions[][256], int num_states, int start,
    const int* accepting, int num_accepting, const char* str)
{
    int state = start;

    for (int i = 0; str[i]; ++i) {
        unsigned char c = (unsigned char)str[i];
        int next = transitions[state][c];
        if (next == -1)
            return DFA_ERR_BAD_SYMBOL;
        state = next;
    }

    for (int i = 0; i < num_accepting; ++i)
        if (state == accepting[i])
            return DFA_OK;

    return DFA_ERR_NOT_ACCEPT;
}
