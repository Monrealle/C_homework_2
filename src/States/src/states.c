#include <stdio.h>
#include <stdlib.h>

// Макрос для сравнения (a меньше b)
#define LESS(a, b) ((a).dist < (b).dist || ((a).dist == (b).dist && (a).owner < (b).owner))

// Структура ребра: куда ведёт и длина
typedef struct {
    int to;
    int len;
} Edge;

// Список смежности для одной вершины
typedef struct {
    Edge* edges;
    int size;
    int capacity;
} AdjList;

// Элемент кучи: расстояние, город, государство
typedef struct {
    int dist;
    int city;
    int owner;
} HeapElem;

// Куча
typedef struct {
    HeapElem* arr;
    int size;
    int capacity;
} Heap;

// Инициализация кучи
void heap_init(Heap* h)
{
    h->arr = NULL;
    h->size = 0;
    h->capacity = 0;
}

// Добавление элемента в кучу
void heap_push(Heap* h, HeapElem elem)
{
    if (h->size >= h->capacity) {
        h->capacity = (h->capacity == 0) ? 8 : h->capacity * 2;
        h->arr = realloc(h->arr, sizeof(HeapElem) * h->capacity);
    }

    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        // Сравнение: сначала по расстоянию, при равенстве – по номеру государства
        if (h->arr[p].dist < elem.dist || (h->arr[p].dist == elem.dist && h->arr[p].owner <= elem.owner))
            break;
        h->arr[i] = h->arr[p];
        i = p;
    }
    h->arr[i] = elem;
}

// Извлечение минимального элемента из кучи
HeapElem heap_pop(Heap* h)
{
    HeapElem top = h->arr[0];
    h->size--;
    if (h->size > 0) {
        HeapElem last = h->arr[h->size];
        int i = 0;
        for (;;) {
            int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
            if (left < h->size && LESS(h->arr[left], h->arr[smallest]))
                smallest = left;
            if (right < h->size && LESS(h->arr[right], h->arr[smallest]))
                smallest = right;
            if (smallest == i)
                break;
            h->arr[i] = h->arr[smallest];
            i = smallest;
        }
        h->arr[i] = last;
    }
    return top;
}

// Освобождение памяти кучи
void heap_free(Heap* h)
{
    free(h->arr);
    h->arr = NULL;
    h->size = 0;
    h->capacity = 0;
}

// Функция сравнения для qsort (сортировка по возрастанию)
int cmp_int(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int main()
{
    int n, m;
    printf("Введите количество городов и количество дорог: ");
    scanf("%d %d", &n, &m);

    // Список смежности (вершины 1..n)
    AdjList* adj = malloc((n + 1) * sizeof(AdjList));
    for (int i = 1; i <= n; i++) {
        adj[i].edges = NULL;
        adj[i].size = 0;
        adj[i].capacity = 0;
    }

    printf("Введите %d дорог в формате: город 1 город 2 длина\n", m);
    for (int i = 0; i < m; i++) {
        int u, v, len;
        scanf("%d %d %d", &u, &v, &len);
        // Добавление ребра u -> v
        if (adj[u].size >= adj[u].capacity) {
            adj[u].capacity = (adj[u].capacity == 0) ? 4 : adj[u].capacity * 2;
            adj[u].edges = realloc(adj[u].edges, sizeof(Edge) * adj[u].capacity);
        }
        adj[u].edges[adj[u].size].to = v;
        adj[u].edges[adj[u].size].len = len;
        adj[u].size++;

        // Добавление ребра v -> u
        if (adj[v].size >= adj[v].capacity) {
            adj[v].capacity = (adj[v].capacity == 0) ? 4 : adj[v].capacity * 2;
            adj[v].edges = realloc(adj[v].edges, sizeof(Edge) * adj[v].capacity);
        }
        adj[v].edges[adj[v].size].to = u;
        adj[v].edges[adj[v].size].len = len;
        adj[v].size++;
    }

    int k;
    printf("Введите количество столиц (государств): ");
    scanf("%d", &k);
    int* capitals = malloc((k + 1) * sizeof(int));
    int* state = calloc(n + 1, sizeof(int)); // 0 – свободен, иначе номер государства

    printf("Введите номера %d столиц: ", k);
    for (int i = 1; i <= k; i++) {
        scanf("%d", &capitals[i]);
        state[capitals[i]] = i;
    }

    // Глобальная куча
    Heap heap;
    heap_init(&heap);

    // Начальное заполнение кучи соседями столиц
    for (int g = 1; g <= k; g++) {
        int c = capitals[g];
        for (int i = 0; i < adj[c].size; i++) {
            int v = adj[c].edges[i].to;
            int w = adj[c].edges[i].len;
            if (state[v] == 0) {
                heap_push(&heap, (HeapElem) { w, v, g });
            }
        }
    }

    int total = k; // уже распределены столицы
    while (total < n) {
        HeapElem top = heap_pop(&heap);
        int v = top.city;
        int g = top.owner;
        if (state[v] != 0)
            continue; // устаревшая запись
        state[v] = g;
        total++;
        // Добавляем соседей нового города
        for (int i = 0; i < adj[v].size; i++) {
            int u = adj[v].edges[i].to;
            int w = adj[v].edges[i].len;
            if (state[u] == 0) {
                heap_push(&heap, (HeapElem) { w, u, g });
            }
        }
    }

    // Сбор результатов
    int** cities = malloc((k + 1) * sizeof(int*));
    int* cities_cnt = calloc(k + 1, sizeof(int));
    for (int i = 1; i <= n; i++) {
        int g = state[i];
        cities_cnt[g]++;
    }
    for (int g = 1; g <= k; g++) {
        cities[g] = malloc(cities_cnt[g] * sizeof(int));
        cities_cnt[g] = 0;
    }
    for (int i = 1; i <= n; i++) {
        int g = state[i];
        cities[g][cities_cnt[g]++] = i;
    }

    // Сортировка городов в каждом государстве
    for (int g = 1; g <= k; g++) {
        qsort(cities[g], cities_cnt[g], sizeof(int), cmp_int);
    }

    // Вывод результата
    printf("\nРезультат распределения:\n");
    for (int g = 1; g <= k; g++) {
        printf("%d:", g);
        for (int i = 0; i < cities_cnt[g]; i++) {
            printf(" %d", cities[g][i]);
        }
        printf("\n");
    }

    // Освобождение памяти
    for (int g = 1; g <= k; g++)
        free(cities[g]);
    free(cities);
    free(cities_cnt);
    free(state);
    free(capitals);
    for (int i = 1; i <= n; i++)
        free(adj[i].edges);
    free(adj);
    heap_free(&heap);

    return 0;
}
