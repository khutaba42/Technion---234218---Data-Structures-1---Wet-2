#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "AVLTree.h"
#include "Table.h"
template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &) = AVLTree_CompareUsingOperators<DATA_t>>
class Rehash
{
private:
    Table<AVLTree<DATA_t, compFunction>>* __table;

public:

    Rehash(Table<AVLTree<DATA_t, compFunction>>* table) : __table(table) {}

    void operator()(const DATA_t &value)
    {
        (*__table)[(*value) % __table->capacity()].insert(value);
    }
};

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &) = AVLTree_CompareUsingOperators<DATA_t>>
class hashTable
{
private:
    Table<AVLTree<DATA_t, compFunction>> __table;
    int __size;

    void rehash(int newSize)
    {
        Table<AVLTree<DATA_t, compFunction>> newTable(newSize);
        Rehash<DATA_t, compFunction> rehash_helper(&newTable);
        for (int bucket = 0; bucket < __table.capacity(); bucket++)
        {
            __table[bucket].in_order_traversal(rehash_helper);
        }

        Table<AVLTree<DATA_t, compFunction>>::swap_tables_contents(__table, newTable);
    }

    AVLTree<DATA_t, compFunction> &getBucket(const DATA_t &value)
    {
        return __table[(*value) % __table.capacity()];
    }

public:
    hashTable() : __size(0) {}
    ~hashTable() = default;

    void insert(const DATA_t &value)
    {
        AVLTree<DATA_t, compFunction>& bucket = getBucket(value);

        bucket.insert(value);

        __size++;
        // Rehash if the load factor exceeds 1
        if (static_cast<double>(__size) / __table.capacity() > 1.0)
            rehash(__table.capacity() * 2);
    }

    DATA_t& find(const DATA_t& value)
    {
        return getBucket(value).find(value);
    }
    
    const DATA_t& find(const DATA_t& value) const 
    {
        return getBucket(value).find(value);
    }
};

#endif