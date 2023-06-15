#ifndef RECORD_H
#define RECORD_H

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

#endif