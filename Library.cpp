#include "Library.h"
#include "Book.h"
#include "Magazine.h"
#include "Input.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <stdexcept>
#include <filesystem>
bool Library::isUniqueId(int id) const { return findItem(id) == nullptr; }
bool Library::isUniqueTicket(int ticket) const {
    for (const auto& reader : readers) if (reader.ticket == ticket) return false;
    return true;
}
LibraryItem* Library::findItem(int id) const {
    for (const auto& item : items) if (item->id == id) return item.get();
    return nullptr;
}
void Library::addBook() {
    int id = readInt("ID: ", 1, INT_MAX);
    if (!isUniqueId(id)) { std::cout << "ID уже существует.\n"; return; }
    auto title = readNonEmpty("Название: "); int year = readInt("Год: ",1500,2100);
    auto author = readNonEmpty("Автор: "); auto isbn = readNonEmpty("ISBN: ");
    items.push_back(std::make_unique<Book>(id,title,year,author,isbn));
    std::cout << "Книга добавлена.\n";
}
void Library::addMagazine() {
    int id = readInt("ID: ",1,INT_MAX);
    if (!isUniqueId(id)) { std::cout << "ID уже существует.\n"; return; }
    auto title = readNonEmpty("Название: "); int year = readInt("Год: ",1500,2100);
    int issue = readInt("Номер выпуска: ",1,INT_MAX); int month = readInt("Месяц: ",1,12);
    items.push_back(std::make_unique<Magazine>(id,title,year,issue,month));
    std::cout << "Журнал добавлен.\n";
}
void Library::registerReader() {
    int ticket = readInt("Номер билета: ",1,INT_MAX);
    if (!isUniqueTicket(ticket)) { std::cout << "Билет уже существует.\n"; return; }
    auto name = readNonEmpty("ФИО: "); readers.emplace_back(ticket,name);
    std::cout << "Читатель зарегистрирован.\n";
}
void Library::searchItem() {
    auto item = findItem(readInt("ID издания: ",1,INT_MAX));
    if (item) item->printInfo(); else std::cout << "Издание не найдено.\n";
}
void Library::lendItem() {
    int ticket = readInt("Номер билета: ",1,INT_MAX);
    if (isUniqueTicket(ticket)) { std::cout << "Читатель не найден.\n"; return; }
    auto item = findItem(readInt("ID издания: ",1,INT_MAX));
    if (!item) { std::cout << "Издание не найдено.\n"; return; }
    if (!item->available) { std::cout << "Издание уже выдано.\n"; return; }
    int maxId = 0; for (const auto& loan : loans) if (loan.id > maxId) maxId = loan.id;
    if (maxId == INT_MAX) throw std::runtime_error("Закончились номера выдач");
    loans.emplace_back(maxId + 1,ticket,item->id); item->available = false;
    std::cout << "Издание выдано. Номер выдачи: " << maxId + 1 << '\n';
}
void Library::returnItem() {
    int id = readInt("Номер выдачи: ",1,INT_MAX);
    for (auto& loan : loans) if (loan.id == id) {
        if (!loan.active) { std::cout << "Издание уже возвращено.\n"; return; }
        findItem(loan.itemId)->available = true; loan.active = false;
        std::cout << "Издание возвращено.\n"; return;
    }
    std::cout << "Выдача не найдена.\n";
}
void Library::printAvailable() const {
    bool found = false;
    for (const auto& item : items) if (item->available) { item->printInfo(); found = true; }
    if (!found) std::cout << "Нет доступных изданий.\n";
}
void Library::printActiveLoans() const {
    bool found = false;
    for (const auto& loan : loans) if (loan.active) {
        std::cout << "Выдача #" << loan.id << ", билет: " << loan.readerTicket << ", издание: " << loan.itemId << '\n'; found = true;
    }
    if (!found) std::cout << "Нет активных выдач.\n";
}
void Library::save() const {
    std::ofstream a("items.txt"), b("readers.txt"), c("loans.txt");
    if (!a || !b || !c) throw std::runtime_error("Не удалось открыть файлы для записи");
    for (const auto& item : items) {
        a << item->type() << ';' << item->id << ';' << item->title << ';' << item->year << ';' << item->available << ';';
        if (auto book = dynamic_cast<Book*>(item.get())) a << book->author << ';' << book->isbn;
        else { auto magazine = dynamic_cast<Magazine*>(item.get()); a << magazine->issueNumber << ';' << magazine->month; }
        a << '\n';
    }
    for (const auto& r : readers) b << r.ticket << ';' << r.fullName << '\n';
    for (const auto& l : loans) c << l.id << ';' << l.readerTicket << ';' << l.itemId << ';' << l.active << '\n';
    a.flush(); b.flush(); c.flush();
    if (!a || !b || !c) throw std::runtime_error("Ошибка записи данных");
    std::cout << "Данные сохранены.\n";
}
void Library::exitProgram() const { std::cout << "Выход. Для сохранения изменений используйте команду 9 перед выходом.\n"; }
void Library::load() {
    // Загружаем во временную библиотеку: при ошибке исходные файлы не изменяются.
    Library temp;
    auto lines = [](const std::string& name) {
        std::vector<std::string> result;
        if (!std::filesystem::exists(name)) return result;
        std::ifstream file(name); if (!file) throw std::runtime_error("Не удалось прочитать " + name);
        std::string line; while (std::getline(file,line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (!line.empty()) result.push_back(line);
        }
        if (file.bad()) throw std::runtime_error("Ошибка чтения " + name);
        return result;
    };
    auto validText = [](const std::string& s) {
        if (s.find_first_not_of(" \t\r") == std::string::npos) throw std::runtime_error("Пустое поле в файле");
    };
    for (const auto& line : lines("items.txt")) {
        auto f = split(line,';'); if (f.size()!=7) throw std::runtime_error("Неверный формат items.txt");
        int id = parseInt(f[1],1,INT_MAX), year = parseInt(f[3],1500,2100), available = parseInt(f[4],0,1);
        validText(f[2]); if (!temp.isUniqueId(id)) throw std::runtime_error("Повторный ID в файле");
        if (f[0]=="Book") { validText(f[5]); validText(f[6]); temp.items.push_back(std::make_unique<Book>(id,f[2],year,f[5],f[6],available)); }
        else if (f[0]=="Magazine") temp.items.push_back(std::make_unique<Magazine>(id,f[2],year,parseInt(f[5],1,INT_MAX),parseInt(f[6],1,12),available));
        else throw std::runtime_error("Неизвестный тип издания");
    }
    for (const auto& line : lines("readers.txt")) {
        auto f=split(line,';'); if (f.size()!=2) throw std::runtime_error("Неверный формат readers.txt");
        int ticket=parseInt(f[0],1,INT_MAX); validText(f[1]);
        if (!temp.isUniqueTicket(ticket)) throw std::runtime_error("Повторный билет в файле");
        temp.readers.emplace_back(ticket,f[1]);
    }
    for (const auto& line : lines("loans.txt")) {
        auto f=split(line,';'); if (f.size()!=4) throw std::runtime_error("Неверный формат loans.txt");
        int id=parseInt(f[0],1,INT_MAX), ticket=parseInt(f[1],1,INT_MAX), item=parseInt(f[2],1,INT_MAX), active=parseInt(f[3],0,1);
        for (const auto& l : temp.loans) if (l.id==id || (active && l.active && l.itemId==item)) throw std::runtime_error("Повторная выдача в файле");
        if (temp.isUniqueTicket(ticket) || !temp.findItem(item)) throw std::runtime_error("Выдача ссылается на отсутствующие данные");
        temp.loans.emplace_back(id,ticket,item,active);
    }
    for (const auto& item : temp.items) {
        bool active=false; for (const auto& l : temp.loans) if (l.itemId==item->id && l.active) active=true;
        if (item->available == active) throw std::runtime_error("Доступность издания противоречит выдачам");
    }
    items=std::move(temp.items); readers=std::move(temp.readers); loans=std::move(temp.loans);
}
