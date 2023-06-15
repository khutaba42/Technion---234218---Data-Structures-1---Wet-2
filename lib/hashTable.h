#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "AVLTree.h"
#include "Table.h"
/*template <typename DATA_t>
class hashFunction
{
private:
    int __n;

public:
    hashFunction(int n) : __n(n) {}

    int operator()(const DATA_t &value) const
    {
        return value % __n;
    }
};*/

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &) = RankTree_CompareUsingOperators<DATA_t>>
class hashTable
{
private:
    Table<AVLTree<DATA_t, compFunction>> __table;
    int __size;

    void rehash(int newSize)
    {
        Table<AVLTree<DATA_t, compFunction>> newTable(newSize);

        for (int bucket = 0; bucket < __table.capacity(); bucket++)
        {
            __table[bucket].in_order_traversal(newTable[bucket].insert); ////////////////////////////////////////////////////////////////////
        }

        Table<AVLTree<DATA_t>>::swap_tables_contents(__table, newTable);
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

        __size++
        // Rehash if the load factor exceeds 1
        if (static_cast<double>(size) / __table.capacity() > 1.0)
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