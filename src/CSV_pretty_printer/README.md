## CSV Pretty Printer
Утилита для преобразования CSV-файлов в таблицу в формате псевдографики.

## Сборка
git clone https://github.com/Monrealle/C_homework_2
cd CSV_pretty_printer
mkdir build && cd build
cmake ..
make

В результате в папке build появится исполняемый файл csv.

## Использование
./csv <входной_файл.csv> <выходной_файл.txt>

## Пример
./csv ../examples/input.csv ../examples/output.txt

## Структура проекта
.
├── src/                    # Исходный код
│   └── csv.c
├── tests/                  # Тесты
│   └── test_csv.c
├── examples/               # Примеры входных файлов
│  └── input.csv
├── CMakeLists.txt          # Конфигурация сборки
└── README.md               # Этот файл

## Лицензия
[MIT LICENSE](https://github.com/Monrealle/C_homework_2/blob/main/LICENSE)
