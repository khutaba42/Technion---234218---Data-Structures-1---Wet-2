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

template <typename DATA_t>
class hashTable
{
private:
    Table<AVLTree<DATA_t>> __table;
    int __size;

    void rehash(int newSize)
    {
        Table<AVLTree<DATA_t>> newTable(newSize);

        for (int bucket = 0; bucket < __table.capacity(); bucket++)
        {
            __table[bucket].in_order_traversal(newTable.insert); ////////////////////////////////////////////////////////////////////
        }

        Table<AVLTree<DATA_t>>::swap_tables_contents(__table, newTable);
    }

    AVLTree<DATA_t> &getBucket(const DATA_t &value)
    {
        return __table[(*value) % __table.capacity()];
    }

public:
    hashTable() : __size(0) {}
    ~hashTable() = default;

    void insert(const DATA_t &value)
    {
        AVLTree<DATA_t>& bucket = getBucket(value);

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

// chatgpt basic implementation
/*
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable {
private:
    std::vector<AVLTree<Key, Value>> buckets;
    Hash hashFunction;
    size_t size;

    void rehash(size_t newBucketCount) {
        std::vector<AVLTree<Key, Value>> newBuckets(newBucketCount);

        for (const auto& bucket : buckets) {
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                const Key& key = it->first;
                const Value& value = it->second;
                size_t index = hashFunction(key) % newBucketCount;
                newBuckets[index].insert(key, value);
            }
        }

        buckets = std::move(newBuckets);
    }

    AVLTree<Key, Value>& getBucket(const Key& key) {
        size_t index = hashFunction(key) % buckets.size();
        return buckets[index];
    }

public:
    explicit HashTable(size_t initialBucketCount = 10, const Hash& hash = Hash())
        : buckets(initialBucketCount), hashFunction(hash), size(0) {}

    void insert(const Key& key, const Value& value) {
        AVLTree<Key, Value>& bucket = getBucket(key);
        if (bucket.find(key) == nullptr)
            ++size;
        bucket.insert(key, value);

        // Rehash if the load factor exceeds 1
        if (static_cast<double>(size) / buckets.size() > 1.0)
            rehash(buckets.size() * 2);
    }

    Value* find(const Key& key) {
        AVLTree<Key, Value>& bucket = getBucket(key);
        return bucket.find(key);
    }

    bool empty() const {
        return size == 0;
    }
};
*/
#endif