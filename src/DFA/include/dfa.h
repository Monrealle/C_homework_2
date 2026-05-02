#ifndef DFA_H
#define DFA_H

#include <stdbool.h>

typedef enum {
    DFA_OK = 0,
    DFA_ERR_BAD_SYMBOL,
    DFA_ERR_NOT_ACCEPT
} DFA_Result;

DFA_Result dfa_run(const int transitions[][256], int num_states, int start,
    const int* accepting, int num_accepting, const char* str);

#endif
