#include "../headers/LRUCache.h"

LRUCache::LRUCache(int capacity)
    : capacity(capacity > 0 ? capacity : 0)
{
    HTINIT(&values, LINEAR_PROBING);
    MINIT(&order);
}

LRUCache::~LRUCache()
{
    HTCLEAR(&values);
    MCLEAR(&order);
}

void LRUCache::makeRecent(const string& key)
{
    for (int i = 0; i < MLENGTH(&order); i++)
    {
        if (MGET(&order, i) == key)
        {
            MDEL(&order, i);
            MPUSH(&order, key);
            return;
        }
    }
}

void LRUCache::SET(int key, int value)
{
    if (capacity == 0) return;

    string keyText = to_string(key);

    if (HT_AT(&values, keyText))
    {
        HTADD(&values, keyText, to_string(value));
        makeRecent(keyText);
        return;
    }

    if (MLENGTH(&order) == capacity)
    {
        string oldest = MGET(&order, 0);
        HTDEL(&values, oldest);
        MDEL(&order, 0);
    }

    HTADD(&values, keyText, to_string(value));
    MPUSH(&order, keyText);
}

int LRUCache::GET(int key)
{
    string keyText = to_string(key);
    string value;

    if (!HTGET(&values, keyText, &value)) return -1;

    makeRecent(keyText);
    return stoi(value);
}
