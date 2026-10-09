#pragma once
#include <string>
struct Loan {
    int id; int readerTicket; int itemId; bool active;
    Loan(int id, int readerTicket, int itemId, bool active = true);
};
