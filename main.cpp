#include "Library.h"
#include "Input.h"
#include <iostream>
#include <climits>
int main() {
    Library library;
    try { library.load(); }
    catch (const std::exception& e) { std::cerr << "Ошибка загрузки: " << e.what() << '\n'; return 1; }
    try {
        for (;;) {
            std::cout << "\nБиблиотека колледжа\n1 — добавить книгу\n2 — добавить журнал\n3 — зарегистрировать читателя\n4 — найти издание по id\n5 — выдать издание\n6 — вернуть издание\n7 — доступные издания\n8 — активные выдачи\n9 — сохранить\n0 — выход\n";
            int command=readInt("Команда: ",INT_MIN,INT_MAX);
            switch(command) {
                case 1: library.addBook(); break;
                case 2: library.addMagazine(); break;
                case 3: library.registerReader(); break;
                case 4: library.searchItem(); break;
                case 5: library.lendItem(); break;
                case 6: library.returnItem(); break;
                case 7: library.printAvailable(); break;
                case 8: library.printActiveLoans(); break;
                case 9: library.save(); break;
                case 0: library.exitProgram(); return 0;
                default: std::cout << "Неизвестная команда\n";
            }
        }
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
