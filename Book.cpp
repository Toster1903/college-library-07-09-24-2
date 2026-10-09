#include "Book.h"
#include <iostream>
Book::Book(int id, std::string title, int year, std::string author, std::string isbn, bool available)
    : LibraryItem(id, title, year, available), author(author), isbn(isbn) {}
std::string Book::type() const { return "Book"; }
void Book::printInfo() const {
    std::cout << type() << " #" << id << ": " << title << ", " << year << ", автор: " << author << ", ISBN: " << isbn
              << (available ? " [доступно]" : " [выдано]") << '\n';
}
