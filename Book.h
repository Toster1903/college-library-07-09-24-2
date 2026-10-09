#pragma once
#include "LibraryItem.h"
class Book : public LibraryItem {
public:
    std::string author; std::string isbn;
    Book(int id, std::string title, int year, std::string author, std::string isbn, bool available = true);
    std::string type() const override;
    void printInfo() const override;
};
