#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "AVLTree.h"

template<typename T>
class hashTable
{
private:
    AVLTree<T> __buckets[];
    int __size;

    void rehash(int newSize)
    {

    }

    AVLTree<T>& getBucket(const T& value)
    {

    }
public:
    hashTable();
    ~hashTable();
};


//chatgpt basic implementation
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

    void remove(const Key& key) {
        AVLTree<Key, Value>& bucket = getBucket(key);
        if (bucket.find(key) != nullptr) {
            --size;
            bucket.remove(key);

            // Rehash if the load factor becomes less than 0.25
            if (static_cast<double>(size) / buckets.size() < 0.25)
                rehash(buckets.size() / 2);
        }
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