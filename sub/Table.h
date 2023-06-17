#ifndef _TABLE_WITH_CAPACITY_H_
#define _TABLE_WITH_CAPACITY_H_

#include <stdexcept>

template <typename DATA_t>
class Table
{
public:
    Table(int capacity = 1);
    ~Table();

    inline int capacity() const { return __capacity; }

    static void swap_tables_contents(Table &first, Table &second);

    DATA_t &operator[](int index);
    const DATA_t &operator[](int index) const;

    // can't use the hash if capacity is 0
    int hash(int num) const { return num % capacity(); }

private:
    DATA_t *__beg;
    int __capacity;

    // helper
    void clear();
    template <typename T>
    inline static void swap(T &first, T &second)
    {
        T temp = first;
        first = second;
        second = temp;
    }

public:
    // prints the Table
    void display(char separator = ' ') const;
};

template <typename DATA_t>
Table<DATA_t>::Table(int capacity) : __beg(nullptr), __capacity(capacity)
{
    if (capacity < 0)
    {
        throw std::invalid_argument("cant have negative capacity");
    }
    else if (capacity > 0)
    {
        __beg = new DATA_t[capacity]();
    }
}

template <typename DATA_t>
Table<DATA_t>::~Table()
{
    clear();
}

template <typename DATA_t>
void Table<DATA_t>::clear()
{
    delete[] __beg;
    __beg = nullptr;
    __capacity = 0;
}

template <typename DATA_t>
DATA_t &Table<DATA_t>::operator[](int index)
{
    if (index < 0 || index >= capacity())
    {
        throw std::out_of_range("can't access this index in operator[].");
    }
    return __beg[index];
}

template <typename DATA_t>
const DATA_t &Table<DATA_t>::operator[](int index) const
{
    return operator[](index);
}

template <typename DATA_t>
void Table<DATA_t>::swap_tables_contents(Table<DATA_t> &first, Table<DATA_t> &second)
{
    swap(first.__beg, second.__beg);
    swap(first.__capacity, second.__capacity);
}

template <typename DATA_t>
void Table<DATA_t>::display(char separator) const
{
    for (size_t i = 0; i < capacity(); i++)
    {
        std::cout << __beg[i];
        std::cout << separator;
    }
    std::cout << '\n';
}

#endif // _TABLE_WITH_CAPACITY_H_