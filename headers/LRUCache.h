#pragma once
#include "Array.h"
#include "HashTable.h"

class LRUCache {
private:
    int capacity;
    HashTable values;
    MArray order;

    void makeRecent(const string& key);

public:
    LRUCache(int capacity);
    ~LRUCache();

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    void SET(int key, int value);
    int GET(int key);
};
