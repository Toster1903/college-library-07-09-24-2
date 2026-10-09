#pragma once
#include "LibraryItem.h"
class Magazine : public LibraryItem {
public:
    int issueNumber; int month;
    Magazine(int id, std::string title, int year, int issueNumber, int month, bool available = true);
    std::string type() const override;
    void printInfo() const override;
};
