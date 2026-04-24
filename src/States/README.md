## Государства
Программа распределяет города по государствам на основе кратчайших расстояний до их столиц.

## Сборка
git clone https://github.com/Monrealle/C_homework_2
cd States
mkdir build && cd build
cmake ..
make

В результате в папке build появится исполняемый файл states.

## Использование
Программа читает данные из стандартного ввода.  

## Пример
./states < ../examples/input.txt

Результат распределения:
1: 1
2: 2 3 4

## Структура проекта
.
├── src/                    # Исходный код
│   └── states.c
├── examples/               # Примеры входных файлов
│   └── input.txt
├── CMakeLists.txt          # Конфигурация сборки
└── README.md               # Этот файл

## Лицензия
[MIT LICENSE](https://github.com/Monrealle/C_homework_2/blob/main/LICENSE)
