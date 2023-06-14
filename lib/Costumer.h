#ifndef COSTUMER_H
#define COSTUMER_H

#include "Record.h"

class Costumer
{
private:
    int __id;
    int __phone;
    bool __isMember;
    double __debt;
    
public:
    Costumer(int id, int phone = 0) : __id(id),
                                  __phone(phone),
                                  __isMember(false),
                                  __debt(0)
    {
    }

    ~Costumer() = default;

    bool makeMember()
    {
        if(__isMember)
            return false;
        __isMember = true;
        return true;
    }

    bool isMember() const{
        return __isMember;
    }

    int getPhone() const{
        return __phone;
    }

    void buy(Record& record)
    {
        __debt += record.purchases();
        record.buy();
    }
    
    void givePrize(double amount)
    {
        __debt -= amount;
    }

    double getExpenses() const
    {
        return __debt;
    }

    bool operator=(const Costumer &other) const
    {
        return __id == other.__id;
    }

    bool operator>(const Costumer &other) const
    {
        return __id > other.__id;
    }

    bool operator<(const Costumer &other) const
    {
        return __id < other.__id;
    }

    int operator%(int n) const
    {
        return __id % n;
    }

    static void resetCostumer(Costumer& costumer)
    {
        costumer.__debt = 0;
    }
};

#endif