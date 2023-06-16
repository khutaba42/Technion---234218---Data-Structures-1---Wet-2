#ifndef COSTUMER_H
#define COSTUMER_H

class Record
{
private:
    int __id;
    int __purchases;

public:
    Record(int id = 0) : __id(id),
                         __purchases(0)
    {
    }

    ~Record() = default;

    int get_id() const
    {
        return __id;
    }

    void buy()
    {
        __purchases++;
    }

    int purchases() const
    {
        return __purchases;
    }

    bool operator==(const Record &other) const
    {
        return __id == other.__id;
    }

    bool operator>(const Record &other) const
    {
        return __id > other.__id;
    }

    bool operator<(const Record &other) const
    {
        return __id < other.__id;
    }
};

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
        if(__isMember)
            __debt += record.purchases() + 100;
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

    bool operator==(const Costumer &other) const
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
    
    ///////////////////////////////////////////////////////////////////
    friend bool operator==(const Costumer &costumer, int n)
    {
        return costumer.__id == n;
    }

    friend bool operator>(const Costumer &costumer, int n)
    {
        return costumer.__id > n;
    }

    friend bool operator<(const Costumer &costumer, int n)
    {
        return costumer.__id < n;
    }


    void resetData()
    {
        __debt = 0;
    }
};

#endif