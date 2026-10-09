#pragma once
#include "LibraryItem.h"
#include "Reader.h"
#include "Loan.h"
#include <memory>
#include <vector>
class Library {
    std::vector<std::unique_ptr<LibraryItem>> items;
    std::vector<Reader> readers;
    std::vector<Loan> loans;
public:
    bool isUniqueId(int id) const;
    bool isUniqueTicket(int ticket) const;
    LibraryItem* findItem(int id) const;
    void addBook(); void addMagazine(); void registerReader(); void searchItem();
    void lendItem(); void returnItem(); void printAvailable() const;
    void printActiveLoans() const; void save() const; void exitProgram() const;
    void load();
};
