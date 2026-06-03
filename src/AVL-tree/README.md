# AeroSoft - база аэропортов на АВЛ-дереве

Программа загружает базу аэропортов (9 000+ записей) в АВЛ-дерево и предоставляет
интерфейс командной строки для мгновенного поиска, добавления и удаления аэропортов
по IATA-коду.

## Сборка

```bash
git clone https://github.com/Monrealle/C_homework_2
cd src/AVL-tree
mkdir build && cd build
cmake ..
make
```

В результате в папке `build` появятся исполняемый файл `avl`.

## Подготовка данных

```bash
wget https://raw.githubusercontent.com/datasets/airport-codes/master/data/airport-codes.csv
python3 scripts/prepare.py
```

В результате появится файл `airports.txt` с 9 000+ записями вида `КОД:Название`.

## Использование

```
./aerosoft airports.txt
```

```
Загружено 9096 аэропортов. Система готова к работе.

> find SVO
SVO → Sheremetyevo International Airport

> find ZZZ
Аэропорт с кодом 'ZZZ' не найден в базе.

> add ZZZ:Nowhere International Airport
Аэропорт 'ZZZ' добавлен в базу.

> delete ZZZ
Аэропорт 'ZZZ' удалён из базы.

> save
База сохранена: 9096 аэропортов.

> quit
```

### Команды

| Команда                     | Действие                           |
|-----------------------------|------------------------------------|
| `find <код>`                | Найти аэропорт по IATA-коду        |
| `add <код>:<название>`      | Добавить аэропорт в базу           |
| `delete <код>`              | Удалить аэропорт из базы           |
| `save`                      | Сохранить текущее состояние в файл |
| `quit`                      | Завершить работу                   |

## Структура проекта

```
.
├── src/                    # Исходный код
│   ├── main.c              # АВЛ-дерево + точка входа + CLI
│   └── bench.c             # Бенчмарк: АВЛ vs связный список
├── scripts/                # Вспомогательные скрипты
│   └── prepare.py          # Подготовка датасета из CSV
├── CMakeLists.txt          # Конфигурация сборки
└── README.md               # Этот файл
```

## Лицензия

[MIT LICENSE](https://github.com/your-username/aerosoft/blob/main/LICENSE)
