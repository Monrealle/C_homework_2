/*
 * main.c - AeroSoft: база аэропортов на АВЛ-дереве.
 *
 * Сборка:  gcc -O2 -o aerosoft main.c
 * Запуск:  ./aerosoft airports.txt
 *
 * Команды: find <код> | add <код>:<название> | delete <код> | save | quit
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KS 16  /* макс. длина IATA-кода */
#define VS 256 /* макс. длина названия  */

/* Узел АВЛ-дерева */
typedef struct N {
    char k[KS], v[VS]; /* ключ (код) и значение (название) */
    struct N *L, *R;   /* левый и правый потомки           */
    int h;             /* высота поддерева                 */
} N;

/* Вспомогательные функции для баланса */
static int H(N *n) { 
    return n ? n->h : 0;
}

static void upd(N *n) {
    int l = H(n->L);
    int r = H(n->R); 
    n->h = 1 + (l > r?l:r);
}

static N *rotR(N *y) {
    N *x = y->L;
    y->L = x->R;
    x->R = y;
    upd(y); upd(x);

    return x;
}

static N *rotL(N *x) {
    N *y = x->R;
    x->R = y->L;
    y->L = x;
    upd(x); upd(y);

    return y;
}

/* Восстановить баланс: если дерево накренилось - повернуть */
static N *bal(N *n) {
    upd(n);
    int b = H(n->L) - H(n->R);
    if (b > 1) { 
        if (H(n->L->L) < H(n->L->R)) n->L = rotL(n->L);
        
        return rotR(n); 
    }

    if (b < -1) {
        if (H(n->R->R) < H(n->R->L)) n->R = rotR(n->R);
        
        return rotL(n);
    }

    return n;
}

/* Вставить ключ k со значением v (или обновить, если уже есть) */
static N *ins(N *n, const char *k, const char *v) {
    if (!n) {
        N *p = malloc(sizeof *p);
        strncpy(p->k, k, KS-1); p->k[KS-1] = 0;
        strncpy(p->v, v, VS-1); p->v[VS-1] = 0;
        p->L = p->R = NULL; p->h = 1;
        return p;
    }

    int c = strcmp(k, n->k);

    if (c < 0) {
        n->L = ins(n->L, k, v);
    }

    else if (c > 0) {
        n->R = ins(n->R, k, v);
    }

    else {
        strncpy(n->v, v, VS-1);   /* обновить существующий */
    }

    return bal(n);
}

/* Найти значение по ключу; вернуть NULL если не найдено */
static const char *get(N *n, const char *k) {
    while (n) {
        int c = strcmp(k, n->k);

        if (c < 0) {
            n = n->L;
        }

        else if (c > 0) {
            n = n->R;
        }

        else {
            return n->v;
        }
    }

    return NULL;
}

/* Удалить узел с ключом k */
static N *del(N *n, const char *k) {
    if (!n) {
        return NULL;
    }

    int c = strcmp(k, n->k);

    if (c < 0) {
        n->L = del(n->L, k);
    }

    else if (c > 0) {
        n->R = del(n->R, k);
    }

    else {
        if (!n->L || !n->R) {           /* 0 или 1 потомок: просто вырезаем */
            N *t = n->L ? n->L : n->R;
            free(n); return t;
        }

        /* 2 потомка: заменяем данными наименьшего узла правого поддерева */
        N *s = n->R;

        while (s->L) {
            s = s->L;
        }

        strncpy(n->k, s->k, KS);
        strncpy(n->v, s->v, VS);
        n->R = del(n->R, s->k);
    }

    return bal(n);
}

/* Обход in-order: ключи идут в алфавитном порядке */
static void each(N *n, void (*f)(const char *, const char *, void *), void *ctx) {
    if (!n) {
        return;
    }

    each(n->L, f, ctx);
    f(n->k, n->v, ctx);
    each(n->R, f, ctx);
}

/* Освободить всё дерево */
static void freeT(N *n) {
    if (!n) {
        return; 
    }

    freeT(n->L); freeT(n->R); free(n);
}

/* Загрузка и сохранение файла */
static N *root = NULL;
static int cnt  = 0;

static int load(const char *path) {
    FILE *f = fopen(path, "r");

    if (!f) {
        perror(path);

        return -1;
    }

    char line[KS + VS + 4];

    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\n")] = 0;
        char *c = strchr(line, ':');
        if (!c) {
            continue;
        }

        *c = 0;
        root = ins(root, line, c + 1);
        cnt++;
    }

    fclose(f);
    return 0;
}

static void savecb(const char *k, const char *v, void *f) {
    fprintf((FILE *)f, "%s:%s\n", k, v);
}

static int save(const char *path) {
    FILE *f = fopen(path, "w");

    if (!f) {
        perror(path); return -1;
    }

    each(root, savecb, f);

    fclose(f);
    return cnt;
}

/* main */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s airports.txt\n", argv[0]);

        return 1;
    }

    if (load(argv[1]) < 0) {
        return 1;
    }

    printf("Загружено %d аэропортов. Система готова к работе.\n\n", cnt);

    char line[KS + VS + 16];

    while (1) {
        printf("> "); fflush(stdout);

        if (!fgets(line, sizeof line, stdin)) {
            break;
        }

        line[strcspn(line, "\n")] = 0;

        if (!strcmp(line, "quit")) {
            break;

        } 
        
        else if (!strncmp(line, "find ", 5)) {
            const char *v = get(root, line + 5);

            v ? printf("%s → %s\n", line + 5, v)
              : printf("Аэропорт с кодом '%s' не найден в базе.\n", line + 5);

        } 
        
        else if (!strncmp(line, "add ", 4)) {
            char *c = strchr(line + 4, ':');

            if (!c) {
                puts("Формат: add <код>:<название>");
            }

            else {
                *c = 0;
                root = ins(root, line + 4, c + 1);
                cnt++;
                printf("Аэропорт '%s' добавлен в базу.\n", line + 4);
            }

        }
        
        else if (!strncmp(line, "delete ", 7)) {
            if (!get(root, line + 7)) {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", line + 7);
            } 
            
            else {
                root = del(root, line + 7);
                cnt--;
                printf("Аэропорт '%s' удалён из базы.\n", line + 7);
            }

        }
        
        else if (!strcmp(line, "save")) {
            int n = save(argv[1]);

            if (n >= 0) {
                printf("База сохранена: %d аэропортов.\n", n);
            }

        } 
        
        else if (line[0]) {
            puts("Команды: find <код> | add <код>:<название> | delete <код> | save | quit");
        }
    }

    freeT(root);
    return 0;
}
