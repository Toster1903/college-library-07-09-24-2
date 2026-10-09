#include "Magazine.h"
#include <iostream>
Magazine::Magazine(int id, std::string title, int year, int issueNumber, int month, bool available)
    : LibraryItem(id, title, year, available), issueNumber(issueNumber), month(month) {}
std::string Magazine::type() const { return "Magazine"; }
void Magazine::printInfo() const {
    std::cout << type() << " #" << id << ": " << title << ", " << year << ", выпуск: " << issueNumber << ", месяц: " << month
              << (available ? " [доступно]" : " [выдано]") << '\n';
}
