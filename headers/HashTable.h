#pragma once
#include <string>

using namespace std;

enum HashMethod {
    LINEAR_PROBING,
    DOUBLE_HASHING
};

struct HashCell {
    string key;
    string value;
    bool occupied;
    bool deleted;
};

struct HashTable {
    HashCell* data;
    int size;
    int used;
    int capacity;
    HashMethod method;
};

void HTINIT(HashTable* table, HashMethod method);
void HTADD(HashTable* table, const string& key, const string& value);
bool HTGET(HashTable* table, const string& key, string* value);
bool HT_AT(HashTable* table, const string& key);
bool HTDEL(HashTable* table, const string& key);
void HTCLEAR(HashTable* table);
