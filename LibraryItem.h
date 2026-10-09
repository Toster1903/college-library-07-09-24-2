#pragma once
#include <string>
class LibraryItem {
public:
    int id; std::string title; int year; bool available;
    LibraryItem(int id, std::string title, int year, bool available = true);
    virtual ~LibraryItem() = default;
    virtual std::string type() const = 0;
    virtual void printInfo() const = 0;
};
