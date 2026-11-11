#include "../headers/HashTable.h"

static unsigned int HASH1(const string& key)
{
    unsigned int hash = 0;
    for (char symbol : key)
    {
        hash = hash * 31 + static_cast<unsigned char>(symbol);
    }
    return hash;
}

static unsigned int HASH2(const string& key)
{
    unsigned int hash = 0;
    for (char symbol : key)
    {
        hash = hash * 37 + static_cast<unsigned char>(symbol);
    }
    return hash;
}

static bool ISPRIME(int number)
{
    if (number < 2) return false;
    for (int i = 2; i <= number / i; i++)
    {
        if (number % i == 0) return false;
    }
    return true;
}

static int NEXTCAPACITY(int capacity)
{
    int result = capacity * 2 + 1;
    while (!ISPRIME(result)) result += 2;
    return result;
}

static int PROBE(HashTable* table, const string& key, int attempt)
{
    unsigned int start = HASH1(key) % table->capacity;
    unsigned int step = 1;

    if (table->method == DOUBLE_HASHING)
    {
        step = 1 + HASH2(key) % (table->capacity - 1);
    }

    return (start + static_cast<unsigned long long>(attempt) * step) % table->capacity;
}

void HTINIT(HashTable* table, HashMethod method)
{
    table->capacity = 17;
    table->size = 0;
    table->used = 0;
    table->method = method;
    table->data = new HashCell[table->capacity]{};
}

static void HTREHASH(HashTable* table)
{
    HashCell* oldData = table->data;
    int oldCapacity = table->capacity;

    table->capacity = NEXTCAPACITY(oldCapacity);
    table->data = new HashCell[table->capacity]{};
    table->size = 0;
    table->used = 0;

    for (int i = 0; i < oldCapacity; i++)
    {
        if (oldData[i].occupied)
        {
            HTADD(table, oldData[i].key, oldData[i].value);
        }
    }

    delete[] oldData;
}

void HTADD(HashTable* table, const string& key, const string& value)
{
    if ((table->used + 1) * 10 > table->capacity * 7)
    {
        HTREHASH(table);
    }

    int firstDeleted = -1;

    for (int i = 0; i < table->capacity; i++)
    {
        int index = PROBE(table, key, i);
        HashCell* cell = &table->data[index];

        if (cell->occupied)
        {
            if (cell->key == key)
            {
                cell->value = value;
                return;
            }
        }
        else if (cell->deleted)
        {
            if (firstDeleted == -1) firstDeleted = index;
        }
        else
        {
            if (firstDeleted != -1) index = firstDeleted;
            cell = &table->data[index];

            cell->key = key;
            cell->value = value;
            cell->occupied = true;
            cell->deleted = false;
            table->size++;
            if (firstDeleted == -1) table->used++;
            return;
        }
    }

    if (firstDeleted != -1)
    {
        HashCell* cell = &table->data[firstDeleted];
        cell->key = key;
        cell->value = value;
        cell->occupied = true;
        cell->deleted = false;
        table->size++;
    }
}

bool HTGET(HashTable* table, const string& key, string* value)
{
    for (int i = 0; i < table->capacity; i++)
    {
        int index = PROBE(table, key, i);
        HashCell* cell = &table->data[index];

        if (!cell->occupied && !cell->deleted) return false;
        if (cell->occupied && cell->key == key)
        {
            *value = cell->value;
            return true;
        }
    }
    return false;
}

bool HT_AT(HashTable* table, const string& key)
{
    string value;
    return HTGET(table, key, &value);
}

bool HTDEL(HashTable* table, const string& key)
{
    for (int i = 0; i < table->capacity; i++)
    {
        int index = PROBE(table, key, i);
        HashCell* cell = &table->data[index];

        if (!cell->occupied && !cell->deleted) return false;
        if (cell->occupied && cell->key == key)
        {
            cell->key.clear();
            cell->value.clear();
            cell->occupied = false;
            cell->deleted = true;
            table->size--;
            return true;
        }
    }
    return false;
}

void HTCLEAR(HashTable* table)
{
    delete[] table->data;
    table->data = nullptr;
    table->size = 0;
    table->used = 0;
    table->capacity = 0;
}
