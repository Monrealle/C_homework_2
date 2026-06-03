"""
prepare.py — готовит airports.txt из airport-codes.csv.

Скачайте CSV:
    wget `https://raw.githubusercontent.com/datasets/airport-codes/master/data/airport-codes.csv`

Запуск:
    python3 prepare.py
    python3 prepare.py airport-codes.csv airports.txt   # явные пути
"""
import csv, sys, os

csv_path = sys.argv[1] if len(sys.argv) > 1 else "airport-codes.csv"
out_path = sys.argv[2] if len(sys.argv) > 2 else "airports.txt"

if not os.path.exists(csv_path):
    print(f"Файл '{csv_path}' не найден.")
    print("Скачайте его:")
    print("wget https://raw.githubusercontent.com/datasets/airport-codes/master/data/airport-codes.csv")
    sys.exit(1)

count = 0
with open(csv_path, encoding="utf-8") as fin, \
     open(out_path, "w", encoding="utf-8") as fout:
    for row in csv.DictReader(fin):
        code = row["iata_code"].strip()
        name = row["name"].strip()
        if code:
            fout.write(f"{code}:{name}\n")
            count += 1

print(f"Готово: {count} аэропортов → '{out_path}'")
